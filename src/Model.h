#pragma once

#include "Mesh.h"
#include "Shader.h"
#include <memory>
#include <list>


class ModelObserver
{
public:
	virtual void update( const glm::mat4 & transformationMatrix ) = 0;
};


class Model
{
private:
	std::vector<std::shared_ptr<ModelObserver>> observers;

protected:
	glm::mat4 modelMatrix;

public:
	Model() : modelMatrix( 1.0f )
	{}

	virtual void draw( const Shader & shader ) = 0;

	Model & transform( const glm::mat4 & matrix );

	Model & rotate( float angle, const glm::vec3 & rotationAxis );

	Model & translate( const glm::vec3 & vector );

	Model & scale( const glm::vec3 & scaleVector );

	Model & toOrigin();

	inline void addObserver( const std::shared_ptr<ModelObserver> & observer )
	{
		observers.push_back( observer );
	}

	inline Model & rotate( float angle, float x, float y, float z )
	{
		return rotate( angle, glm::vec3( x, y, z ) );
	}

	inline Model & scale( float x, float y, float z )
	{
		return scale( glm::vec3( x, y, z ) );
	}

	inline Model & translate( float x, float y, float z )
	{
		return translate( glm::vec3( x, y, z ) );
	}

protected:
	void notifyObservers();
};


typedef std::shared_ptr<std::vector<Mesh>> MeshesPtr;

class SimpleModel : public Model
{
private:
	MeshesPtr meshes;

public:
	// constructor, expects a filepath to a 3D model.
	SimpleModel( MeshesPtr & meshes );

	// draws the model, and thus all its meshes
	void draw( const Shader & shader ) override;
};
