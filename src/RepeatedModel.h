//
// Created by bartosz on 1/27/21.
//

#pragma once


#include "Model.h"


class RepeatedModel : public Model
{
private:
	std::shared_ptr<Model> model;
	std::vector<glm::vec3> positions;
	glm::vec3 scaleVector;

public:
	RepeatedModel( const std::shared_ptr<Model> & model, const std::vector<glm::vec3> & positions,
				   const glm::vec3 & scale = glm::vec3( 1.0f, 1.0f, 1.0f ) )
			: model( model ), positions( positions ), scaleVector( scale )
	{}

	void draw( const Shader & shader ) override
	{
		for ( auto & pos : positions )
		{
			model->toOrigin().translate( pos ).scale( scaleVector );
			model->draw( shader );
		}
	}
};
