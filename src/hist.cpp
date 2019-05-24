

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

nucmath::Hist& nucmath::Hist::operator=(const nucmath::Hist& hist)
{
    clear();
    field = hist.data();

    lowerEdge = hist.getLowestEdge();
    binWidth = hist.getBinWidth();
    hRangeL = hist.getRange().first;
    hRangeR = hist.getRange().second;
    changed = true;
    initialized = true;

    return *this;
}

std::pair<double, double> nucmath::Hist::data(size_t bin) const
{
    if(bin < field.size())
        return std::pair<double, double>(lowerEdge+binWidth*(bin+0.5), field.at(bin));
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
        double bin_diff = ceil((lowerEdge-x)/binWidth);
        for(double i = 0; i < bin_diff; i++)
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
            field.resize(bin+1, 0);
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

bool nucmath::Hist::create(nucmath::DataTable &datatable, size_t column, double binWidth)
{
    if(column >= datatable.getNumberOfColumns())
        return false;

    auto data = datatable.getData();
    if(data.size() == 0)
        return false;

    init(data.at(0)[column], binWidth, 8);

    for(size_t i = 0; i < data.size(); i++)
    {
        add(data.at(i)[column], 1, true);
    }

    return true;
}

bool nucmath::Hist::create(nucmath::DataTable &datatable, size_t xColumn, size_t yColumn, double xWidth, double binWidth)
{
    if(xColumn >= datatable.getNumberOfColumns() || yColumn >= datatable.getNumberOfColumns())
        return false;

    auto data = datatable.getData();
    if(data.size() == 0)
        return false;

    size_t binNr = 0;
    lowerEdge = data.at(0)[xColumn]- xWidth/2.0;
    binWidth = binWidth;
    const double endValue = data.at(data.size()-1)[xColumn]+xWidth/2.0;
    const size_t numOfBins = ceil((endValue-lowerEdge)/binWidth);  // round up
    field.clear();
    field.resize(numOfBins,0);


    for(size_t i = 0; i < data.size();i++)
    {
        double newBinValue = 0;
        const double xi = data.at(i)[xColumn];
        binNr = floor((xi - xWidth/2.0 - lowerEdge)/binWidth);

        // value fit completely into the bin width
        if(xi+xWidth/2.0 <= lowerEdge + binWidth*(binNr+1))
        {
            newBinValue += data.at(i)[yColumn];
        }
        else if(xi+xWidth/2.0 > lowerEdge + binWidth*(binNr+1)
                && xi+xWidth/2.0 <= lowerEdge + binWidth*(binNr+2))
        {
            double RestFraction = ((xi+xWidth/2.0)-(lowerEdge + binWidth*(binNr+1)))/xWidth;
            newBinValue += data.at(i)[yColumn]*(1.0-RestFraction);
            double restF = data.at(i)[yColumn]*RestFraction;
            add(binNr+1, restF, true);
        }
        else
        {
            std::cout<< "Hist2d: a data point can't be splitted over more than 2 bins."<<std::endl;
        }

        add(binNr, newBinValue, true);
    }

    changed = true;
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

double nucmath::Hist::standardDeviation() const
{
    return standardDeviation(hRangeL, hRangeR);
}

double nucmath::Hist::standardDeviation(double start, double end) const
{
    double s = 0.0;
    double mean = this->mean(start, end);
    double n = 0;
    for(size_t i = 0; i < field.size(); i++)
    {
        const auto& [x,y] = data(i);
        if(start <= x && x<= end)
        {
            s += (mean-y)*(mean-y);
            n++;
        }
    }

    if(nBins() >= 2)
        s = std::sqrt(s/(n-1));
    else
        s = 0;

    return s;
}

double nucmath::Hist::sum() const
{
    return std::accumulate(field.begin(), field.end(), 0);
}

double nucmath::Hist::mean() const
{
    return sum()/nBins();
}

double nucmath::Hist::mean(double start, double end) const
{
    start = std::max(start, lowerEdge);
    end = std::min(end, this->hRangeR);

    size_t sBin = 0;
    size_t eBin = 0;
    double m = 0;

    sBin = bin(start);
    eBin = bin(end);
    if(eBin > 0)
        m = std::accumulate(field.begin()+sBin, field.begin()+eBin, 0)/(eBin + 1 - sBin);



    return m;
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

void nucmath::Hist::load(const std::string& path)
{
    std::ifstream in;
    in.open(path);

    if(!in.is_open())
        throw std::invalid_argument("Can't open '" + path + "'.");

    std::vector<std::string> tokens;
    std::vector<double> row = {};
    std::smatch match;
    std::string line = "";
    while(!in.eof())
    {
        std::getline(in, line);

        line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));   // trim leading whitespaces

        if(line.size() == 0 || line.at(0) == '#' || line.at(0) == '_')
            continue;

        tokens = tokenize(line, {' ', '\t', '|', ','});

        double x = 0.0;
        double y = 0.0;
        s2d(tokens.at(0).c_str(), x);
        s2d(tokens.at(1).c_str(), y);

        add(x,y);
    }

    in.close();
}

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

nucmath::Hist nucmath::Hist::getWithNewBinning(double binning)
{
    Hist hist;
    hist.setBinWidth(binning);

    for(size_t i = 0; i < nBins(); i++)
    {
        hist.add(data(i).first, data(i).second);    // TODO: naive lösung, erzeugt artefakte
    }

    return hist;
}

nucmath::Hist nucmath::Hist::getUnfolded(double sigma)
{
    Hist copy = *this;
    Hist hist(this->hRangeL, binWidth, 10);

    double binWidth = copy.getBinWidth();

    bool done = false;
    size_t i = 0;
    while(!done)
    {
        size_t maxBin = copy.maxBin();
        double maxBinX = copy.data(maxBin).first;
        double startX = maxBinX;//nucmath::getRandom(maxBinX - binWidth/2.0, maxBinX + binWidth/2.0);
        double SUM = 0.0;

        int b = maxBin;
        for(double x = maxBinX-binWidth/2.0; x > maxBinX-binWidth/2.0-5*sigma; x-=binWidth)
        {
            double l = x;
            double r = x + binWidth;
            double sum = nucmath::normalDistIntegral(startX, sigma, l, r);

            if(b >= 0)
            {
                SUM += sum;
                copy.data()[b] -= sum;
            }
            else
                break;

            b--;
        }
        b = maxBin+1;
        for(double x = maxBinX+binWidth/2.0; x < maxBinX+binWidth/2.0+5*sigma; x+=binWidth)
        {
            double l = x;
            double r = x + binWidth;
            double sum = nucmath::normalDistIntegral(startX, sigma, l, r);


            if(b < copy.nBins())
            {
                SUM += sum;
                copy.data()[b] -= sum;
            }
            else
                break;

            b++;
        }

        hist.add(startX, SUM);

        if(copy.mean() < 0.0 )
            done = true;

        i++;
    }

    return hist;
}
