
#include "DataTable.h"


using namespace std;

using namespace nucmath;


nucmath::DataTable::DataTable()
{

	clear();
}

DataTable::DataTable(size_t channels)
{ // TODO: !!
/*	for(size_t i = 0; i < channels; i++)
		addData(TableRow());

	clear();
	*/

}

DataTable::DataTable(const std::vector<TableRow>& dataTable )
{
	for(size_t i = 0; i < dataTable.size(); i++)
	{
        addRow(dataTable[i]);
	}

	for(size_t i = 0; i < numberOfColumns; i++) { calcMaxMin(i); }

	m_dataTableOriginal = m_dataTable;
}

DataTable::~DataTable()
{


}

void DataTable::addRow(const TableRow& row)
{
    this->m_dataTable.push_back(row);
}

void DataTable::setSize(size_t channels)
{
	if(channels >this->m_dataTable.size())
	{
		size_t len = channels - this->m_dataTable.size();
		for(size_t i = 0; i < len; i++)
            addRow(TableRow());
	}
}

void nucmath::DataTable::clear()
{
/*	for(size_t i = 0; i < m_dataTable.size(); i++)
		for(size_t k = 0; k < numberOfColumns; k++)
		{
			m_dataTable[i].x[k] = 0.0;
		}*/

	m_dataTable.clear();

	for(size_t i = 0; i < numberOfColumns; ++i)
	{
		m_columnProp[i].max = -10e30;
		m_columnProp[i].min = 10e10;
	}
}

bool DataTable::load(const std::string& file, SPECTRUMFORMAT format)
{
	std::ifstream in(file);

	if(in.is_open() == false)
	{
		cout << "Datei konnte nicht geöffnet werden" << endl;
		//cin.get();
		return false;
	}

	size_t line = 0;


	// read first line for file format
	char buf[255];
	std::string strBuf;
	in.get(buf, 250,'\n');
	strBuf = buf;

	if(strBuf.find("Format:") != string::npos)
	{
		// TODO: Formaterkennung fehlt

		if(strBuf.find("MathematicaTable2"))
		{
			format = SPECTRUMFORMAT::MathematicaTable2;
		}

	}
	else
	{
		in.seekg(0);

        // TODO: regexpattern scheint fehlerhaft zu sein
        /*

		// example: "{50.4, 324398.87053571426}";
        std::string regexPattern = ("^( )*[{]{1}( )*[0-9]+(?:\\.[0-9]*)?( )*,( )*[0-9]+(?:\\.[0-9]*)?( \)*[}]{1}");

		std::regex rx(regexPattern, std::regex_constants::basic);
		if(std::regex_match(strBuf, rx))
		{
			format = SPECTRUMFORMAT::MathematicaTable2;
		}
        */
	}


	if(format == SPECTRUMFORMAT::OneColumn)
	{
		while(!in.eof())
		{
			char stringWithData[32];
			in.getline(stringWithData, 30, '\n');
			const double dataValue = atof(stringWithData);

			size_t s = m_dataTable.size();
			if(line >= s)
			{
				for(size_t i = 0; i < (line - s + 1); i++)
				{
					m_dataTable.push_back(TableRow());
				}
			}

			m_dataTable[line].x[0] = line;
			m_dataTable[line].x[1] = dataValue;
			line++;
		}
	}
	else if(format == SPECTRUMFORMAT::TwoColumn)
	{
		while(!in.eof())
		{
			char stringWithData[252];
			std::string str;

			in.getline(stringWithData, 250, '\n');
			str = stringWithData;

			std::vector<char> delims =  {' ','\t'};
			std::vector<std::string> tokens =  tokenize(str,delims);

			if(tokens.size() < 2)
				continue;

			const double dataValue1 = atof(tokens.at(0).c_str());
			const double dataValue2 = atof(tokens.at(1).c_str());

			size_t s = m_dataTable.size();
			if(line >= s)
			{
				for(size_t i = 0; i < (line - s + 1); i++)
				{
					m_dataTable.push_back(TableRow());
				}
			}

			m_dataTable[line].x[0] = dataValue1;
			m_dataTable[line].x[1] = dataValue2;
			line++;
		}
	}
	else if(format == SPECTRUMFORMAT::MathematicaTable2)
	{
		in.seekg(0);

		while(!in.eof())
		{
			char stringWithData[252];
			std::string str;

			in.get(stringWithData, 250, ',');
			str = stringWithData;
			str.erase(0,1);
			const double dataValue1 = atof(str.c_str());

			in.getline(stringWithData, 250, '\n');
			str = stringWithData;
			str.erase(0,1);
			str.erase(str.length()-1,1);
			const double dataValue2 = atof(str.c_str());

			size_t s = m_dataTable.size();
			if(line >= s)
			{
				for(size_t i = 0; i < (line - s + 1); i++)
				{
					m_dataTable.push_back(TableRow());
				}
			}

			m_dataTable[line].x[0] = dataValue1;
			m_dataTable[line].x[1] = dataValue2;
			line++;
		}


	}
	else
	{
		in.close();
		return false;
	}


	in.close();

	calcMaxMin();

	m_dataTableOriginal = m_dataTable;

	return true;
}

bool DataTable::save(const string &file)
{
	std::ofstream out(file, ios_base::out);

	if(out.is_open())
	{
		for(size_t i = 0; i < m_dataTable.size(); ++i)
		{
			for(size_t k = 0; k < numberOfColumns; ++k)
			{
				out << m_dataTable[i].x[k];

				if(k+1 != numberOfColumns)
					out<< " ";
			}

			if(i+1 != m_dataTable.size())
				out<< std::endl;
		}
	}

	out.close();

	return true;
}


void DataTable::calcMaxMin()
{
		for(size_t column = 0; column < numberOfColumns; column++)
		{
			calcMaxMin(column);
		}
}

void DataTable::calcMaxMin(size_t column)
{

		for(size_t i = 0; i < m_dataTable.size(); ++i)
		{
			m_columnProp[column].max = std::max<double>(m_columnProp[column].max, m_dataTable[i].x[column]);
			m_columnProp[column].min = std::min<double>(m_columnProp[column].min, m_dataTable[i].x[column]);
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


void DataTable::multiply(double factor, size_t dataIndx)
{
	for(size_t i = 0; i < m_dataTable.size(); ++i)
	{
			this->m_dataTable[i].x[dataIndx] *=factor;
	}
}

void DataTable::add(double shift, size_t column)
{
	for(size_t i = 0; i < m_dataTable.size(); ++i)
	{
        this->m_dataTable[i].x[column]+=shift;
	}
}

double DataTable::calculateSumOfSquares(const DataTable& hist, size_t dataIndx) const
{
	return calculateSumOfSquares(hist, dataIndx, getMin(0), getMax(0));
}

double DataTable::calculateSumOfSquares(const DataTable& modelHist, size_t dataIndx, double beginCh, double endCh) const
{
	double res = 0.0;

	auto modelData = modelHist.getData();
	size_t len = std::min<size_t>(this->m_dataTable.size(), modelData.size());
	size_t k = 0;
	for(size_t i = 0; i < len; ++i)
	{
		double v = this->m_dataTable[i].x[0];

		if(v >= beginCh && v < endCh)
		{
			const double y = this->m_dataTable[i].x[dataIndx];
			const double model_y = modelData[i].x[dataIndx];

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
	folded.clear();
	folded.setSize(len);


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

		double integr = 0.0, integrSum = 0.0;

		for(int k = -kernelWidth2; k <= kernelWidth2; k++)
		{
			if(mirrorMode)
			{
				const size_t indx = std::abs(static_cast<int>(ch)+k);

				// das array erweitern, falls zu klein
				folded.setSize(indx+1);

				const double center = static_cast<double>(k);
				integr = normalDistIntegral(0, sigma, center-0.5, center+0.5);
				folded.getData().at(indx).x[channel] += m_dataTable[ch].x[channel]*integr;
				folded.getData().at(indx).x[0] = indx;
			}
			else
			{
				const int indx = static_cast<int>(ch)+k;
				if(indx >=0)
				{
					// das array erweitern, falls zu klein
					folded.setSize(indx+1);

					const double center = static_cast<double>(k);
					integr = normalDistIntegral(0, sigma, center-0.5, center+0.5);
					folded.getData().at(indx).x[channel] += m_dataTable[ch].x[channel]*integr;
					folded.getData().at(indx).x[0] = indx;
				}
			}


			//integrSum += integr;
		}

		//std::cout << integrSum << std::endl;

	}

	for(int j = 0; j < numberOfColumns; j++)
		folded.calcMaxMin(j);

//	std::cout << " Max: " << folded.getMax(1) << std::endl;

//	std::cout << "Faltung abgeschlossen." << std::endl;
}

const std::vector<TableRow> DataTable::getData(double binWidth, size_t rowIndxToResample, double oldBinWidth) const
{
	DataTable temp = *this;
	temp.sort(rowIndxToResample);
	std::vector<TableRow>& dat = temp.getData();

	if(dat.size() < 2)
		return dat;

	// neue Tabelle erstellen und speicher reservieren
	std::vector<TableRow> resampledData;
	const double startValue = dat[0].x[rowIndxToResample];
	size_t newLength = static_cast<size_t>(floor(static_cast<double>(dat[dat.size()-1].x[rowIndxToResample]-startValue)/binWidth))+2;
	resampledData.reserve(newLength);
	for(size_t i = 0; i <newLength; ++i) { resampledData.push_back(TableRow());}

	double xBin = 0.0;
	if(oldBinWidth > 0.0)
		xBin = oldBinWidth;

	size_t ch = 0;
	for(size_t i = 0; i < dat.size(); ++i)
	{
		TableRow row = dat[i];
		double x = dat[i].x[rowIndxToResample];
	   // double xBin = 1.0;

		// das alte Bin passt komplett in das neue Bin rein -> übernehmen
		if(startValue+(ch-0.0000)*binWidth <= x && x+xBin < startValue+(ch+1)*binWidth)
		{
			resampledData.at(ch) += row;
			resampledData.at(ch).x[rowIndxToResample] = startValue+ch;
		}
		else // das alte Bin muss auf zwei neue aufgeteilt werden
		{
			double fraction = (x+xBin-ch*binWidth-binWidth)/xBin;

			resampledData.at(ch) += row*(1.0-fraction);
			resampledData.at(ch).x[rowIndxToResample] = startValue+ch;

			ch++;

			resampledData.at(ch) += row*fraction;
			resampledData.at(ch).x[rowIndxToResample] = startValue+ch;
		}
	}

	return resampledData;
}

/*
Hist2d DataTable::meanHist(double binWidth, size_t columnToRebin, size_t columnWithValues, double oldBinWidth)
{
	DataTable temp = *this;
	temp.sort(columnToRebin);
	std::vector<TableRow>& dat = temp.getData();

	Hist2d hist;

	if(dat.size() < 2)
		return hist;

	const double startValue = dat[0].x[columnToRebin];
	size_t newLength = static_cast<size_t>(floor(static_cast<double>(dat[dat.size()-1].x[columnToRebin]-startValue)/binWidth))+1;
	for(size_t i = 0; i <newLength; ++i) { hist.data().push_back(0.0);}


	hist.init(startValue, binWidth);

	double xBin = 0.0000;
	if(oldBinWidth > 0.0)
		xBin = oldBinWidth;

	size_t ch = 0, last_ch = 0;
	double counter = 0.0;
	for(size_t i = 0; i < dat.size(); ++i)
	{
		double x = dat[i].x[columnToRebin];
		double y = dat[i].x[columnWithValues];

		ch = static_cast<size_t>(floor((x-startValue)/binWidth));

		if(ch != last_ch && i != 0)
		{
			hist.data().at(last_ch) /= counter;
			counter = 0.0;
		}

		hist.data().at(ch) += y;
		counter++;

		last_ch = ch;
	}

	if(counter > 0.0)
	{
		hist.data().at(ch) /= counter;
	}


	return hist;


}
*/


double DataTable::getSum(size_t column)
{
	double result = 0.0;
	for(size_t i = 0; i < m_dataTable.size(); ++i)
	{
		result+= m_dataTable[i].x[column];
	}

	return result;
}


/*
void Histogram::setNumberOfBins(size_t number)
{
	if(binSize < 0)
	{
		double mi = getMin(1);
		double ma = getMax(1);

		binSize = (ma-0)/number;

		std::vector<TableRow> result;
		TableRow dataStruct;
		size_t bin = 0;

		for(size_t i = 0, j=0; i < data.size(); ++i,++j)
		{
			auto v = data[i].x[1];
			auto v0 = data[i].x[0];

			if(v0 >= bin*binSize+binSize)
			{
				dataStruct.x[0] = bin;
				result.push_back(dataStruct);
				dataStruct = TableRow();
				bin++;
			}

			dataStruct.x[1] += v;

			
		}
	}

}*/
