//
// Created by bartosz on 1/7/21.
//

#pragma once

#include "glm/glm.hpp"


class Camera
{
private:
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 up;
	glm::vec3 right;
	const float speed;
	float yaw = -90.0f;    // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
	float pitch = 0.0f;
	float sensitivity = 0.5f; // change this value to your liking

public:
	Camera( glm::vec3 position, glm::vec3 target, float speed );

	inline const glm::vec3 & getPosition() const
	{
		return position;
	}

	inline const glm::vec3 getFront() const
	{
		return -direction;
	}

	glm::mat4 viewMatrix();

	void moveForward();

	void moveBack();

	void moveRight();

	void moveLeft();

	void rotate( float xOffset, float yOffset );
};



