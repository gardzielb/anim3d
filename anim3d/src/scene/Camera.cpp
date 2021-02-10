//
// Created by bartosz on 1/7/21.
//

#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <spdlog/spdlog.h>


Camera::Camera( const glm::vec3 & position, const glm::vec3 & front,
				const std::shared_ptr<RotationBehavior> & rotationBehavior,
				const std::shared_ptr<MoveBehavior> & moveBehavior )
		: position( position ), front( glm::normalize( front ) ), rotationBehavior( rotationBehavior ),
		  moveBehavior( moveBehavior )
{
	right = glm::normalize( glm::cross( front, glm::vec3( 0.0f, 1.0f, 0.0f ) ) );
	up = glm::normalize( glm::cross( right, front ) );
}

glm::mat4 Camera::viewMatrix()
{
	return glm::lookAt( position, position + front, up );
}

void Camera::moveForward()
{
	position = moveBehavior->forward( position, front, up, right );
}

void Camera::moveBack()
{
	position = moveBehavior->back( position, front, up, right );
}

void Camera::moveLeft()
{
	position = moveBehavior->left( position, front, up, right );
}

void Camera::moveRight()
{
	position = moveBehavior->right( position, front, up, right );
}

void Camera::setRotationLock( bool isLocked )
{
	canRotate = !isLocked;
}

void Camera::handleMouseMove( float x, float y )
{
	if ( canRotate )
		rotationBehavior->handleMouseMove( x, y, front, up, right );
}

void ModelObservingCamera::update( const glm::mat4 & transformationMatrix )
{
	glm::vec4 frontPos = transformationMatrix * glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );
	front = glm::normalize( glm::vec3( frontPos ) - position );
}

ModelObservingCamera::ModelObservingCamera( const glm::vec3 & position, const glm::vec3 & front )
		: Camera( position, front, std::make_shared<NoRotationBehavior>(), std::make_shared<NoMoveBehavior>() )
{}

void ModelBoundCamera::update( const glm::mat4 & transformationMatrix )
{
	glm::vec4 pos4 = transformationMatrix * glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );
	position = glm::vec3( pos4 );
}

ModelBoundCamera::ModelBoundCamera( const glm::vec3 & position, const glm::vec3 & front ) : Camera(
		position, front, std::make_shared<SimpleRotationBehavior>( 0.5f, -90.0f, -90.0f ),
		std::make_shared<NoMoveBehavior>()
)
{}
