//
// Created by bartosz on 1/16/21.
//

#include "LightSource.h"


void LightSourceSet::setInShader( const Shader & shader ) const
{
	shader.setInt( "pointCount", pointLightSources.size() );
	shader.setInt( "spotCount", spotLightSources.size() );

	for ( int i = 0; i < pointLightSources.size(); i++ )
	{
		pointLightSources[i]->setInShader( shader, "pointLights[" + std::to_string( i ) + "]" );
	}

	for ( int i = 0; i < spotLightSources.size(); i++ )
	{
		spotLightSources[i]->setInShader( shader, "spotLights[" + std::to_string( i ) + "]" );
	}
}

LightSourceSet::LightSourceSet( int pointCount, int spotCount )
{
	pointLightSources.reserve( pointCount );
	spotLightSources.reserve( spotCount );
}

LightSource::LightSource( const Light & light, const glm::vec3 & position )
		: light( light ), position( position )
{}

void LightSource::setInShader( const Shader & shader, const std::string & sourceName ) const
{
	light.setInShader( shader, sourceName );
	shader.setVector( sourceName + ".position", position );
	setInShaderTypeSpecific( shader, sourceName );
}

PointLightSource::PointLightSource( const Light & light, const glm::vec3 & position, float linear, float quadratic )
		: LightSource( light, position ), linear( linear ), quadratic( quadratic )
{}

void PointLightSource::setInShaderTypeSpecific( const Shader & shader, const std::string & sourceName ) const
{
	shader.setFloat( sourceName + ".constant", constant );
	shader.setFloat( sourceName + ".linear", linear );
	shader.setFloat( sourceName + ".quadratic", quadratic );
}

SpotLightSource::SpotLightSource( const Light & light, const glm::vec3 & position, const glm::vec3 & direction,
								  float innerCutOff, float outerCutOff, float linear, float quadratic )
		: LightSource( light, position ), direction( direction ), innerCutOff( innerCutOff ),
		  outerCutOff( outerCutOff ), linear( linear ), quadratic( quadratic )
{}

void SpotLightSource::setInShaderTypeSpecific( const Shader & shader, const std::string & sourceName ) const
{
	shader.setVector( sourceName + ".direction", direction );
	shader.setFloat( sourceName + ".innerCutOff", std::cos( innerCutOff ) );
	shader.setFloat( sourceName + ".outerCutOff", std::cos( outerCutOff ) );
	shader.setFloat( sourceName + ".constant", constant );
	shader.setFloat( sourceName + ".linear", linear );
	shader.setFloat( sourceName + ".quadratic", quadratic );
}

void SpotLightSource::rotate( const glm::vec3 & axis, float angle )
{
	glm::mat4 matrix = glm::rotate( glm::mat4( 1.0f ), angle, axis );
	glm::vec4 dir4 = matrix * glm::vec4( direction, 1.0f );
	direction = glm::vec3( dir4 );
}

void Light::setInShader( const Shader & shader, const std::string & sourceName ) const
{
	shader.setVector( sourceName + ".ambient", ambient );
	shader.setVector( sourceName + ".diffuse", diffuse );
	shader.setVector( sourceName + ".specular", specular );
}

void LightSource::update( const glm::mat4 & transformationMatrix )
{
	glm::vec4 pos4 = transformationMatrix * glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );
	position = glm::vec3( pos4 );
}
