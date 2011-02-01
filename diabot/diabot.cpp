#include <stdio.h>
#include <engine.h>
#include "web.h"

using namespace std;

int main()
{
	printf("can i print stuff\n");
	
	Engine *ep;
	Web concepts;
	concepts = Web();
	
	/*
	 * Start the MATLAB engine locally by executing the string
	 * "matlab"
	 *
	 * To start the session on a remote host, use the name of
	 * the host as the string rather than \0
	 *
	 * For more complicated cases, use any string with whitespace,
	 * and that string will be executed literally to start MATLAB
	 */
	if (!(ep = engOpen("\0"))) {
		printf("\nCan't start MATLAB engine\n");
		return -1;
	}
	printf("engine open\n");
	
	concepts.parseXMLFile("xml/concepts.xml");
	
	printf("parse done\n");
}
