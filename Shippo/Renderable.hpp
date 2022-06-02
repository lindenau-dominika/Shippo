#pragma once
#include "Shader.hpp"

class Renderable {
public:
	virtual void render(Shader& shader) const = 0;
};