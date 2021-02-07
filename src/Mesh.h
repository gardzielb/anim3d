#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

#include <string>
#include <vector>


struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

struct Texture
{
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
private:
	VertexBuffer vertexBuffer;
	VertexArray vertexArray;
	IndexBuffer indexBuffer;
	std::vector<Texture> textures;

public:
	Mesh( const std::vector<Vertex> & vertices, const std::vector<unsigned int> & indices,
		  const std::vector<Texture> & textures )
			: indexBuffer( &indices[0], indices.size() ),
			  vertexBuffer( &vertices[0], vertices.size() * sizeof( Vertex ) ),
			  textures( std::move( textures ) )
	{
		VertexBufferLayout vbLayout;

		vbLayout.pushFloat( 3 );  // position
		vbLayout.pushFloat( 3 );  // normal
		vbLayout.pushFloat( 2 );  // tex coords
		vbLayout.pushFloat( 3 );  // tangent
		vbLayout.pushFloat( 3 );  // bitangent

		vertexArray.addBuffer( vertexBuffer, vbLayout );
	}

	// render the mesh
	void draw( const Shader & shader ) const
	{
		// bind appropriate textures
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		for ( unsigned int i = 0; i < textures.size(); i++ )
		{
			glActiveTexture( GL_TEXTURE0 + i ); // active proper texture unit before binding
			// retrieve texture number (the N in diffuse_textureN)
			std::string number;
			std::string name = textures[i].type;
			if ( name == "diffuse" )
				number = std::to_string( diffuseNr++ );
			else if ( name == "specular" )
				number = std::to_string( specularNr++ ); // transfer unsigned int to stream
			else if ( name == "normal" )
				number = std::to_string( normalNr++ ); // transfer unsigned int to stream
			else if ( name == "height" )
				number = std::to_string( heightNr++ ); // transfer unsigned int to stream

			shader.setInt( "material." + name + number, i );
			glBindTexture( GL_TEXTURE_2D, textures[i].id );
		}

		// draw mesh
		vertexArray.bind();
		indexBuffer.bind();
		glDrawElements( GL_TRIANGLES, indexBuffer.getCount(), GL_UNSIGNED_INT, 0 );

		// always good practice to set everything back to defaults once configured.
		glActiveTexture( GL_TEXTURE0 );
	}
};