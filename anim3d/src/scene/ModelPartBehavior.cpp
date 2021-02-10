//
// Created by bartosz on 1/19/21.
//

#include "ModelPartBehavior.h"


void NoModelPartBehavior::apply( ModelPtr & model ) const
{}

void RotationModelPartBehavior::apply( ModelPtr & model ) const
{
	currentAngle += rotationAngle;
	if ( currentAngle > 360 )
		currentAngle = 0.0f;

	model->rotate( currentAngle, rotationAxis );
}

RotationModelPartBehavior::RotationModelPartBehavior( float angle, const glm::vec3 & rotationAxis )
		: currentAngle( 0.0f ), rotationAngle( angle ), rotationAxis( rotationAxis )
{}
