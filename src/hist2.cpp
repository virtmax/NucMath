#include "hist2.h"

nucmath::Hist2::Hist2()
{
    clear();
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
    binWidthX = 1.0;
    binWidthY = 1.0;
    initialized = false;
    changed = true;
}

void nucmath::Hist2::init(double startPosX, double startPosY, double binWidthX, double binWidthY, size_t nBinsX, size_t nBinsY)
{
    this->startPosX = startPosX;
    this->startPosY = startPosY;
    this->binWidthX = binWidthX;
    this->binWidthY = binWidthY;
    field.clear();

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
        startPosX = x - binWidthX/2.0;
        startPosY = y - binWidthY/2.0;
        initialized = true;
    }
    changed = true;


    // expand to smaller y values
    if(y < startPosY && expand)
    {
        size_t binsToAdd = static_cast<size_t>(ceil((startPosY-y)/binWidthY));
        size_t binsXtoFill = (field.size() > 0) ? field.at(0).size() : 0;
        field.insert(field.begin(), binsToAdd, std::vector<double>(binsXtoFill, 0));
        startPosY -= binWidthY*binsToAdd;
    }

    size_t binY = floor((y-startPosY)/binWidthY);

    // expand to bigger y values
    if(binY >= field.size() && expand)
    {
        size_t binsXtoFill = (field.size() > 0) ? field.at(0).size() : 0;
        field.resize(binY+1, std::vector<double>(binsXtoFill, 0.0));
    }

    // expand to smaller x values
    if(x < startPosX && expand)
    {
        size_t binsToAdd = ceil((startPosX-x)/binWidthX);
        for(size_t j = 0; j < field.size(); j++)
        {
            field.at(j).insert(field.at(j).begin(), binsToAdd, 0.0);
        }
        startPosX -= binWidthX*binsToAdd;
    }

    size_t binX = floor((x-startPosX)/binWidthX);

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

std::tuple<double, double, double> nucmath::Hist2::data(size_t bin) const
{
    if(field.size() == 0 || field.at(0).size() == 0)
        throw std::invalid_argument("Hist2::data(size_t bin): the histogram is empty");

    size_t binX = bin % field.at(0).size();
    size_t binY = (bin-binX)/field.at(0).size();

    if(binY < field.size() && binX < field.at(binY).size())
        return std::tuple<double, double, double>(startPosX+binX*binWidthX+binWidthX/2.0,
                                                  startPosY+binY*binWidthY+binWidthY/2.0, field.at(binY).at(binX));
    else
        throw std::invalid_argument("Hist2::data(size_t binX, size_t binY): requested bins are outside the data size");

}

bool nucmath::Hist2::isChanged(bool leaveChanged)
{
    bool temp = changed;
    changed = (leaveChanged==true) ? true : false;
    return temp;
}

std::pair<double, double> nucmath::Hist2::getRangeX() const
{
    return {startPosX, startPosX + (field.size()>0 ? field.at(0).size()+1 : 0)*binWidthX};
}

std::pair<double, double> nucmath::Hist2::getRangeY() const
{
    return {startPosY, startPosY + (field.size()+1)*binWidthY};
}

std::tuple<double, double, double, double> nucmath::Hist2::getRange() const
{
    return {startPosX, startPosX + (field.size()>0 ? field.at(0).size()+1 : 0)*binWidthX,
                startPosY, startPosY + (field.size()+1)*binWidthY};
}

void nucmath::Hist2::setName(const std::string &name)
{
    if(name.size()>0)
        this->name = name;
    else
        this->name = "noname";
}
