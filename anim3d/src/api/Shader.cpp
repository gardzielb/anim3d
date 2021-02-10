//
// Created by bartosz on 1/5/21.
//

#include "Shader.h"
#include "glUtils.h"
#include <glm/gtc/type_ptr.hpp>
#include <tuple>


Shader::Shader( const std::string & vertexPath, const std::string & fragmentPath )
{
	auto[vsSource, fsSource] = readShaderSources( vertexPath, fragmentPath );
	programId = createShader( vsSource, fsSource );
}

Shader::Shader( const std::string & path )
{
	auto[vsSource, fsSource] = readShaderSources( path + "_vertex.shader", path + "_fragment.shader" );
	programId = createShader( vsSource, fsSource );
}

Shader::~Shader()
{
	glCall( glDeleteProgram( programId ) );
}

void Shader::bind() const
{
	glCall( glUseProgram( programId ) );
}

void Shader::unbind() const
{
	glCall( glUseProgram( 0 ) );
}

void Shader::setBool( const std::string & name, bool value ) const
{
	glCall( glUniform1i( glGetUniformLocation( programId, name.c_str() ), (int) value ) );
}

void Shader::setInt( const std::string & name, int value ) const
{
	glCall( glUniform1i( glGetUniformLocation( programId, name.c_str() ), value ) );
}

void Shader::setFloat( const std::string & name, float value ) const
{
	glCall( glUniform1f( glGetUniformLocation( programId, name.c_str() ), value ) );
}

void Shader::setMatrix( const std::string & name, const glm::mat4 & matrix ) const
{
	glCall( glUniformMatrix4fv(
			glGetUniformLocation( programId, name.c_str() ), 1, GL_FALSE, glm::value_ptr( matrix )
	) );
}

void Shader::setVector( const std::string & name, const glm::vec3 & vector ) const
{
	glUniform3fv( glGetUniformLocation( programId, name.c_str() ), 1, glm::value_ptr( vector ) );
}

unsigned int Shader::createShader( const std::string & vertexSource, const std::string & fragmentSource )
{
	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader( GL_VERTEX_SHADER, vertexSource );
	unsigned int fs = compileShader( GL_FRAGMENT_SHADER, fragmentSource );

	glCall( glAttachShader( program, vs ) );
	glCall( glAttachShader( program, fs ) );
	glCall( glLinkProgram( program ) );
	glCall( glValidateProgram( program ) );

	glCall( glDeleteShader( vs ) );
	glCall( glDeleteShader( fs ) );

	return program;
}

unsigned int Shader::compileShader( unsigned int type, const std::string & source )
{
	unsigned int id = glCreateShader( type );
	const char * src = source.c_str();
	glCall( glShaderSource( id, 1, &src, nullptr ) );
	glCall( glCompileShader( id ) );

	int result;
	glGetShaderiv( id, GL_COMPILE_STATUS, &result );

	if ( result == GL_FALSE )
	{
		int length;
		glGetShaderiv( id, GL_INFO_LOG_LENGTH, &length );
		char * message = (char *) alloca( length * sizeof( char ) );
		glGetShaderInfoLog( id, length, &length, message );
		std::cout << "[Shader compilation error]: " << message << "\n";
		return 0;
	}

	return id;
}

std::tuple<std::string, std::string> Shader::readShaderSources( const std::string & vertexPath,
																const std::string & fragmentPath )
{
	std::ifstream vsFile( vertexPath );
	std::ifstream fsFile( fragmentPath );
	std::stringstream vsStream, fsStream;

	vsStream << vsFile.rdbuf();
	fsStream << fsFile.rdbuf();
	vsFile.close();
	fsFile.close();

	return std::make_tuple( vsStream.str(), fsStream.str() );
}
