//
// Created by bartosz on 1/16/21.
//

#include "LightSource.h"


void LightSourceSet::addToShader( const Shader & shader ) const
{
	setDirLight( shader );
	for ( int i = 0; i < pointLightSources.size(); i++ )
	{
		setPointLight( shader, i );
	}
	for ( int i = 0; i < spotLightSources.size(); i++ )
	{
		setSpotLight( shader, i );
	}
}

void LightSourceSet::setSpotLight( const Shader & shader, int index ) const
{
	std::string name = "spotLights[" + std::to_string( index ) + "]";
	const SpotLightSource & lightSource = spotLightSources[index];

	setLightUniversal( shader, lightSource, name );
	shader.setVector( name + ".position", lightSource.position );
	shader.setVector( name + ".direction", lightSource.direction );
	shader.setFloat( name + ".innerCutOff", std::cos( lightSource.innerCutOff ) );
	shader.setFloat( name + ".outerCutOff", std::cos( lightSource.outerCutOff ) );
	shader.setFloat( name + ".constant", lightSource.constant );
	shader.setFloat( name + ".linear", lightSource.linear );
	shader.setFloat( name + ".quadratic", lightSource.quadratic );
}

void LightSourceSet::setPointLight( const Shader & shader, int index ) const
{
	std::string name = "pointLights[" + std::to_string( index ) + "]";
	const PointLightSource & lightSource = pointLightSources[index];

	setLightUniversal( shader, lightSource, name );
	shader.setVector( name + ".position", lightSource.position );
	shader.setFloat( name + ".constant", lightSource.constant );
	shader.setFloat( name + ".linear", lightSource.linear );
	shader.setFloat( name + ".quadratic", lightSource.quadratic );
}

void LightSourceSet::setDirLight( const Shader & shader ) const
{
	setLightUniversal( shader, dirLightSource, "directionalLight" );
	shader.setVector( "directionalLight.direction", dirLightSource.direction );
}

void LightSourceSet::setLightUniversal( const Shader & shader, const LightSource & lightSource,
										const std::string & name ) const
{
	shader.setVector( name + ".ambient", lightSource.ambient );
	shader.setVector( name + ".diffuse", lightSource.diffuse );
	shader.setVector( name + ".specular", lightSource.specular );
}
