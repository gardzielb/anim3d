//
// Created by bartosz on 1/19/21.
//

#pragma once


#include "Model.h"
#include <memory>


class ModelPartBehavior
{
protected:
	typedef std::shared_ptr<Model> ModelPtr;

public:
	virtual void apply( ModelPtr & model ) const = 0;
};


class NoModelPartBehavior : public ModelPartBehavior
{
public:
	void apply( ModelPtr & model ) const override;
};


class RotationModelPartBehavior : public ModelPartBehavior
{
private:
	float rotationAngle;
	mutable float currentAngle;
	glm::vec3 rotationAxis;

public:
	RotationModelPartBehavior( float angle, const glm::vec3 & rotationAxis );

	void apply( ModelPtr & model ) const override;
};
