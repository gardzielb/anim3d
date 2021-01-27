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


std::vector<std::shared_ptr<Model>> createStaticModels( ModelLoader & loader );

std::vector<std::shared_ptr<Model>> createPointLightModels( ModelLoader & loader,
															const std::vector<std::shared_ptr<PointLightSource>> & lights );

std::shared_ptr<ComplexModel> createChopper( ModelLoader & loader, const std::shared_ptr<SpotLightSource> & light );

std::vector<std::shared_ptr<PointLightSource>> createPointLightSources();

Sun createSun();
