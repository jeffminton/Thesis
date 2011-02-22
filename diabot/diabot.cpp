#include <stdio.h>
#include <engine.h>
#include "web.h"


using namespace std;

int main()
{
	bool result;
	Engine *ep;
	Web concepts;
	concepts = Web();
	char matBuf[2048], matCommand[2048];
	string thesisPath = "c:/Users/ffej/Documents/Thesis/";
	

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
	if (!(ep = engOpen(NULL))) {
		printf("\nCan't start MATLAB engine\n");
		return -1;
	}
	printf("engine open\n");

	//create a buffer to store matlab output in
	engOutputBuffer(ep, matBuf, 2048);
	engEvalString(ep, "pwd");
	printf(matBuf);
	sprintf(matCommand, "img = imread('%smatlab/img/segOrig.jpg')", thesisPath.c_str());
	printf(matCommand);
	engEvalString(ep, matCommand);
	printf(matBuf);
	engEvalString(ep, "imshow(img);");
	printf(matBuf);
	
	if(!concepts.parseXMLFile("C:/Users/ffej/Documents/Thesis/diabot/xml/concepts.xml"))
	{
		printf("concepts parse failed\n");
		return -1;
	}
	printf("concept parse done\n");

	if(!concepts.parseXMLFile("C:/Users/ffej/Documents/Thesis/diabot/xml/words.xml"))
	{
		printf("words parse failed\n");
		return -1;
	}
	printf("word parse done\n");

	concepts.genGraph();

	printf("graph done\n");

	if(!concepts.writeXML("xmlOut"))
	{
		printf("xml write failed\n");
		return -1;
	}
	printf("write done\n");
}
