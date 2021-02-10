#include "Application.h"


int main( int argc, char ** argv )
{
	const unsigned int SCR_WIDTH = 1400;
	const unsigned int SCR_HEIGHT = 950;

	int lightCount = 4;
	if ( argc > 1 )
		lightCount = std::atoi( argv[1] );

	Application app( SCR_WIDTH, SCR_HEIGHT );
	app.run( lightCount );

	return 0;
}
