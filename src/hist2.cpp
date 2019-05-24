#include "hist2.h"

nucmath::Hist2::Hist2()
{
    clear();
    binWidth = 1.0;
    binHeight = 1.0;
}

nucmath::Hist2::Hist2(double startPosX, double startPosY, double binWidthX, double binWidthY, size_t nBinsX, size_t nBinsY)
{
    init(startPosX, startPosY, binWidthX, binWidthY, nBinsX, nBinsY);
}

void nucmath::Hist2::clear()
{
    field.clear();
    startPosX = 0;
    startPosY = 0;
    initialized = false;
    changed = true;
}

void nucmath::Hist2::init(double startPosX, double startPosY, double binWidthX, double binWidthY, size_t nBinsX, size_t nBinsY)
{
    field.clear();
    this->startPosX = startPosX;
    this->startPosY = startPosY;
    this->binWidth = binWidthX;
    this->binHeight = binWidthY;

    for(size_t i = 0; i < nBinsY; i++)
    {
        field.push_back(std::vector<double>());
    }
    for(size_t i = 0; i < nBinsY; i++)
    {
        field.at(i).resize(nBinsX, 0.0);
    }

    initialized = true;
    changed = true;
}

bool nucmath::Hist2::add(double x, double y, double z, bool expand)
{
    // initialize, before adding any data
    if(!initialized)
    {
        // set the left edge of the first bin
        startPosX = x;
        startPosY = y;
        initialized = true;
    }
    changed = true;


    // expand to smaller y values
    if(y < startPosY && expand)
    {
        size_t binsToAdd = static_cast<size_t>(ceil((startPosY-y)/binHeight));
        size_t binsXtoFill = (field.size() > 0) ? field.at(0).size() : 0;
        field.insert(field.begin(), binsToAdd, std::vector<double>(binsXtoFill, 0));
        startPosY -= binHeight*binsToAdd;
    }

    size_t binY = floor((y-startPosY)/binHeight);

    // expand to bigger y values
    if(binY >= field.size() && expand)
    {
        size_t binsXtoFill = (field.size() > 0) ? field.at(0).size() : 0;
        field.resize(binY+1, std::vector<double>(binsXtoFill, 0.0));
    }

    // expand to smaller x values
    if(x < startPosX && expand)
    {
        size_t binsToAdd = ceil((startPosX-x)/binWidth);
        for(size_t j = 0; j < field.size(); j++)
        {
            field.at(j).insert(field.at(j).begin(), binsToAdd, 0.0);
        }
        startPosX -= binWidth*binsToAdd;
    }

    size_t binX = floor((x-startPosX)/binWidth);

    // expand to bigger x values
    if(field.size()> 0 && binX >= field.at(0).size() && expand)
    {
        for(size_t j = 0; j < field.size(); j++)
        {
            field.at(j).resize(binX+1, 0.0);
        }
    }

    if(binY < field.size() && binX < field.at(binY).size())
        field.at(binY).at(binX) += z;
    else
        return false;


    return true;
}

size_t nucmath::Hist2::nBins() const
{
    if(field.size() > 0)
        return field.size()*field.at(0).size();
    return 0;
}

double nucmath::Hist2::meanOverColumn(size_t column) const
{
    double sum = 0.0;
    for(size_t j = 0; j < field.size(); j++)
    {
        sum += field.at(j).at(column);
    }
    return sum/static_cast<double>(field.size());
}

double nucmath::Hist2::meanOverRow(size_t row) const
{
    double sum = 0.0;
    for(size_t j = 0; j < field.at(row).size(); j++)
    {
        sum += field.at(row).at(j);
    }
    return sum/static_cast<double>(field.at(row).size());
}

std::tuple<double, double, double> nucmath::Hist2::max() const
{
    double max = std::numeric_limits<double>::lowest();
    size_t maxBin = 0;
    for(size_t j = 0; j < field.size()*field.at(0).size(); j++)
    {
        double d = dataref(j);
        if(d > max)
        {
            max = d;
            maxBin = j;
        }
    }

    return data(maxBin);
}

std::tuple<double, double, double> nucmath::Hist2::max(size_t column) const
{
    double max = std::numeric_limits<double>::lowest();
    size_t binY = 0;
    for(size_t j = 0; j < nBinsY(); j++)
    {
        double d = field.at(j).at(column);
        if(d > max)
        {
            max = d;
            binY = j;
        }
    }

    return data(column, binY);
}

double& nucmath::Hist2::dataref(size_t binX, size_t binY)
{
    if(field.size() == 0 || field.at(0).size() == 0)
        throw std::invalid_argument("Hist2::dataref(size_t binX, size_t binY): the histogram is empty");
    if(field.size()*field.at(0).size() <= binX*binY)
        throw std::invalid_argument("Hist2::dataref(size_t binX, size_t binY): requested bin is outside the data field.");

    return field.at(binY).at(binX);
}

double& nucmath::Hist2::dataref(size_t bin)
{
    if(field.size() == 0 || field.at(0).size() == 0)
        throw std::invalid_argument("Hist2::data(size_t bin): the histogram is empty");
    if(field.size()*field.at(0).size() <= bin)
        throw std::invalid_argument("Hist2::data(size_t bin): requested bin is outside the data field.");

    size_t rowLen = field.at(0).size();
    size_t colBin = bin % rowLen;
    size_t rowBin = (bin-colBin)/rowLen;

    return field.at(rowBin).at(colBin);
}

const double& nucmath::Hist2::dataref(size_t bin) const
{
    if(field.size() == 0 || field.at(0).size() == 0)
        throw std::invalid_argument("Hist2::data(size_t bin): the histogram is empty");
    if(field.size()*field.at(0).size() <= bin)
        throw std::invalid_argument("Hist2::data(size_t bin): requested bin is outside the data field.");

    size_t rowLen = field.at(0).size();
    size_t colBin = bin % rowLen;
    size_t rowBin = (bin-colBin)/rowLen;

    return field.at(rowBin).at(colBin);
}

const double& nucmath::Hist2::dataref(size_t binX, size_t binY) const
{
    if(field.size() == 0 || field.at(0).size() == 0)
        throw std::invalid_argument("Hist2::dataref(size_t binX, size_t binY): the histogram is empty");
    if(field.size()*field.at(0).size() <= binX*binY)
        throw std::invalid_argument("Hist2::dataref(size_t binX, size_t binY): requested bin is outside the data field.");

    return field.at(binY).at(binX);
}

std::tuple<double, double, double> nucmath::Hist2::data(size_t bin) const
{
    if(field.size() == 0 || field.at(0).size() == 0)
        throw std::invalid_argument("Hist2::data(size_t bin): the histogram is empty");
    if(field.size()*field.at(0).size() <= bin)
        throw std::invalid_argument("Hist2::data(size_t bin): requested bin is outside the data field.");

    size_t rowLen = field.at(0).size();
    size_t colLen = field.size();
    size_t colBin = bin % rowLen;
    size_t rowBin = (bin-colBin)/rowLen;

    return std::tuple<double, double, double>(startPosX+(colBin+0.5)*binWidth,
                                                  startPosY+(rowBin+0.5)*binHeight, field.at(rowBin).at(colBin));
}

std::tuple<double, double, double> nucmath::Hist2::data(size_t binX, size_t binY) const
{
    return std::tuple<double, double, double>(startPosX+(binX+0.5)*binWidth,
                                                  startPosY+(binY+0.5)*binHeight, field.at(binY).at(binX));
}


size_t nucmath::Hist2::nBinsX() const
{
    if(field.size() == 0)
        return 0;

    return field.at(0).size();
}
size_t nucmath::Hist2::nBinsY() const
{
    return field.size();
}

bool nucmath::Hist2::isChanged(bool leaveChanged)
{
    bool temp = changed;
    changed = (leaveChanged==true) ? true : false;
    return temp;
}

std::pair<double, double> nucmath::Hist2::getRangeX() const
{
    return {startPosX, startPosX + (field.size()>0 ? field.at(0).size()+1 : 0)*binWidth};
}

std::pair<double, double> nucmath::Hist2::getRangeY() const
{
    return {startPosY, startPosY + (field.size()+1)*binHeight};
}

std::tuple<double, double, double, double> nucmath::Hist2::getRange() const
{
    return {startPosX, startPosX + (field.size()>0 ? field.at(0).size()+1 : 0)*binWidth,
                startPosY, startPosY + (field.size()+1)*binHeight};
}

void nucmath::Hist2::setName(const std::string &name)
{
    if(name.size()>0)
        this->name = name;
    else
        this->name = "noname";
}

bool nucmath::Hist2::save(const std::string& path, nucmath::Hist2::FileFormat fileFormat) const
{
    std::ofstream stream;
    stream.open(path);

    if(!stream.is_open())
    {
        std::cout<< "Hist2::save: can't open " << path << std::endl;
        return false;
    }

    if(fileFormat == FileFormat::List)
    {
        // header
        stream << "# 2d histogram" << std::endl
                << "# format: list" << std::endl
                << "# bin width: " << binWidth << std::endl
                << "# bin height: " << binHeight << std::endl
                << "# elements (h x w): " << nBinsY() <<" x " << nBinsX() << std::endl;

        for(size_t iy = 0; iy < nBinsY(); iy++)
        {
            for(size_t ix = 0; ix < nBinsX(); ix++)
            {
                const auto& [x,y,z] = data(iy*nBinsX()+ix);
                stream << y << "\t" << x << "\t" << z << std::endl;
            }
        }
    }
    else if(fileFormat == FileFormat::Array)
    {
        // header
        stream << "# 2d histogram" << std::endl
                << "# format: array" << std::endl
                << "# bin width: " << binWidth << std::endl
                << "# bin height: " << binHeight << std::endl
                << "# elements (h x w): " << nBinsY() <<" x " << nBinsX() << std::endl;

        for(size_t iy = 0; iy < nBinsY(); iy++)
        {
            for(size_t ix = 0; ix < nBinsX(); ix++)
            {
                const double z = dataref(ix, iy);

                if(ix+1 < nBinsX())
                    stream << z << "\t";
                else
                    stream << z;
            }

            if(iy+1 < nBinsY())
                stream << std::endl;
        }
    }

    stream.close();
    return true;
}
