#include "Application.h"


int main()
{
	const unsigned int SCR_WIDTH = 1400;
	const unsigned int SCR_HEIGHT = 950;

	Application app( SCR_WIDTH, SCR_HEIGHT );
	app.run();

	return 0;
}
