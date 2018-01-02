#include "Intersect.h"

bool glm::intersects(const Ray & ray, const Sphere & sphere)
{
	const vec3 rayOriginToSphereCenter = ray.origin - sphere.center;
	float a = powf(glm::length(ray.direction),2);
	float b = 2.0f * glm::dot(ray.direction, rayOriginToSphereCenter);
	float c = dot(rayOriginToSphereCenter, rayOriginToSphereCenter) - powf(sphere.radius, 2);
	auto discriminant = (powf(b, 2) - 4 * a * c);
	if (discriminant >= 0) {
		float root = sqrtf(discriminant);
		float positive = -b + root;
		float negative = -b - root;
		return positive >= 0 && negative >= 0;
	}
	return false;
}
