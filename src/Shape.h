#pragma once
#include <vector>
#include <cmath>
#include "Vector3d.h"
#include "Gl/glew.h"
#include <array>
#include "PhysicsComponent.h"
namespace GameLib
{
class Shape
{
private: 
	float radius;
	#define PI 3.1415926535897932384626433832795f
	vector<Vector3d> v;
public:
	unsigned int glVertexArrayID;
	Shape(int vertices, float radius) : radius(radius)
	{
		std::array<float, 108> vertexes = {
			-0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f,  0.5f, -0.5f,
			0.5f,  0.5f, -0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f, -0.5f,  0.5f,
			0.5f, -0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,

			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,

			0.5f,  0.5f,  0.5f,
			0.5f,  0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,

			-0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f,  0.5f,
			0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f,  0.5f, -0.5f,
			0.5f,  0.5f, -0.5f,
			0.5f,  0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f
		};
		glGenVertexArrays(1, &glVertexArrayID);
		glBindVertexArray(glVertexArrayID);
		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3d), (void*)0);
		glEnableVertexAttribArray(0);
		for (size_t i = 0; i < vertexes.size(); i += 3) {
			Vector3d vect{ vertexes[i], vertexes[i + 1], vertexes[i + 2] };
			auto a = Vector3d(0, 0, 0);
			auto distance = PhysicsComponent::calculateDistance(a - vect);
			auto diff = vect - a;
			diff *= 1 / distance;
			v.push_back(vect);
		}
	}
	Shape(const Shape&) = default;
	Shape(Shape&& rhs) = default;
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
