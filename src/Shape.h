#pragma once
#include <vector>
#include <cmath>
#include "Vector3d.h"

namespace GameLib
{
class Shape
{
private: 
	float radius;
	#define PI 3.1415926535897932384626433832795f
	vector<Vector3d> v;
public:
	Shape(int vertices, float radius) : radius(radius)
	{
		v.reserve(vertices);
		for(float i = 0; i < (360); i += (360.0f / vertices))
		{
			float x,y;
			x = this->radius * cosf(i * PI / 180);
			y = this->radius * sinf(i * PI / 180);
			v.emplace_back(x, y, 0.0f);
		}
	}
	Shape(Shape&& rhs){
		radius = rhs.radius;
		v = vector<Vector3d>(std::forward<vector<Vector3d>>(rhs.v));
	}
	const vector<Vector3d>& getVertices() const
	{
		return v;
	}
	inline float getRadius() const {
		return radius;
	}
	~Shape() {}
};
}
