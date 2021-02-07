//
// Created by bartosz on 2/6/21.
//

#define STB_IMAGE_IMPLEMENTATION

#include "Application.h"
#include "Timer.h"
#include "dependencies/stb_image.h"
#include "Renderer.h"
#include "glUtils.h"
#include "Camera.h"
#include "Model.h"
#include "dependencies/imgui/imgui.h"
#include "dependencies/imgui/imgui_impl_glfw_gl3.h"


Application::Application( int width, int height )
		: width( width ), height( height )
{
	initOpenGl();
	stbi_set_flip_vertically_on_load( true );
}

void Application::run()
{
	auto spotLight = createSpotLight();
	Sun sun = createSun();
	Fog fog = { 0.08f, glm::vec3( 0.6f, 0.6f, 0.6f ) };

	ModelLoader modelLoader;
	auto models = createStaticModels( modelLoader );
	std::shared_ptr<ComplexModel> mi28 = createChopper( modelLoader, spotLight );
	models.push_back( mi28 );

	auto cameras = createCameras( mi28 );

	auto[pointLights, lightsModel] = createPointLights( modelLoader );

	LightSourceSet lightSet( pointLights.size(), 1 );
	lightSet.addSpotLightSource( spotLight );
	for ( auto & light : pointLights )
		lightSet.addPointLightSource( light );

	glCall( glEnable( GL_DEPTH_TEST ) );
	glCall( glDisable( GL_BLEND ) );

	DeferredRenderer dsRenderer(
			"../src/shaders/phong/geopass", "../src/shaders/phong/lighting", "../src/shaders/light/default",
			width, height
	);
	ForwardRenderer fwRenderer( "../src/shaders/phong/forward", "../src/shaders/light/default" );
	ForwardRenderer gsRenderer( "../src/shaders/gouraud/default", "../src/shaders/light/default" );

	std::array<Renderer *, 3> renderers = { &dsRenderer, &fwRenderer, &gsRenderer };
	int rendererIndex = 0;

	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init( window, true );
	ImGui::StyleColorsDark();

//    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	int cameraIndex = 0;
	Timer timer;

	// render loop
	while ( !glfwWindowShouldClose( window ) )
	{
		timer.loop();

		ImGui_ImplGlfwGL3_NewFrame();

		// render
//		glCall( glClearColor( fog.color.r, fog.color.g, fog.color.b, fog.density ) );
		glCall( glClearColor( 0.0f, 0.0f, 0.0f, 1.0f ) );
		glCall( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) );

		glm::mat4 viewMatrix = cameras[cameraIndex]->viewMatrix();
		sun.move();
		mi28->translate( 0.0f, 0.0f, 0.1f ).rotate( 0.3f, 0.0f, 0.3f, 0.0f );

		renderers[rendererIndex]->renderScene(
				models, { lightsModel }, sun, lightSet, fog, cameras[cameraIndex]
		);

		// ImGui window
		{
			ImGui::Begin( "Control panel" );

			const char * shadingOptions[] = { "Phong deferred", "Phong forward", "Gouraud" };
			ImGui::Combo( "Shading", &rendererIndex, shadingOptions, 3 );

			const char * cameraNames[] = { "static", "chopper", "spy" };
			ImGui::Combo( "Camera", &cameraIndex, cameraNames, 3 );

			ImGui::SliderFloat( "Fog density", &fog.density, 0.0f, 1.0f );
			ImGui::ColorEdit3( "Fog color", (float *) &fog.color );

			ImGui::Text(
					"Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
					ImGui::GetIO().Framerate
			);

			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData( ImGui::GetDrawData() );

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glCall( glfwSwapBuffers( window ) );
		glCall( glfwPollEvents() );

		// input
		glm::vec3 heliPos = mi28->offset();
		glm::vec3 heliDir = glm::cross( -heliPos, glm::vec3( 0.0f, 1.0f, 0.0f ) );
		processInput( spotLight, heliDir, -heliPos );
	}
}

Application::~Application()
{
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();

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

std::array<std::shared_ptr<Camera>, 3> Application::createCameras( const std::shared_ptr<Model> & chopper )
{
	std::shared_ptr<Camera> staticCamera = std::make_shared<Camera>(
			glm::vec3( 8.0f, 8.0f, 8.0f ), glm::vec3( -1.0f, -1.0f, -1.0f ), glm::vec3( 0.0f, 1.0f, 0.0f )
	);
	std::shared_ptr<ModelBoundCamera> chopperCamera = std::make_shared<ModelBoundCamera>(
			glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 0.3f, -1.0f, 0.3f ), glm::vec3( 0.0f, 1.0f, 0.0f )
	);
	std::shared_ptr<ModelObservingCamera> orcCamera = std::make_shared<ModelObservingCamera>(
			glm::vec3( 0.0f, 1.0f, 0.0f ), glm::vec3( 1.0f, 0.0f, 0.0f ), glm::vec3( 0.0f, 1.0f, 0.0f )
	);

	chopper->addObserver( chopperCamera, glm::vec3( 0.0f, -0.5f, 0.0f ) );
	chopper->addObserver( orcCamera );

	return { staticCamera, chopperCamera, orcCamera };
}

std::shared_ptr<SpotLightSource> Application::createSpotLight()
{
	Light flashLight = {
			glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 1.0f, 1.0f, 1.0f ), glm::vec3( 1.0f, 1.0f, 1.0f )
	};
	return std::make_shared<SpotLightSource>(
			flashLight, glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 0.3f, -1.0f, 0.3f ), glm::radians( 12.5 ),
			glm::radians( 15.0 ), 0.09f, 0.032f
	);
}

static std::shared_ptr<SimpleModel> loadModel( ModelLoader & loader, const std::string & path )
{
	std::shared_ptr<SimpleModel> model = loader.loadModel( path );
	if ( !model )
		exit( 1 );
	return model;
}

static std::shared_ptr<RepeatedModel> createBuildings( ModelLoader & loader )
{
	std::shared_ptr<SimpleModel> building = loadModel( loader, "models/building01/building01.obj" );
	std::vector<glm::vec3> positions = {
			glm::vec3( -4.0f, -0.5f, 0.0f ),
			glm::vec3( 4.0f, -0.5f, 0.0f ),
			glm::vec3( 0.0f, -0.5f, -4.0f ),
			glm::vec3( 0.0f, -0.5f, 4.0f ),
			glm::vec3( -4.0f, -0.5f, -4.0f ),
			glm::vec3( 4.0f, -0.5f, -4.0f ),
			glm::vec3( -4.0f, -0.5f, 4.0f ),
			glm::vec3( 4.0f, -0.5f, 4.0f ),
			glm::vec3( -2.0f, -0.5f, -4.0f ),
			glm::vec3( -2.0f, -0.5f, 0.0f ),
			glm::vec3( -2.0f, -0.5f, 4.0f ),
			glm::vec3( 2.0f, -0.5f, -4.0f ),
			glm::vec3( 2.0f, -0.5f, 0.0f ),
			glm::vec3( 2.0f, -0.5f, 4.0f )
	};
	return std::make_shared<RepeatedModel>( building, positions );
}

std::vector<std::shared_ptr<Model>> Application::createStaticModels( ModelLoader & loader )
{
	std::shared_ptr<SimpleModel> statue = loadModel( loader, "models/orc_statue/orc_statue.obj" );
	statue->scale( glm::vec3( 0.4f, 0.4f, 0.4f ) );

	std::shared_ptr<SimpleModel> ground = loadModel( loader, "models/ground/ground2.obj" );
	ground->translate( glm::vec3( 0.0f, -0.5f, 0.0f ) ).scale( 3.0f, 3.0f, 3.0f );

	std::shared_ptr<SimpleModel> pripyat = loadModel( loader, "models/pripyat/pripyat.obj" );
	pripyat->translate( glm::vec3( 0.0f, -0.6f, -2.0f ) );

	std::shared_ptr<RepeatedModel> buildings = createBuildings( loader );

	return { ground, statue, pripyat, buildings };
}

std::shared_ptr<ComplexModel> Application::createChopper( ModelLoader & loader,
														  const std::shared_ptr<SpotLightSource> & light )
{
	std::shared_ptr<SimpleModel> mi28NoDisc = loadModel( loader, "models/mi28/mi28.obj" );
	std::shared_ptr<SimpleModel> disc = loadModel( loader, "models/mi28/disc.obj" );

	ComplexModelBuilder cmBuilder;
	auto mi28 = cmBuilder.addPart( mi28NoDisc, std::make_shared<NoModelPartBehavior>() )
						 .addPart(
								 disc, std::make_shared<RotationModelPartBehavior>(
										 5.0f, glm::vec3( 0.0f, 1.0f, 0.0f )
								 ), glm::vec3( 0.0f, 0.62f, 0.4f )
						 )
						 .construct();

	mi28->scale( 0.2f, 0.2f, 0.2f ).translate( -20.0f, 15.0f, 0.0f );
	mi28->addObserver( light, glm::vec3( 0.0f, -0.5f, 0.0f ) );
	return mi28;
}

std::pair<std::vector<std::shared_ptr<PointLightSource>>, std::shared_ptr<RepeatedModel>>
Application::createPointLights( ModelLoader & loader )
{
	const int count = 64;
	std::vector<glm::vec3> positions;
	positions.reserve( count );

	for ( int x = 3; x < 16; x += 4 )
	{
		for ( int z = 3; z < 16; z += 4 )
		{
			positions.push_back( glm::vec3( x, -0.3f, z ) );
			positions.push_back( glm::vec3( -x, -0.3f, z ) );
			positions.push_back( glm::vec3( x, -0.3f, -z ) );
			positions.push_back( glm::vec3( -x, -0.3f, -z ) );
		}
	}

	std::vector<std::shared_ptr<PointLightSource>> lights;
	lights.reserve( count );

	Light iceBallLight = {
			glm::vec3( 0.05f, 0.05f, 0.05f ), glm::vec3( 0.8f, 0.8f, 0.8f ), glm::vec3( 1.0f, 1.0f, 1.0f )
	};

//	for ( int i = 0; i < count; i++ )
//	{
//		auto pointLight = std::make_shared<PointLightSource>( iceBallLight, positions[i], 0.09f, 0.032f );
//		lights.push_back( pointLight );
//	}

	auto model = loadModel( loader, "models/ice_ball/ice_ball.obj" );
	auto lightsModel = std::make_shared<RepeatedModel>( model, positions, glm::vec3( 0.1f, 0.1f, 0.1f ) );

	return std::make_pair( lights, lightsModel );
}

Sun Application::createSun()
{
	Light sunLight = { glm::vec3( 0.05f, 0.05f, 0.05f ), glm::vec3( 0.4f, 0.4f, 0.4f ), glm::vec3( 0.6f, 0.6f, 0.6f ) };
	return Sun( sunLight, 0.5f );
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void Application::processInput( const std::shared_ptr<SpotLightSource> & heliLight, const glm::vec3 & heliFront,
								const glm::vec3 & heliRight )
{
	if ( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
		glfwSetWindowShouldClose( window, true );

	if ( glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS )
		heliLight->rotate( -heliRight, 0.01f );
	if ( glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS )
		heliLight->rotate( heliRight, 0.01f );
	if ( glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS )
		heliLight->rotate( heliFront, 0.01f );
	if ( glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS )
		heliLight->rotate( -heliFront, 0.01f );

//	if ( glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS )
//		camera->rotate( 0.0f, 2.0f );
//	if ( glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS )
//		camera->rotate( 0.0f, -2.0f );
//	if ( glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS )
//		camera->rotate( 2.0f, 0.0f );
//	if ( glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS )
//		camera->rotate( -2.0f, 0.0f );
}
