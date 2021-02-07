//
// Created by bartosz on 1/7/21.
//

#pragma once

#include "glm/glm.hpp"
#include "Model.h"


class Camera
{
protected:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
//	const float speed;
//	float yaw = -90.0f;    // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
//	float pitch = 0.0f;
//	float sensitivity = 0.5f; // change this value to your liking

public:
	Camera( const glm::vec3 & position, const glm::vec3 & front, const glm::vec3 & worldUp );

	inline const glm::vec3 & getPosition() const
	{
		return position;
	}

	glm::mat4 viewMatrix();

//	void moveForward();

//	void moveBack();

//	void moveRight();

//	void moveLeft();

//	void rotate( float xOffset, float yOffset );

//private:
//	void updateVectors();
};


class ModelObservingCamera : public Camera, public ModelObserver
{
public:
	ModelObservingCamera(const glm::vec3 & position, const glm::vec3 & front, const glm::vec3 & worldUp);

	void update(const glm::mat4 &transformationMatrix) override;
};


class ModelBoundCamera : public Camera, public ModelObserver
{
public:
	ModelBoundCamera(const glm::vec3 & position, const glm::vec3 & front, const glm::vec3 & worldUp);

	void update(const glm::mat4 &transformationMatrix) override;
};
