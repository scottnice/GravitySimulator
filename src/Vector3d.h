#pragma once
#include <math.h>
using namespace std;
namespace amp_physics{
	class amp_vector;
}
class Vector3d
{
public:
	float x, y, z;
	// constructors
	Vector3d() 
	{
		x = 0;
		y = 0;
		z = 0;
	}

	~Vector3d() 
	{
	}

	Vector3d(float xVal, float yVal, float zVal) 
	{
		x = xVal;
		y = yVal;
		z = zVal;
	}

	// member functions
	float length() const  { return sqrtf(x*x + y*y + z*z); }

	void setLength(float val) 
	{
		if (val != 0)
		{
			normalize();
			x *= val;
			y *= val;
			z *= val;
		}
		else
			x = y = z = 0;
	}

	Vector3d& operator=(const Vector3d& v) 
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	Vector3d& operator +=(const Vector3d& v) 
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return (*this);
	}

	Vector3d& operator -=(const Vector3d& v) 
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return (*this);
	}

	Vector3d& operator *=(float t) 
	{
		x *= t;
		y *= t;
		z *= t;
		return (*this);
	}

	Vector3d& operator *=(const Vector3d& v) 
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return (*this);
	}

	Vector3d& operator /=(float t) 
	{
		float f = 1.0F / t;
		x *= f;
		y *= f;
		z *= f;
		return (*this);
	}

	Vector3d operator -(void) const 
	{
		return (Vector3d(-x, -y, -z));
	}

	Vector3d operator +(const Vector3d& v) const 
	{
		return (Vector3d(x + v.x, y + v.y, z + v.z));
	}

	Vector3d operator -(const Vector3d& v) const 
	{
		return (Vector3d(x - v.x, y - v.y, z - v.z));
	}

	Vector3d operator *(float t) const 
	{
		return (Vector3d(x * t, y * t, z * t));
	}

	Vector3d operator /(float t) const 
	{
		float f = 1.0f / t;
		return (Vector3d(x * f, y * f, z * f));
	}

	float operator *(const Vector3d& v) const 
	{
		return (x * v.x + y * v.y + z * v.z);
	}

	Vector3d operator &(const Vector3d& v) const 
	{
		return (Vector3d(x * v.x, y * v.y, z * v.z));
	}

	bool operator ==(const Vector3d& v) const 
	{
		return ((x == v.x) && (y == v.y) && (z == v.z));
	}

	bool operator !=(const Vector3d& v) const 
	{
		return ((x != v.x) || (y != v.y) || (z != v.z));
	}

	Vector3d& normalize(void) 
	{
		float l = length();
		if (l != 0)
		{
			x /= l;
			y /= l;
			z /= l;
		}
		else
			x = y = z = 0;

		return *this;
	}

	Vector3d& operator=(const amp_physics::amp_vector& other);
};

inline Vector3d operator *(float t, const Vector3d& v)
{
	return (Vector3d(t * v.x, t * v.y, t * v.z));
}

inline Vector3d crossProduct(Vector3d& v1, Vector3d& v2)
{
	return{ v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x };
}

