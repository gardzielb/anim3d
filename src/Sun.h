//
// Created by bartosz on 1/22/21.
//

#pragma once

#include <glm/glm.hpp>
#include "Shader.h"
#include "LightSource.h"


class Sun : public LightSource
{
private:
	Light fullLight;
	glm::vec3 rotationAxis;
	float rotationAngle;
	float currentAngle = 0.0f;

public:
	Sun( const Light & light, float rotationAngle );

	void move();

protected:
	void setInShaderTypeSpecific( const Shader & shader, const std::string & sourceName ) const override;
};



