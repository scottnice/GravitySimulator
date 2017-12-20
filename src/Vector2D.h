#pragma once
#include <cmath>
#include "Vector3d.h"
using namespace std;

namespace GameLib
{
class Vector2D
{
    public:
		float x;
        float y;
        Vector2D() : x(0), y(0){}

        Vector2D(float xVal, float yVal){x = xVal; y = yVal;}

        ~Vector2D() {}

        float Length() const {return sqrt(x*x + y*y);}

		void SetLength(float val) 
		{
			if(val != 0)
			{
				float l = Length(); 
				x*=val/l; 
				y*=val/l;
			}
			else
				x = y = 0;
		}

		Vector2D& operator=(const Vector2D& v)
		{
			x = v.x;
			y = v.y;
			return *this;
		}

        Vector2D& operator +=(const Vector2D& v)
        {
            x += v.x;
            y += v.y;
            return (*this);
        }

        Vector2D& operator -=(const Vector2D& v)
        {
            x -= v.x;
            y -= v.y;
            return (*this);
        }

        Vector2D& operator *=(float t)
        {
            x *= t;
            y *= t;
            return (*this);
        }

		Vector2D& operator *=(const Vector2D& v)
        {
            x *= v.x;
            y *= v.y;
            return (*this);
        }

        Vector2D& operator /=(float t)
        {
            float f = 1.0F / t;
            x *= f;
            y *= f;
            return (*this);
        }

        Vector2D operator -(void) const
        {
            return (Vector2D(-x, -y));
        }

        Vector2D operator +(const Vector2D& v) const
        {
            return (Vector2D(x + v.x, y + v.y));
        }

        Vector2D operator -(const Vector2D& v) const
        {
            return (Vector2D(x - v.x, y - v.y));
        }

        Vector2D operator *(float t) const
        {
            return (Vector2D(x * t, y * t));
        }

        Vector2D operator /(float t) const
        {
            float f = 1.0f / t;
            return (Vector2D(x * f, y * f));
        }

        float operator *(const Vector2D& v) const
        {
            return (x * v.x + y * v.y);
        }

        Vector2D operator &(const Vector2D& v) const
        {
            return (Vector2D(x * v.x, y * v.y));
        }

        bool operator ==(const Vector2D& v) const
        {
            return ((x == v.x) && (y == v.y));
        }

        bool operator !=(const Vector2D& v) const
        {
            return ((x != v.x) || (y != v.y));
        }

        Vector2D& Normalize(void)
        {
            float l = Length();
            if(l != 0)
            {
                x /= l;
                y /= l;
            }
            else
                x = y = 0;

            return *this;
        }

		Vector2D& operator=(const Vector3d& v)
		{
			x = v.x;
			y = v.y;
			return *this;
		}

		Vector2D(const Vector3d& v)
		{
			x = v.x;
			y = v.y;
		}
};

inline Vector2D operator *(float t, const Vector2D& v)
{
    return (Vector2D(t * v.x, t * v.y));
}

//inline float DotProduct(const Vector2D& v1, const Vector2D& v2)
//{
//    return (v1 * v2);
//}
//
//inline float Magnitude(const Vector2D& v)
//{
//    return (sqrt(v.x * v.x + v.y * v.y));
//}
//
//inline float InverseMag(const Vector2D& v)
//{
//    return (1.0F / sqrt(v.x * v.x + v.y * v.y));
//}
//
//inline float SquaredMag(const Vector2D& v)
//{
//    return (v.x * v.x + v.y * v.y);
//}
}