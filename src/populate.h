//
// Created by bartosz on 1/27/21.
//

#pragma once

#include <vector>
#include "Model.h"
#include "ComplexModel.h"
#include "ModelLoader.h"
#include "LightSource.h"
#include "Sun.h"
#include "ModelPopulation.h"


std::shared_ptr<SimpleModel> loadModel( ModelLoader & loader, const std::string & path );

std::vector<std::shared_ptr<Model>> createStaticModels( ModelLoader & loader );

std::vector<std::shared_ptr<Model>> createPointLightModels( ModelLoader & loader,
															const std::vector<std::shared_ptr<PointLightSource>> & lights );

std::shared_ptr<ComplexModel> createChopper( ModelLoader & loader, const std::shared_ptr<SpotLightSource> & light );

std::vector<std::shared_ptr<PointLightSource>> createPointLightSources();

Sun createSun();

template<int N>
ModelPopulation<N> createBuildings( ModelLoader & loader )
{
	std::shared_ptr<SimpleModel> building = loadModel( loader, "models/building01/building01.obj" );
	std::array<glm::vec3, N> positions = {
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
	return ModelPopulation<N>( building, positions );
}
