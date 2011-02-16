#include <stdio.h>
#include <engine.h>
#include "web.h"
#include <gvc.h>

using namespace std;

int main()
{
	Engine *ep;
	Web concepts;
	concepts = Web();
	Agraph_t *g;
	Agnode_t *n, *m;
	Agedge_t *e;
	Agsym_t *a;
	GVC_t *gvc;
	/* set up a graphviz context */
	gvc = gvContext();

	char* args[3];
	args[0] = strdup ("dot" );
	args[1] = strdup ("-Tgif" );
	args[2] = strdup ("-oabc.gif" );
	gvParseArgs (gvc, sizeof (args)/sizeof (char*), args);


	/* Create a simple digraph */
	g = agopen("g", AGDIGRAPH);
	n = agnode(g, "n");
	m = agnode(g, "m");
	e = agedge(g, n, m);
	/* Set an attribute - in this case one that affects the visible rendering */
	agsafeset(n, "color", "red", "");
	/* Compute a layout using layout engine from command line args */
	gvLayoutJobs(gvc, g);
	/* Write the graph according to -T and -o options */
	gvRenderJobs(gvc, g);
	/* Free layout data */
	gvFreeLayout(gvc, g);
	/* Free graph structures */
	agclose(g);
	gvFreeContext(gvc);

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



	concepts.writeXML("conceptsOut.xml");

	printf("write done\n");
}
