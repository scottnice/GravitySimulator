#include "RenderComponent.h"


RenderComponent::RenderComponent(Shape shape) : shape(std::move(shape)), color({ { 1.0f, 1.0f, 1.0f } })
{
}

RenderComponent::RenderComponent(Shape shape, std::array<float, 3> color) : shape(std::move(shape)), color(color)
{
}


RenderComponent::~RenderComponent()
{
}
