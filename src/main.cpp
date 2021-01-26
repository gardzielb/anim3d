#include "dependencies/glad.h"
#include "glUtils.h"
#include "Shader.h"
#include "Camera.h"
#include "LightSource.h"
#include "Model.h"
#include "ModelLoader.h"

#include <GLFW/glfw3.h>

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#define STB_IMAGE_IMPLEMENTATION

#include "dependencies/stb_image.h"
#include "Sun.h"
#include "ComplexModel.h"

#include <spdlog/spdlog.h>


void framebuffer_size_callback( GLFWwindow * window, int width, int height );

void processInput( GLFWwindow * window, Camera & camera );

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;


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
	Shader lightShader( "../src/shaders/light" );

	glm::vec3 lightPosition( 0.5f, 0.5f, 0.5f );

	glm::mat4 projection = glm::perspective( glm::radians( 45.0f ), 800.0f / 600.0f, 0.1f, 100.0f );

	Camera camera( glm::vec3( 0.0f, 0.0f, 3.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ), 0.1f );

	Light sunLight = { glm::vec3( 0.05f, 0.05f, 0.05f ), glm::vec3( 0.4f, 0.4f, 0.4f ), glm::vec3( 0.6f, 0.6f, 0.6f ) };
	Light iceBallLight = {
			glm::vec3( 0.05f, 0.05f, 0.05f ), glm::vec3( 0.8f, 0.8f, 0.8f ), glm::vec3( 1.0f, 1.0f, 1.0f )
	};
	Light flashLight = {
			glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 1.0f, 1.0f, 1.0f ), glm::vec3( 1.0f, 1.0f, 1.0f )
	};

	Sun sun( sunLight, 0.1f );
	PointLightSource pointLight( iceBallLight, lightPosition, 0.09f, 0.032f );
	SpotLightSource spotLight(
			flashLight, camera.getPosition(), camera.getFront(), glm::radians( 12.5 ), glm::radians( 15.0 ), 0.09f,
			0.032f
	);

	LightSourceSet lightSet( 1, 1 );
	lightSet.addPointLightSource( pointLight );
	lightSet.addSpotLightSource( spotLight );

	lightShader.bind();
	lightShader.setMatrix( "projection", projection );

	shader.bind();
	shader.setMatrix( "projection", projection );
	shader.setFloat( "material.shininess", 64.0f );
	lightSet.setInShader( shader );

	glm::mat4 model = glm::mat4( 1.0f );
	model = glm::translate(
			model, glm::vec3( 0.0f, 0.0f, 0.0f )
	); // translate it down so it's at the center of the scene
	model = glm::scale( model, glm::vec3( 1.0f, 1.0f, 1.0f ) );    // it's a bit too big for our scene, so scale it down
	shader.setMatrix( "model", model );

	typedef std::shared_ptr<SimpleModel> ModelPtr;
	ModelLoader modelLoader;

	ModelPtr statue = modelLoader.loadModel( "models/orc_statue/orc_statue.obj" );
	if ( !statue )
		exit( 1 );
	statue->scale( glm::vec3( 0.4f, 0.4f, 0.4f ) );

	ModelPtr ground = modelLoader.loadModel( "models/ground/ground2.obj" );
	if ( !ground )
		exit( 1 );
	ground->translate( glm::vec3( 0.0f, -0.5f, 0.0f ) );

	ModelPtr iceBall = modelLoader.loadModel( "models/ice_ball/ice_ball.obj" );
	if ( !iceBall )
		exit( 1 );
	iceBall->translate( lightPosition ).scale( glm::vec3( 0.1f, 0.1f, 0.1f ) );

	ModelPtr building = modelLoader.loadModel( "models/building01/building01.obj" );
	if ( !building )
		exit( 1 );
	building->translate( glm::vec3( -4.0f, -0.5f, 0.0f ) );

	ModelPtr pripyat = modelLoader.loadModel( "models/pripyat/pripyat.obj" );
	if ( !pripyat )
		exit( 1 );
	pripyat->translate( glm::vec3( 0.0f, -0.6f, -2.0f ) );

	ModelPtr mi28NoDisc = modelLoader.loadModel( "models/mi28/mi28.obj" );
	if ( !mi28NoDisc )
		exit( 1 );

	ModelPtr disc = modelLoader.loadModel( "models/mi28/disc.obj" );
	if ( !disc )
		exit( 1 );

	ComplexModelBuilder cmBuilder;
	auto mi28 = cmBuilder.addPart( mi28NoDisc, std::make_shared<NoModelPartBehavior>() )
						 .addPart(
								 disc, std::make_shared<RotationModelPartBehavior>(
										 5.0f, glm::vec3( 0.0f, 1.0f, 0.0f )
								 ), glm::vec3( 0.0f, 0.62f, 0.4f )
						 )
						 .construct();
	mi28->scale( 0.2f, 0.2f, 0.2f ).translate( -15.0f, 10.0f, 0.0f );

	glCall( glEnable( GL_DEPTH_TEST ) );

	glm::vec3 transVector( 0.02f, -0.5f, 0.0f );
	float angle = 0.8f;

	// render loop
	while ( !glfwWindowShouldClose( window ) )
	{
		// input
		processInput( window, camera );

		// render
		glCall( glClearColor( 0.2f, 0.2f, 0.2f, 1.0f ) );
		glCall( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) );

		glm::mat4 view = camera.viewMatrix();

		lightShader.bind();
		lightShader.setMatrix( "view", view );
		iceBall->draw( lightShader );

		shader.bind();
		shader.setMatrix( "view", view );
		shader.setVector( "viewPos", camera.getPosition() );
		shader.setVector( "spotLights[0].position", camera.getPosition() );
		shader.setVector( "spotLights[0].direction", camera.getFront() );

		sun.move();
		sun.setInShader( shader, "directionalLight" );

		transVector += glm::vec3( 0.0f, 0.0f, 0.02f );
//		angle += 0.1f;
		mi28->translate( 0.0f, 0.0f, 0.2f )
			.rotate( angle, 0.0f, 1.0f, 0.0f );

		ground->draw( shader );
		statue->draw( shader );
		building->draw( shader );
		pripyat->draw( shader );
		mi28->draw( shader );

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glCall( glfwSwapBuffers( window ) );
		glCall( glfwPollEvents() );
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput( GLFWwindow * window, Camera & camera )
{
	if ( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
		glfwSetWindowShouldClose( window, true );

	if ( glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS )
	{
		camera.moveForward();
	}
	if ( glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS )
	{
		camera.moveBack();
	}
	if ( glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS )
	{
		camera.moveLeft();
	}
	if ( glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS )
	{
		camera.moveRight();
	}

	double xPos, yPos;
	glfwGetCursorPos( window, &xPos, &yPos );

	static bool firstMouse = true;
	static float lastX, lastY;

	if ( firstMouse )
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos; // reversed since y-coordinates go from bottom to top
	lastX = xPos;
	lastY = yPos;

	camera.rotate( xOffset, yOffset );
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback( GLFWwindow * window, int width, int height )
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport( 0, 0, width, height );
}
