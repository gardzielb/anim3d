//
// Created by bartosz on 1/27/21.
//

#include "populate.h"


std::shared_ptr<SimpleModel> loadModel( ModelLoader & loader, const std::string & path )
{
	std::shared_ptr<SimpleModel> model = loader.loadModel( path );
	if ( !model )
		exit( 1 );
	return model;
}

std::vector<std::shared_ptr<Model>> createStaticModels( ModelLoader & loader )
{
	std::shared_ptr<SimpleModel> statue = loadModel( loader, "models/orc_statue/orc_statue.obj" );
	statue->scale( glm::vec3( 0.4f, 0.4f, 0.4f ) );

	std::shared_ptr<SimpleModel> ground = loadModel( loader, "models/ground/ground2.obj" );
	ground->scale( 3.0f, 3.0f, 3.0f ).translate( glm::vec3( 0.0f, -0.5f, 0.0f ) );

	std::shared_ptr<SimpleModel> pripyat = loadModel( loader, "models/pripyat/pripyat.obj" );
	pripyat->translate( glm::vec3( 0.0f, -0.6f, -2.0f ) );

	return { ground, statue, pripyat };
}

std::shared_ptr<ComplexModel> createChopper( ModelLoader & loader, const std::shared_ptr<SpotLightSource> & light )
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

std::vector<std::shared_ptr<PointLightSource>> createPointLightSources()
{
	Light iceBallLight = {
			glm::vec3( 0.05f, 0.05f, 0.05f ), glm::vec3( 0.8f, 0.8f, 0.8f ), glm::vec3( 1.0f, 1.0f, 1.0f )
	};
	auto pointLight = std::make_shared<PointLightSource>( iceBallLight, glm::vec3(), 0.09f, 0.032f );
	return { pointLight };
}

Sun createSun()
{
	Light sunLight = { glm::vec3( 0.05f, 0.05f, 0.05f ), glm::vec3( 0.4f, 0.4f, 0.4f ), glm::vec3( 0.6f, 0.6f, 0.6f ) };
	return Sun( sunLight, 0.1f );
}

std::vector<std::shared_ptr<Model>> createPointLightModels( ModelLoader & loader,
															const std::vector<std::shared_ptr<PointLightSource>> & lights )
{
	std::shared_ptr<Model> iceBall = loader.loadModel( "models/ice_ball/ice_ball.obj" );
	if ( !iceBall )
		exit( 1 );

	iceBall->addObserver( lights[0] );
	iceBall->translate( 0.5f, 0.5f, 0.5f ).scale( glm::vec3( 0.1f, 0.1f, 0.1f ) );
	return { iceBall };
}
