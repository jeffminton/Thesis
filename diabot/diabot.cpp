#include <stdio.h>
//#include <engine.h>
#include "web.h"
#include "matlab.h"
#include "fmap.h"
//#include "lib\util.h"


using namespace std;


vector<vector<Word *>> getMeanings(Web *concepts, vector<string> words)
{
	vector<vector<Word *>> meanings;
	vector<Word *> missingMeanings;

	//get the word * vectors for all the words int the sentence
	for(int i = 0; i < words.size(); i++)
	{
		meanings.push_back(concepts->getWordList(words[i]));
	}

	return meanings;
}


vector<vector<Word *>> getMissingMeanings(Web *concepts, vector<string> words)
{
	string choice;
	int wordChoice, conceptChoice;
	string pos;
	bool haveNulls = true, needConcepts = true;
	vector<vector<Word *>> meanings;
	vector<Word *> wordsNeeded;
	vector<string> unknownWords;

	meanings = getMeanings(concepts, words);

	//check is null words are important or not, unimportant words will be set to the NA word
	for(int i = 0; i < words.size(); i++)
	{
		if(meanings[i] == (vector<Word *>) NULL)
		{
			printf("is the word \"%s\" important (y or n): ", words[i].c_str());
			getline(cin, choice);
			if(choice[0] == 'n' || choice[0] == 'N')
			{
				concepts->addWordNA(words[i]);
			}
		}
	}

	while(haveNulls == true && needConcepts == true)
	{
		haveNulls = false;
		needConcepts = false;
		unknownWords.clear();

		meanings = getMeanings(concepts, words);
		wordsNeeded = concepts->missingWords(meanings);

		if(!wordsNeeded.empty())
		{
			needConcepts = true;
		}
		
		for(int i = 0; i < words.size(); i++)
		{
			if(meanings[i] == (vector<Word *>) NULL)
			{
				haveNulls = true;
				unknownWords.push_back(words[i]);
			}
		}

		if(haveNulls == true || needConcepts == true)
		{
			if(haveNulls == true)
				{
				printf("there are words that are not known\n");
				for(int i = 0; i < unknownWords.size(); i++)
				{
					printf("%d: \"%s\"\n", i + 1, unknownWords[i].c_str());
				}
				printf("\nPossible meanings are\n");
				for(int i = 0; i < wordsNeeded.size(); i++)
				{
					printf("%d: concept: %s, concept's parent: %s\n", i + 1, wordsNeeded[i]->getConcept()->getName().c_str(), wordsNeeded[i]->getConcept()->getParent()->getName().c_str());
				}
				printf("%d: none of the above\n\n", wordsNeeded.size() + 1);
				printf("Enter the number coresponding to a word you wish to define: ");
				cin >> wordChoice;
				printf("Enter the number coresponding to the concept the word represents: ");
				cin >> conceptChoice;
			}
			else if(needConcepts == true)
			{
				unknownWords = words;
				printf("The following concepts are missing from the sentence\n");
				for(int i = 0; i < wordsNeeded.size(); i++)
				{
					printf("%d: concept: %s, concept's parent: %s\n", i + 1, wordsNeeded[i]->getConcept()->getName().c_str(), wordsNeeded[i]->getConcept()->getParent()->getName().c_str());
				}
				printf("%d: none of these concepts are represented by the words in the sentence\n", wordsNeeded.size() + 1);
				printf("\nthe following words are in the sentence\n");
				for(int i = 0; i < unknownWords.size(); i++)
				{
					printf("%d: \"%s\"\n", i + 1, unknownWords[i].c_str());
				}
				printf("Enter the number coresponding to the meaning you wish to assign a word to: ");
				cin >> conceptChoice;
				printf("Enter the number coresponding to a word you wish to define: ");
				cin >> wordChoice;
			}
			
			

			if(wordChoice >= unknownWords.size() + 1)
			{
				printf("word choice not valid, fail\n");
			}
			else if(conceptChoice == wordsNeeded.size() + 1)
			{
				printf("concept for word doesn't exist\n");
				return (vector<vector<Word *>>) NULL;
			}
			else if(conceptChoice > wordsNeeded.size() + 1)
			{
				printf("concept choice not valid, fail\n");
			}
			else
			{
				printf("are you sure that \"%s\" represents concept: %s with parent concept: %s? (y or n): "
					, unknownWords[wordChoice - 1].c_str()
					, wordsNeeded[conceptChoice - 1]->getConcept()->getName().c_str()
					, wordsNeeded[conceptChoice - 1]->getConcept()->getParent()->getName().c_str());

				cin.sync();
				getline(cin, choice);

				if(choice[0] == 'y' || choice[0] == 'Y')
				{
					printf("what is the words part of speech i.e. verb, noun, etc..: ");
					cin >> pos;
					concepts->addWord(pos, unknownWords[wordChoice - 1], wordsNeeded[conceptChoice - 1]);
				}
			}
		}
	}

	return meanings;
}


void process(Util myUtil, Web *concepts, Matlab *mat, string path)
{
	int funcPtrRet;
	bool dependsProcessed = false;
	vector<string> *dependsDone = new vector<string>();
	FMap functionMap = FMap();
	FMap::ExecFunc funcToCall;
	ExecFunctions *ef;
	char * query = (char *) calloc(2048, sizeof(char));
	string queryStr;
	vector<string> queryWords;
	vector<vector<Word *>> wordConcepts, realMeanings;
	vector<vector<int>> reqConceptsIdx;
	vector<Word *> wordsNeeded;

	printf("Ready to talk.\n");
	cin.getline(query, 2048);
	while(!cin.eof())
	{
		queryStr = query;
		queryWords = myUtil.split(queryStr, WORD_SPLIT);
		wordConcepts = getMissingMeanings(concepts, queryWords);
		realMeanings = concepts->getRealConcept(queryWords);
		reqConceptsIdx = concepts->getIdxValidReqGrp(realMeanings);

		for(int i = 0; i < realMeanings.size(); i++){
			for(int j = 0; j < realMeanings[i].size(); j++){
				printf("%s - concept: %s concepts parent: %s\n", queryWords[i].c_str()
					, realMeanings[i][j]->getConcept()->getName().c_str()
					, realMeanings[i][j]->getConcept()->getParent()->getName().c_str());
			}
		}

		while(dependsProcessed == false){
			dependsProcessed = true;
			for(int i = 0; i < realMeanings.size(); i++)
			{
				for(int j = 0; j < realMeanings[i].size(); j++){
					funcToCall = functionMap.theFuncMap.descrToFuncMap[realMeanings[i][j]->getConcept()->getName()];
					//funcPtrRet = (*funcToCall)(concepts, mat, realMeanings[i][j], reqConceptsIdx[i][j], queryWords[i], dependsDone);
				}
			}
		}

		cin.getline(query, 2048);
	}

	return;
}


int main()
{
	bool result;
	Util myUtils = Util();
	Web *concepts = new Web();
	Matlab *mat = new Matlab();
	string thesisPath = "C:/Users/ffej/Documents/Thesis/matlab/";
	
	/*********************************************************************************/
	/* Parse XML and build web                                                       */
	/*********************************************************************************/

	if(!concepts->parseXMLFile("C:/Users/ffej/Documents/Thesis/diabot_vs/diabot_vs/xmlOut/concepts.xml"))
	//if(!concepts->parseXMLFile("C:/Users/ffej/Documents/Thesis/diabot/xml/concepts.xml"))
	{
		printf("concepts parse failed\n");
		return -1;
	}
	printf("concept parse done\n");

	if(!concepts->parseXMLFile("C:/Users/ffej/Documents/Thesis/diabot_vs/diabot_vs/xmlOut/words.xml"))
	//if(!concepts->parseXMLFile("C:/Users/ffej/Documents/Thesis/diabot/xml/words.xml"))
	{
		printf("words parse failed\n");
		return -1;
	}
	printf("word parse done\n");
	
	/*********************************************************************************/
	/* Done parse XML and build web                                                  */
	/*********************************************************************************/

	process(myUtils, concepts, mat, thesisPath);

	/*********************************************************************************/
	/* Output current state of web                                                   */
	/*********************************************************************************/

	concepts->genGraph();
	printf("graph done\n");

	if(!concepts->writeXML("xmlOut"))
	{
		printf("xml write failed\n");
		return -1;
	}
	printf("write done\n");

	/*********************************************************************************/
	/* Done output current state of web                                              */
	/*********************************************************************************/
}
