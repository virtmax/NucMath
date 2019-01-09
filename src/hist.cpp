

#include "hist.h"


nucmath::Hist::Hist()
{
    clear();
    binWidth = 1;
}

nucmath::Hist::Hist(double startValue, double binWidth, size_t nBins)
{
    init(startValue, binWidth, nBins);
}

nucmath::Hist::~Hist()
{


}

void nucmath::Hist::init(double lowerEdge, double binWidth, size_t nBins)
{
    field.clear();
    field.resize(nBins, 0);
    initialized = true;
    changed = true;
    this->lowerEdge = lowerEdge;
    this->binWidth = binWidth;
    hRangeL = lowerEdge;
    hRangeR = lowerEdge;
}

void nucmath::Hist::clear()
{
    field.clear();
    lowerEdge = 0;
    initialized = false;
    changed = true;
    hRangeL = std::numeric_limits<double>::max();
    hRangeR = std::numeric_limits<double>::lowest();
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
    if(x>=lowerEdge && x < lowerEdge + field.size()*binWidth)
        return floor((x-lowerEdge)/binWidth);
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

    lowerEdge = hist2d.getLowestEdge();
    binWidth = hist2d.getBinWidth();
    changed = true;

    return *this;
}

std::pair<double, double> nucmath::Hist::data(size_t bin) const
{
    if(bin < field.size())
        return std::pair<double, double>(lowerEdge+binWidth*bin, field.at(bin));
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
    return std::pair<double, double>(lowerEdge, lowerEdge+binWidth*field.size());
}

void nucmath::Hist::setCopy(const nucmath::Hist &hist)
{
    *this = hist;
    changed = true;
}

double nucmath::Hist::max()
{
    double maxVal = std::numeric_limits<double>::lowest();
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
            lowerEdge = x - fmod(x, binWidth); // value of the first bin
        else
            lowerEdge = x - fmod(x, binWidth) - binWidth; // value of the first bin

        initialized = true;
    }

    changed = true;


    // expand to smaller values
    if(x < lowerEdge)
    {
        size_t bin_diff = ceil((lowerEdge-x)/binWidth);
        for(size_t i = 0; i < bin_diff; i++)
        {
            field.emplace(field.begin(), 0.0);
        }
        lowerEdge -= bin_diff*binWidth;
    }


    size_t bin = floor((x-lowerEdge)/binWidth);

    if(bin < field.size())
    {
        field.at(bin) += y;
        hRangeL = std::min(hRangeL, x);
        hRangeR = std::max(hRangeR, x);
    }
    else
    {
        if(expand)
        {
            field.resize(bin+1,0);
            field.at(bin) += y;
            hRangeL = std::min(hRangeL, x);
            hRangeR = std::max(hRangeR, x);
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

double nucmath::Hist::centerOfMass()
{
    double wmass = 0.0;
    for(size_t i = 0; i < field.size(); i++)
    {
        const auto& [x,y] = data(i);
        wmass += x*y;
    }
    wmass /= sum();
    return wmass;
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
    double maxVal = std::numeric_limits<double>::lowest();
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
    while(i > 1) {if(isEqual(field[i], 0.0)) break; i--;}
//    size_t remove_counter = field.size() - i;
//    m_data.erase(std::remove(m_data.begin()+remove_counter, m_data.end(), 0), m_data.end());

    i= 0;
    while(i < field.size()) {if(field[i] > 0) break; i++;}
    if(i != 0)
    {
        field.erase(std::remove(field.begin(), field.begin()+i, 0), field.end());
        lowerEdge += binWidth*i;
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
        const double x = hist.getLowestEdge() + hist.getBinWidth()*i;

        for(size_t j = 0; j < (size_t)parts; j++)
        {
            if(j == 0)
            {
                double are_fraction = getBinWidth()-fmod(x - getLowestEdge(), getBinWidth());
                add(x, y*are_fraction/parts);
            }
            else if(j == (size_t)parts-1)
            {
                double are_fraction = fmod(x - getLowestEdge(), getBinWidth());
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
    folded.init(this->getLowestEdge(),this->getBinWidth(),len);


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
        binWidth = width;

    return true;
}
/*
bool nucmath::Hist::setStartX(double startX)
{
    this->lowerEdge = startX;
	return true;
}
*/

std::pair<double, double> nucmath::Hist::getRangeOfInsertedData() const
{
    return {hRangeL, hRangeR};
}

double nucmath::Hist::getLowestEdge() const { return lowerEdge; }
double nucmath::Hist::getHighestEdge() const{ return lowerEdge + nBins()*binWidth; };

double nucmath::Hist::getBinWidth() const { return binWidth; }


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
            const auto& [x,y] = data(i);
            stream << nucmath::to_string(x,10) << "\t" << y << std::endl;
        }
    }
    else
    {
        std::cout<< "Hist::save: can't open " << path << std::endl;
    }

    stream.close();
}
