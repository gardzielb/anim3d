#include "dependencies/glad.h"
#include "glUtils.h"
#include "Shader.h"
#include "Camera.h"
#include "LightSource.h"
#include "ModelLoader.h"

#include <GLFW/glfw3.h>

#include <glm/ext/matrix_clip_space.hpp>

#define STB_IMAGE_IMPLEMENTATION

#include "dependencies/stb_image.h"
#include "Sun.h"
#include "ComplexModel.h"

#include <spdlog/spdlog.h>

#include "populate.h"
#include "GeometryBuffer.h"


void framebuffer_size_callback( GLFWwindow * window, int width, int height );

int processInput( GLFWwindow * window, int current, const std::shared_ptr<SpotLightSource> & heliLight,
				  const glm::vec3 & heliFront, const glm::vec3 & heliRight );

void renderQuad();


// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;


struct Fog
{
	float density;
	glm::vec3 color;
};


int main()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	// glfw window creation
	GLFWwindow * window = glfwCreateWindow( SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr );
	if ( !window )
	{
		spdlog::error( "Failed to create GLFW window" );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent( window );
	glfwSetFramebufferSizeCallback( window, framebuffer_size_callback );

	// glad: load all OpenGL function pointers
	if ( !gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress ) )
	{
		spdlog::error( "Failed to initialize GLAD" );
		glfwTerminate();
		return -1;
	}

// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	stbi_set_flip_vertically_on_load( true );

	Shader lightingShader( "../src/shaders/phong/lighting" );
	Shader geoPassShader( "../src/shaders/phong/geopass" );
//	Shader lightSourceShader( "../src/shaders/light" );

	glm::mat4 projection = glm::perspective( glm::radians( 45.0f ), 800.0f / 600.0f, 0.1f, 100.0f );

	std::shared_ptr<Camera> staticCamera = std::make_shared<Camera>(
			glm::vec3( 8.0f, 8.0f, 8.0f ), glm::vec3( -1.0f, -1.0f, -1.0f ), glm::vec3( 0.0f, 1.0f, 0.0f )
	);
	std::shared_ptr<ModelBoundCamera> chopperCamera = std::make_shared<ModelBoundCamera>(
			glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 0.3f, -1.0f, 0.3f ), glm::vec3( 0.0f, 1.0f, 0.0f )
	);
	std::shared_ptr<ModelObservingCamera> orcCamera = std::make_shared<ModelObservingCamera>(
			glm::vec3( 0.0f, 1.0f, 0.0f ), glm::vec3( 1.0f, 0.0f, 0.0f ), glm::vec3( 0.0f, 1.0f, 0.0f )
	);

	std::array<std::shared_ptr<Camera>, 3> cameras = { staticCamera, chopperCamera, orcCamera };
	int cameraIndex = 0;

	Light flashLight = {
			glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 1.0f, 1.0f, 1.0f ), glm::vec3( 1.0f, 1.0f, 1.0f )
	};
	auto spotLight = std::make_shared<SpotLightSource>(
			flashLight, glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 0.3f, -1.0f, 0.3f ), glm::radians( 12.5 ),
			glm::radians( 15.0 ), 0.09f, 0.032f
	);
	auto pointLights = createPointLightSources();

	LightSourceSet lightSet( pointLights.size(), 1 );
	lightSet.addSpotLightSource( spotLight );
	for ( auto & light : pointLights )
		lightSet.addPointLightSource( light );

	Sun sun = createSun();
	Fog fog = { 0.05f, glm::vec3( 0.6f, 0.6f, 0.6f ) };

//	lightSourceShader.bind();
//	lightSourceShader.setMatrix( "projection", projection );

	ModelLoader modelLoader;
	auto buildings = createBuildings<14>( modelLoader );
	auto lightModels = createPointLightModels( modelLoader, pointLights );
	auto staticModels = createStaticModels( modelLoader );
	std::shared_ptr<ComplexModel> mi28 = createChopper( modelLoader, spotLight );
	mi28->addObserver( chopperCamera, glm::vec3( 0.0f, -0.5f, 0.0f ) );
	mi28->addObserver( orcCamera );

	glCall( glEnable( GL_DEPTH_TEST ) );

	GeometryBuffer gBuffer( SCR_WIDTH, SCR_HEIGHT );

	geoPassShader.bind();
	geoPassShader.setMatrix( "projection", projection );

	lightingShader.bind();
	lightingShader.setFloat( "shininess", 64.0f );
	lightingShader.setFloat( "fog.density", fog.density );
	lightingShader.setVector( "fog.color", fog.color );
	lightSet.setInShader( lightingShader );
	gBuffer.setShaderTextures( lightingShader );

	// render loop
	while ( !glfwWindowShouldClose( window ) )
	{
		// render
		glCall( glClearColor( fog.color.r, fog.color.g, fog.color.b, fog.density ) );
		glCall( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) );

		// --------------------------------- GEOMETRY PASS ----------------------------------------
		gBuffer.bind();
		glCall( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) );

		geoPassShader.bind();
		geoPassShader.setMatrix( "view", cameras[cameraIndex]->viewMatrix() );

		mi28->translate( 0.0f, 0.0f, 0.1f ).rotate( 0.3f, 0.0f, 0.3f, 0.0f );
		mi28->draw( lightingShader );
		buildings.drawAll( lightingShader );
		for ( auto & m : staticModels )
			m->draw( lightingShader );

		// --------------------------------- LIGHTING PASS ----------------------------------------
		sun.move();
		lightingShader.bind();
		lightingShader.setVector( "viewPos", cameras[cameraIndex]->getPosition() );
		sun.setInShader( lightingShader );
		lightSet.setInShader( lightingShader );

		renderQuad();

		// lightSourceShader.bind();
		// lightSourceShader.setMatrix( "view", view );
		// for ( auto & m : lightModels )
		// m->draw( lightSourceShader );

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glCall( glfwSwapBuffers( window ) );
		glCall( glfwPollEvents() );

		// input
		glm::vec3 heliPos = mi28->offset();
		glm::vec3 heliDir = glm::cross( -heliPos, glm::vec3( 0.0f, 1.0f, 0.0f ) );
		cameraIndex = processInput( window, cameraIndex, spotLight, heliDir, -heliPos );
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}

void renderQuad()
{

}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
int processInput( GLFWwindow * window, int current, const std::shared_ptr<SpotLightSource> & heliLight,
				  const glm::vec3 & heliFront, const glm::vec3 & heliRight )
{
	if ( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
		glfwSetWindowShouldClose( window, true );

	if ( glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS )
		heliLight->rotate( heliRight, 0.01f );
	if ( glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS )
		heliLight->rotate( -heliRight, 0.01f );
	if ( glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS )
		heliLight->rotate( heliFront, 0.01f );
	if ( glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS )
		heliLight->rotate( -heliFront, 0.01f );

	if ( glfwGetKey( window, GLFW_KEY_1 ) == GLFW_PRESS )
		return 0;
	if ( glfwGetKey( window, GLFW_KEY_2 ) == GLFW_PRESS )
		return 1;
	if ( glfwGetKey( window, GLFW_KEY_3 ) == GLFW_PRESS )
		return 2;

	return current;

//	if ( glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS )
//		camera->rotate( 0.0f, 2.0f );
//	if ( glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS )
//		camera->rotate( 0.0f, -2.0f );
//	if ( glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS )
//		camera->rotate( 2.0f, 0.0f );
//	if ( glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS )
//		camera->rotate( -2.0f, 0.0f );
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback( GLFWwindow * window, int width, int height )
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport( 0, 0, width, height );
}
