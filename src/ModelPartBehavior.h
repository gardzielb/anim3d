//
// Created by bartosz on 1/19/21.
//

#pragma once


#include "Model.h"
#include <memory>


typedef std::shared_ptr<Model> ModelPtr;


class ModelPartBehavior
{
public:
	virtual void apply( ModelPtr & model ) const = 0;
};


class NoModelPartBehavior : ModelPartBehavior
{
public:
	void apply( ModelPtr & model ) const override;
};


class RotationModelPartBehavior : ModelPartBehavior
{
private:
	float angle;
	glm::vec3 rotationAxis;

public:
	RotationModelPartBehavior( float angle, const glm::vec3 & rotationAxis );

	void apply( ModelPtr & model ) const override;
};
