#include <stdio.h>
#include <engine.h>
#include "web.h"


using namespace std;

int main()
{
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
	
	concepts.parseXMLFile("C:/Users/ffej/Documents/Thesis/diabot/xml/concepts.xml");
	
	printf("parse done\n");

	concepts.genGraph();

	printf("graph done\n");

	concepts.writeXML("conceptsOut.xml");

	printf("write done\n");
}
