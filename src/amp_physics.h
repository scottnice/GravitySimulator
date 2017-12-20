#pragma once
#include <amp.h>
#include <amp_math.h>
#include "PhysicsComponent.h"
namespace amp_physics{
	using namespace concurrency;

	class amp_vector
	{
	public:
		float x, y, z;
		// constructors
		amp_vector() restrict(amp)
		{
			x = 0;
			y = 0;
			z = 0;
		}

		~amp_vector() restrict(amp)
		{
		}

		amp_vector(float xVal, float yVal, float zVal) restrict(amp) 
		{ 
			x = xVal; 
			y = yVal; 
			z = zVal;
		}

		// member functions
		float length() const restrict(amp) { return fast_math::sqrtf(x*x + y*y + z*z); }

		void setLength(float val) restrict(amp)
		{
			if (val != 0)
			{
				float l = length();
				x *= val / l;
				y *= val / l;
				z *= val / l;
			}
			else
				x = y = z = 0;
		}

		amp_vector& operator=(const amp_vector& v) restrict(amp)
		{
			x = v.x;
			y = v.y;
			z = v.z;
			return *this;
		}

		amp_vector& operator +=(const amp_vector& v) restrict(amp)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			return (*this);
		}

		amp_vector& operator -=(const amp_vector& v) restrict(amp)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return (*this);
		}

		amp_vector& operator *=(float t) restrict(amp)
		{
			x *= t;
			y *= t;
			z *= t;
			return (*this);
		}

		amp_vector& operator *=(const amp_vector& v) restrict(amp)
		{
			x *= v.x;
			y *= v.y;
			z *= v.z;
			return (*this);
		}

		amp_vector& operator /=(float t) restrict(amp)
		{
			float f = 1.0F / t;
			x *= f;
			y *= f;
			z *= f;
			return (*this);
		}

		amp_vector operator -(void) const restrict(amp)
		{
			return (amp_vector(-x, -y, -z));
		}

		amp_vector operator +(const amp_vector& v) const restrict(amp)
		{
			return (amp_vector(x + v.x, y + v.y, z + v.z));
		}

		amp_vector operator -(const amp_vector& v) const restrict(amp)
		{
			return (amp_vector(x - v.x, y - v.y, z - v.z));
		}

		amp_vector operator *(float t) const restrict(amp)
		{
			return (amp_vector(x * t, y * t, z * t));
		}

		amp_vector operator /(float t) const restrict(amp)
		{
			float f = 1.0f / t;
			return (amp_vector(x * f, y * f, z * f));
		}

		float operator *(const amp_vector& v) const restrict(amp)
		{
			return (x * v.x + y * v.y + z * v.z);
		}

		amp_vector operator &(const amp_vector& v) const restrict(amp)
		{
			return (amp_vector(x * v.x, y * v.y, z * v.z));
		}

		bool operator ==(const amp_vector& v) const restrict(amp)
		{
			return ((x == v.x) && (y == v.y) && (z == v.z));
		}

		bool operator !=(const amp_vector& v) const restrict(amp)
		{
			return ((x != v.x) || (y != v.y) || (z != v.z));
		}

		amp_vector& normalize(void) restrict(amp)
		{
			float l = length();
			if (l != 0)
			{
				x /= l;
				y /= l;
				z /= l;
			}
			else
				x = y = z = 0;

			return *this;
		}
		
		amp_vector& operator=(const Vector3d& other)
		{
			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
			return *this;
		}
	};

	inline amp_vector operator *(float t, const amp_vector& v) restrict(amp)
	{
		return (amp_vector(t * v.x, t * v.y, t * v.z));
	}

	inline float calculateSquaredDistance(const amp_vector& tangent) restrict(amp)
	{
		return fast_math::powf(tangent.x, 2) + fast_math::powf(tangent.y, 2) + fast_math::powf(tangent.z, 2);
	}

	// distance^2 = a^2 + b^2
	inline float calculateDistance(const amp_vector& tangent) restrict(amp)
	{
		return fast_math::sqrtf(calculateSquaredDistance(tangent));
	}

	class amp_physics_component
	{
	public:
		amp_vector location;
		amp_vector velocity;
		unsigned int mass;
		unsigned int radius;
		amp_physics_component()restrict(amp){}
		~amp_physics_component()restrict(amp){}
		amp_physics_component& operator = (const PhysicsComponent& other) restrict(cpu){
			mass = other.mass;
			radius = other.radius;
			location = other.location;
			velocity = other.velocity;
			return *this;
		}
		inline amp_vector forceGravity(const amp_physics_component& p, const float G = 0.1f) const restrict(amp)
		{
			if (p.location == location)
				return amp_vector();
			auto r = p.location - location;
			const float squaredDistance = calculateSquaredDistance(r);
			return G * ((static_cast<float>(mass)* p.mass) / squaredDistance) * (r.normalize());
		}

		inline float tangentalVelocityToOrbit(float distance, amp_vector forceOfGravity) const restrict(amp)
		{
			return fast_math::sqrtf((distance * forceOfGravity.length()) / this->mass);
		}

		void Update(const amp_physics_component* const v, const unsigned int size) restrict(amp)
		{
			amp_vector force;
			
			for (unsigned int i = 0; i < size; ++i)
			{
				const amp_physics_component& p = v[i];
				if (&p != this && p.mass > 1)
				{
					force += forceGravity(p);
				}
			}

			velocity += force / mass;
		}
		inline void move() restrict(amp)
		{
			location += velocity;
		}
	};
}