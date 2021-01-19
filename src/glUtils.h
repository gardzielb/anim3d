//
// Created by bartosz on 1/5/21.
//

#pragma once

#include <iostream>
#include "dependencies/glad.h"


#define glCall( x ) glClearError(); \
    x; \
    glLogCall( __FILE__, __LINE__ )


static void glClearError()
{
	while ( glGetError() != GL_NO_ERROR );
}

static void glLogCall( const char * file, int line )
{
	if ( GLenum error = glGetError() )
	{
		std::cout << "[OpenGl error] [File: " << file << ", Line: " << line << "] [Code: " << error << "]\n";
		exit( EXIT_FAILURE );
	}
}