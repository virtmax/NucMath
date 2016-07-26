/*

  Datencontainer für Tabellenform.




 */



#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <regex>
#include <array>
#include <cmath>
#include <math.h>


#include "constants.h"
#include "integration.h"
#include "stringoperations.h"


#pragma once

namespace nucmath
{

	enum class SPECTRUMFORMAT { _Unknown, TryAutomatic, OneColumn, TwoColumn, MathematicaTable2 };


	const static size_t numberOfColumns = 5;

	class TableRow
	{
	public:
		TableRow() { for (size_t i = 0; i < numberOfColumns; i++) x[i] = 0.0; }
		TableRow(const TableRow& data) { for (size_t i = 0; i < numberOfColumns; i++) { this->x[i] = data.x[i]; } }
		//int nr;
		double x[numberOfColumns];

		std::string str()
		{
			std::string s = "";
			for (size_t i = 0; i < numberOfColumns; i++)
			{
				s.append(std::to_string(x[i]).c_str()).append("  ");
			}
			return s;
		}

		friend std::ostream& operator << (std::ostream& os, const TableRow& data)
		{
			for (size_t i = 0; i < numberOfColumns; i++)
			{
				os << data.x[i] << "  ";
			}
			return os;     // Ref. auf Stream
		}

		TableRow operator*(double factor)
		{
			TableRow result;
			for (size_t i = 0; i < numberOfColumns; i++)
			{
				result.x[i] = x[i] * factor;
			}
			return result;
		}

		TableRow & operator+=(const TableRow& row)
		{
			for (size_t i = 0; i < numberOfColumns; i++)
			{
				x[i] += row.x[i];
			}

			return *this;
		}
	};


	class ColumnProperties
	{
	public:
		std::string title;

		double binWidth;

		double min;
		double max;
	};

	class DataTable
	{

	public:



	private:
		std::vector<TableRow> m_dataTableOriginal;
		std::vector<TableRow> m_dataTable;
		std::array<ColumnProperties, numberOfColumns> m_columnProp;

	public:
		DataTable();
		DataTable(const std::vector<TableRow>& dataTable);
		DataTable(size_t columns);

		~DataTable();

		bool load(const std::string &file, SPECTRUMFORMAT format);
		bool save(const std::string& file);



        void addRow(const TableRow& row);



		void multiply(double factor, size_t column);
        void add(double shift, size_t column);


		void setSize(size_t channels);
		void clear();

		// Faltet die Daten mit der Normalverteilung
		void fold(DataTable& folded, float resolution, size_t column) const;



		double calculateSumOfSquares(const DataTable& hist, size_t column) const;
		double calculateSumOfSquares(const DataTable& hist, size_t column, double beginCh, double endCh) const;


		double getMax(size_t column) const;
		double getMin(size_t column) const;

		void calcMaxMin();
		void calcMaxMin(size_t column);

		void sort(size_t column);

    //    void getHist(size_t columnToRebin, size_t columnWithValues, double binWidth,  Hist2d& hist);

		double getSum(size_t column);

	public:
		const std::vector<TableRow>& getData() const { return m_dataTable; };
		std::vector<TableRow>& getData() { return m_dataTable; };

		const std::vector<TableRow> getData(double binSize, size_t rowIndxToResample, double oldBinWidth = -1.0) const;

	private:


		// ist nötig für den Vergleich von Werten mit STL
		struct sort_comparator
		{
			sort_comparator(size_t column){ this->column = column; }
			bool operator()(const TableRow &a, const TableRow &b) { return (a.x[column] < b.x[column]); }
			size_t column;
		};
	};

}
