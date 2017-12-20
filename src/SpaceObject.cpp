#include "SpaceObject.h"
#include "GravityGame.h"


SpaceObject::SpaceObject(unsigned short physicsComponent, unsigned short renderComponent) : physicsComponent(physicsComponent), renderComponent(renderComponent)
{
}


SpaceObject::~SpaceObject()
{
}

PhysicsComponent& SpaceObject::getPhysicsComponent() const{
	return GravityGame::getPhysicsComponent(*this);
}

RenderComponent& SpaceObject::getRenderComponent() const{
	return GravityGame::getRenderComponent(*this);
}
