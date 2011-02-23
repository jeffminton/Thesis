#include "matlab.h"


Matlab::Matlab(void)
{
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
	if (!(me = engOpen(NULL))) {
		printf("\nCan't start MATLAB engine\n");
		exit(-1);
	}
	printf("engine open\n");

	engOutputBuffer(me, matBuf, 2048);
}


Matlab::~Matlab(void)
{
}


void Matlab::matExec(string command)
{
	printf("exec: %s\n", command.c_str());
	engEvalString(me, command.c_str());
	printf("%s\n", matBuf);
}