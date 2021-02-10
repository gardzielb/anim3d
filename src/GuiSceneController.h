//
// Created by bartosz on 2/7/21.
//

#pragma once


#include "scene/Renderer.h"
#include <GLFW/glfw3.h>


class GuiSceneController
{
private:
	typedef std::array<std::shared_ptr<Renderer>, 3> RendererArray;
	typedef std::array<std::shared_ptr<Camera>, 3> CameraArray;

	Fog fog;
	RendererArray renderers;
	CameraArray cameras;

	int cameraIndex = 0;
	int rendererIndex = 0;

public:
	GuiSceneController( GLFWwindow * window, const RendererArray & renderers, const CameraArray & cameras, const Fog & fog );

	void render( const std::string & title );

	const inline Fog & getFog() const
	{
		return fog;
	}

	inline std::shared_ptr<Renderer> & getRenderer()
	{
		return renderers[rendererIndex];
	}

	inline std::shared_ptr<Camera> & getCamera()
	{
		return cameras[cameraIndex];
	}

	~GuiSceneController();
};



