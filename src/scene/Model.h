#pragma once

#include "Mesh.h"
#include "../api/Shader.h"
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
	typedef std::pair<std::shared_ptr<ModelObserver>, glm::vec3> ObserverData;
	std::vector<ObserverData> observers;

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

	inline void addObserver( const std::shared_ptr<ModelObserver> & observer, const glm::vec3 & offset = glm::vec3() )
	{
		observers.push_back( std::make_pair( observer, offset ) );
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

	inline glm::vec3 offset() const
	{
		return glm::vec3( modelMatrix * glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f ) );
	}

protected:
	void notifyObservers();
};


class SimpleModel : public Model
{
private:
	typedef std::shared_ptr<std::vector<Mesh>> MeshesPtr;
	MeshesPtr meshes;

public:
	// constructor, expects a filepath to a 3D model.
	SimpleModel( MeshesPtr & meshes );

	// draws the model, and thus all its meshes
	void draw( const Shader & shader ) override;
};
