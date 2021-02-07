//
// Created by bartosz on 2/7/21.
//

#include "SceneBuilder.h"


std::array<std::shared_ptr<Camera>, 3> SceneBuilder::createCameras( const std::shared_ptr<Model> & chopper )
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

std::shared_ptr<SpotLightSource> SceneBuilder::createSpotLight()
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

std::vector<std::shared_ptr<Model>> SceneBuilder::createStaticModels()
{
	std::shared_ptr<SimpleModel> statue = loadModel( modelLoader, "models/orc_statue/orc_statue.obj" );
	statue->scale( glm::vec3( 0.4f, 0.4f, 0.4f ) );

	std::shared_ptr<SimpleModel> ground = loadModel( modelLoader, "models/ground/ground2.obj" );
	ground->translate( glm::vec3( 0.0f, -0.5f, 0.0f ) ).scale( 3.0f, 3.0f, 3.0f );

	std::shared_ptr<SimpleModel> pripyat = loadModel( modelLoader, "models/pripyat/pripyat.obj" );
	pripyat->translate( glm::vec3( 0.0f, -0.6f, -2.0f ) );

	std::shared_ptr<RepeatedModel> buildings = createBuildings( modelLoader );

	return { ground, statue, pripyat, buildings };
}

std::shared_ptr<ComplexModel> SceneBuilder::createChopper( const std::shared_ptr<SpotLightSource> & light )
{
	std::shared_ptr<SimpleModel> mi28NoDisc = loadModel( modelLoader, "models/mi28/mi28.obj" );
	std::shared_ptr<SimpleModel> disc = loadModel( modelLoader, "models/mi28/disc.obj" );

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
SceneBuilder::createPointLights()
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
//		auto pointLight = std::make_shared<PointLightSource>( iceBallLight, positions[i], 0.5f, 0.064f );
//		lights.push_back( pointLight );
//	}

	auto model = loadModel( modelLoader, "models/ice_ball/ice_ball.obj" );
	auto lightsModel = std::make_shared<RepeatedModel>( model, positions, glm::vec3( 0.1f, 0.1f, 0.1f ) );

	return std::make_pair( lights, lightsModel );
}

Sun SceneBuilder::createSun()
{
	Light sunLight = { glm::vec3( 0.05f, 0.05f, 0.05f ), glm::vec3( 0.4f, 0.4f, 0.4f ), glm::vec3( 0.6f, 0.6f, 0.6f ) };
	return Sun( sunLight, 0.3f );
}

std::array<std::shared_ptr<Renderer>, 3> SceneBuilder::createRenderers( const std::string & shadersPath,
																		int windowWidth, int windowHeight )
{
	auto dsRenderer = std::make_shared<DeferredRenderer>(
			shadersPath + "phong/geopass", shadersPath + "phong/lighting", shadersPath + "light/default",
			windowWidth, windowHeight
	);
	auto fwRenderer = std::make_shared<ForwardRenderer>(
			shadersPath + "phong/forward", shadersPath + "light/default", windowWidth, windowHeight
	);
	auto gsRenderer = std::make_shared<ForwardRenderer>(
			shadersPath + "gouraud/default", shadersPath + "light/default", windowWidth, windowHeight
	);

	return { dsRenderer, fwRenderer, gsRenderer };
}
