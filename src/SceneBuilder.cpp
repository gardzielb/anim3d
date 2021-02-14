//
// Created by bartosz on 2/7/21.
//

#include "SceneBuilder.h"


std::array<std::shared_ptr<Camera>, 3> SceneBuilder::createCameras( const std::shared_ptr<Model> & chopper )
{
	std::shared_ptr<Camera> staticCamera = std::make_shared<Camera>(
			glm::vec3( 8.0f, 8.0f, 8.0f ), glm::vec3( -1.0f, -1.0f, -1.0f ),
			std::make_shared<SimpleRotationBehavior>(), std::make_shared<SimpleXYMoveBehavior>( 0.2f )
	);
	std::shared_ptr<ModelBoundCamera> chopperCamera = std::make_shared<ModelBoundCamera>(
			glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 0.3f, -1.0f, 0.3f )
	);
	std::shared_ptr<ModelObservingCamera> orcCamera = std::make_shared<ModelObservingCamera>(
			glm::vec3( 0.0f, 1.5f, 0.0f ), glm::vec3( 1.0f, 0.0f, 0.0f )
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

static std::shared_ptr<RepeatedModel> createGround( ModelLoader & loader )
{
	int sectorRowCount = 18;
	int blockSize = 6;

	std::vector<glm::vec3> positions;
	positions.reserve( sectorRowCount * sectorRowCount * 4 );

	for ( int x = 0; x < sectorRowCount * blockSize; x += blockSize )
	{
		for ( int z = 0; z < sectorRowCount * blockSize; z += blockSize )
		{
			positions.push_back( glm::vec3( x + blockSize / 2, 0.0f, z + blockSize / 2 ) );
			positions.push_back( glm::vec3( -x - blockSize / 2, 0.0f, z + blockSize / 2 ) );
			positions.push_back( glm::vec3( x + blockSize / 2, 0.0f, -z - blockSize / 2 ) );
			positions.push_back( glm::vec3( -x - blockSize / 2, 0.0f, -z - blockSize / 2 ) );
		}
	}

	return std::make_shared<RepeatedModel>( loader.loadModel( "../models/ground/ground3.obj" ), positions );
}

static std::vector<std::shared_ptr<RepeatedModel>> createBuildings( ModelLoader & loader )
{
	int sectorRowCount = 8;
	int step = 2;

	std::vector<glm::vec3> pos1, pos2;
	pos1.reserve( sectorRowCount * sectorRowCount * 4 );
	pos2.reserve( sectorRowCount * sectorRowCount * 4 );

	for ( int x = 0; x < sectorRowCount * step; x += step )
	{
		for ( int z = 0; z < sectorRowCount * step; z += step )
		{
			int choice = rand() % 2;
			std::vector<glm::vec3> & posVector = choice ? pos1 : pos2;

			posVector.push_back( glm::vec3( x + step, 0.0f, z + step ) );
			posVector.push_back( glm::vec3( -x - step, 0.0f, z + step ) );
			posVector.push_back( glm::vec3( x + step, 0.0f, -z - step ) );
			posVector.push_back( glm::vec3( -x - step, 0.0f, -z - step ) );
		}
	}

	std::shared_ptr<SimpleModel> building1 = loadModel( loader, "../models/building02/building22.obj" );
	std::shared_ptr<SimpleModel> building2 = loadModel( loader, "../models/building01/building01.obj" );

	return { std::make_shared<RepeatedModel>( building1, pos1 ),
			 std::make_shared<RepeatedModel>( building2, pos2 ) };
}

std::vector<std::shared_ptr<Model>> SceneBuilder::createStaticModels( bool hardcoreGround )
{
	std::shared_ptr<SimpleModel> statue = loadModel( modelLoader, "../models/orc_statue/orc_statue2.obj" );
	statue->translate( 0.0f, 0.5f, 0.0f ).scale( glm::vec3( 0.4f, 0.4f, 0.4f ) );

	std::shared_ptr<Model> ground;
	if ( hardcoreGround )
		ground = createGround( modelLoader );
	else
	{
		ground = loadModel( modelLoader, "../models/ground/ground4.obj" );
		ground->scale( 120.0f, 120.0f, 120.0f );
	}

	std::shared_ptr<SimpleModel> pripyat = loadModel( modelLoader, "../models/pripyat/pripyat.obj" );
	pripyat->translate( 0.0f, -0.1f, -2.0f );

	std::vector<std::shared_ptr<RepeatedModel>> buildings = createBuildings( modelLoader );

	return { ground, statue, pripyat, buildings[0], buildings[1] };
}

std::shared_ptr<ComplexModel> SceneBuilder::createChopper( const std::shared_ptr<SpotLightSource> & light )
{
	std::shared_ptr<SimpleModel> mi28NoDisc = loadModel( modelLoader, "../models/mi28/mi28.obj" );
	std::shared_ptr<SimpleModel> disc = loadModel( modelLoader, "../models/mi28/disc.obj" );

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
SceneBuilder::createPointLights( int count )
{
	std::vector<glm::vec3> positions;
	positions.reserve( count );

	int loopCount = std::max( 1, (int) std::ceil( std::log2( count / 4 ) ) );
	int step = 16 / loopCount;

	for ( int x = 0; x < 16; x += step )
	{
		for ( int z = 0; z < 16; z += step )
		{
			positions.push_back( glm::vec3( x + 3, 0.2f, z + 3 ) );
			positions.push_back( glm::vec3( -x - 3, 0.2f, z + 3 ) );
			positions.push_back( glm::vec3( x + 3, 0.2f, -z - 3 ) );
			positions.push_back( glm::vec3( -x - 3, 0.2f, -z - 3 ) );
		}
	}

	std::vector<std::shared_ptr<PointLightSource>> lights;
	lights.reserve( count );

	Light iceBallLight = {
			glm::vec3( 0.05f, 0.05f, 0.05f ), glm::vec3( 0.8f, 0.8f, 0.8f ), glm::vec3( 1.0f, 1.0f, 1.0f )
	};

	for ( int i = 0; i < count; i++ )
	{
		auto pointLight = std::make_shared<PointLightSource>( iceBallLight, positions[i], 0.5f, 0.064f );
		lights.push_back( pointLight );
	}

	auto model = loadModel( modelLoader, "../models/ice_ball/ice_ball.obj" );
	auto lightsModel = std::make_shared<RepeatedModel>( model, positions, glm::vec3( 0.1f, 0.1f, 0.1f ) );

	return std::make_pair( lights, lightsModel );
}

Sun SceneBuilder::createSun()
{
	Light sunLight = { glm::vec3( 0.05f, 0.05f, 0.05f ), glm::vec3( 0.4f, 0.4f, 0.4f ), glm::vec3( 0.6f, 0.6f, 0.6f ) };
	return Sun( sunLight, 720.0f );
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

std::shared_ptr<AnimatedModel> SceneBuilder::createScavenger()
{
	AnimationBuilder builder;
	auto scavenger = builder.createAnimation( "../models/scavenger/scavenger", 30, modelLoader );
	scavenger->translate( -1.0f, -0.02f, -16.0f ).scale( 0.5f, 0.5f, 0.5f );
	return scavenger;
}
