//
// Created by bartosz on 1/19/21.
//

#include "Model.h"


SimpleModel::SimpleModel( MeshesPtr & meshes )
		: meshes( std::move( meshes ) )
{}

// draws the model, and thus all its meshes
void SimpleModel::draw( const Shader & shader )
{
	shader.setMatrix( "model", modelMatrix );
	for ( auto & mesh : *meshes )
		mesh.draw( shader );
}

Model & Model::rotate( float angle, const glm::vec3 & rotationAxis )
{
	modelMatrix = glm::rotate( modelMatrix, glm::radians( angle ), rotationAxis );
	return *this;
}

Model & Model::scale( const glm::vec3 & scaleVector )
{
	modelMatrix = glm::scale( modelMatrix, scaleVector );
	return *this;
}

Model & Model::translate( const glm::vec3 & vector )
{
	modelMatrix = glm::translate( modelMatrix, vector );
	return *this;
}

Model & Model::toOrigin()
{
	modelMatrix = glm::mat4( 1.0f );
	return *this;
}

Model & Model::transform( const glm::mat4 & matrix )
{
	modelMatrix *= matrix;
	return *this;
}
