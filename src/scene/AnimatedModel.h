//
// Created by bartosz on 2/11/21.
//

#pragma once

#include "Model.h"
#include "ModelLoader.h"
#include <vector>
#include <glm/glm.hpp>


class AnimatedModel : public Model
{
private:
	std::vector<std::shared_ptr<Model>> frames;
	int current = 0;
	char increase = -1;

public:
	AnimatedModel( std::vector<std::shared_ptr<Model>> & frames );

	void draw( const Shader & shader ) override;
};


class AnimationBuilder
{
public:
	std::shared_ptr<AnimatedModel> createAnimation( const std::string & path, int frameCount, ModelLoader & loader );
};
