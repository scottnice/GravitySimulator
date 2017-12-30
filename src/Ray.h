#pragma once
#include "glm/glm/glm.hpp"
namespace glm {
	class Ray
	{
	public:
		Ray(const vec3& origin, const vec3& direction);
		~Ray();
		const vec3 origin;
		const vec3 direction;
	};
}


