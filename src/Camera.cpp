//
// Created by bartosz on 1/7/21.
//

#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>


Camera::Camera( const glm::vec3 & position, const glm::vec3 & front, const glm::vec3 & worldUp )
		: position( position ), front( glm::normalize( front ) ), worldUp( glm::normalize( worldUp ) )
{
	right = glm::normalize( glm::cross( front, worldUp ) );
	up = glm::normalize( glm::cross( right, front ) );
}

glm::mat4 Camera::viewMatrix()
{
	return glm::lookAt( position, position + front, up );
}

//void Camera::moveForward()
//{
//	position -= speed * direction;
//}
//
//void Camera::moveBack()
//{
//	position += speed * direction;
//}
//
//void Camera::moveRight()
//{
//	position -= glm::normalize( glm::cross( direction, up ) ) * speed;
//}
//
//void Camera::moveLeft()
//{
//	position += glm::normalize( glm::cross( direction, up ) ) * speed;
//}
//
//void Camera::rotate( float xOffset, float yOffset )
//{
//	yaw += xOffset * sensitivity;
//	pitch += yOffset * sensitivity;
//
//	 make sure that when pitch is out of bounds, screen doesn't get flipped
//	if ( pitch > 89.0f )
//		pitch = 89.0f;
//	if ( pitch < -89.0f )
//		pitch = -89.0f;
//
//	glm::vec3 front;
//	front.x = cos( glm::radians( yaw ) ) * cos( glm::radians( pitch ) );
//	front.y = sin( glm::radians( pitch ) );
//	front.z = sin( glm::radians( yaw ) ) * cos( glm::radians( pitch ) );
//	direction = -glm::normalize( front );
//}

//void Camera::updateVectors()
//{
//	 calculate the new Front vector
//	front.x = cos( glm::radians( yaw ) ) * cos( glm::radians( pitch ) );
//	front.y = sin( glm::radians( pitch ) );
//	front.z = sin( glm::radians( yaw ) ) * cos( glm::radians( pitch ) );
//	front = glm::normalize( front );
//	 also re-calculate the Right and Up vector
//	 normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
//	right = glm::normalize( glm::cross( front, worldUp ) );
//	up = glm::normalize( glm::cross( right, front ) );
//}

void ModelObservingCamera::update( const glm::mat4 & transformationMatrix )
{
	glm::vec4 frontPos = transformationMatrix * glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );
	front = glm::normalize( glm::vec3( frontPos ) - position );
}

ModelObservingCamera::ModelObservingCamera( const glm::vec3 & position, const glm::vec3 & front,
											const glm::vec3 & worldUp )
		: Camera( position, front, worldUp )
{}

void ModelBoundCamera::update( const glm::mat4 & transformationMatrix )
{
	glm::vec4 pos4 = transformationMatrix * glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );
	position = glm::vec3( pos4 );
}

ModelBoundCamera::ModelBoundCamera( const glm::vec3 & position, const glm::vec3 & front, const glm::vec3 & worldUp )
		: Camera( position, front, worldUp )
{}
