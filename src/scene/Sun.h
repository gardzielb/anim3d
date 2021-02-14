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
	Light fullLight, zeroLight;
	mutable Light light;
	glm::vec3 rotationAxis;
	float currentAngle = 0.0f;
	float brightness;

public:
	glm::vec3 brightSkyColor, darkSkyColor;
	float dayLength;

	Sun( const Light & light, float dayLenght );

	void move();

	void setInShader( const Shader & shader ) const;

	glm::vec3 getSkyColor() const;

private:
	float computeBrightness() const;
};
