
#pragma once

#include <algorithm>
#include <array>
#include <vector>
#include <string>
#include <stdexcept>

#include "utils.h"
#include "vector.h"



namespace nucmath
{

template <typename T>
class Matrix
{
public:
    Matrix();
    Matrix(size_t nRows, size_t nColumns);
    Matrix(size_t nRows, size_t nColumns, const std::vector<T>& elements);
    Matrix(const std::vector<std::vector<T>>& elements);
public:

    size_t rows() const;
    size_t columns() const;

    const T& operator() (size_t row, size_t column) const;
    T& operator() (size_t row, size_t column);

    Matrix& operator = (const Matrix& mat);
    Matrix& operator = (const std::vector<std::vector<T>>& elements);

    const Matrix operator * (const Matrix& mat) const;
    Vector<T> operator * (const Vector<T>& vec) const;

    bool operator == (const Matrix& mat) const;
    bool operator != (const Matrix& mat) const;

    std::string str() const;

    nucmath::Matrix<T> &transpose();

private:
    size_t nRows;
    size_t nColumns;

    std::vector<double> data;
};
}

template<typename T>
nucmath::Matrix<T>::Matrix()
{
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
nucmath::Matrix<T>::Matrix(const std::vector<std::vector<T>>& elements)
{
    *this = elements;
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
nucmath::Matrix<T>& nucmath::Matrix<T>::operator = (const Matrix& mat)
{
    nColumns = mat.columns();
    nRows = mat.rows();
    data = mat.data();
    return *this;
}

template<typename T>
nucmath::Matrix<T>& nucmath::Matrix<T>::operator = (const std::vector<std::vector<T>>& elements)
{
    if(elements.size() == 0)
        throw std::invalid_argument("nucmath::Matrix<T>::operator = (const std::vector<std::vector<T>>& elements): "
                                    "elements list is empty.");

    nRows = elements.size();
    nColumns = elements.at(0).size();

    for(const auto& row : elements)
    {
        if(nColumns != row.size())
            throw std::invalid_argument("nucmath::Matrix<T>::operator = (const std::vector<std::vector<T>>& elements): "
                                        "rows should have the same number of columns.");
        for(const auto& el : row)
        {
            data.push_back(el);
        }
    }

    return *this;
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

template<typename T>
nucmath::Matrix<T> &nucmath::Matrix<T>::transpose()
{
    std::vector<T> transposed;
    for(int n = 0; n < nRows*nColumns; n++)
    {
        int i = n/nRows;
        int j = n%nRows;
        transposed[n] = data[nColumns*j + i];
    }
    data = transposed;

    return *this;
}
