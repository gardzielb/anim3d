//
// Created by bartosz on 2/6/21.
//

#pragma once


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <array>
#include <vector>
#include <memory>

#include "scene/LightSource.h"
#include "scene/ComplexModel.h"
#include "scene/Camera.h"
#include "scene/ModelLoader.h"
#include "scene/Sun.h"
#include "scene/RepeatedModel.h"
#include "scene/Renderer.h"


class Application
{
private:
	GLFWwindow * window;
	int width, height;

public:
	Application( int width, int height );

	void run( int lightCount, bool hardcoreGround = false );

	~Application();

private:
	void initOpenGl();

	void processInput( const std::shared_ptr<SpotLightSource> & heliLight, const glm::vec3 & heliFront,
					   const glm::vec3 & heliRight, std::shared_ptr<Camera> & camera,
					   std::shared_ptr<Renderer> & renderer );
};



