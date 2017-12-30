#pragma once
#include "glm/glm/glm.hpp"
namespace glm {
	class Sphere
	{
	public:
		Sphere(const vec3& center, const float radius);
		~Sphere();
		const vec3 center;
		const float radius;
	};
}

