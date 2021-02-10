//
// Created by bartosz on 1/5/21.
//

#pragma once


class IndexBuffer
{
private:
	unsigned int bufferId;
	unsigned int count;

public:
	IndexBuffer( const void * data, int count );

	~IndexBuffer();

	void bind() const;

	void unbind() const;

	inline unsigned int getCount() const
	{
		return count;
	}
};



