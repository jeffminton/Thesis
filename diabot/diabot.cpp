#include <stdio.h>
#include <engine.h>
#include "web.h"
#include "matlab.h"
//#include "lib\util.h"


using namespace std;


vector<vector<Word *>> parseSentence(string sentence, Web &concepts)
{
	Util myUtils = Util();
	vector<string> words, conceptList;
	vector<vector<Word *>> meaningList;
	vector<Word *> currMeaning;
	string concept, parentConcept;

	words = myUtils.split(sentence, WORD_SPLIT);
	
	for(int i = 0; i < words.size(); i++)
	{
		currMeaning = concepts.getWordList(words[i]);
		if(currMeaning == (vector<Word *>) NULL)
		{
			printf("Please tell me what concept \"%s\" describes: ", words[i].c_str());
			cin >> concept;
			if(concept != "unimportant" && concept != "na" && concept != "NA")
			{
				printf("Please tell me the name of this concepts parent: ");
				cin >> parentConcept;
			}
		}
		meaningList.push_back(currMeaning);
	}
}


void process(Util myUtil, Web concepts, Matlab mat, string path)
{
	string query;
	vector<vector<Word *>> queryParse;

	cin >> query;

	while(!cin.eof())
	{
		queryParse = parseSentence(query, concepts);
	}
}


int main()
{
	bool result;
	Util myUtils = Util();
	Web concepts = Web();
	Matlab mat = Matlab();
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

	concepts.genGraph();
	printf("graph done\n");

	if(!concepts.writeXML("xmlOut"))
	{
		printf("xml write failed\n");
		return -1;
	}
	printf("write done\n");
}
