#include "Vector3d.h"
#include "amp_physics.h"
Vector3d& Vector3d::operator=(const amp_physics::amp_vector& other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	return *this;
}
