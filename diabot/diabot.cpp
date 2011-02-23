#include <stdio.h>
#include <engine.h>
#include "web.h"
#include "matlab.h"


using namespace std;

int main()
{
	bool result;
	Engine *ep;
	Web concepts = Web();
	Matlab mat = Matlab();
	char matBuf[2048], matCommand[2048];
	string thesisPath = "C:/Users/ffej/Documents/Thesis/matlab/";
	

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

	mat.matExec("cd " + thesisPath);
	mat.matExec("img = imread('img/segOrig.jpg');");
	mat.matExec("[edges sig rgb_segment_mask] = findObjects(img, 6);");
	

	concepts.genGraph();
	printf("graph done\n");

	if(!concepts.writeXML("xmlOut"))
	{
		printf("xml write failed\n");
		return -1;
	}
	printf("write done\n");
}
