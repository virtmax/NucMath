

#include "hist.h"


nucmath::Hist::Hist()
{
    clear();
    changed = true;
}


nucmath::Hist::~Hist()
{


}

void nucmath::Hist::init(double startValue, double binWidth, size_t nBins)
{
    startValue = startValue;
    m_binWidth = binWidth;
    m_data.clear();
    m_data.resize(nBins,0);
    initialized = true;
    changed = true;
}

void nucmath::Hist::clear()
{
    m_data.clear();
    startValue = 0;
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

    startValue = hist2d.getStartX();
    m_binWidth = hist2d.getBinWidth();
    changed = true;

    return *this;
}

std::pair<double, double> nucmath::Hist::data(size_t bin) const
{
    if(bin < m_data.size())
        return std::pair<double, double>(startValue+m_binWidth*bin, m_data.at(bin));
    else
        throw std::out_of_range("nucmath::Hist::data(std::size_t bin): bin="+std::to_string(bin));
}

void nucmath::Hist::setCopy(const nucmath::Hist &hist)
{
    *this = hist;
    changed = true;
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



bool nucmath::Hist::add(double x, double y, bool expand)
{
    if(!initialized)
    {
        if(x>=0)
            startValue = x - fmod(x, m_binWidth); // value of the first bin
        else
            startValue = x - fmod(x, m_binWidth) - m_binWidth; // value of the first bin

        initialized = true;
    }

    changed = true;


    // expand to smaller values
    if(x < startValue)
    {
        size_t bin_diff = ceil((startValue-x)/m_binWidth);
        for(size_t i = 0; i < bin_diff; i++)
        {
            m_data.emplace(m_data.begin(), 0.0);
        }
        startValue -= bin_diff*m_binWidth;
    }


    size_t bin = floor((x-startValue)/m_binWidth);

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

double nucmath::Hist::mean() const
{
    return sum()/nBins();
}

size_t nucmath::Hist::maxBin()
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

size_t nucmath::Hist::meanBin()
{
    double m = mean();
    for(size_t i = 0; i < m_data.size(); i++)
    {
        if(m_data[i] >= m)
        {
            return i;
        }
    }

    return std::numeric_limits<size_t>::quiet_NaN();
}
/*
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
        s+= m_data[i]*(startValue+m_binWidth*i);
    }

    return s/sum();
}
*/
/*
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
*/

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
        startValue += m_binWidth*i;
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

void nucmath::Hist::fold(Hist& folded,double sigmaResolution)
{

    const bool mirrorMode = false;

    const size_t len = nBins();
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

        const int kernelWidth2 = static_cast<int>(ceil(sigmaResolution*3)+1);

        double integr = 0.0, integrSum = 0.0;

        for(int k = -kernelWidth2; k <= kernelWidth2; k++)
        {
            if(mirrorMode)
            {
                const size_t indx = std::abs(static_cast<int>(bin)+k);
                while(indx >= len)
                    continue;

                integr = normalDistIntegral(0, sigmaResolution, k, k+this->getBinWidth());
                folded.data()[bin] += m_data[bin]*integr;
            }
            else
            {
                const int indx = static_cast<int>(bin)+k;
                if(indx >=0)
                {
                    integr = normalDistIntegral(0, sigmaResolution, k-this->getBinWidth()/2.0, k+this->getBinWidth()/2.0);
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

bool nucmath::Hist::setBinWidth(double width)
{
    if(width <= 0)
        return false;
    else
        m_binWidth = width;
}

double nucmath::Hist::getStartX() const { return startValue; }
double nucmath::Hist::getBinWidth() const { return m_binWidth; }


size_t nucmath::Hist::nBins() const { return m_data.size(); }

std::vector<double>& nucmath::Hist::data()  { return m_data; }
const std::vector<double>& nucmath::Hist::data() const { return m_data; }
