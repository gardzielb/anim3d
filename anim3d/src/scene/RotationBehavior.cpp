//
// Created by bartosz on 2/10/21.
//

#include "RotationBehavior.h"
#include <algorithm>


void RotationBehavior::handleMouseMove( float x, float y, glm::vec3 & front, glm::vec3 & up, glm::vec3 & right )
{
	if ( isFirst )
		isFirst = false;
	else
		updateVectors( x, y, front, up, right );

	xLast = x;
	yLast = y;
}

void NoRotationBehavior::updateVectors( float x, float y, glm::vec3 & front, glm::vec3 & up, glm::vec3 & right )
{}

void SimpleRotationBehavior::updateVectors( float x, float y, glm::vec3 & front, glm::vec3 & up, glm::vec3 & right )
{
	float xOffset = sensitivity * (x - xLast);
	float yOffset = sensitivity * (yLast - y);

	yaw += xOffset;
	pitch += yOffset;
	pitch = std::min( 89.0f, std::max( -89.0f, pitch ) );

	front.x = cos( glm::radians( yaw ) ) * cos( glm::radians( pitch ) );
	front.y = sin( glm::radians( pitch ) );
	front.z = sin( glm::radians( yaw ) ) * cos( glm::radians( pitch ) );
	front = glm::normalize( front );

	// also re-calculate the Right and Up vector
	// normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	right = glm::normalize( glm::cross( front, worldUp ) );
	up = glm::normalize( glm::cross( right, front ) );
}

SimpleRotationBehavior::SimpleRotationBehavior( float sensitivity, float yaw, float pitch )
		: yaw( yaw ), pitch( pitch ), sensitivity( sensitivity ), worldUp( 0.0f, 1.0f, 0.0f )
{}
