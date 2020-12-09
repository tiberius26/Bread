#ifndef VECTOR_H
#define VECTOR_H

/*==============================================================================================#
|                                                                                               |
| Handmade Lite is an educational Visual Studio framework, written by Karsten Vermeulen,        |
| designed to assist with educating other fellow programmers, programming students, and anyone  |
| else wishing to learn about game development, C++, and object-oriented programming. This      |
| project is not open source and therefore has no license affiliated with it. Please be         |
| respectful and do not distribute the engine or its code as your work. You may alter or tweak  |
| the engine code as a learning experience and you may also 'borrow' code and functionality     |
| and use it to create your own game engine. Finally, please do use Handmade Lite to develop    |
| a game.                                                                                       |
|                                                                                               |
| Designed to teach. Made from scratch. Built by hand.							                |
|	                                                                                            |
#===============================================================================================#
|                                                                                               |
| If you like Handmade Lite and wish to show your support, if you have any questions about      |
| the project, or if you just want to reach out, please do get in touch:                        |
|																						        |
| Facebook | https://www.facebook.com/Handmade-Lite-107188757745777								|
| Twitter  | https://twitter.com/KarstensCorner													|
| GitHub   | https://github.com/djkarstenv/Handmade-Lite									    |
|                                                                                               |
#===============================================================================================#
| 'Vector' source file last updated on 3 August 2020						                    |
#==============================================================================================*/

#include <math.h>

template <class T> class Vector
{

public:

	static const Vector<T> Up;
	static const Vector<T> Down;
	static const Vector<T> Left;
	static const Vector<T> Right;
	static const Vector<T> Zero;
	static const Vector<T> One;

public:

	Vector(T x = 0, T y = 0);
	
public:

	Vector<T> operator+(const Vector<T>& second) const;
	Vector<T>& operator+=(const Vector<T>& second);
	
	Vector<T> operator-(const Vector<T>& second) const;
	Vector<T>& operator-=(const Vector<T>& second);
	
	Vector<T> operator*(const T second) const;
	Vector<T>& operator*=(const T second);

	Vector<T> operator/(const T second) const;
	Vector<T>& operator/=(const T second);

	Vector<T> operator-() const;
	
public:

	T Magnitude() const;
	T SqrMagnitude() const;
	T Distance(const Vector<T>& second) const;
	T Dot(const Vector<T>& second) const;

	Vector<T> Normalize() const;
	Vector<T> Lerp(const Vector<T>& second, float delta) const;
	Vector<T> Slerp(const Vector<T>& second, float delta) const;

public:

	T x;
	T y;
};

template <class T> const Vector<T> Vector<T>::Up = Vector<T>(0, 1);
template <class T> const Vector<T> Vector<T>::Down = Vector<T>(0, -1);
template <class T> const Vector<T> Vector<T>::Left = Vector<T>(-1, 0);
template <class T> const Vector<T> Vector<T>::Right = Vector<T>(1, 0);
template <class T> const Vector<T> Vector<T>::Zero = Vector<T>(0, 0);
template <class T> const Vector<T> Vector<T>::One = Vector<T>(1, 1);
 
//======================================================================================================
template <class T> Vector<T>::Vector(T x, T y)
{
	this->x = x;
	this->y = y;
}
//======================================================================================================
template <class T> Vector<T> Vector<T>::operator+(const Vector<T>& second) const
{
	Vector<T> result(*this);
    return (result += second);
}
//======================================================================================================
template <class T> Vector<T>& Vector<T>::operator+=(const Vector<T>& second)
{
	x += second.x;
	y += second.y;
	return *this;
}
//======================================================================================================
template <class T> Vector<T> Vector<T>::operator-(const Vector<T>& second) const
{
	Vector<T> result(*this);
	return (result -= second);
}
//======================================================================================================
template <class T> Vector<T>& Vector<T>::operator-=(const Vector<T>& second)
{
	x -= second.x;
	y -= second.y;
	return *this;
}
//======================================================================================================
template <class T> Vector<T> Vector<T>::operator*(const T second) const
{
	Vector<T> result(*this);
	return (result *= second);
}
//======================================================================================================
template <class T> Vector<T>& Vector<T>::operator*=(const T second)
{
	x *= second;
	y *= second;
	return *this;
}
//======================================================================================================
template <class T> Vector<T> Vector<T>::operator/(const T second) const
{
	Vector<T> result(*this);
	return (result /= second);
}
//======================================================================================================
template <class T> Vector<T>& Vector<T>::operator/=(const T second)
{
	if (second != 0) x /= second;
	if (second != 0) y /= second;
	return *this;
}
//======================================================================================================
template <class T> Vector<T> Vector<T>::operator-() const
{
	Vector<T> result(-x, -y);
	if (result.x == -0) result.x = 0;
	if (result.y == -0) result.y = 0;
	return result;
}
//======================================================================================================
template <class T> T Vector<T>::Magnitude() const
{
	return static_cast<T>(sqrtf(static_cast<float>(SqrMagnitude())));
}
//======================================================================================================
template <class T> T Vector<T>::SqrMagnitude() const
{
	return (x * x) + (y * y);
}
//======================================================================================================
template <class T> T Vector<T>::Distance(const Vector<T>& second) const
{
	return (*this - second).Magnitude();
}
//======================================================================================================
template <class T> T Vector<T>::Dot(const Vector<T>& second) const
{
	return (x * second.x) + (y * second.y);
}
//======================================================================================================
template <class T> Vector<T> Vector<T>::Normalize() const
{
	return *this / Magnitude();
}
//======================================================================================================
template <class T> Vector<T> Vector<T>::Lerp(const Vector<T>& second, float delta) const
{
	return *this + (second - *this) * delta;
}
//======================================================================================================
template<class T> Vector<T> Vector<T>::Slerp(const Vector<T>& second, float delta) const
{
	float dot = Dot(second);
	dot = max(min(dot, 1), -1);
	float angle = acosf(dot) * delta;
	Vector<T> relative = (second - *this * dot).Normalize();
	return (*this * cosf(angle)) + (relative * sinf(angle));
}

#endif