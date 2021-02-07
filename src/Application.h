//
// Created by bartosz on 2/6/21.
//

#pragma once


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <array>
#include <vector>
#include <memory>

#include "LightSource.h"
#include "ComplexModel.h"
#include "Camera.h"
#include "ModelLoader.h"
#include "Sun.h"
#include "RepeatedModel.h"


class Application
{
private:
	GLFWwindow * window;
	int width, height;

public:
	Application( int width, int height );

	void run();

	~Application();

private:
	void initOpenGl();

	std::array<std::shared_ptr<Camera>, 3> createCameras( const std::shared_ptr<Model> & chopper );

	std::shared_ptr<SpotLightSource> createSpotLight();

	std::vector<std::shared_ptr<Model>> createStaticModels( ModelLoader & loader );

	std::shared_ptr<ComplexModel> createChopper( ModelLoader & loader, const std::shared_ptr<SpotLightSource> & light );

	std::pair<std::vector<std::shared_ptr<PointLightSource>>, std::shared_ptr<RepeatedModel>>
	createPointLights( ModelLoader & loader );

	Sun createSun();

	void processInput( const std::shared_ptr<SpotLightSource> & heliLight, const glm::vec3 & heliFront,
					   const glm::vec3 & heliRight );
};



