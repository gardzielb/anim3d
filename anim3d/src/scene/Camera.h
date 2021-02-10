//
// Created by bartosz on 1/7/21.
//

#pragma once

#include "glm/glm.hpp"
#include "Model.h"
#include "RotationBehavior.h"
#include "MoveBehavior.h"


class Camera
{
protected:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;

	std::shared_ptr<RotationBehavior> rotationBehavior;
	std::shared_ptr<MoveBehavior> moveBehavior;

	bool canRotate = false;

public:
	Camera( const glm::vec3 & position, const glm::vec3 & front,
			const std::shared_ptr<RotationBehavior> & rotationBehavior,
			const std::shared_ptr<MoveBehavior> & moveBehavior );

	inline const glm::vec3 & getPosition() const
	{
		return position;
	}

	glm::mat4 viewMatrix();

	void moveForward();

	void moveBack();

	void moveLeft();

	void moveRight();

	void setRotationLock( bool isLocked );

	void handleMouseMove( float x, float y );
};


class ModelObservingCamera : public Camera, public ModelObserver
{
public:
	ModelObservingCamera( const glm::vec3 & position, const glm::vec3 & front );

	void update( const glm::mat4 & transformationMatrix ) override;
};


class ModelBoundCamera : public Camera, public ModelObserver
{
public:
	ModelBoundCamera( const glm::vec3 & position, const glm::vec3 & front );

	void update( const glm::mat4 & transformationMatrix ) override;
};


