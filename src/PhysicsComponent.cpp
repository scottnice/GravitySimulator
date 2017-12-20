#include "PhysicsComponent.h"
#include "amp_physics.h"


PhysicsComponent::PhysicsComponent(Vector3d location, Vector3d velocity, unsigned short mass, unsigned short radius) : location(location), velocity(velocity), mass(mass), radius(radius)
{
}


PhysicsComponent::~PhysicsComponent()
{
}

PhysicsComponent& PhysicsComponent::operator =(const amp_physics::amp_physics_component& other) restrict(cpu){
	mass = other.mass;
	radius = other.radius;
	location = other.location;
	velocity = other.velocity;
	return *this;
}

