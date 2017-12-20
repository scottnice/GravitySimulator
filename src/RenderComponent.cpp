#include "RenderComponent.h"


RenderComponent::RenderComponent(Shape shape) : shape(shape), color({ { 1.0f, 1.0f, 1.0f } })
{
}

RenderComponent::RenderComponent(Shape shape, std::array<float, 3> color) : shape(shape), color(color)
{
}


RenderComponent::~RenderComponent()
{
}
