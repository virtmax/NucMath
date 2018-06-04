
#pragma once

#include <algorithm>
#include <array>
#include <vector>
#include <stdexcept>

#include "utils.h"
#include "vector.h"



namespace nucmath
{

template <typename T>
class Matrix
{
public:
    Matrix(size_t nRows, size_t nColumns);
    Matrix(size_t nRows, size_t nColumns, const std::vector<T>& elements);
public:

    size_t rows() const;
    size_t columns() const;

    const T& operator() (size_t row, size_t column) const;
    T& operator() (size_t row, size_t column);

    const Matrix operator * (const Matrix& mat) const;
    Vector<T> operator * (const Vector<T>& vec) const;

    bool operator == (const Matrix& mat) const;
    bool operator != (const Matrix& mat) const;

    std::string str() const;

private:
    size_t nRows;
    size_t nColumns;

    std::vector<double> data;
};
}

template<typename T>
nucmath::Matrix<T>::Matrix(size_t nRows, size_t nColumns)
{
    this->nRows = nRows;
    this->nColumns = nColumns;
    data.assign(nColumns*nRows, 0.0);
}

template<typename T>
nucmath::Matrix<T>::Matrix(size_t nRows, size_t nColumns, const std::vector<T>& elements)
{
    if(elements.size() != nColumns*nRows)
        throw std::invalid_argument("Matrix(std::vector<T> elements): elements length not enough to fill the matrix.");

    this->nRows = nRows;
    this->nColumns = nColumns;

    data = elements;
}

template<typename T>
size_t nucmath::Matrix<T>::rows() const
{
    return nRows;
}

template<typename T>
size_t nucmath::Matrix<T>::columns() const
{
    return nColumns;
}

template<typename T>
const T& nucmath::Matrix<T>::operator() (size_t row, size_t column) const
{
    return data[row*nColumns + column];
}

template<typename T>
T& nucmath::Matrix<T>::operator() (size_t row, size_t column)
{
    return data[row*nColumns + column];
}

template<typename T>
const nucmath::Matrix<T> nucmath::Matrix<T>::operator * (const Matrix& mat) const
{
    if(nColumns != mat.rows())
        throw std::invalid_argument("Matrix<T>::operator == (const Matrix& mat): mat has wrong dimensions.");

    Matrix<T> matC(nRows, mat.columns());

    for(size_t i = 0; i < nRows; i++)
    {
        for(size_t j = 0; j < nColumns; j++)
        {
            for(size_t k = 0; k < mat.columns(); k++)
            {
                matC(i,k) += this->operator()(i,j) * mat(j,k);
            }
        }
    }

    return matC;
}

template<typename T>
nucmath::Vector<T> nucmath::Matrix<T>::operator * (const Vector<T>& vec) const
{
    if(nColumns != vec.dim())
        throw std::invalid_argument("Matrix<T>::operator * (const Vector<T>& vec): Vector dimension should be equal to the number of matrix columns.");

    nucmath::Vector<T> res(nRows);

    for(size_t i = 0; i < nRows; i++)
    {
        for(size_t j = 0; j < nColumns; j++)
        {
            res(i) += this->operator()(i,j) * vec(j);
        }
    }

    return res;
}

template<typename T>
bool nucmath::Matrix<T>::operator == (const Matrix& mat) const
{
    if(mat.columns() != nColumns || mat.rows() != nRows)
        throw std::invalid_argument("Matrix<T>::operator == (const Matrix& mat): mat has wrong dimensions.");

    for(size_t i = 0; i < nRows; i++)
    {
        for(size_t j = 0; j < nColumns; j++)
        {
            if(!nucmath::isEqual(mat(i,j), this->operator()(i,j)))
                return false;
        }
    }

    return true;
}

template<typename T>
bool nucmath::Matrix<T>::operator != (const Matrix& mat) const
{
    if(mat.columns() != nColumns || mat.rows() != nRows)
        throw std::invalid_argument("Matrix<T>::operator != (const Matrix& mat): mat has wrong dimensions.");

    return !this->operator==(mat);
}

template<typename T>
std::string nucmath::Matrix<T>::str() const
{
    std::string s = "";
    for(size_t i = 0; i < nRows; i++)
    {
        s.append("(");
        for(size_t i = 0; i < nColumns; i++)
        {
            if(i+1 != nColumns)
                s.append("\t");

        }
        s.append(std::to_string(data[i]));
    }
    return s;
}
