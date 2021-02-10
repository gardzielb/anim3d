//
// Created by bartosz on 2/6/21.
//

#define STB_IMAGE_IMPLEMENTATION

#include "dependencies/stb_image.h"

#include "Application.h"
#include "api/Timer.h"
#include "scene/Renderer.h"
#include "api/glUtils.h"
#include "scene/Model.h"
#include "SceneBuilder.h"
#include "GuiSceneController.h"

#include <spdlog/spdlog.h>


Application::Application( int width, int height )
		: width( width ), height( height )
{
	initOpenGl();
	stbi_set_flip_vertically_on_load( true );
}

void Application::run( int lightCount )
{
	SceneBuilder builder;
	auto spotLight = builder.createSpotLight();
	Sun sun = builder.createSun();

	auto models = builder.createStaticModels();
	std::shared_ptr<ComplexModel> mi28 = builder.createChopper( spotLight );
	models.push_back( mi28 );

	auto[pointLights, lightsModel] = builder.createPointLights( lightCount );

	LightSourceSet lightSet( pointLights.size(), 1 );
	lightSet.addSpotLightSource( spotLight );
	for ( auto & light : pointLights )
		lightSet.addPointLightSource( light );

	glCall( glEnable( GL_DEPTH_TEST ) );
	glCall( glDisable( GL_BLEND ) );

	GuiSceneController controller(
			window, builder.createRenderers( "../src/shaders/", width, height ), builder.createCameras( mi28 ),
			{ 0.05f, glm::vec3( 0.8f, 0.8f, 0.8f ) }
	);

	// render loop
	while ( !glfwWindowShouldClose( window ) )
	{
		glm::mat4 viewMatrix = controller.getCamera()->viewMatrix();
		sun.move();
		mi28->translate( 0.0f, 0.0f, 0.1f ).rotate( 0.3f, 0.0f, 0.3f, 0.0f );

		controller.getRenderer()->renderScene(
				models, { lightsModel }, sun, lightSet, controller.getFog(), controller.getCamera()
		);

		controller.render( "Control panel" );

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glCall( glfwSwapBuffers( window ) );
		glCall( glfwPollEvents() );

		// input
		glm::vec3 heliPos = mi28->offset();
		glm::vec3 heliDir = glm::cross( -heliPos, glm::vec3( 0.0f, 1.0f, 0.0f ) );
		processInput( spotLight, heliDir, -heliPos, controller.getCamera(), controller.getRenderer() );
	}
}

Application::~Application()
{
	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
static void framebuffer_size_callback( GLFWwindow * window, int width, int height )
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport( 0, 0, width, height );
}

void Application::initOpenGl()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	// glfw window creation
	window = glfwCreateWindow( width, height, "Pripyat, Khorinis", nullptr, nullptr );
	if ( !window )
	{
		spdlog::error( "Failed to create GLFW window" );
		glfwTerminate();
		exit( 1 );
	}
	glfwMakeContextCurrent( window );
	glfwSetFramebufferSizeCallback( window, framebuffer_size_callback );

	if ( glewInit() != GLEW_OK )
		throw std::runtime_error( "Failed to init GLEW" );
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void Application::processInput( const std::shared_ptr<SpotLightSource> & heliLight, const glm::vec3 & heliFront,
								const glm::vec3 & heliRight, std::shared_ptr<Camera> & camera,
								std::shared_ptr<Renderer> & renderer )
{
	if ( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
		glfwSetWindowShouldClose( window, true );

	if ( glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS )
		heliLight->rotate( -heliRight, 0.01f );
	if ( glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS )
		heliLight->rotate( heliRight, 0.01f );
	if ( glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS )
		heliLight->rotate( heliFront, 0.01f );
	if ( glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS )
		heliLight->rotate( -heliFront, 0.01f );

	if ( glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS )
		camera->moveForward();
	if ( glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS )
		camera->moveBack();
	if ( glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS )
		camera->moveRight();
	if ( glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS )
		camera->moveLeft();

	if ( glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_RIGHT ) == GLFW_PRESS )
		camera->setRotationLock( true );
	if ( glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_MIDDLE ) == GLFW_PRESS )
		camera->setRotationLock( false );

	double xMouse, yMouse;
	glCall( glfwGetCursorPos( window, &xMouse, &yMouse ) );
	camera->handleMouseMove( xMouse, yMouse );

	if ( glfwGetKey( window, GLFW_KEY_EQUAL ) == GLFW_PRESS )
		renderer->zoom( -0.5f );
	if ( glfwGetKey( window, GLFW_KEY_MINUS ) == GLFW_PRESS )
		renderer->zoom( 0.5f );
}
