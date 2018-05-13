

#include "hist.h"


nucmath::Hist::Hist()
{
    clear();
    changed = true;
}

nucmath::Hist::Hist(double startValue, double binWidth, size_t nBins)
{
    init(startValue, binWidth, nBins);
}

nucmath::Hist::~Hist()
{


}

void nucmath::Hist::init(double startValue, double binWidth, size_t nBins)
{
    this->startValue = startValue;
    m_binWidth = binWidth;
    field.clear();
    field.resize(nBins,0);
    initialized = true;
    changed = true;
}

void nucmath::Hist::clear()
{
    field.clear();
    startValue = 0;
    m_binWidth = 1;
    initialized = false;
    changed = true;
}

nucmath::Hist nucmath::Hist::getCFD()
{
    Hist copy = *this;
    copy.truncateZeroBins();
    Hist cfd;
    cfd.init(0, copy.getBinWidth(), copy.nBins());
    double sum = 0.0;
    for(size_t i = 0; i < copy.nBins(); i++)
    {
        const auto& [bin, val] = copy.data(i);
        sum += val;
        cfd.add(bin, sum);
    }

    return cfd;
}

nucmath::Hist& nucmath::Hist::normalizeArea()
{
    const double s = this->sum();
    for(size_t i = 0; i < field.size(); i++)
    {
        field[i] /= s;
    }

    return *this;
}

nucmath::Hist& nucmath::Hist::normalizeToMax(double max)
{
    const double hm = this->max();
    for(size_t i = 0; i < field.size(); i++)
    {
        field[i] *= (max/hm);
    }

    return *this;
}

size_t nucmath::Hist::bin(double x) const
{
    if(x>=startValue && x < startValue + field.size()*m_binWidth)
        return floor((x-startValue)/m_binWidth);
    else
        throw std::invalid_argument("Hist::bin(double x): x is outside the data range.");
}

void nucmath::Hist::fill(double x)
{
    field.assign(field.size(), x);
}

nucmath::Hist& nucmath::Hist::operator=(const nucmath::Hist & hist2d)
{
    clear();
    for(auto it = hist2d.data().begin(); it !=hist2d.data().end(); it++)
    {
        field.push_back(*it);
    }

    startValue = hist2d.getStartX();
    m_binWidth = hist2d.getBinWidth();
    changed = true;

    return *this;
}

std::pair<double, double> nucmath::Hist::data(size_t bin) const
{
    if(bin < field.size())
        return std::pair<double, double>(startValue+m_binWidth*bin, field.at(bin));
    else
        throw std::out_of_range("nucmath::Hist::data(std::size_t bin): bin="+std::to_string(bin));
}

std::pair<double, double> nucmath::Hist::dataFromX(double x) const
{
    return data(bin(x));
}

nucmath::Hist nucmath::Hist::operator* (const double factor) const
{
    Hist hist = *this;
    for(double &v : hist)
    {
        v *= factor;
    }
    return hist;
}

std::pair<double, double> nucmath::Hist::getRange() const
{
    return std::pair<double, double>(startValue, startValue+m_binWidth*field.size());
}

void nucmath::Hist::setCopy(const nucmath::Hist &hist)
{
    *this = hist;
    changed = true;
}

double nucmath::Hist::max()
{
    double maxVal = std::numeric_limits<double>::min();
    for(auto it = field.begin(); it !=field.end(); it++)
    {
        maxVal = std::max<double>(maxVal, *it);
    }

    return maxVal;
}

double nucmath::Hist::min()
{
    double minVal = std::numeric_limits<double>::max();
    for(auto it = field.begin(); it !=field.end(); it++)
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
            field.emplace(field.begin(), 0.0);
        }
        startValue -= bin_diff*m_binWidth;
    }


    size_t bin = floor((x-startValue)/m_binWidth);

    if(bin < field.size())
        field.at(bin) += y;
    else
    {
        if(expand)
        {
            field.resize(bin+1,0);
            field.at(bin) += y;
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
    return std::accumulate(field.begin(), field.end(), 0);
}

double nucmath::Hist::mean() const
{
    return sum()/nBins();
}

size_t nucmath::Hist::maxBin() const
{
    double maxVal = std::numeric_limits<double>::min();
    size_t max_pos = 0;
    for(size_t i = 0; i < field.size(); i++)
    {
        if(field[i] > maxVal)
        {
            maxVal = field[i];
            max_pos = i;
        }
    }

    return max_pos;
}

size_t nucmath::Hist::meanBin()
{
    double m = mean();
    for(size_t i = 0; i < field.size(); i++)
    {
        if(field[i] >= m)
        {
            return i;
        }
    }

    return std::numeric_limits<size_t>::quiet_NaN();
}

bool nucmath::Hist::isChanged(bool leaveChanged)
{
    bool temp = changed;
    changed = (leaveChanged==true) ? true : false;
    return temp;
}

void nucmath::Hist::setChanged()
{
    changed = true;
}

void nucmath::Hist::truncateZeroBins()
{
    if(field.size() == 0)
        return;

    // remove zeros on the end of the histogram
    size_t i = field.size()-1;
    while(i > 1) {if(field[i] == 0) break; i--;}
    size_t remove_counter = field.size() - i;
//    m_data.erase(std::remove(m_data.begin()+remove_counter, m_data.end(), 0), m_data.end());

    i= 0;
    while(i < field.size()) {if(field[i] > 0) break; i++;}
    if(i != 0)
    {
        field.erase(std::remove(field.begin(), field.begin()+i, 0), field.end());
        startValue += m_binWidth*i;
    }

    changed = true;
}

void nucmath::Hist::getThresholdBorders(double threshold, size_t& left_border, size_t& right_border) const
{
    for(size_t i = 0; i < field.size(); i++)
    {
        if(field.at(i) >= threshold)
        {
            left_border = i;
            break;
        }
    }

    for(size_t i = field.size(); i > 1; i--)
    {
        if(field.at(i-1) >= threshold)
        {
            right_border = i-1;
            break;
        }
    }
}

bool nucmath::Hist::sumUp(const Hist& hist)
{
    if(hist.getBinWidth() < getBinWidth())
    {
        std::cout << "nucmath::Hist::sumUp error: bin width should be equal to or bigger than of the destination histogram" << std::endl;
        return false;
    }

    double frac = hist.getBinWidth()/getBinWidth();
    double parts = ceil(frac+1.0);

    for(size_t i = 0; i < hist.nBins(); i++)
    {
        const double y = hist.data().at(i);
        const double x = hist.getStartX() + hist.getBinWidth()*i;

        for(size_t j = 0; j < (size_t)parts; j++)
        {
            if(j == 0)
            {
                double are_fraction = getBinWidth()-fmod(x - getStartX(), getBinWidth());
                add(x, y*are_fraction/parts);
            }
            else if(j == (size_t)parts-1)
            {
                double are_fraction = fmod(x - getStartX(), getBinWidth());
                add(x, y*are_fraction/parts);
            }
            else
            {
                add(x, y/parts);
            }
        }
    }

    return true;
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
                folded.data()[bin] += field[bin]*integr;
            }
            else
            {
                const int indx = static_cast<int>(bin)+k;
                if(indx >=0)
                {
                    integr = normalDistIntegral(0, sigmaResolution, k-this->getBinWidth()/2.0, k+this->getBinWidth()/2.0);
                    //integr = normalDistIntegral(0, resolution_sigma, k, k+this->getBinWidth());
                    //folded.data()[indx] += m_data[bin]*integr;
                    folded.add(indx, field[bin]*integr);
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

bool nucmath::Hist::setStartX(double startX)
{
    this->startValue = startX;

	return true;
}

double nucmath::Hist::getStartX() const { return startValue; }
double nucmath::Hist::getBinWidth() const { return m_binWidth; }


size_t nucmath::Hist::nBins() const { return field.size(); }

std::vector<double>& nucmath::Hist::data()  { return field; }
const std::vector<double>& nucmath::Hist::data() const { return field; }

void nucmath::Hist::save(const std::string& path) const
{
    std::ofstream stream;
    stream.open(path);

    if(stream.is_open())
    {
        for(size_t i = 0; i < nBins(); i++)
        {
            auto v = data(i);
            stream << v.first << "\t" << v.second << std::endl;
        }
    }
    else
    {
        std::cout<< "Hist::save: can't open " << path << std::endl;
    }

    stream.close();
}
