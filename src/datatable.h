/*
    @brief Container for tables with double values


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

    enum class DATATABLEFORMAT { _Unknown, TryAutomatic, OneColumn, TwoColumn, MathematicaTable2 };

    /**
     *  Definition of a row inside the table
     */
    template<size_t nColumns>
	class TableRow
	{
	public:
        TableRow() { for(size_t i = 0; i < nColumns; i++) x[i] = 0.0; }
        TableRow(const TableRow& data) { for (size_t i = 0; i < nColumns; i++) { this->x[i] = data.x[i]; } }

        double x[nColumns];

        /**
         * @brief Return the row values as a string.
         * @return
         */
        std::string str()
		{
			std::string s = "";
            for (size_t i = 0; i < nColumns; i++)
			{
                s.append(std::to_string(x[i]).c_str()).append("\t");
			}
			return s;
		}

        friend std::ostream& operator << (std::ostream& os, const TableRow& tablerow)
		{
            for (size_t i = 0; i < nColumns; i++)
			{
                os << tablerow.x[i] << "\t";
			}
            return os;
		}


        /**
         * @brief multiply all elements inside the row with a @param factor.
         * @param factor
         * @return
         */
		TableRow operator*(double factor)
		{
			TableRow result;
            for (size_t i = 0; i < nColumns; i++)
			{
				result.x[i] = x[i] * factor;
			}
			return result;
		}

		TableRow & operator+=(const TableRow& row)
		{
            for (size_t i = 0; i < nColumns; i++)
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


    template<size_t nColumns>
	class DataTable
	{
	private:

        typedef TableRow<nColumns> TABLEROW;

        std::vector<TABLEROW> m_dataTableOriginal;
        std::vector<TABLEROW> m_dataTable;
        std::array<ColumnProperties, nColumns> m_columnProp;

	public:
		DataTable();
        DataTable(const std::vector<TABLEROW>& dataTable);
		DataTable(size_t columns);

		~DataTable();

        bool load(const std::string &file, DATATABLEFORMAT format);
        bool save(const std::string &file);



        void addRow(const TABLEROW &row);



		void multiply(double factor, size_t column);
        void add(double shift, size_t column);


		void setSize(size_t channels);
		void clear();

		// Faltet die Daten mit der Normalverteilung
		void fold(DataTable& folded, float resolution, size_t column) const;



        double calcSumOfSquares(const DataTable& hist, size_t column) const;
        double calcSumOfSquares(const DataTable& hist, size_t column, double beginCh, double endCh) const;


		double getMax(size_t column) const;
		double getMin(size_t column) const;

		void calcMaxMin();
		void calcMaxMin(size_t column);

		void sort(size_t column);

    //    void getHist(size_t columnToRebin, size_t columnWithValues, double binWidth,  Hist2d& hist);

		double getSum(size_t column);

	public:
        const std::vector<TABLEROW>& getData() const { return m_dataTable; };
        std::vector<TABLEROW>& getData() { return m_dataTable; };

        const std::vector<TABLEROW> getData(double binSize, size_t rowIndxToResample, double oldBinWidth = -1.0) const;

	private:


        /**
         * @brief Comparator for the std::sort function.
         */
		struct sort_comparator
		{
			sort_comparator(size_t column){ this->column = column; }
            bool operator()(const TABLEROW &a, const TABLEROW &b) { return (a.x[column] < b.x[column]); }
			size_t column;
		};
	};

}
