#include "Ray.h"
#include "glm/glm/gtc/matrix_transform.hpp"

using namespace glm;
Ray::Ray(const vec3& origin, const vec3& direction):origin(origin), direction(normalize(direction))
{
}


Ray::~Ray()
{
}
