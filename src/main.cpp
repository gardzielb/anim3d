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


void framebuffer_size_callback( GLFWwindow * window, int width, int height );

int processInput( GLFWwindow * window, int current, const std::shared_ptr<SpotLightSource> & heliLight,
				  const glm::vec3 & heliFront, const glm::vec3 & heliRight );

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

	Shader shader( "../src/shaders/phong" );
	Shader geoPassShader( "../src/shaders/phong/vertex.shader", "../src/shaders/phong/geopass.shader" );

	glm::mat4 projection = glm::perspective( glm::radians( 45.0f ), 800.0f / 600.0f, 0.1f, 100.0f );

	std::shared_ptr<Camera> camera = std::make_shared<Camera>(
			glm::vec3( 8.0f, 8.0f, 8.0f ), glm::vec3( -1.0f, -1.0f, -1.0f ), glm::vec3( 0.0f, 1.0f, 0.0f )
	);

	auto pointLights = createPointLightSources();
	LightSourceSet lightSet( pointLights.size(), 0 );
	for ( auto & light : pointLights )
		lightSet.addPointLightSource( light );

	Sun sun = createSun();

	geoPassShader.bind();
	geoPassShader.setMatrix( "projection", projection );

	shader.bind();
	shader.setMatrix( "projection", projection );
	shader.setFloat( "shininess", 64.0f );
	lightSet.setInShader( shader );

	ModelLoader modelLoader;
	std::shared_ptr<SimpleModel> model = modelLoader.loadModel( "models/backpack/backpack.obj" );

	glCall( glEnable( GL_DEPTH_TEST ) );

	// render loop
	while ( !glfwWindowShouldClose( window ) )
	{
		// render
		glCall( glClearColor( 0.1f, 0.1f, 0.1f, 1.0f ) );
		glCall( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) );

		glm::mat4 view = camera->viewMatrix();

		shader.bind();
		shader.setMatrix( "view", view );
		shader.setVector( "viewPos", camera->getPosition() );

		sun.move();
		sun.setInShader( shader );
		lightSet.setInShader( shader );

		model->draw( shader );

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glCall( glfwSwapBuffers( window ) );
		glCall( glfwPollEvents() );
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
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
