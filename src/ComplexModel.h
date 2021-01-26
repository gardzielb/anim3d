//
// Created by bartosz on 1/19/21.
//

#pragma once

#include <memory>
#include "Model.h"
#include "ModelPartBehavior.h"


class ComplexModel : public Model
{
	typedef std::shared_ptr<Model> ModelPtr;
	typedef std::shared_ptr<ModelPartBehavior> BehaviorPtr;

private:
	std::vector<std::tuple<ModelPtr, BehaviorPtr, glm::vec3>> parts;

public:
	ComplexModel( const std::vector<std::tuple<ModelPtr, BehaviorPtr, glm::vec3>> & parts );

	void draw( const Shader & shader ) override;
};


class ComplexModelBuilder
{
	typedef std::shared_ptr<Model> ModelPtr;
	typedef std::shared_ptr<ModelPartBehavior> BehaviorPtr;

private:
	std::vector<std::tuple<ModelPtr, BehaviorPtr, glm::vec3>> parts;

public:
	ComplexModelBuilder & reset();

	ComplexModelBuilder & addPart( ModelPtr model, BehaviorPtr behavior, const glm::vec3 & offset = glm::vec3() );

	std::shared_ptr<ComplexModel> construct() const;
};
