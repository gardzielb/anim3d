//
// Created by bartosz on 1/19/21.
//

#pragma once

#include <memory>
#include "Model.h"
#include "ModelPartBehavior.h"


typedef std::shared_ptr<Model> ModelPtr;
typedef std::shared_ptr<ModelPartBehavior> BehaviorPtr;


class ComplexModel : public Model
{
private:
	std::vector<std::pair<ModelPtr, BehaviorPtr>> parts;

public:
	ComplexModel( const std::vector<std::pair<ModelPtr, BehaviorPtr>> & parts );

	Model & translate( const glm::vec3 & vector ) override;

	Model & scale( const glm::vec3 & scaleVector ) override;

	Model & rotate( float angle, const glm::vec3 & rotationAxis ) override;

	void draw( const Shader & shader ) override;
};
