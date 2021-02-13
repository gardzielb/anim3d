//
// Created by bartosz on 1/22/21.
//

#pragma once

#include <glm/glm.hpp>
#include "../api/Shader.h"
#include "LightSource.h"


class Sun
{
private:
	Light fullLight, light;
	glm::vec3 rotationAxis;
	float rotationAngle;
	float currentAngle = 0.0f;

public:
	Sun( const Light & light, float rotationAngle );

	void move();

	void setInShader( const Shader & shader ) const;

	float getBrightness() const;
};
