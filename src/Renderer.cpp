//
// Created by bartosz on 2/6/21.
//

#include "Renderer.h"
#include "glUtils.h"


void ForwardRenderer::renderScene( std::vector<ModelPtr> & models, std::vector<ModelPtr> lightModels,
								   const Sun & sun, const LightSourceSet & lightSourceSet, const Fog & fog,
								   const std::shared_ptr<Camera> & camera ) const
{
	glCall( glClearColor( 0.0f, 0.0f, 0.0f, 1.0f ) );
	glCall( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) );

	glm::mat4 viewMatrix = camera->viewMatrix();

	modelShader.bind();
	modelShader.setMatrix( "view", viewMatrix );
	modelShader.setVector( "viewPos", camera->getPosition() );
	modelShader.setFloat( "shininess", 64.0f );
	modelShader.setFloat( "fog.density", fog.density );
	modelShader.setVector( "fog.color", fog.color );
	sun.setInShader( modelShader );
	lightSourceSet.setInShader( modelShader );

	for ( auto & model : models )
		model->draw( modelShader );

	lightShader.bind();
	lightShader.setMatrix( "view", viewMatrix );
	for ( auto & lm : lightModels )
		lm->draw( lightShader );
}

ForwardRenderer::ForwardRenderer( const std::string msPath, const std::string lsPath )
		: modelShader( msPath ), lightShader( lsPath )
{
	glm::mat4 projectionMatrix = glm::perspective( glm::radians( 45.0f ), 800.0f / 600.0f, 0.1f, 100.0f );
	modelShader.bind();
	modelShader.setMatrix( "projection", projectionMatrix );
	lightShader.bind();
	lightShader.setMatrix( "projection", projectionMatrix );
}

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
	lightSourceShader.setMatrix( "view", viewMatrix );
	for ( auto & lm : lightModels )
		lm->draw( lightSourceShader );
}

DeferredRenderer::DeferredRenderer( const std::string & mgsPath, const std::string & mlsPath,
									const std::string & lsPath, int scrWitdth, int scrHeight )
		: geoPassShader( mgsPath ), lightingShader( mlsPath ), lightSourceShader( lsPath ),
		  gBuffer( scrWitdth, scrHeight ), scrWitdth( scrWitdth ), scrHeight( scrHeight ),
		  quadVb( ndcVertices, 4 * 5 * sizeof( float ) )
{
	VertexBufferLayout layout;
	layout.pushFloat( 3 );  // position
	layout.pushFloat( 2 );  // texture coords
	quadVa.addBuffer( quadVb, layout );

	glm::mat4 projectionMatrix = glm::perspective(
			glm::radians( 45.0f ), (float) scrWitdth / (float) scrHeight, 0.1f, 100.0f
	);
	geoPassShader.bind();
	geoPassShader.setMatrix( "projection", projectionMatrix );
	lightSourceShader.bind();
	lightSourceShader.setMatrix( "projection", projectionMatrix );

	lightingShader.bind();
	gBuffer.setShaderTextures( lightingShader );
}

float DeferredRenderer::ndcVertices[] = {
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
};
