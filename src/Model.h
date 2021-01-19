#pragma once

#include "Mesh.h"
#include "Shader.h"


class Model
{
public:
	virtual void draw( const Shader & shader ) = 0;

	virtual Model & rotate( float angle, const glm::vec3 & rotationAxis ) = 0;

	virtual Model & translate( const glm::vec3 & vector ) = 0;

	virtual Model & scale( const glm::vec3 & scaleVector ) = 0;
};


class SimpleModel : public Model
{
private:
	std::vector<Mesh> meshes;
	glm::mat4 modelMatrix;

public:
	// constructor, expects a filepath to a 3D model.
	SimpleModel( const std::vector<Mesh> & meshes, const glm::mat4 & modelMatrix );

	// draws the model, and thus all its meshes
	void draw( const Shader & shader ) override;

	Model & rotate( float angle, const glm::vec3 & rotationAxis ) override;

	Model & scale( const glm::vec3 & scaleVector ) override;

	Model & translate( const glm::vec3 & vector ) override;
};
