#include "hist3.h"

nucmath::Hist3::Hist3()
{
    clear();
    binWidth = 1.0;
    binHeight = 1.0;
    binDepth = 1.0;
}

nucmath::Hist3::Hist3(double startPosX, double startPosY, double startPosZ, double binWidth, double binHeight, double binDepth)
{
    init(startPosX, startPosY, startPosZ, binWidth, binHeight, binDepth);
}

void nucmath::Hist3::clear()
{
    histmap.clear();
    startPosX = 0;
    startPosY = 0;
    startPosZ = 0;
    endPosX = 0;
    endPosY = 0;
    endPosZ = 0;
    initialized = false;
    changed = true;
}

void nucmath::Hist3::init(double startPosX, double startPosY, double startPosZ, double binWidth, double binHeight, double binDepth)
{
    histmap.clear();
    this->startPosX = startPosX;
    this->startPosY = startPosY;
    this->startPosZ = startPosZ;
    endPosX = startPosX + binWidth;
    endPosY = startPosY + binHeight;
    endPosZ = startPosZ + binDepth;
    this->binWidth = binWidth;
    this->binHeight = binHeight;
    this->binDepth = binDepth;

    initialized = true;
    changed = true;
}

bool nucmath::Hist3::add(double x, double y, double z, double c, bool expand)
{
    // initialize, before adding any data
    if(!initialized)
    {
        initialized = true;
        startPosX = x;
        startPosY = y;
        startPosZ = z;
        endPosX = x + binWidth;
        endPosY = y + binHeight;
        endPosZ = z + binDepth;
    }
    changed = true;

    double binZ = floor(z / binDepth);
    double binY = floor(y / binHeight);
    double binX = floor(x / binWidth);

    histmap[{binX, binY, binZ}] += c;

    startPosX = std::min(startPosX, x);
    startPosY = std::min(startPosY, y);
    startPosZ = std::min(startPosZ, z);
    endPosX = std::max(endPosX, x + binWidth);
    endPosY = std::max(endPosY, y + binHeight);
    endPosZ = std::max(endPosZ, z + binDepth);

    expand = false;
    /*
    // expand to smaller z values
    size_t binsToAddZ = 0;
    if(z < startPosZ && expand)
    {
        binsToAddZ = static_cast<size_t>(ceil((startPosZ-z)/binDepth));
        startPosZ -= binDepth*binsToAddZ;
    }
    size_t binZ = static_cast<size_t>(floor((z-startPosZ)/binDepth));
    endPosZ = startPosZ + (binZ+1)*binDepth;

    size_t binsToAddY = 0;
    if(y < startPosY && expand)
    {
        binsToAddY = static_cast<size_t>(ceil((startPosY-y)/binHeight));
        startPosY -= binHeight*binsToAddY;
    }
    size_t binY = static_cast<size_t>(floor((y-startPosY)/binHeight));
    endPosY = startPosY + (binY+1)*binHeight;

    size_t binsToAddX = 0;
    if(x < startPosX && expand)
    {
        binsToAddX = static_cast<size_t>(ceil((startPosX-x)/binWidth));
        startPosX -= binWidth*binsToAddX;
    }
    size_t binX = static_cast<size_t>(floor((x-startPosX)/binWidth));
    endPosX = startPosX + (binX+1)*binWidth;
*/

    /*
    // expand to smaller z values
    size_t binsToAddZ = 0;
    if(z < startPosZ && expand)
    {
        binsToAddZ = static_cast<size_t>(ceil((startPosZ-z)/binDepth));
        startPosZ -= binDepth*binsToAddZ;
    }
    size_t binZ = static_cast<size_t>(floor((z-startPosZ)/binDepth));

    size_t binsToAddY = 0;
    if(y < startPosY && expand)
    {
        binsToAddY = static_cast<size_t>(ceil((startPosY-y)/binHeight));
        startPosY -= binHeight*binsToAddY;
    }
    size_t binY = static_cast<size_t>(floor((y-startPosY)/binHeight));

    size_t binsToAddX = 0;
    if(x < startPosX && expand)
    {
        binsToAddX = static_cast<size_t>(ceil((startPosX-x)/binWidth));
        startPosX -= binWidth*binsToAddX;
    }
    size_t binX = static_cast<size_t>(floor((x-startPosX)/binWidth));

    std::vector<double> row(binX+1, 0.0);
    std::vector<std::vector<double>> yx_field(binY+1, row);

    if(binsToAddZ > 0)
        field.insert(field.begin(), binsToAddZ, yx_field);

    for(size_t iz = binsToAddZ; iz < field.size(); iz++)
    {
        if(binsToAddY > 0)
            field.at(iz).insert(field.at(iz).begin(), binsToAddY, row);

        for(size_t iy = binsToAddY; iy < field.at(iz).size(); iy++)
        {
            field.at(iz).at(iy).insert(field.at(iz).at(iy).begin(), binsToAddX, 0);
            field.at(iz).at(iy).resize(binX+1, 0);
        }

        if(binY >= field.at(iz).size() && expand)
            field.at(iz).resize(binY+1, row);
    }

    if(binZ >= field.size() && expand)
        field.resize(binZ+1, yx_field);


    if(binZ < field.size() && binY < field.at(binZ).size() && binX < field.at(binZ).at(binY).size())
        field.at(binZ).at(binY).at(binX) += c;
    else
    {
        std::cout << "Hist3::add error: index < size: " << binZ << " " << binY << " " << binX <<"\t"
                  << field.size() << " " << field.at(binZ).size() << " " << field.at(binZ).at(binY).size() << std::endl;
        return false;
    }
*/

    return true;
}
/*
size_t nucmath::Hist3::nBins() const
{
    return histmap.size();
}
*/

/*
std::tuple<double, double, double, double> nucmath::Hist3::data(size_t bin) const
{
    if(histmap.size() == 0)
        throw std::invalid_argument("Hist3::data(size_t bin): the histogram is empty");
    if(histmap.size() <= bin)
        throw std::invalid_argument("Hist3::data(size_t bin): requested bin is outside the data field.");

    std::map<std::tuple<double, double, double>, double>::iterator it = histmap.begin();
    it += bin;

    return {it->first, it->second};
*/
/*
    if(field.size() == 0 || field.at(0).size() == 0 || field.at(0).at(0).size() == 0)
        throw std::invalid_argument("Hist3::data(size_t bin): the histogram is empty");
    if(field.size()*field.at(0).size()*field.at(0).at(0).size() <= bin)
        throw std::invalid_argument("Hist3::data(size_t bin): requested bin is outside the data field.");

    size_t xLen = field.at(0).at(0).size();
    size_t yLen = field.at(0).size();
    size_t zLen = field.size();
    size_t zBin = floor(bin /(xLen*yLen));
    size_t yBin = floor((bin - zBin*xLen*yLen) / xLen);
    size_t xBin = (bin - zBin*xLen*yLen - yBin*xLen)/xLen;

    return std::tuple<double, double, double, double>(startPosX+(xBin+0.5)*binWidth,
                                              startPosY+(yBin+0.5)*binHeight,
                                              startPosZ+(zBin+0.5)*binDepth,
                                              field.at(zBin).at(yBin).at(xBin));*/
/*
}
*/
bool nucmath::Hist3::isChanged(bool leaveChanged)
{
    bool temp = changed;
    changed = (leaveChanged == true) ? true : false;
    return temp;
}

std::tuple<double, double, double, double> nucmath::Hist3::binToData(std::map<std::tuple<double, double, double>, double>::iterator it) const
{
    const auto& [bx, by, bz] = it->first;
    return {(bx + 0.5) * binWidth, (by + 0.5) * binHeight, (bz + 0.5) * binDepth, it->second};
}

std::tuple<double, double, double, double, double, double> nucmath::Hist3::getRange() const
{
    return {startPosX, endPosX, startPosY, endPosY, startPosZ, endPosZ};
}
