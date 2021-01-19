//
// Created by bartosz on 1/19/21.
//

#include "ModelPartBehavior.h"


void NoModelPartBehavior::apply( ModelPtr & model ) const
{}

void RotationModelPartBehavior::apply( ModelPtr & model ) const
{
	model->rotate( angle, rotationAxis );
}

RotationModelPartBehavior::RotationModelPartBehavior( float angle, const glm::vec3 & rotationAxis )
		: angle( angle ), rotationAxis( rotationAxis )
{}
