cmake_minimum_required(VERSION 3.17)
project(anim3d)

set(CMAKE_CXX_STANDARD 17)

find_package(spdlog REQUIRED)

add_executable(anim3d src/main.cpp src/dependencies/glad.c src/VertexBuffer.cpp src/VertexBuffer.h src/IndexBuffer.cpp src/IndexBuffer.h src/glUtils.h src/Shader.cpp src/Shader.h src/VertexArray.cpp src/VertexArray.h src/VertexBufferLayout.h src/Camera.cpp src/Camera.h src/LightSource.cpp src/LightSource.h src/Mesh.h src/Model.h src/ModelLoader.cpp src/ModelLoader.h src/Model.cpp src/ModelPartBehavior.cpp src/ModelPartBehavior.h src/ComplexModel.cpp src/ComplexModel.h src/Sun.cpp src/Sun.h src/populate.cpp src/populate.h src/ModelPopulation.h)
target_link_libraries(anim3d -lglut -lGL -lGLU glfw -lGLEW -ldl -lassimp spdlog::spdlog)
