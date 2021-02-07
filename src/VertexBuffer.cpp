//
// Created by bartosz on 1/5/21.
//

#include "VertexBuffer.h"
#include <GL/glew.h>
#include "glUtils.h"


VertexBuffer::VertexBuffer( const void * data, int size )
{
	glCall( glGenBuffers( 1, &bufferId ) );
	glCall( glBindBuffer( GL_ARRAY_BUFFER, bufferId ) );
	glCall( glBufferData( GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW ) );
}

VertexBuffer::~VertexBuffer()
{
	glCall( glDeleteBuffers( 1, &bufferId ) );
}

void VertexBuffer::bind() const
{
	glCall( glBindBuffer( GL_ARRAY_BUFFER, bufferId ) );
}

void VertexBuffer::unbind() const
{
	glCall( glBindBuffer( GL_ARRAY_BUFFER, 0 ) );
}
