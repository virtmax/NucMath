

#include "hist.h"


nucmath::Hist::Hist()
{
    clear();
    changed = true;
}


nucmath::Hist::~Hist()
{


}

void nucmath::Hist::init(double startValue, double binWidth, size_t numberOfBins)
{
    m_startX = startValue;
    m_binWidth = binWidth;
    m_data.clear();
    m_data.resize(numberOfBins,0);
    initialized = true;
    changed = true;
}

void nucmath::Hist::clear()
{
    m_data.clear();
    m_startX = 0;
    m_binWidth = 1;
    initialized = false;
    changed = true;
}

nucmath::Hist& nucmath::Hist::operator=(const nucmath::Hist & hist2d)
{
    clear();
    for(auto it = hist2d.data().begin(); it !=hist2d.data().end(); it++)
    {
        m_data.push_back(*it);
    }

    m_startX = hist2d.getStartX();
    m_binWidth = hist2d.getBinWidth();
    changed = true;

    return *this;
}

std::pair<double, double> nucmath::Hist::data(std::size_t bin) const
{
    if(bin < m_data.size())
        return std::pair<double, double>(m_startX+m_binWidth*bin, m_data.at(bin));
    else
        throw std::out_of_range("nucmath::Hist::data(std::size_t bin): bin="+std::to_string(bin));
}

double nucmath::Hist::max()
{
    double maxVal = std::numeric_limits<double>::min();
    for(auto it = m_data.begin(); it !=m_data.end(); it++)
    {
        maxVal = std::max<double>(maxVal, *it);
    }

    return maxVal;
}

double nucmath::Hist::min()
{
    double minVal = std::numeric_limits<double>::max();
    for(auto it = m_data.begin(); it !=m_data.end(); it++)
    {
        minVal = std::min<double>(minVal, *it);
    }

    return minVal;
}

void nucmath::Hist::create(DataTable &datatable, size_t column, size_t columnData, double xWidth, double binWidth)
{
    auto data = datatable.getData();

    size_t binNr = 0;
    if(data.size() == 0)
        return;

    m_startX = data.at(0).x[column]- xWidth/2.0;
    m_binWidth = binWidth;
    const double endValue = data.at(data.size()-1).x[column]+xWidth/2.0;
    const size_t numOfBins = ceil((endValue-m_startX)/binWidth);  // round up
    m_data.clear();
    m_data.resize(numOfBins,0);


    for(size_t i = 0; i < data.size();i++)
    {
        double newBinValue = 0;

        const double xi = data.at(i).x[column];

        binNr = floor((xi - xWidth/2.0 - m_startX)/binWidth);

        // value fit completely into the bin width
        if(xi+xWidth/2.0 <= m_startX + binWidth*(binNr+1))
        {
            newBinValue += data.at(i).x[columnData];
        }
        else if(xi+xWidth/2.0 > m_startX + binWidth*(binNr+1)
                && xi+xWidth/2.0 <= m_startX + binWidth*(binNr+2))
        {
            double RestFraction = ((xi+xWidth/2.0)-(m_startX + binWidth*(binNr+1)))/xWidth;
            newBinValue += data.at(i).x[columnData]*(1.0-RestFraction);
            double restF = data.at(i).x[columnData]*RestFraction;
            add(binNr+1, restF, true);
        }
        else
        {
            std::cout<< "Hist2d: a data point can't be splitted over more than 2 bins."<<std::endl;
        }

        add(binNr, newBinValue, true);
    }

    changed = true;
}

bool nucmath::Hist::add(double x, double y, bool expand)
{
    if(!initialized)
    {
        if(x>=0)
            m_startX = floor(x/m_binWidth); // value of the first bin
        else
            m_startX = ceil(x/m_binWidth); // value of the first bin

        initialized = true;
    }

    changed = true;


    // expand to smaller values
    if(x < m_startX)
    {
        size_t bin_diff = ceil((m_startX-x)/m_binWidth);
        for(size_t i = 0; i < bin_diff; i++)
        {
            m_data.emplace(m_data.begin(), 0.0);
        }
        m_startX -= bin_diff*m_binWidth;
    }


    size_t bin = floor((x-m_startX)/m_binWidth);

    if(bin < m_data.size())
        m_data.at(bin) += y;
    else
    {
        if(expand)
        {
            m_data.resize(bin+1,0);
            m_data.at(bin) += y;
        }
        else
        {
#ifdef DEBUG_HIST_MSG_OUTPUT
            std::cout << "Hist2d::add : bin="<<bin << " >=  histrogram size. Use expand=true."<<std::endl;
#endif
            return false;
        }

    }


    return true;
}

double nucmath::Hist::sum() const
{
    return std::accumulate(m_data.begin(), m_data.end(), 0);
}

size_t nucmath::Hist::max_bin()
{
    double maxVal = std::numeric_limits<double>::min();
    size_t max_pos = 0;
    for(size_t i = 0; i < m_data.size(); i++)
    {
        if(m_data[i] > maxVal)
        {
            maxVal = m_data[i];
            max_pos = i;
        }
    }

    return max_pos;
}

size_t nucmath::Hist::mean_bin()
{
    const double m = mean_x();
    double s = 0;
    for(size_t i = 0; i < m_data.size(); i++)
    {
        s+= m_data[i]*(m_startX+m_binWidth*i);
        if(s >= m)
        {
            return i;
        }
    }

    return 0;
}

double nucmath::Hist::mean_x() const
{
    if(m_data.size()==0)
    {
        std::cout << "Hist::mean: size == 0. " << std::endl;
        return std::numeric_limits<double>::quiet_NaN();
    }

    double s = 0;
    for(size_t i = 0; i < m_data.size(); i++)
    {
        s+= m_data[i]*(m_startX+m_binWidth*i);
    }

    return s/sum();
}

double nucmath::Hist::standardDeviation() const
{
    if(m_data.size()<2)
        return 0.0;
    else
    {
        double mb = mean_x();
        double var = 0;
        for(size_t i = 0; i < m_data.size(); i++)
        {
            var+= ((m_startX+m_binWidth*i)-mb)*((m_startX+m_binWidth*i)-mb);
        }

        return std::sqrt(var/(m_data.size()*(m_data.size()-1.0)));
    }
}

bool nucmath::Hist::isChanged()
{
    bool temp = changed;
    changed = false;
    return temp;
}

void nucmath::Hist::truncateZeroBins()
{
    if(m_data.size() == 0)
        return;

    // remove zeros on the end of the histogram
    size_t i = m_data.size()-1;
    while(i > 1) {if(m_data[i] == 0) break; i--;}
    size_t remove_counter = m_data.size() - i;
//    m_data.erase(std::remove(m_data.begin()+remove_counter, m_data.end(), 0), m_data.end());

    i= 0;
    while(i < m_data.size()) {if(m_data[i] > 0) break; i++;}
    if(i != 0)
    {
        m_data.erase(std::remove(m_data.begin(), m_data.begin()+i, 0), m_data.end());
        m_startX += m_binWidth*i;
    }

    changed = true;
}

void nucmath::Hist::getThresholdBorders(double threshold, size_t& left_border, size_t& right_border) const
{
    for(size_t i = 0; i < m_data.size(); i++)
    {
        if(m_data.at(i) >= threshold)
        {
            left_border = i;
            break;
        }
    }

    for(size_t i = m_data.size(); i > 1; i--)
    {
        if(m_data.at(i-1) >= threshold)
        {
            right_border = i-1;
            break;
        }
    }
}

void nucmath::Hist::setName(const std::string &name)
{
    if(name.size()>0)
        this->name = name;
    else
        this->name = "noname";
}

void nucmath::Hist::fold(Hist& folded,double resolution_sigma)
{

    const bool mirrorMode = false;

    const size_t len = numberOfBins();
    folded.init(this->getStartX(),this->getBinWidth(),len);


//    std::cout << "Faltung beginnt..." << std::endl;
//    std::cout << "Aufloesung: " << resolution_sigma << std::endl;

    double progress = 0.00;
    for(size_t bin = 0; bin < len; bin++)
    {
     /* if((double)bin/(double)len > progress)
        {
            std::cout << "Kanal: "<< ch << "/"<< len << "   " << progress*100 <<" %"<< std::endl;
            progress += 0.05;
        }*/

        const int kernelWidth2 = static_cast<int>(ceil(resolution_sigma*3)+1);

        double integr = 0.0, integrSum = 0.0;

        for(int k = -kernelWidth2; k <= kernelWidth2; k++)
        {
            if(mirrorMode)
            {
                const size_t indx = std::abs(static_cast<int>(bin)+k);
                while(indx >= len)
                    continue;

                integr = normalDistIntegral(0, resolution_sigma, k, k+this->getBinWidth());
                folded.data()[bin] += m_data[bin]*integr;
            }
            else
            {
                const int indx = static_cast<int>(bin)+k;
                if(indx >=0)
                {
                    integr = normalDistIntegral(0, resolution_sigma, k-this->getBinWidth()/2.0, k+this->getBinWidth()/2.0);
                    //integr = normalDistIntegral(0, resolution_sigma, k, k+this->getBinWidth());
                    //folded.data()[indx] += m_data[bin]*integr;
                    folded.add(indx, m_data[bin]*integr);
                }
            }


            //integrSum += integr;
        }

        //std::cout << integrSum << std::endl;

    }


}


double nucmath::Hist::getStartX() const { return m_startX; }
double nucmath::Hist::getBinWidth() const { return m_binWidth; }


size_t nucmath::Hist::numberOfBins() const { return m_data.size(); }

std::vector<int>& nucmath::Hist::data()  { return m_data; }
const std::vector<int>& nucmath::Hist::data() const { return m_data; }

