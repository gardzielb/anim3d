//
// Created by bartosz on 1/5/21.
//

#include "VertexArray.h"
#include "glUtils.h"

void VertexArray::addBuffer( const VertexBuffer & buffer, const VertexBufferLayout & layout )
{
	bind();
	buffer.bind();

	const auto & elements = layout.getElements();
	unsigned int offset = 0;

	for ( unsigned int i = 0; i < elements.size(); i++ )
	{
		const auto & element = elements[i];
		glCall( glEnableVertexAttribArray( i ) );
		glCall( glVertexAttribPointer(
				i, element.count, element.type, element.normalized, layout.getStride(), (const void *) offset
		) );
		offset += element.count * VertexBufferElement::typeSize( element.type );
	}
}

VertexArray::VertexArray()
{
	glCall( glGenVertexArrays( 1, &arrayId ) );
}

VertexArray::~VertexArray()
{
	glCall( glDeleteVertexArrays( 1, &arrayId ) );
}

void VertexArray::bind() const
{
	glCall( glBindVertexArray( arrayId ) );
}

void VertexArray::unbind() const
{
	glCall( glBindVertexArray( 0 ) );
}
