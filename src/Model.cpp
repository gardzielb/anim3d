//
// Created by bartosz on 1/19/21.
//

#include "Model.h"


SimpleModel::SimpleModel( const std::vector<Mesh> & meshes, const glm::mat4 & modelMatrix )
		: meshes( std::move( meshes ) ), modelMatrix( modelMatrix )
{}

// draws the model, and thus all its meshes
void SimpleModel::draw( const Shader & shader )
{
	shader.setMatrix( "model", modelMatrix );
	for ( unsigned int i = 0; i < meshes.size(); i++ )
		meshes[i].draw( shader );
}

Model & SimpleModel::rotate( float angle, const glm::vec3 & rotationAxis )
{
	modelMatrix = glm::rotate( modelMatrix, glm::radians( angle ), rotationAxis );
	return *this;
}

Model & SimpleModel::scale( const glm::vec3 & scaleVector )
{
	modelMatrix = glm::scale( modelMatrix, scaleVector );
	return *this;
}

Model & SimpleModel::translate( const glm::vec3 & vector )
{
	modelMatrix = glm::translate( modelMatrix, vector );
	return *this;
}
