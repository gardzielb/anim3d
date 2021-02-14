//
// Created by bartosz on 2/7/21.
//

#include "GuiSceneController.h"
#include "dependencies/imgui/imgui.h"
#include "dependencies/imgui/imgui_impl_glfw_gl3.h"


GuiSceneController::GuiSceneController( GLFWwindow * window, const RendererArray & renderers,
										const CameraArray & cameras, Sun & sun, float minFogDensity )
		: minFogDensity( minFogDensity ), sun( sun )
{
	fog.density = minFogDensity;
	fog.color = sun.getSkyColor();

	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init( window, true );
	ImGui::StyleColorsDark();

	std::copy( cameras.begin(), cameras.end(), this->cameras.begin() );
	std::copy( renderers.begin(), renderers.end(), this->renderers.begin() );
}

GuiSceneController::~GuiSceneController()
{
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
}

void GuiSceneController::render( const std::string & title )
{
	ImGui_ImplGlfwGL3_NewFrame();

	ImGui::Begin( title.c_str() );

	const char * shadingOptions[] = { "Phong deferred", "Phong forward", "Gouraud" };
	ImGui::Combo( "Shading", &rendererIndex, shadingOptions, 3 );

	const char * cameraNames[] = { "static", "chopper", "spy" };
	ImGui::Combo( "Camera", &cameraIndex, cameraNames, 3 );

	ImGui::SliderFloat( "Fog density", &fog.density, minFogDensity, 1.0f );

	ImGui::SliderFloat( "Day length [frames]", &sun.dayLength, 360.0f, 1800.0f );
	ImGui::ColorEdit3( "Day sky color", (float *) &sun.brightSkyColor );
	ImGui::ColorEdit3( "Night sky color", (float *) &sun.darkSkyColor );
	fog.color = sun.getSkyColor();

	ImGui::Text(
			"Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
			ImGui::GetIO().Framerate
	);

	ImGui::End();

	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData( ImGui::GetDrawData() );
}
