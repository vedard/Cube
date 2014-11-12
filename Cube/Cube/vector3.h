#ifndef VECTOR3_H__
#define VECTOR3_H__
#include <iostream>
#include <cmath>



template <class T>
class Vector3
{
public:
	Vector3();
	Vector3(const T& nx, const T& ny, const T& nz);
	~Vector3();

	T Length() const;
	void Normalize();
	void Zero();

	T Dot(const Vector3<T>& v) const;
	Vector3<T> Cross(const Vector3<T>& v) const;

	void Afficher() const;


	//Surcharge d'operateur
	Vector3<T> operator+(const Vector3<T>& v) const;
	Vector3<T> operator-(const Vector3<T>& v) const;
	Vector3<T> operator-() const;

	Vector3<T> operator+(const T& v) const;
	Vector3<T> operator-(const T& v) const;
	Vector3<T> operator/(const T& v) const;
	Vector3<T> operator*(const T& v) const;

	Vector3<T>& operator=(const Vector3<T>& v);

	Vector3<T>& operator+=(const Vector3<T>& v);
	Vector3<T>& operator-=(const Vector3<T>& v);
	Vector3<T>& operator+=(const T& v);
	Vector3<T>& operator-=(const T& v);
	Vector3<T>& operator/=(const T& v);
	Vector3<T>& operator*=(const T& v);
	
	template <class U>
	friend std::ostream& operator<<( std::ostream& o,const Vector3<U>& v );

	bool operator==(const Vector3<T>& v) const;
	bool operator!=(const Vector3<T>& v) const;



public:
	T x, y, z;
};

template <class T>
Vector3<T>::Vector3()
{

}

template <class T>
Vector3<T>::Vector3(const T& nx, const T& ny, const T&nz) : x(nx), y(ny), z(nz)
{

}

template <class T>
Vector3<T>::~Vector3()
{

}

template <class T>
T Vector3<T>::Length() const
{
	return sqrt(x * x + y*y + z*z);
}

template <class T>
void Vector3<T>::Normalize()
{
	T len = Length();
	if (len)
	{
		x /= len;
		y /= len;
		z /= len;
	}
}

template <class T>
void Vector3<T>::Zero()
{
	x = y = z = 0;
}

template <class T>
T Vector3<T>::Dot(const Vector3<T>& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

template <class T>
Vector3<T> Vector3<T>::Cross(const Vector3<T>& v) const
{
	return Vector3<T>(
		y * v.z - v.y * z,
		z * v.x - v.z * x,
		x * v.y - v.x * y);
}

template <class T>
void Vector3<T>::Afficher() const
{
	std::cout << "( " << x << ", " << y << ", " << z << " )" << std::endl;
}

template <class T>
Vector3<T> Vector3<T>::operator+(const Vector3<T>& v) const
{
	return Vector3<T>(x + v.x, y + v.y, z + v.z);
}

template <class T>
Vector3<T> Vector3<T>::operator-(const Vector3<T>& v) const
{
	return Vector3<T>(x - v.x, y - v.y, z - v.z);
}

template <class T>
Vector3<T> Vector3<T>::operator-() const
{
	return Vector3<T>(-x, -y, -z);
}

template <class T>
Vector3<T> Vector3<T>::operator+(const T& v) const
{
	return Vector3<T>(x + v, y + v, z + v);
}

template <class T>
Vector3<T> Vector3<T>::operator-(const T& v) const
{
	return Vector3<T>(x - v, y - v, z - v);
}

template <class T>
Vector3<T> Vector3<T>::operator/(const T& v) const
{
	return Vector3<T>(x / v, y / v, z / v);
}

template <class T>
Vector3<T> Vector3<T>::operator*(const T& v) const
{
	return Vector3<T>(x * v, y * v, z * v);
}

template <class T>
Vector3<T>& Vector3<T>::operator=(const Vector3<T>& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

template <class T>
Vector3<T>& Vector3<T>::operator+=(const Vector3<T>& v)
{
	return (*this = *this + v);
}

template <class T>
Vector3<T>& Vector3<T>::operator-=(const Vector3<T>& v)
{
	return (*this = *this - v);
}

template <class T>
Vector3<T>& Vector3<T>::operator+=(const T& v)
{
	return (*this = *this + v);

}

template <class T>
Vector3<T>& Vector3<T>::operator-=(const T& v)
{
	return (*this = *this - v);
}

template <class T>
Vector3<T>& Vector3<T>::operator/=(const T& v)
{
	return (*this = *this / v);
}

template <class T>
Vector3<T>& Vector3<T>::operator*=(const T& v)
{
	return (*this = *this * v);

}


template <class T>
bool Vector3<T>::operator==(const Vector3<T>& v) const
{
	return  (x == v.x && y == v.y && z == v.z);
}

template <class T>
bool Vector3<T>::operator!=(const Vector3<T>& v) const
{
	return !(*this == v);
}

template <class T>
std::ostream& operator<<( std::ostream& o,const Vector3<T>& v )
{
	return o << v.x << " " << v.y << " " << v.z;
}

#endif // VECTOR3_H__
