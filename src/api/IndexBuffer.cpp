//
// Created by bartosz on 1/5/21.
//

#include "IndexBuffer.h"
#include "glUtils.h"


IndexBuffer::IndexBuffer( const void * data, int count )
		: count( count )
{
	glCall( glGenBuffers( 1, &bufferId ) );
	glCall( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bufferId ) );
	glCall( glBufferData( GL_ELEMENT_ARRAY_BUFFER, count * sizeof( unsigned int ), data, GL_STATIC_DRAW ) );
}

IndexBuffer::~IndexBuffer()
{
	glCall( glDeleteBuffers( 1, &bufferId ) );
}

void IndexBuffer::bind() const
{
	glCall( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bufferId ) );
}

void IndexBuffer::unbind() const
{
	glCall( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 ) );
}
