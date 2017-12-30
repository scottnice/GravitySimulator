#pragma once
#include "Ray.h"
#include "Sphere.h"
namespace glm {
	bool intersects(const Ray& ray, const Sphere& sphere);
}