
#include "datatable.h"

using namespace nucmath;


nucmath::DataTable::DataTable(size_t columns)
{
    reset();
    setNumOfColumns(columns);
}

DataTable::DataTable(const std::vector<TableRow> &dataTable)
{
    reset();

    if(dataTable.size() != 0)
        setNumOfColumns(dataTable.at(0).getNumOfColumns());

    for(size_t i = 0; i < dataTable.size(); i++)
    {
        addRow(dataTable[i]);
    }

    for(size_t i = 0; i < nColumns; i++)
    {
        calcMaxMin(i);
    }

    m_dataTableOriginal = m_dataTable;
}


DataTable::~DataTable()
{


}


void DataTable::addRow(const TableRow& row)
{
    if(row.getNumOfColumns() != nColumns)
        throw std::invalid_argument("DataTable::addRow(const TableRow& row): row.size() != columns");

    m_dataTable.push_back(row);

    updateStatistics(m_dataTable.size()-1);
}


void DataTable::addRow(const std::vector<double> &row)
{
    if(row.size() != nColumns)
        throw std::invalid_argument("DataTable::addRow(const std::vector<double> &row): row.size() != columns");

    m_dataTable.push_back(row);

    updateStatistics(m_dataTable.size()-1);
}

void DataTable::setNumOfColumns(size_t columns, double defaultValue)
{
    nColumns = columns;
    for(size_t i = 0; i < m_dataTable.size(); i++)
    {
        m_dataTable[i].setNumOfColumns(columns, defaultValue);
    }

    for(size_t i = 0; i < nColumns; ++i)
    {
        if(m_columnProp.size() <= i)
            m_columnProp.push_back(ColumnProperties());

        m_columnProp[i].max = std::numeric_limits<double>::max();
        m_columnProp[i].min = std::numeric_limits<double>::min();
    }
}

void DataTable::setNumOfRows(size_t rows, double defaultValue)
{
    if(rows > m_dataTable.size())
    {
        size_t len = rows - m_dataTable.size();
        for(size_t i = 0; i < len; i++)
            addRow(TableRow(nColumns, defaultValue));
    }
}


void nucmath::DataTable::reset()
{
    format = DATATABLEFORMAT::_Unknown;
    m_dataTable.clear();
    m_columnProp.clear();
    changed = true;
    header = "";
    nColumns = 0;
}

void DataTable::setHeader(std::string str)
{
    header = str;
}

bool DataTable::load(const std::string& file)
{
    std::ifstream in(file);

    if(!in.is_open())
        throw std::invalid_argument("Can't open '" + file + "'.");

    size_t lineNr = 0;

    // table with (white space, commas, | ) delimiters between columns. one line = one row.
    const std::regex patternStandardTable("^\\s*(([-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?)(\\s*[\\|,]?\\s*))+$");

    // example: "{50.4, 3.426, 398.8}"
    //const std::regex paternMathematicaTable2("^( )*[{]{1}( )*[0-9]+(?:\\.[0-9]*)?( )*,( )*[0-9]+(?:\\.[0-9]*)?( \\)*[}]{1}");

    std::vector<std::string> tokens;
    std::vector<double> row;
    std::smatch match;
    std::string line = "";
    while(!in.eof())
    {
        std::getline(in, line);

        line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));   // trim leading whitespaces

        if(line.size() == 0 || line.at(0) == '#' || line.at(0) == '-' || line.at(0) == '_')
            continue;

        if(format == DATATABLEFORMAT::_Unknown)
        {
            if(std::regex_search(line, match, patternStandardTable))
            {
                format = DATATABLEFORMAT::Standard;
                tokens = tokenize(line, {' ', '\t', '|', ','});

                row.clear();
                for(const auto& token : tokens)
                {
                    row.push_back(std::atof(token.c_str()));
                }
                setNumOfColumns(row.size(), 0.0);
                addRow(row);
            }
            else if(std::regex_search(line, match, patternStandardTable))
            {
                format = DATATABLEFORMAT::MathematicaTable2;
            }
            else
                format = DATATABLEFORMAT::_Unknown;


        }
        else
        {
            switch(format)
            {
            case DATATABLEFORMAT::Standard:
                tokens = tokenize(line, {' ', '\t', '|', ','});
                row.clear();
                for(const auto& token : tokens)
                {
                    row.push_back(std::atof(token.c_str()));
                }
                addRow(row);
                break;
            default:
                break;
            }
        }


        lineNr++;
    }

    in.close();

    m_dataTableOriginal = m_dataTable;
    calcMaxMin();

    return true;
}


bool DataTable::save(const std::string &file)
{
    std::ofstream out(file, std::ios_base::out);

    if(out.is_open())
    {
        if(!header.empty())
        {
            const auto& tokens = tokenize(header, {'\n'});
            for(const auto& token : tokens)
            {
                out << "# " << token << std::endl;
            }
        }

        for(size_t i = 0; i < m_dataTable.size(); ++i)
        {
            for(size_t k = 0; k < nColumns; ++k)
            {
                out << m_dataTable[i].data[k];

                if(k+1 != nColumns)
                    out << " ";
            }

            if(i+1 != m_dataTable.size())
                out << std::endl;
        }
    }

    out.close();

    return true;
}



void DataTable::calcMaxMin()
{
    for(size_t i = 0; i < nColumns; i++)
    {
        calcMaxMin(i);
    }
}


void DataTable::calcMaxMin(size_t column)
{
    for(size_t i = 0; i < m_dataTable.size(); ++i)
    {
        m_columnProp[column].max = std::max<double>(m_columnProp[column].max, m_dataTable[i].data[column]);
        m_columnProp[column].min = std::min<double>(m_columnProp[column].min, m_dataTable[i].data[column]);
    }
}


void DataTable::sort(size_t channel)
{
    std::sort(m_dataTable.begin(), m_dataTable.end(), sort_comparator(channel));
}

double DataTable::getMax(size_t column) const
{ 
    return m_columnProp[column].max;
}

double DataTable::getMin(size_t column) const
{ 
    return m_columnProp[column].min;
}

void DataTable::mul(double factor, size_t dataIndx)
{
    for(size_t i = 0; i < m_dataTable.size(); ++i)
    {
        m_dataTable[i].data[dataIndx] *=factor;
    }
}

void DataTable::add(double shift, size_t column)
{
    for(size_t i = 0; i < m_dataTable.size(); ++i)
    {
        m_dataTable[i].data[column]+=shift;
    }
}

double DataTable::calcSumOfSquares(const DataTable& hist, size_t dataIndx) const
{
    return calcSumOfSquares(hist, dataIndx, getMin(0), getMax(0));
}

double DataTable::calcSumOfSquares(const DataTable& modelHist, size_t dataIndx, double beginCh, double endCh) const
{
    double res = 0.0;

    auto modelData = modelHist.getData();
    size_t len = std::min<size_t>(this->m_dataTable.size(), modelData.size());
    size_t k = 0;
    for(size_t i = 0; i < len; ++i)
    {
        double v = this->m_dataTable[i].data[0];

        if(v >= beginCh && v < endCh)
        {
            const double y = this->m_dataTable[i].data[dataIndx];
            const double model_y = modelData[i].data[dataIndx];

            // sigma^2 = (sqrt(sigma1^2 + sigma2^2))^2
            // sigma^2 = sigma1^2 + sigma2^2
            // sigma1 = sqrt(y)
            // sigma^2 = y + model_y
            double sigma_square = fabs(y) + fabs(model_y);
            //  sigma_square = 0;

            sigma_square = y;

            if(sigma_square > 0)
            {
                res += (y-model_y)*(y-model_y)/sigma_square;
            }
            else
                res += (y-model_y)*(y-model_y);

            k++;
        }
        else if(v >= endCh)
            break;
    }

    if(k != 1)
        return res/(k-1);
    else
        return 0;
}

void DataTable::fold(DataTable& folded, float resolution, size_t channel) const
{

    const bool mirrorMode = false;

    const size_t len = m_dataTable.size();
    folded.reset();
    folded.setNumOfRows(len);


    //	std::cout << "Faltung beginnt..." << std::endl;
    //	std::cout << "Aufloesung: " << resolution << std::endl;

    double progress = 0.00;
    for(size_t ch = 0; ch < len; ch++)
    {
        if((double)ch/(double)len > progress)
        {
            //	std::cout << "Kanal: "<< ch << "/"<< len << "   " << progress*100 <<" %"<< std::endl;
            progress += 0.05;
        }

        const double fwhm = (ch+1)*resolution;
        const double sigma = fwhm*FWHM2SIGMA;
        const int kernelWidth2 = static_cast<int>(ceil(sigma*3)+1);

        double integr = 0.0;

        for(int k = -kernelWidth2; k <= kernelWidth2; k++)
        {
            if(mirrorMode)
            {
                const size_t indx = std::abs(static_cast<int>(ch)+k);

                // das array erweitern, falls zu klein
                folded.setNumOfRows(indx+1);

                const double center = static_cast<double>(k);
                integr = normalDistIntegral(0, sigma, center-0.5, center+0.5);
                folded.getData().at(indx).data[channel] += m_dataTable[ch].data[channel]*integr;
                folded.getData().at(indx).data[0] = indx;
            }
            else
            {
                const int indx = static_cast<int>(ch)+k;
                if(indx >=0)
                {
                    // das array erweitern, falls zu klein
                    folded.setNumOfRows(indx+1);

                    const double center = static_cast<double>(k);
                    integr = normalDistIntegral(0, sigma, center-0.5, center+0.5);
                    folded.getData().at(indx).data[channel] += m_dataTable[ch].data[channel]*integr;
                    folded.getData().at(indx).data[0] = indx;
                }
            }


            //integrSum += integr;
        }

        //std::cout << integrSum << std::endl;

    }

    for(int j = 0; j < nColumns; j++)
        folded.calcMaxMin(j);

    //	std::cout << " Max: " << folded.getMax(1) << std::endl;

    //	std::cout << "Faltung abgeschlossen." << std::endl;
}

double DataTable::getSum(size_t column)
{
    double result = 0.0;
    for(size_t i = 0; i < m_dataTable.size(); ++i)
    {
        result+= m_dataTable[i].data[column];
    }

    return result;
}

const nucmath::TableRow& nucmath::DataTable::operator[](size_t row) const
{
    if(row >= m_dataTable.size())
        throw std::invalid_argument("DataTable::operator[]: index >= rows");

    return m_dataTable[row];
}

nucmath::TableRow& nucmath::DataTable::operator[](size_t row)
{
    if(row >= m_dataTable.size())
        throw std::invalid_argument("DataTable::operator[]: index >= rows");

    return m_dataTable[row];
}

void nucmath::DataTable::updateStatistics(size_t newRowIndex)
{
    for(size_t i = 0; i < nColumns; i++)
    {
        m_columnProp[i].max = std::max<double>(m_columnProp[i].max, m_dataTable[newRowIndex].data[i]);
        m_columnProp[i].min = std::min<double>(m_columnProp[i].min, m_dataTable[newRowIndex].data[i]);
    }
}
