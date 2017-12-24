#pragma once
#include "PhysicsComponent.h"
#include "RenderComponent.h"
class SpaceObject
{
public:
	unsigned short physicsComponent;
	unsigned short renderComponent;
	SpaceObject(unsigned short physicsComponent, unsigned short renderComponent);
	~SpaceObject();
	PhysicsComponent& getPhysicsComponent() const;
	RenderComponent& getRenderComponent() const;
};

