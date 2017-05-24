

#include "datatable.h"


#pragma once


namespace nucmath
{
	enum InterpolationMode { Linear = 0, Steffen };

	class Interpolation
	{
	public:
		Interpolation();
		virtual ~Interpolation();

		void interpolate(InterpolationMode mode);

        void setData(const DataTable<2>& data);
		void setData(const std::vector<double>& data);

	private:

        DataTable<2> m_dataTable;
		std::vector<std::pair<double, double>> m_data;

		InterpolationMode m_mode;


		std::vector<std::pair<double,double>> LinearInterpolation();

		std::vector<std::pair<double,double>> SteffenInterpolation();
	};

}
