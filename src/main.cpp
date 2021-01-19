#include "dependencies/glad.h"
#include "glUtils.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Camera.h"
#include "LightSource.h"
#include "Model.h"
#include "ModelLoader.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <array>
#include <optional>

#define STB_IMAGE_IMPLEMENTATION

#include "dependencies/stb_image.h"

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
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent( window );
	glfwSetFramebufferSizeCallback( window, framebuffer_size_callback );

	// glad: load all OpenGL function pointers
	if ( !gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress ) )
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	stbi_set_flip_vertically_on_load( true );

	float lightVertices[] = {
			-0.5f, -0.5f, -0.5,
			0.5f, -0.5f, -0.5f,
			0.5f, 0.5f, -0.5f,
			-0.5f, 0.5f, -0.5f,

			-0.5f, -0.5f, 0.5f,
			0.5f, -0.5f, 0.5f,
			0.5f, 0.5f, 0.5f,
			-0.5f, 0.5f, 0.5f,

			-0.5f, 0.5f, 0.5f,
			-0.5f, 0.5f, -0.5f,
			-0.5f, -0.5f, -0.5,
			-0.5f, -0.5f, 0.5f,

			0.5f, 0.5f, 0.5f,
			0.5f, 0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, 0.5f,

			-0.5f, -0.5f, -0.5,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, 0.5f,
			-0.5f, -0.5f, 0.5f,

			-0.5f, 0.5f, -0.5f,
			0.5f, 0.5f, -0.5f,
			0.5f, 0.5f, 0.5f,
			-0.5f, 0.5f, 0.5f,
	};
	int indices[] = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4,
			8, 9, 10, 10, 11, 8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,
			20, 21, 22, 22, 23, 20
	};

	IndexBuffer ib( indices, 6 * 6 );

	VertexArray lightVa;
	VertexBuffer lightVb( lightVertices, 4 * 6 * 3 * sizeof( float ) );
	VertexBufferLayout lightLayout;
	lightLayout.pushFloat( 3 );  // position
	lightVa.addBuffer( lightVb, lightLayout );

	Shader shader( "../src/shaders/phong" );
	Shader lightShader( "../src/shaders/light" );

	glm::vec3 lightPosition( 1.2f, 1.0f, 2.0f );
	glm::mat4 lightModel = glm::mat4( 1.0f );
	lightModel = glm::translate( lightModel, lightPosition );
	lightModel = glm::scale( lightModel, glm::vec3( 0.2f ) );

	glm::mat4 projection = glm::perspective( glm::radians( 45.0f ), 800.0f / 600.0f, 0.1f, 100.0f );

	Camera camera( glm::vec3( 0.0f, 0.0f, 3.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ), 0.1f );

	DirectionalLightSource sun;
	sun.direction = glm::vec3( -0.2f, -1.0f, -0.3f );
	sun.ambient = glm::vec3( 0.05f, 0.05f, 0.05f );
	sun.diffuse = glm::vec3( 0.4f, 0.4f, 0.4f );
	sun.specular = glm::vec3( 0.5f, 0.5f, 0.5f );

	PointLightSource pointLight;
	pointLight.position = lightPosition;
	pointLight.constant = 1.0f;
	pointLight.linear = 0.09f;
	pointLight.quadratic = 0.032f;
	pointLight.ambient = glm::vec3( 0.05f, 0.05f, 0.05f );
	pointLight.diffuse = glm::vec3( 0.8f, 0.8f, 0.8f );
	pointLight.specular = glm::vec3( 1.0f, 1.0f, 1.0f );

	SpotLightSource spotLight;
	spotLight.position = camera.getPosition();
	spotLight.direction = camera.getFront();
	spotLight.constant = 1.0f;
	spotLight.linear = 0.09f;
	spotLight.quadratic = 0.032f;
	spotLight.ambient = glm::vec3( 0.0f, 0.0f, 0.0f );
	spotLight.diffuse = glm::vec3( 1.0f, 1.0f, 0.5f );
	spotLight.specular = glm::vec3( 1.0f, 1.0f, 0.5f );
	spotLight.innerCutOff = glm::radians( 12.5 );
	spotLight.outerCutOff = glm::radians( 15.0 );

	LightSourceSet lightSet( sun );
	lightSet.addPointLightSource( pointLight );
	lightSet.addSpotLightSource( spotLight );

	shader.bind();
	shader.setMatrix( "projection", projection );
	lightSet.addToShader( shader );
	shader.setFloat( "material.shininess", 64.0f );

	glm::mat4 model = glm::mat4( 1.0f );
	model = glm::translate(
			model, glm::vec3( 0.0f, 0.0f, 0.0f )
	); // translate it down so it's at the center of the scene
	model = glm::scale( model, glm::vec3( 1.0f, 1.0f, 1.0f ) );    // it's a bit too big for our scene, so scale it down
	shader.setMatrix( "model", model );

	lightShader.bind();
	lightShader.setMatrix( "projection", projection );
	lightShader.setMatrix( "model", lightModel );

	ModelLoader modelLoader;
	std::optional<SimpleModel> modelOpt = modelLoader.loadModel( "backpack/backpack.obj", glm::mat4( 1.0f ) );
	if ( !modelOpt )
		exit( 1 );
	SimpleModel backpack = modelOpt.value();
	backpack.rotate( 180, glm::vec3( 0.0f, 1.0f, 0.0f ) )
			.scale( glm::vec3( 0.2f, 0.2f, 0.2f ) )
			.translate( glm::vec3( 1.0f, 0.0f, 0.0f ) );
//	SimpleModel backpack( "sleeper.obj" );

	glCall( glEnable( GL_DEPTH_TEST ) );

	// render loop
	while ( !glfwWindowShouldClose( window ) )
	{
		// input
		processInput( window, camera );

		// render
		glCall( glClearColor( 0.2f, 0.2f, 0.2f, 1.0f ) );
		glCall( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) );

		glm::mat4 view = camera.viewMatrix();
		ib.bind();

		lightVa.bind();
		lightShader.bind();
		lightShader.setMatrix( "view", view );

		glCall( glDrawElements( GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr ) );

		shader.bind();
		shader.setMatrix( "view", view );
		shader.setVector( "viewPos", camera.getPosition() );
		shader.setVector( "spotLights[0].position", camera.getPosition() );
		shader.setVector( "spotLights[0].direction", camera.getFront() );

		backpack.draw( shader );

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
