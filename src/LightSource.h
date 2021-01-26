//
// Created by bartosz on 1/16/21.
//

#pragma once

#include <list>
#include <glm/vec3.hpp>
#include <vector>
#include "Shader.h"


struct Light
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};


class LightSource
{
protected:
	Light light;

public:
	LightSource( const Light & light );

	void setInShader( const Shader & shader, const std::string & sourceName ) const;

protected:
	virtual void setInShaderTypeSpecific( const Shader & shader, const std::string & sourceName ) const = 0;

	virtual ~LightSource() = default;
};


class PointLightSource : public LightSource
{
private:
	glm::vec3 position;
	float constant = 1.0f;
	float linear;
	float quadratic;

public:
	PointLightSource( const Light & light, const glm::vec3 & position, float linear, float quadratic );

protected:
	void setInShaderTypeSpecific( const Shader & shader, const std::string & sourceName ) const override;
};


class SpotLightSource : public LightSource
{
private:
	glm::vec3 position;
	glm::vec3 direction;
	float innerCutOff, outerCutOff;
	float constant = 1.0f;
	float linear;
	float quadratic;

public:
	SpotLightSource( const Light & light, const glm::vec3 & position, const glm::vec3 & direction, float innerCutOff,
					 float outerCutOff, float linear, float quadratic );

protected:
	void setInShaderTypeSpecific( const Shader & shader, const std::string & sourceName ) const override;
};


class LightSourceSet
{
private:
	std::vector<PointLightSource> pointLightSources;
	std::vector<SpotLightSource> spotLightSources;

public:
	LightSourceSet( int pointCount, int spotCount );

	inline void addPointLightSource( const PointLightSource & lightSource )
	{
		pointLightSources.push_back( lightSource );
	}

	inline void addSpotLightSource( const SpotLightSource & lightSource )
	{
		spotLightSources.push_back( lightSource );
	}

	void setInShader( const Shader & shader ) const;
};