//
// Created by bartosz on 1/7/21.
//

#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>


Camera::Camera( glm::vec3 position, glm::vec3 target, float speed )
		: position( std::move( position ) ), speed( speed )
{
	direction = glm::normalize( position - target );
	glm::vec3 yVec = glm::vec3( 0.0f, 1.0f, 0.0f );
	right = glm::normalize( glm::cross( yVec, direction ) );
	up = glm::cross( direction, right );
}

glm::mat4 Camera::viewMatrix()
{
	return glm::lookAt( position, position - direction, up );
}

void Camera::moveForward()
{
	position -= speed * direction;
}

void Camera::moveBack()
{
	position += speed * direction;
}

void Camera::moveRight()
{
	position -= glm::normalize( glm::cross( direction, up ) ) * speed;
}

void Camera::moveLeft()
{
	position += glm::normalize( glm::cross( direction, up ) ) * speed;
}

void Camera::rotate( float xOffset, float yOffset )
{
	yaw += xOffset * sensitivity;
	pitch += yOffset * sensitivity;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if ( pitch > 89.0f )
		pitch = 89.0f;
	if ( pitch < -89.0f )
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos( glm::radians( yaw ) ) * cos( glm::radians( pitch ) );
	front.y = sin( glm::radians( pitch ) );
	front.z = sin( glm::radians( yaw ) ) * cos( glm::radians( pitch ) );
	direction = -glm::normalize( front );
}

void Camera::update( const glm::mat4 & transformationMatrix )
{
	glm::vec4 pos4( position, 1.0f );
	position = glm::vec3( transformationMatrix * pos4 );
}
