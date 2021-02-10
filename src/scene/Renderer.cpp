//
// Created by bartosz on 2/6/21.
//

#include "Renderer.h"
#include "../api/glUtils.h"


void ForwardRenderer::renderScene( std::vector<ModelPtr> & models, std::vector<ModelPtr> lightModels,
								   const Sun & sun, const LightSourceSet & lightSourceSet, const Fog & fog,
								   const std::shared_ptr<Camera> & camera ) const
{
	glCall( glClearColor( 0.0f, 0.0f, 0.0f, 1.0f ) );
	glCall( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) );

	glm::mat4 viewMatrix = camera->viewMatrix();

	modelShader.bind();
	setProjection( modelShader );
	modelShader.setMatrix( "view", viewMatrix );
	modelShader.setVector( "viewPos", camera->getPosition() );
	modelShader.setFloat( "material.shininess", 64.0f );
	modelShader.setFloat( "fog.density", fog.density );
	modelShader.setVector( "fog.color", fog.color );
	sun.setInShader( modelShader );
	lightSourceSet.setInShader( modelShader );

	for ( auto & model : models )
		model->draw( modelShader );

	lightShader.bind();
	lightShader.setMatrix( "view", viewMatrix );
	setProjection( lightShader );
	for ( auto & lm : lightModels )
		lm->draw( lightShader );
}

ForwardRenderer::ForwardRenderer( const std::string msPath, const std::string lsPath, int scrWitdth, int scrHeight )
		: Renderer( scrWitdth, scrHeight ), modelShader( msPath ), lightShader( lsPath )
{}

void DeferredRenderer::renderScene( std::vector<ModelPtr> & models, std::vector<ModelPtr> lightModels,
									const Sun & sun, const LightSourceSet & lightSourceSet, const Fog & fog,
									const std::shared_ptr<Camera> & camera ) const
{
	glCall( glClearColor( 0.0f, 0.0f, 0.0f, 1.0f ) );
	glCall( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) );

	glm::mat4 viewMatrix = camera->viewMatrix();

	// --------------------------------- GEOMETRY PASS ----------------------------------------
	gBuffer.bind();
	glCall( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) );

	geoPassShader.bind();
	geoPassShader.setMatrix( "view", viewMatrix );
	setProjection( geoPassShader );

	for ( auto & model : models )
		model->draw( geoPassShader );

	gBuffer.unbind();

	// --------------------------------- LIGHTING PASS ----------------------------------------
	glCall( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) );
	gBuffer.bindTextures();

	lightingShader.bind();
	lightingShader.setVector( "viewPos", camera->getPosition() );
	lightingShader.setFloat( "shininess", 64.0f );
	lightingShader.setFloat( "fog.density", fog.density );
	lightingShader.setVector( "fog.color", fog.color );
	sun.setInShader( lightingShader );
	lightSourceSet.setInShader( lightingShader );

	quadVa.bind();
	quadVb.bind();
	glCall( glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 ) );

	// --------------------------------- DEPTH BUFFER ----------------------------------------
	gBuffer.blitDepthBuffer( scrWitdth, scrHeight );

	// --------------------------------- LIGHTS ---------------------------------
	lightSourceShader.bind();
	setProjection( lightSourceShader );
	lightSourceShader.setMatrix( "view", viewMatrix );
	for ( auto & lm : lightModels )
		lm->draw( lightSourceShader );
}

DeferredRenderer::DeferredRenderer( const std::string & mgsPath, const std::string & mlsPath,
									const std::string & lsPath, int scrWitdth, int scrHeight )
		: Renderer( scrWitdth, scrHeight ), geoPassShader( mgsPath ), lightingShader( mlsPath ),
		  lightSourceShader( lsPath ), gBuffer( scrWitdth, scrHeight ), quadVb( ndcVertices, 4 * 5 * sizeof( float ) )
{
	VertexBufferLayout layout;
	layout.pushFloat( 3 );  // position
	layout.pushFloat( 2 );  // texture coords
	quadVa.addBuffer( quadVb, layout );

	lightingShader.bind();
	gBuffer.setShaderTextures( lightingShader );
}

float DeferredRenderer::ndcVertices[] = {
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
};

void Renderer::zoom( float offset )
{
	zoomValue = std::min( 45.0f, std::max( 1.0f, zoomValue + offset ) );
	projectionMatrix = glm::perspective(
			glm::radians( zoomValue ), (float) scrWitdth / (float) scrHeight, 0.1f, 100.0f
	);
}

Renderer::Renderer( int scrWitdth, int scrHeight ) : scrWitdth( scrWitdth ), scrHeight( scrHeight )
{
	projectionMatrix = glm::perspective(
			glm::radians( zoomValue ), (float) scrWitdth / (float) scrHeight, 0.1f, 100.0f
	);
}

void Renderer::setProjection( const Shader & shader ) const
{
	shader.bind();
	shader.setMatrix( "projection", projectionMatrix );
}
