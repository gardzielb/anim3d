//
// Created by bartosz on 1/27/21.
//

#pragma once


#include "Model.h"

template<int N>
class ModelPopulation
{
private:
	std::shared_ptr<Model> model;
	std::array<glm::vec3, N> positions;

public:
	ModelPopulation( const std::shared_ptr<Model> & model, const std::array<glm::vec3, N> & positions )
			: model( model ), positions( positions )
	{}

	void drawAll( const Shader & shader )
	{
		for ( auto & pos : positions )
		{
			model->toOrigin().translate( pos );
			model->draw( shader );
		}
	}
};
