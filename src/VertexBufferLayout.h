//
// Created by bartosz on 1/5/21.
//

#pragma once

#include <vector>
#include "dependencies/glad.h"


struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int typeSize( unsigned int type )
	{
		switch ( type )
		{
			case GL_FLOAT:
				return 4;
			case GL_UNSIGNED_INT:
				return 4;
			case GL_UNSIGNED_BYTE:
				return 1;
		}
		return 0;
	}
};


class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> elements;
	unsigned int stride = 0;

public:
	void pushFloat( unsigned int count )
	{
		elements.push_back( { GL_FLOAT, count, GL_FALSE } );
		stride += count * VertexBufferElement::typeSize( GL_FLOAT );
	}

	void pushUInt( unsigned int count )
	{
		elements.push_back( { GL_UNSIGNED_INT, count, GL_FALSE } );
		stride += count * VertexBufferElement::typeSize( GL_UNSIGNED_INT );
	}

	void pushUChar( unsigned int count )
	{
		elements.push_back( { GL_UNSIGNED_BYTE, count, GL_TRUE } );
		stride += count * VertexBufferElement::typeSize( GL_UNSIGNED_BYTE );
	}

//	template<typename T>
//	void push( unsigned int count )
//	{
//		static_assert( false );
//	}
//
//	template<>
//	void push<float>( unsigned int count )
//	{
//		elements.push_back( { GL_FLOAT, count, GL_FALSE } );
//		stride += VertexBufferElement::typeSize( GL_FLOAT );
//	}
//
//	template<>
//	void push<unsigned int>( unsigned int count )
//	{
//		elements.push_back( { GL_UNSIGNED_INT, count, GL_FALSE } );
//		stride += VertexBufferElement::typeSize( GL_UNSIGNED_INT );
//	}
//
//	template<>
//	void push<unsigned char>( unsigned int count )
//	{
//		elements.push_back( { GL_UNSIGNED_BYTE, count, GL_TRUE } );
//		stride += VertexBufferElement::typeSize( GL_UNSIGNED_BYTE );
//	}

	inline unsigned int getStride() const
	{
		return stride;
	}

	inline const std::vector<VertexBufferElement> & getElements() const
	{
		return elements;
	}
};



