//
// Created by bartosz on 2/3/21.
//

#pragma once


#include "Shader.h"

class GeometryBuffer
{
	unsigned int id;
	unsigned int gPosition, gNormal, gAlbedoSpec;
	unsigned int depthBuffer;

public:
	GeometryBuffer( int screenWidth, int screenHeight );

	bool checkIsComplete() const;

	void blitDepthBuffer( int screenWidth, int screenHeight ) const;

	void bind() const;

	void unbind() const;

	void bindTextures() const;

	void setShaderTextures( const Shader & shader ) const;

private:
	void creaateColorAttachment( unsigned int * texId, unsigned int format, unsigned int type,
								 unsigned int attachmentNr, int screenWidth, int screenHeight );
};



