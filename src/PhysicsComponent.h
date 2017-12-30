#pragma once
#include "Component.h"
#include "Vector3d.h"
#include <vector>
#include <assert.h>
#include <limits>
namespace amp_physics{
	class amp_physics_component;
}
class PhysicsComponent : public Component
{

private:
	inline bool checkCollision(const PhysicsComponent& p)
	{
		Vector3d tangent = p.location - location;
		float distance = calculateDistance(tangent);
		float addedRadii = static_cast<float>(radius + p.radius);
		if (distance <= addedRadii)
		{
			tangent.setLength(addedRadii);
			location = p.location - tangent;
			velocity = -0.75f*velocity;
			return true;
		}
		return false;
	}

	static inline float calculateSquaredDistance(const Vector3d& tangent)
	{
		return powf(tangent.x, 2) + powf(tangent.y, 2) + powf(tangent.z,2);
	}

	inline Vector3d forceGravity(const PhysicsComponent& p, const float G = 0.1f) const
	{
		if (p.location == location)
			return Vector3d();
		auto r = p.location - location;
		const float squaredDistance = calculateSquaredDistance(r);
		return G * ( (static_cast<float>(mass) * p.mass) / squaredDistance ) * (r.normalize());
	}

	inline float tangentalVelocityToOrbit(float distance, Vector3d forceOfGravity) const
	{
		return sqrtf( (distance * forceOfGravity.length()) / this->mass);
	}


public:

	Vector3d location;
	Vector3d velocity;
	unsigned int mass;
	float radius;

	// distance^2 = a^2 + b^2
	static inline float calculateDistance(const Vector3d& tangent)
	{
		return sqrtf(calculateSquaredDistance(tangent));
	}

	inline void Update2(const vector<PhysicsComponent>* const p)
	{
		Update(*p);
	}

	void Update(const std::vector<PhysicsComponent>& v)
	{
		Vector3d force;

		for (const auto &p : v)
		{
			if(&p != this && p.mass > 1)
			{
				force += forceGravity(p);
				checkCollision(p);
			}
		}
			
		velocity += force / mass;
	}

	inline void changeVelocity(Vector3d& speed)
	{
		velocity += speed; 
	}

	void move()
	{
		location += velocity;
	}

	inline void orbitObject(const PhysicsComponent& p, bool clockWise = true)
	{ 
		auto gravity = forceGravity(p);
		float v = tangentalVelocityToOrbit(calculateDistance(location - p.location), gravity);
		
		if (clockWise)
		{
			velocity.x = -gravity.y;
			velocity.y = gravity.z;
			velocity.z = gravity.x;
		}
		else
		{
			velocity.x = gravity.y;
			velocity.y = -gravity.z;
			velocity.z = gravity.y;
		}
		velocity.setLength(v);
		velocity += p.velocity;
	}

	PhysicsComponent(Vector3d location, Vector3d velocity, unsigned short mass, const float radius);
	~PhysicsComponent();
	PhysicsComponent& operator =(const amp_physics::amp_physics_component& other) restrict(cpu);
};

