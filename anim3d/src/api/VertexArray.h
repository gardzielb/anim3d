//
// Created by bartosz on 1/5/21.
//

#pragma once

#include "VertexBufferLayout.h"
#include "VertexBuffer.h"


class VertexArray
{
private:
	unsigned int arrayId;

public:
	VertexArray();

	~VertexArray();

	void addBuffer( const VertexBuffer & buffer, const VertexBufferLayout & layout );

	void bind() const;

	void unbind() const;
};



