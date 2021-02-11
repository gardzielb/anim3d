cmake_minimum_required(VERSION 3.17)
project(anim3d)

set(CMAKE_CXX_STANDARD 17)

find_package(spdlog REQUIRED)

add_executable(anim3d src/main.cpp src/api/VertexBuffer.cpp src/api/VertexBuffer.h src/api/IndexBuffer.cpp src/api/IndexBuffer.h src/api/glUtils.h src/api/Shader.cpp src/api/Shader.h src/api/VertexArray.cpp src/api/VertexArray.h src/api/VertexBufferLayout.h src/scene/Camera.cpp src/scene/Camera.h src/scene/LightSource.cpp src/scene/LightSource.h src/scene/Mesh.h src/scene/Model.h src/scene/ModelLoader.cpp src/scene/ModelLoader.h src/scene/Model.cpp src/scene/ModelPartBehavior.cpp src/scene/ModelPartBehavior.h src/scene/ComplexModel.cpp src/scene/ComplexModel.h src/scene/Sun.cpp src/scene/Sun.h src/scene/RepeatedModel.h src/api/GeometryBuffer.cpp src/api/GeometryBuffer.h src/scene/Renderer.cpp src/scene/Renderer.h src/api/Timer.h src/Application.cpp src/Application.h src/dependencies/imgui/imconfig.h src/dependencies/imgui/imgui.cpp src/dependencies/imgui/imgui.h src/dependencies/imgui/imgui_demo.cpp src/dependencies/imgui/imgui_draw.cpp src/dependencies/imgui/imgui_impl_glfw_gl3.cpp src/dependencies/imgui/imgui_impl_glfw_gl3.h src/dependencies/imgui/imgui_internal.h src/dependencies/imgui/stb_truetype.h src/dependencies/imgui/stb_textedit.h src/dependencies/imgui/stb_rect_pack.h src/SceneBuilder.cpp src/SceneBuilder.h src/GuiSceneController.cpp src/GuiSceneController.h src/scene/RotationBehavior.cpp src/scene/RotationBehavior.h src/scene/MoveBehavior.cpp src/scene/MoveBehavior.h src/scene/AnimatedModel.cpp src/scene/AnimatedModel.h)
target_link_libraries(anim3d -lglut -lGL -lGLU glfw -lGLEW -ldl -lassimp spdlog::spdlog)
