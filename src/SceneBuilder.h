//
// Created by bartosz on 2/7/21.
//

#pragma once

#include <array>
#include <vector>
#include <memory>

#include "scene/Camera.h"
#include "scene/LightSource.h"
#include "scene/ComplexModel.h"
#include "scene/RepeatedModel.h"
#include "scene/ModelLoader.h"
#include "scene/Sun.h"
#include "scene/Renderer.h"
#include "scene/AnimatedModel.h"


class SceneBuilder
{
private:
	ModelLoader modelLoader;

public:
	std::array<std::shared_ptr<Renderer>, 3> createRenderers( const std::string & shadersPath, int windowWidth,
															  int windowHeight );

	std::array<std::shared_ptr<Camera>, 3> createCameras( const std::shared_ptr<Model> & chopper );

	std::shared_ptr<SpotLightSource> createSpotLight();

	std::vector<std::shared_ptr<Model>> createStaticModels();

	std::shared_ptr<ComplexModel> createChopper( const std::shared_ptr<SpotLightSource> & light );

	std::pair<std::vector<std::shared_ptr<PointLightSource>>, std::shared_ptr<RepeatedModel>>
	createPointLights( int count = 4 );

	Sun createSun();

	std::shared_ptr<AnimatedModel> createScavenger();
};



