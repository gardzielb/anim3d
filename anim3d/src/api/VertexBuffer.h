//
// Created by bartosz on 1/5/21.
//

#pragma once


class VertexBuffer
{
private:
	unsigned int bufferId;

public:
	VertexBuffer( const void * data, int size );

	~VertexBuffer();

	void bind() const;

	void unbind() const;
};
