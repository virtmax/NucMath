


#pragma once

#include "utils.h"

namespace nucmath
{

template <typename T>
class Vector
{
public:
    Vector(size_t numElements);
    Vector(const std::vector<T>& elements);

    size_t dim() const;

    const std::vector<T>& rawData() const;
    std::vector<T>& rawData();

    const T& operator() (size_t index) const;
    T& operator() (size_t index);

    Vector<T>& operator =  (const Vector<T>& vec);

    Vector<T> operator + (const Vector<T>& vec) const;
    Vector<T> operator - (const Vector<T>& vec) const;
    Vector<T> operator - () const	;
    T         operator * (const Vector<T>& vec) const;
    Vector<T> operator * (const T scalar) const;
    Vector<T> operator / (const T scalar) const;

    Vector<T>& operator += (const Vector<T>& vec);
    Vector<T>& operator -= (const Vector<T>& vec);
    Vector<T>& operator *= (const T scalar);
    Vector<T>& operator /= (const T scalar);

    bool operator == (const Vector<T>& vec) const;
    bool operator == (const std::vector<T>& vec) const;
    bool operator != (const Vector<T>& vec) const;
    bool operator != (const std::vector<T>& vec) const;

    double length() const;

private:
    std::vector<T> data;
};

}

template<typename T>
nucmath::Vector<T>::Vector(size_t numElements)
{
    data.assign(numElements, 0.0);
}

template<typename T>
nucmath::Vector<T>::Vector(const std::vector<T>& elements)
{
    this->data = elements;
}

template<typename T>
size_t nucmath::Vector<T>::dim() const
{
    return data.size();
}

template<typename T>
const std::vector<T>& nucmath::Vector<T>::rawData() const
{
    return data;
}

template<typename T>
std::vector<T>& nucmath::Vector<T>::rawData()
{
    return data;
}

template<typename T>
const T& nucmath::Vector<T>::operator() (size_t index) const
{
    return data.at(index);
}

template<typename T>
T& nucmath::Vector<T>::operator() (size_t index)
{
    return data.at(index);
}

template<typename T>
nucmath::Vector<T>& nucmath::Vector<T>::operator = (const Vector<T>& vec)
{
    data = vec.rawData();
    return *this;
}

template<typename T>
nucmath::Vector<T> nucmath::Vector<T>::operator + (const Vector<T>& vec) const
{
    if(vec.dim() != dim())
        throw std::invalid_argument("Vector<T>::operator + (const Vector<T>& vec): vec has wrong dimensions.");

    Vector<T> temp(dim());
    for(size_t i = 0; i < data.size(); i++)
    {
        temp(i) = data[i] + vec(i);
    }

    return temp;
}

template<typename T>
nucmath::Vector<T> nucmath::Vector<T>::operator - (const Vector<T>& vec) const
{
    if(vec.dim() != dim())
        throw std::invalid_argument("Vector<T>::operator - (const Vector<T>& vec): vec has wrong dimensions.");

    Vector<T> temp(dim());
    for(size_t i = 0; i < data.size(); i++)
    {
        temp(i) = data[i] - vec(i);
    }

    return temp;
}

template<typename T>
nucmath::Vector<T> nucmath::Vector<T>::operator - () const
{
    Vector<T> temp(dim());
    for(size_t i = 0; i < data.size(); i++)
    {
        temp(i) = -data[i];
    }

    return temp;
}

template<typename T>
T nucmath::Vector<T>::operator * (const Vector<T>& vec) const
{
    if(vec.dim() != dim())
        throw std::invalid_argument("Matrix<T>::operator == (const Matrix& mat): mat has wrong dimensions.");

    double scalar = 0.0;
    for(size_t i = 0; i < data.size(); i++)
    {
        scalar += data[i]*vec(i);
    }

    return scalar;
}

template<typename T>
nucmath::Vector<T> nucmath::Vector<T>::operator * (const T scalar) const
{
    Vector<T> temp(dim());
    for(size_t i = 0; i < data.size(); i++)
    {
        temp(i) = data[i]*scalar;
    }

    return temp;
}

template<typename T>
nucmath::Vector<T> nucmath::Vector<T>::operator / (const T scalar) const
{
    Vector<T> temp(dim());
    for(size_t i = 0; i < data.size(); i++)
    {
        temp(i) = data[i]/scalar;
    }

    return temp;
}

template<typename T>
nucmath::Vector<T>& nucmath::Vector<T>::operator += (const Vector<T>& vec)
{
    if(vec.dim() != dim())
        throw std::invalid_argument("Vector<T>::operator += (const Vector<T>& vec): vec has wrong dimensions.");

    for(size_t i = 0; i < data.size(); i++)
    {
        data[i] += vec(i);
    }

    return *this;
}

template<typename T>
nucmath::Vector<T>& nucmath::Vector<T>::operator -= (const Vector<T>& vec)
{
    if(vec.dim() != dim())
        throw std::invalid_argument("Vector<T>::operator -= (const Vector<T>& vec): vec has wrong dimensions.");

    for(size_t i = 0; i < data.size(); i++)
    {
        data[i] -= vec(i);
    }

    return *this;
}

template<typename T>
nucmath::Vector<T>& nucmath::Vector<T>::operator *= (const T scalar)
{
    for(size_t i = 0; i < data.size(); i++)
    {
        data[i] *= scalar;
    }

    return *this;
}

template<typename T>
nucmath::Vector<T>& nucmath::Vector<T>::operator /= (const T scalar)
{
    for(size_t i = 0; i < data.size(); i++)
    {
        data[i] /= scalar;
    }

    return *this;
}

template<typename T>
bool nucmath::Vector<T>::operator == (const Vector<T>& vec) const
{
    if(vec.dim() != dim())
        throw std::invalid_argument("nucmath::Vector<T>::operator == (const Vector<T>& vec): vec has wrong dimension.");

    for(size_t i = 0; i < dim(); i++)
    {
        if(!nucmath::isEqual(vec(i), data[i]))
            return false;
    }

    return true;
}

template<typename T>
bool nucmath::Vector<T>::operator == (const std::vector<T>& vec) const
{
    if(vec.size() != dim())
        throw std::invalid_argument("nucmath::Vector<T>::operator == (const std::vector<T>& vec): vec has wrong size.");

    for(size_t i = 0; i < dim(); i++)
    {
        if(!nucmath::isEqual(vec[i], data[i]))
            return false;
    }

    return true;
}

template<typename T>
bool nucmath::Vector<T>::operator != (const Vector<T>& vec) const
{
    if(vec.dim() != dim())
        throw std::invalid_argument("Vector<T>::operator != (const Vector<T>& vec): mat has wrong dimension.");

    return !this->operator==(vec);
}

template<typename T>
bool nucmath::Vector<T>::operator != (const std::vector<T>& vec) const
{
    if(vec.size() != dim())
        throw std::invalid_argument("Vector<T>::operator != (const std::vector<T>& vec): vec has wrong size.");

    return !this->operator==(vec);
}

template<typename T>
double nucmath::Vector<T>::length() const
{
    double scalar = 0.0;
    for(size_t i = 0; i < data.size(); i++)
    {
        scalar += data[i]*data[i];
    }

    return std::sqrt(scalar);
}
