#pragma once
#include "Component.h"
#include "Screen.h"
#include <vector>
#include <array>
using namespace GameLib;
class RenderComponent : public Component
{
	friend class Screen;
private:
	const Shape shape;
	std::array<float, 3> color;
public:
	RenderComponent(Shape shape);
	RenderComponent(Shape shape, std::array<float, 3> color);
	~RenderComponent();

	const Shape& getShape() const
	{
		return shape;
	}

	operator const Shape&() const
	{
		return shape;
	}
	const std::array<float, 3>& getColor() const{
		return color;
	}
};

