//
// Created by bartosz on 1/16/21.
//

#pragma once

#include <list>
#include <glm/vec3.hpp>
#include <vector>
#include "Shader.h"
#include "Model.h"


struct Light
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	void setInShader( const Shader & shader, const std::string & sourceName ) const;
};


class LightSource : public ModelObserver
{
private:
	Light light;
	glm::vec3 position;

public:
	LightSource( const Light & light, const glm::vec3 & position );

	void setInShader( const Shader & shader, const std::string & sourceName ) const;

	void update( const glm::mat4 & transformationMatrix ) override;

protected:
	virtual void setInShaderTypeSpecific( const Shader & shader, const std::string & sourceName ) const = 0;

	virtual ~LightSource() = default;
};


class PointLightSource : public LightSource
{
private:
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
	std::vector<std::shared_ptr<PointLightSource>> pointLightSources;
	std::vector<std::shared_ptr<SpotLightSource>> spotLightSources;

public:
	LightSourceSet( int pointCount, int spotCount );

	inline void addPointLightSource( const std::shared_ptr<PointLightSource> & lightSource )
	{
		pointLightSources.push_back( lightSource );
	}

	inline void addSpotLightSource( const std::shared_ptr<SpotLightSource> & lightSource )
	{
		spotLightSources.push_back( lightSource );
	}

	void setInShader( const Shader & shader ) const;
};