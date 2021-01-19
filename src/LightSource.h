//
// Created by bartosz on 1/16/21.
//

#pragma once

#include <list>
#include <glm/vec3.hpp>
#include <vector>
#include "Shader.h"


struct LightSource
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct DirectionalLightSource : public LightSource
{
	glm::vec3 direction;
};

struct PointLightSource : public LightSource
{
	glm::vec3 position;
	float constant = 1.0f;
	float linear;
	float quadratic;
};

struct SpotLightSource : public LightSource
{
	glm::vec3 position;
	glm::vec3 direction;
	float innerCutOff, outerCutOff;
	float constant = 1.0f;
	float linear;
	float quadratic;
};


class LightSourceSet
{
private:
	DirectionalLightSource dirLightSource;
	std::vector<PointLightSource> pointLightSources;
	std::vector<SpotLightSource> spotLightSources;

public:
	LightSourceSet( DirectionalLightSource dirLightSource )
			: dirLightSource( std::move( dirLightSource ) )
	{}

	inline void addPointLightSource( const PointLightSource & lightSource )
	{
		pointLightSources.push_back( lightSource );
	}

	inline void addSpotLightSource( const SpotLightSource & lightSource )
	{
		spotLightSources.push_back( lightSource );
	}

	void addToShader( const Shader & shader ) const;

private:
	void setDirLight( const Shader & shader ) const;

	void setSpotLight( const Shader & shader, int index ) const;

	void setPointLight( const Shader & shader, int index ) const;

	void setLightUniversal( const Shader & shader, const LightSource & lightSource, const std::string & name ) const;
};