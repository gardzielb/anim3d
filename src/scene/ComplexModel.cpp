//
// Created by bartosz on 1/19/21.
//

#include "ComplexModel.h"

ComplexModel::ComplexModel( const std::vector<std::tuple<ModelPtr, BehaviorPtr, glm::vec3>> & parts )
		: parts( std::move( parts ) )
{}

void ComplexModel::draw( const Shader & shader )
{
	for ( auto & part : parts )
	{
		auto &[model, behavior, offset] = part;
		model->toOrigin().transform( modelMatrix ).translate( offset );
		behavior->apply( model );
		model->draw( shader );
	}
}

ComplexModelBuilder & ComplexModelBuilder::reset()
{
	parts.clear();
	return *this;
}

ComplexModelBuilder & ComplexModelBuilder::addPart( const ModelPtr & model, const BehaviorPtr & behavior,
													const glm::vec3 & offset )
{
	parts.emplace_back( model, behavior, offset );
	return *this;
}

std::shared_ptr<ComplexModel> ComplexModelBuilder::construct() const
{
	return std::make_shared<ComplexModel>( parts );
}
