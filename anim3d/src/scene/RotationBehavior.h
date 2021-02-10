//
// Created by bartosz on 2/10/21.
//

#pragma once

#include <glm/glm.hpp>


class RotationBehavior
{
private:
	bool isFirst = true;

protected:
	float xLast, yLast;

public:
	void handleMouseMove( float x, float y, glm::vec3 & front, glm::vec3 & up, glm::vec3 & right );

protected:
	virtual void updateVectors( float x, float y, glm::vec3 & front, glm::vec3 & up, glm::vec3 & right ) = 0;
};


class NoRotationBehavior : public RotationBehavior
{
public:
	void updateVectors( float x, float y, glm::vec3 & front, glm::vec3 & up, glm::vec3 & right ) override;
};


class SimpleRotationBehavior : public RotationBehavior
{
private:
	float yaw, pitch, sensitivity;
	glm::vec3 worldUp;

public:
	SimpleRotationBehavior( float sensitivity = 0.5f, float yaw = -90.0f, float pitch = 0.0f );

	void updateVectors( float x, float y, glm::vec3 & front, glm::vec3 & up, glm::vec3 & right ) override;
};
