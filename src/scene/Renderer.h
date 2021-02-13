//
// Created by bartosz on 2/6/21.
//

#pragma once

#include "Model.h"
#include "LightSource.h"
#include "Sun.h"
#include "../api/Shader.h"
#include "Camera.h"
#include "../api/GeometryBuffer.h"
#include "../api/VertexArray.h"


struct Fog
{
	float density;
	glm::vec3 color;
};


class Renderer
{
protected:
	int scrWitdth, scrHeight;
	float zoomValue = 45.0f;
	glm::mat4 projectionMatrix;

public:
	typedef std::shared_ptr<Model> ModelPtr;

	Renderer( int scrWitdth, int scrHeight );

	virtual void renderScene( std::vector<ModelPtr> & models, std::vector<ModelPtr> lightModels, const Sun & sun,
							  const LightSourceSet & lightSourceSet, const Fog & fog,
							  const std::shared_ptr<Camera> & camera ) const = 0;

	void zoom( float offset );

	virtual ~Renderer() = default;

protected:
	void setProjection( const Shader & shader ) const;
};


class ForwardRenderer : public Renderer
{
private:
	Shader modelShader;
	Shader lightShader;
//	Shader skyboxShader;

public:
	ForwardRenderer( const std::string msPath, const std::string lsPath, int scrWitdth, int scrHeight );

	void renderScene( std::vector<ModelPtr> & models, std::vector<ModelPtr> lightModels, const Sun & sun,
					  const LightSourceSet & lightSourceSet, const Fog & fog,
					  const std::shared_ptr<Camera> & camera ) const override;
};


class DeferredRenderer : public Renderer
{
private:
	Shader geoPassShader;
	Shader lightingShader;
	Shader lightSourceShader;
//	Shader skyboxShader;
	GeometryBuffer gBuffer;
	VertexArray quadVa;
	VertexBuffer quadVb;

	static float ndcVertices[];

public:
	DeferredRenderer( const std::string & mgsPath, const std::string & mlsPath, const std::string & lsPath,
					  int scrWitdth, int scrHeight );

	void renderScene( std::vector<ModelPtr> & models, std::vector<ModelPtr> lightModels, const Sun & sun,
					  const LightSourceSet & lightSourceSet, const Fog & fog,
					  const std::shared_ptr<Camera> & camera ) const override;
};
