//
// Created by bartosz on 2/6/21.
//

#pragma once

#include "Model.h"
#include "LightSource.h"
#include "Sun.h"
#include "Shader.h"
#include "Camera.h"
#include "GeometryBuffer.h"
#include "VertexArray.h"


struct Fog
{
	float density;
	glm::vec3 color;
};


class Renderer
{
public:
	typedef std::shared_ptr<Model> ModelPtr;

	virtual void renderScene( std::vector<ModelPtr> & models, std::vector<ModelPtr> lightModels, const Sun & sun,
							  const LightSourceSet & lightSourceSet, const Fog & fog,
							  const std::shared_ptr<Camera> & camera ) const = 0;

	virtual ~Renderer() = default;
};


class ForwardRenderer : public Renderer
{
private:
	Shader modelShader;
	Shader lightShader;

public:
	ForwardRenderer( const std::string msPath, const std::string lsPath );

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
	GeometryBuffer gBuffer;
	int scrWitdth, scrHeight;
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
