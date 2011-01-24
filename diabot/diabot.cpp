#include <stdlib.h>
#include <stdio.h>
#include <engine.h>

int main()
{
	Engine *ep;
	
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
		fprintf(stderr, "\nCan't start MATLAB engine\n");
		return -1;
	}
}
