#include "Intersect.h"

bool glm::intersects(const Ray & ray, const Sphere & sphere)
{
	const vec3 rayOriginToSphereCenter = ray.origin - sphere.center;
	float b = dot(ray.direction, (rayOriginToSphereCenter));
	float c = dot(rayOriginToSphereCenter, rayOriginToSphereCenter) - powf(sphere.radius, 2);
	return (powf(b,2) - c) >= 0;
}
