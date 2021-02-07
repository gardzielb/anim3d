//
// Created by bartosz on 2/3/21.
//

#include "GeometryBuffer.h"
#include "glUtils.h"

GeometryBuffer::GeometryBuffer( int screenWidth, int screenHeight )
{
	glCall( glGenFramebuffers( 1, &id ) );
	bind();

	// position color buffer
	creaateColorAttachment( &gPosition, GL_RGBA16F, GL_FLOAT, GL_COLOR_ATTACHMENT0, screenWidth, screenHeight );

	// normal color buffer
	creaateColorAttachment( &gNormal, GL_RGBA16F, GL_FLOAT, GL_COLOR_ATTACHMENT1, screenWidth, screenHeight );

	// color + specular color buffer
	creaateColorAttachment( &gAlbedoSpec, GL_RGBA, GL_UNSIGNED_BYTE, GL_COLOR_ATTACHMENT2, screenWidth, screenHeight );

	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glCall( glDrawBuffers( 3, attachments ) );

	// create and attach depth buffer (renderbuffer)
	glCall( glGenRenderbuffers( 1, &depthBuffer ) );
	glCall( glBindRenderbuffer( GL_RENDERBUFFER, depthBuffer ) );
	glCall( glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenWidth, screenHeight ) );
	glCall( glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer ) );

	unbind();
}

void GeometryBuffer::bind() const
{
	glCall( glBindFramebuffer( GL_FRAMEBUFFER, id ) );
}

void GeometryBuffer::unbind() const
{
	glCall( glBindFramebuffer( GL_FRAMEBUFFER, 0 ) );
}

void GeometryBuffer::bindTextures() const
{
	glCall( glActiveTexture( GL_TEXTURE0 ) );
	glCall( glBindTexture( GL_TEXTURE_2D, gPosition ) );
	glCall( glActiveTexture( GL_TEXTURE1 ) );
	glCall( glBindTexture( GL_TEXTURE_2D, gNormal ) );
	glCall( glActiveTexture( GL_TEXTURE2 ) );
	glCall( glBindTexture( GL_TEXTURE_2D, gAlbedoSpec ) );
}

void GeometryBuffer::setShaderTextures( const Shader & shader ) const
{
	shader.setInt( "gPosition", 0 );
	shader.setInt( "gNormal", 1 );
	shader.setInt( "gAlbedoSpec", 2 );
}

bool GeometryBuffer::checkIsComplete() const
{
	bind();
	bool result = glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE;
	unbind();
	return result;
}

void GeometryBuffer::creaateColorAttachment( unsigned int * texId, unsigned int format, unsigned int type,
											 unsigned int attachmentNr, int screenWidth, int screenHeight )
{
	glCall( glGenTextures( 1, texId ) );
	glCall( glBindTexture( GL_TEXTURE_2D, *texId ) );
	glCall( glTexImage2D( GL_TEXTURE_2D, 0, format, screenWidth, screenHeight, 0, GL_RGBA, type, NULL ) );
	glCall( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST ) );
	glCall( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ) );
	glCall( glFramebufferTexture2D( GL_FRAMEBUFFER, attachmentNr, GL_TEXTURE_2D, *texId, 0 ) );
}

void GeometryBuffer::blitDepthBuffer( int screenWidth, int screenHeight ) const
{
	glCall( glBindFramebuffer( GL_READ_FRAMEBUFFER, id ) );
	glCall( glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 ) );
	glCall( glBlitFramebuffer(
			0, 0, screenWidth, screenHeight, 0, 0, screenWidth, screenHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST
	) );
	glCall( glBindFramebuffer( GL_FRAMEBUFFER, 0 ) );
}
