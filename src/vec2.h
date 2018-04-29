


#pragma once

namespace nucmath
{

template <typename T> 
class Vec2
{
public:
	// Variablen
	union
	{
		struct
		{
            T x;
			T y;
		};

        T c[2];
	};

    inline Vec2(void);
    inline Vec2(const T f);
    inline Vec2(const T _x, const T _y);
    inline Vec2(const T* pfComponent);

	inline operator T* ();

    inline Vec2 operator + (const Vec2& v) const;
    inline Vec2 operator - (const Vec2& v) const;
    inline Vec2 operator - () const	;
    inline Vec2 operator * (const Vec2& v) const;
    inline Vec2 operator * (const T f) const;
    inline Vec2 operator / (const Vec2& v) const;
    inline Vec2 operator / (const T f) const;

    inline Vec2 operator =  (const Vec2& v);
    inline Vec2 operator += (const Vec2& v);
    inline Vec2 operator -= (const Vec2& v);
    inline Vec2 operator *= (const Vec2& v);
    inline Vec2 operator *= (const T f);
    inline Vec2 operator /= (const Vec2& v);
    inline Vec2 operator /= (const T f);

    inline bool operator == (const Vec2& v) const;
    inline bool operator != (const Vec2& v) const;

    inline T norm() const;
};


template <class T> Vec2<T>::Vec2(void)	{}
template <class T> Vec2<T>::Vec2(const T f) : x(f), y(f)	{}
template <class T> Vec2<T>::Vec2(const T _x, const T _y) : x(_x), y(_y)	{}
template <class T> Vec2<T>::Vec2(const T* pComponent) : x(pComponent[0]), y(pComponent[1])	{}

template <class T> Vec2<T>::operator T* () {return (T*)(c);}

template <class T> Vec2<T> Vec2<T>::operator + (const Vec2& v) const	{return Vec2(x + v.x, y + v.y);}
template <class T> Vec2<T> Vec2<T>::operator - (const Vec2& v) const	{return Vec2(x - v.x, y - v.y);}
template <class T> Vec2<T> Vec2<T>::operator - () const					{return Vec2(-x, -y);}
template <class T> Vec2<T> Vec2<T>::operator * (const Vec2& v) const	{return Vec2(x * v.x, y * v.y);}
template <class T> Vec2<T> Vec2<T>::operator * (const T f) const        {return Vec2(x * f, y * f);}
template <class T> Vec2<T> Vec2<T>::operator / (const Vec2& v) const	{return Vec2(x / v.x, y / v.y);}
template <class T> Vec2<T> Vec2<T>::operator / (const T f) const        {return Vec2(x / f, y / f);}

template <class T> Vec2<T> Vec2<T>::operator =  (const Vec2& v)	{x = v.x; y = v.y; return *this;}
template <class T> Vec2<T> Vec2<T>::operator += (const Vec2& v)	{x += v.x; y += v.y; return *this;}
template <class T> Vec2<T> Vec2<T>::operator -= (const Vec2& v)	{x -= v.x; y -= v.y; return *this;}
template <class T> Vec2<T> Vec2<T>::operator *= (const Vec2& v)	{x *= v.x; y *= v.y; return *this;}
template <class T> Vec2<T> Vec2<T>::operator *= (const T f)		{x *= f; y *= f; return *this;}
template <class T> Vec2<T> Vec2<T>::operator /= (const Vec2& v)	{x /= v.x; y /= v.y; return *this;}
template <class T> Vec2<T> Vec2<T>::operator /= (const T f)		{x /= f; y /= f; return *this;}

template <class T> bool Vec2<T>::operator == (const Vec2& v) const	{if(x != v.x) return false; return y == v.y;}
template <class T> bool Vec2<T>::operator != (const Vec2& v) const	{if(x != v.x) return true; return y != v.y;}

template <class T> T Vec2<T>::norm() const {return std::sqrt(x*x+y*y);}
}
