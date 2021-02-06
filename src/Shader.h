//
// Created by bartosz on 1/5/21.
//

#pragma once

#include "dependencies/glad.h"
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


//struct TextureMaterial
//{
//	Texture diffuseMap;
//	Texture specularMap;
//	float shininess;
//};


struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};


class Shader
{

public:
	Shader( const std::string & vertexPath, const std::string & fragmentPath );

	Shader( const std::string & path );

	~Shader();

	void bind() const;

	void unbind() const;

	void setBool( const std::string & name, bool value ) const;

	void setInt( const std::string & name, int value ) const;

	void setFloat( const std::string & name, float value ) const;

	void setMatrix( const std::string & name, const glm::mat4 & matrix ) const;

	void setVector( const std::string & name, const glm::vec3 & vector ) const;

//	void setMaterial( const Material & material ) const;

//	void setMaterial( const TextureMaterial & material ) const;

//	void setLightProperties( const LightProperties & lightProperties ) const;

	unsigned int programId;
private:
	static unsigned int compileShader( unsigned int type, const std::string & source );

	static unsigned int createShader( const std::string & vertexSource, const std::string & fragmentSource );

	static std::tuple<std::string, std::string> readShaderSources( const std::string & vertexPath,
																   const std::string & fragmentPath );
};


