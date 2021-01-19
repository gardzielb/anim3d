//
// Created by bartosz on 1/19/21.
//

#include "ComplexModel.h"

ComplexModel::ComplexModel( const std::vector<std::pair<ModelPtr, BehaviorPtr>> & parts )
		: parts( std::move( parts ) )
{}

Model & ComplexModel::translate( const glm::vec3 & vector )
{
	for ( auto &[model, _] : parts )
	{
		model->translate( vector );
	}
	return *this;
}

Model & ComplexModel::scale( const glm::vec3 & scaleVector )
{
	for ( auto &[model, _] : parts )
	{
		model->scale( scaleVector );
	}
	return *this;
}

Model & ComplexModel::rotate( float angle, const glm::vec3 & rotationAxis )
{
	for ( auto &[model, _] : parts )
	{
		model->rotate( angle, rotationAxis );
	}
	return *this;
}

void ComplexModel::draw( const Shader & shader )
{
	for ( auto & part : parts )
	{
		auto &[model, behavior] = part;
		behavior->apply( model );
		model->draw( shader );
	}
}
