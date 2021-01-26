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
		pointLightSources[i].setInShader( shader, "pointLights[" + std::to_string( i ) + "]" );
	}

	for ( int i = 0; i < spotLightSources.size(); i++ )
	{
		spotLightSources[i].setInShader( shader, "spotLights[" + std::to_string( i ) + "]" );
	}
}

LightSourceSet::LightSourceSet( int pointCount, int spotCount )
{
	pointLightSources.reserve( pointCount );
	spotLightSources.reserve( spotCount );
}

LightSource::LightSource( const Light & light )
		: light( light )
{}

void LightSource::setInShader( const Shader & shader, const std::string & sourceName ) const
{
	shader.setVector( sourceName + ".ambient", light.ambient );
	shader.setVector( sourceName + ".diffuse", light.diffuse );
	shader.setVector( sourceName + ".specular", light.specular );
	setInShaderTypeSpecific( shader, sourceName );
}

PointLightSource::PointLightSource( const Light & light, const glm::vec3 & position, float linear, float quadratic )
		: LightSource( light ), position( position ), linear( linear ), quadratic( quadratic )
{}

void PointLightSource::setInShaderTypeSpecific( const Shader & shader, const std::string & sourceName ) const
{
	shader.setVector( sourceName + ".position", position );
	shader.setFloat( sourceName + ".constant", constant );
	shader.setFloat( sourceName + ".linear", linear );
	shader.setFloat( sourceName + ".quadratic", quadratic );
}

SpotLightSource::SpotLightSource( const Light & light, const glm::vec3 & position, const glm::vec3 & direction,
								  float innerCutOff, float outerCutOff, float linear, float quadratic )
		: LightSource( light ), position( position ), direction( direction ), innerCutOff( innerCutOff ),
		  outerCutOff( outerCutOff ), linear( linear ), quadratic( quadratic )
{}

void SpotLightSource::setInShaderTypeSpecific( const Shader & shader, const std::string & sourceName ) const
{
	shader.setVector( sourceName + ".position", position );
	shader.setVector( sourceName + ".direction", direction );
	shader.setFloat( sourceName + ".innerCutOff", innerCutOff );
	shader.setFloat( sourceName + ".outerCutOff", outerCutOff );
	shader.setFloat( sourceName + ".constant", constant );
	shader.setFloat( sourceName + ".linear", linear );
	shader.setFloat( sourceName + ".quadratic", quadratic );
}
