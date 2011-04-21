//web.cpp

#include "web.h"


/**********************************************************************/
/**
 * Class:	Attr
 */
/**********************************************************************/

Attr::Attr()
{
	attrMap = map<string, string>();
	keyList = vector<string>();
}

Attr::Attr(string key, string value)
{
	attrMap = map<string, string>();
	keyList = vector<string>();
	
	addAttr(key, value);
}

Attr::Attr(const Attr &a)
{
	attrMap = a.attrMap;
	keyList = a.keyList;
}

Attr::~Attr()
{}

void Attr::addAttr(string key, string value)
{
	if(attrMap[key] == "")
	{
		keyList.push_back(key);
	}
	attrMap[key] = value;
}

vector<string> Attr::getKeyList()
{
	return keyList;
}

string Attr::getAttr(string key)
{
	return attrMap[key];
}



/**********************************************************************/
/**
 * Class:	Node
 */
/**********************************************************************/
Node::Node()
{
	attrList = new Attr();
	nodeVal = "";
	nodeName = "";
	children = map<string, Node *>();
	childNames = vector<string>();
	reqs = vector<Node *>();
	reqParent = vector<string>();
	reqConcept = vector<string>();
	reqPtr = vector<Node *>();
	parent = NULL;
	parentName = "";
}

Node::Node(string name, Node *nodeParent)
{
	attrList = new Attr();
	nodeName = name;
	children = map<string, Node *>();
	childNames = vector<string>();
	reqs = vector<Node *>();
	reqParent = vector<string>();
	reqConcept = vector<string>();
	reqPtr = vector<Node *>();
	parent = nodeParent;
	if(parent != NULL)
	{
		parentName = nodeParent->getName();
	}
	else
	{
		parentName = "";
	}
	addAttr("id", nodeName);
}

Node::Node(Node &n)
{
	attrList = n.getAttrList();
	nodeName = n.getName();
	children = n.children;
	childNames = n.getChildren();
	reqs = n.reqs;
	reqParent = n.reqParent;
	reqConcept = n.reqConcept;
	reqPtr = n.reqPtr;
	parent = n.getParent();
	parentName = n.parentName;
}

Node::~Node()
{
}

Attr *Node::getAttrList()
{
	return attrList;
}

void Node::addAttr(string key, string value)
{
	attrList->addAttr(key, value);
}

void Node::addChild(string name)
{
	children[name] = new Node(name, this);
	childNames.push_back(name);
}

int Node::addReqGrp()
{
	reqs.push_back(new Node("reqgrp", this));
	return reqs.size() - 1;
}

void Node::addReq(string reqParentStr, string reqStr)
{
	reqParent.push_back(reqParentStr);
	reqConcept.push_back(reqStr);
	reqPtr.push_back(NULL);
}

void Node::setVal(string val)
{
	nodeVal = val;
}

void Node::setReqPtr(int idx, Node *ptr)
{
	reqPtr[idx] = ptr;
}

string Node::getVal()
{
	return nodeVal;
}

Node * Node::getParent()
{
	return parent;
}

Node * Node::getChild(string childName)
{
	Node *found = children[childName];
	return found;
}

string Node::getName()
{
	return nodeName;
}

Node * Node::getReqGrp(int idx)
{
	return reqs[idx];
}

int Node::getNumReqGrp()
{
	return reqs.size();
}

vector<string> Node::getReqConcept()
{
	return reqConcept;
}

vector<string> Node::getReqParent()
{
	return reqParent;
}

int Node::getNumChildren()
{
	return childNames.size();
}

vector<string> Node::getChildren()
{
	return childNames;
}




/**********************************************************************/
/**
Class:		Word
*/
/**********************************************************************/
Word::Word()
{
	pos = "";
	concept = (Node *) 0;
}

Word::Word(string wordPOS, Node *wordsConcept)
{
	pos = wordPOS;
	concept = wordsConcept;
}

Word::Word(Word &w)
{
	pos = w.getPOS();
	concept = w.getConcept();
}

Node * Word::getConcept()
{
	return concept;
}

string Word::getPOS()
{
	return pos;
}

vector<string> Word::getTenses()
{
	return tenses;
}

void Word::addTense(string tense)
{
	tenses.push_back(tense);
}

void Word::addTense(vector<string> tensesIn)
{
	bool tenseExists = true;

	for(int i = 0; i < tensesIn.size(); i++)
	{
		for(int j = 0; j < tenses.size(); j++)
		{
			if(tenses[j] == tensesIn[i])
			{
				tenseExists = false;
			}
		}
		if(tenseExists == false)
		{
			tenses.push_back(tensesIn[i]);
			tenseExists = true;
		}
	}
}

/**********************************************************************/
/**
 * Class:	Web
 */
/**********************************************************************/
Web::Web()
{
	root = NULL;
	curr = NULL;
	myUtil = Util();
}

Web::Web(Web &w)
{
	root = w.root;
	curr = w.curr;
	words = w.words;
	myUtil = w.myUtil;
}

Web::~Web()
{
}

bool Web::parseXML(string xml)
{
	int iter = 0;
	vector<string> lines;
	string rootName;

	splitStr(xml, '\n', lines);

	while(myUtil.match(lines[iter], IGNORE_TAG))
	{
		iter++;
	}

	rootName = myUtil.getMatch(lines[iter], OPEN_TAG_NAME, 1);

	if(rootName == "concepts")
	{
		return parseConcepts(lines);
	}
	else if(rootName == "words")
	{
		return parseWords(lines);
	}
	else
	{
		printf("Invalid root name in xml file\n");
		return false;
	}
}


bool Web::parseWords(vector<string> xml)
{
	//a stack conataining all open tags, checks that
	//closing tag matches the last opened tag
	stack<string> tagFlow, tenses;
	//attributes - contains attributes for a tag
	vector<string> attributes;
	//vector of Word objects that a word points to
	vector<Word *> wordArray;
	//name - tag name
	//openTag - open tag in a one line tag
	//closeTag - close tag in a one line tag
	//value - value in a one line tag
	//attrKey - the key for an attribute
	//attrVal - an attributes value
	//nextPOS - the pos for the current word
	//currWord - the current word being defined
	string name, openTag, closeTag, value, attrKey, attrVal, nextPOS, currWord;
	//nextConcept - the concept for the current word
	Node *nextConcept;
	//tempWord - a pointer to a temporary word object
	Word *tempWord;
	
	for(int i = 0; i < (int) xml.size(); i++)
	{
		
		if(myUtil.match(xml[i], TAG) && !myUtil.match(xml[i], IGNORE_TAG))
		{
			if(myUtil.match(xml[i], CLOSE_TAG))
			{
				name = myUtil.getMatch(xml[i], CLOSE_TAG_NAME, 1);
				if(tagFlow.top() != name)
				{
					printf("Invalid closing tag\n");
					return false;
				}

				if(name == "word")
				{
					tempWord = new Word(nextPOS, nextConcept);

					while(!tenses.empty())
					{
						currWord = tenses.top();
						tempWord->addTense(currWord);
						wordArray = words[currWord];
						wordArray.push_back(tempWord);
						words[currWord] = wordArray;
						tenses.pop();
					}

					nextConcept = (Node *) 0;
					nextPOS = "";
				}
				tagFlow.pop();
				printf("Close Tag: %s\n", name.c_str());
				//curr = curr->getParent();
			}
			else if(myUtil.match(xml[i], ONE_LINE_TAG))
			{
				printf("One Line Tag: %s\n", xml[i].c_str());
			}
			else if(myUtil.match(xml[i], ONE_LINE_OPEN_CLOSE))
			{
				printf("One line open close tag: %s\n", xml[i].c_str());

				string currLine = xml[i];
				string closeName;

				openTag = myUtil.getMatch(currLine, ONE_LINE_OPEN_CLOSE, 1);
				name = myUtil.getMatch(openTag, OPEN_TAG_NAME, 1);
				attributes = myUtil.split(openTag, ATTRIBUTE);
				value = myUtil.getMatch(currLine, ONE_LINE_OPEN_CLOSE, 2);
				closeTag = myUtil.getMatch(currLine, ONE_LINE_OPEN_CLOSE, 3);
				closeName = myUtil.getMatch(closeTag, CLOSE_TAG_NAME, 1);

				if(name != closeName)
				{
					printf("invalid close tag\n");
					return false;
				}

				if(name == "tense")
				{
					/*attrKey = myUtil.getMatch(attributes[0], ATTR_SPLIT, 1);
					attrVal = myUtil.getMatch(attributes[0], ATTR_SPLIT, 2);*/
					tenses.push(value);
				}
				else if(name == "pos")
				{
					nextPOS = value;
				}
				else if(name == "concept")
				{
					attrKey = myUtil.getMatch(attributes[0], ATTR_SPLIT, 1);
					attrVal = myUtil.getMatch(attributes[0], ATTR_SPLIT, 2);
					if(attrKey != "parent")
					{
						printf("invalid concept tag, no parent described\n");
						return false;
					}
					nextConcept = search(root, value, attrVal);
				}
				else
				{
					printf("invalid tag in word group\n");
					return false;
				}
			}
			else
			{
				attributes = myUtil.split(xml[i], ATTRIBUTE);
				name = myUtil.getMatch(xml[i], OPEN_TAG_NAME, 1);
				printf("Open Tag: %s\n", name.c_str());
				tagFlow.push(name);
				for(int i = 0; i < attributes.size(); i++)
				{
					printf("Attr %d: %s\n", i, attributes[i].c_str());
				}
			}
		}
	}

	if(!linkReqs(root))
	{
		printf("Requirment linking failed\n");
		return false;
	}
	
	return true;
}


void Web::addWord(vector<string> tenses, string concept, string parentConcept, string pos)
{
	vector<string> existingTenses;
	vector<vector<Word *>> existingWordVectors;
	vector<Word *> wordVector;
	Node * currConcept;
	Word * currWord;
	bool conceptMatched = false;

	currConcept = search(root, concept, parentConcept);
	
	for(int i = 0; i < tenses.size(); i++)
	{
		if(words[tenses[i]] != (vector<Word *>) NULL)
		{
			existingWordVectors.push_back(words[tenses[i]]);
		}
	}

	if(!existingWordVectors.empty())
	{
		currWord = new Word(pos, currConcept);
		for(int i = 0; i < tenses.size(); i++)
		{
			currWord->addTense(tenses[i]);
		}
		wordVector.push_back(currWord);
		for(int i = 0; i < tenses.size(); i++)
		{
			words[tenses[i]] = wordVector;
		}
	}
	else
	{
		for(int i = 0; i < existingWordVectors.size(), conceptMatched == false; i++)
		{
			wordVector = existingWordVectors.back();
			existingWordVectors.pop_back();
			for(int j = 0; j < wordVector.size(), conceptMatched == false; j++)
			{
				if(wordVector[j]->getConcept() == currConcept)
				{
					conceptMatched = true;

				}
			}
		}
	}
}


bool Web::parseConcepts(vector<string> xml)
{
	//a stack conataining all open tags, checks that
	//closing tag matches the last opened tag
	stack<string> tagFlow;
	//attributes - contains attributes for a tag
	vector<string> attributes;
	//name - tag name
	//openTag - open tag in a one line tag
	//closeTag - close tag in a one line tag
	//value - value in a one line tag
	//attrKey - the key for an attribute
	//attrVal - an attributes value
	string name, openTag, closeTag, value, attrKey, attrVal;
	
	for(int i = 0; i < (int) xml.size(); i++)
	{
		
		if(myUtil.match(xml[i], TAG) && !myUtil.match(xml[i], IGNORE_TAG))
		{
			if(myUtil.match(xml[i], CLOSE_TAG))
			{
				name = myUtil.getMatch(xml[i], CLOSE_TAG_NAME, 1);
				if(tagFlow.top() != name)
				{
					printf("Invalid closing tag\n");
					return false;
				}
				tagFlow.pop();
				printf("Close Tag: %s\n", name.c_str());
				curr = curr->getParent();
			}
			else if(myUtil.match(xml[i], ONE_LINE_TAG))
			{
				printf("One Line Tag: %s\n", xml[i].c_str());
			}
			else if(myUtil.match(xml[i], ONE_LINE_OPEN_CLOSE))
			{
				printf("One line open close tag: %s\n", xml[i].c_str());

				string currLine = xml[i];
				string closeName;

				openTag = myUtil.getMatch(currLine, ONE_LINE_OPEN_CLOSE, 1);
				name = myUtil.getMatch(openTag, OPEN_TAG_NAME, 1);
				attributes = myUtil.split(openTag, ATTRIBUTE);
				value = myUtil.getMatch(currLine, ONE_LINE_OPEN_CLOSE, 2);
				closeTag = myUtil.getMatch(currLine, ONE_LINE_OPEN_CLOSE, 3);
				closeName = myUtil.getMatch(closeTag, CLOSE_TAG_NAME, 1);

				if(name != closeName)
				{
					printf("invalid close tag\n");
					return false;
				}

				if(name == "req")
				{
					attrKey = myUtil.getMatch(attributes[0], ATTR_SPLIT, 1);
					attrVal = myUtil.getMatch(attributes[0], ATTR_SPLIT, 2);
					if(attrKey != "parent")
					{
						printf("Invalid attribute in 'req' tag\n");
						return false;
					}
					curr->addReq(attrVal, value);
				}
				else
				{
					curr->addChild(name);
					curr = curr->getChild(name);
					for(int i = 0; i < attributes.size(); i++)
					{
						attrKey = myUtil.getMatch(attributes[i], ATTR_SPLIT, 1);
						attrVal = myUtil.getMatch(attributes[i], ATTR_SPLIT, 2);
						curr->addAttr(attrKey, attrVal);
					}
					curr->setVal(value);
					curr = curr->getParent();
				}
			}
			else
			{
				attributes = myUtil.split(xml[i], ATTRIBUTE);
				name = myUtil.getMatch(xml[i], OPEN_TAG_NAME, 1);
				printf("Open Tag: %s\n", name.c_str());
				tagFlow.push(name);
				for(int i = 0; i < attributes.size(); i++)
				{
					printf("Attr %d: %s\n", i, attributes[i].c_str());
				}

				if(root == NULL)
				{
					root = new Node(name, NULL);
					curr = root;
				}
				else
				{
					if(name == "reqgrp")
					{
						curr = curr->getReqGrp(curr->addReqGrp());
					}
					else
					{
						curr->addChild(name);
						curr = curr->getChild(name);
					}
				}

				for(int i = 0; i < attributes.size(); i++)
				{
					attrKey = myUtil.getMatch(attributes[i], ATTR_SPLIT, 1);
					attrVal = myUtil.getMatch(attributes[i], ATTR_SPLIT, 2);
					curr->addAttr(attrKey, attrVal);
				}
			}
		}
		else if(!myUtil.match(xml[i], IGNORE_TAG) && !tagFlow.empty())
		{
			curr->setVal(xml[i]);
		}
	}

	if(!linkReqs(root))
	{
		printf("Requirment linking failed\n");
		return false;
	}
	
	return true;
}

bool Web::linkReqs(Node *currNode)
{
	bool result = false;

	if(currNode->getName() == "reqgrp")
	{
		vector<string> reqConcepts, reqParents;
		reqConcepts = currNode->getReqConcept();
		reqParents = currNode->getReqParent();
		Node *foundPtr;

		for(int i = 0; i < reqConcepts.size(); i++)
		{
			foundPtr = search(root, reqConcepts[i], reqParents[i]);
			if(foundPtr != NULL)
			{
				currNode->setReqPtr(i, foundPtr);
				result = true;
				printf("Found link for %s to %s child of %s\n", currNode->getName().c_str(), reqConcepts[i].c_str(), reqParents[i].c_str());
			}
			else
			{
				printf("couldn't find required node to link\n");
			}
		}

		return result;
	}
	else
	{
		if(currNode->getNumReqGrp() > 0)
		{
			for(int i = 0; i < currNode->getNumReqGrp(); i++)
			{
				result = linkReqs(currNode->getReqGrp(i));
			}
		}
		else
		{
			vector<string> children = currNode->getChildren();

			for(int i = 0; i < children.size(); i++)
			{
				result = linkReqs(currNode->getChild(children[i]));
			}
		}
	}

	return result;
}

Node * Web::search(Node *currNode, string nodeName, string nodeParent)
{
	if(currNode->getName() == nodeName && currNode->getParent()->getName() == nodeParent)
	{
		Node *found = currNode;
		return found;
	}
	else if(currNode->getName() == nodeParent)
	{
		Node *found = currNode->getChild(nodeName);
		return found;
	}
	else
	{
		vector<string> children = currNode->getChildren();

		if(children.empty())
		{
			return (Node *) NULL;
		}
		else
		{
			Node *desiredNode;
			for(int i = 0; i < children.size(); i++)
			{
				desiredNode = search(currNode->getChild(children[i]), nodeName, nodeParent);
				if(desiredNode != NULL)
				{
					return desiredNode;
				}
			}

			return (Node *) 0;
		}
	}
}


bool Web::parseXMLFile(string xmlFileName)
{
	ifstream xmlFile;
	char in;
	string xmlString;
	int fileLen;
	
	xmlFile.open(xmlFileName.c_str(), ifstream::in);

	while(xmlFile.get(in))
	{
		xmlString += in;
	}

	if(!xmlFile.eof())
	{
		printf("reading stopped before end of file\n");
		return false;
	}

	xmlFile.close();
	
	return parseXML(string(xmlString));
}

string Web::writeXML(Node *currNode, string prefix)
{
	string xmlOut;
	Attr *currAttributes;
	vector<string> attrKeys;

	xmlOut += prefix + "<" + currNode->getName();

	currAttributes = currNode->getAttrList();
	attrKeys = currAttributes->getKeyList();

	if(attrKeys.size() > 0)
	{
		for(int i = 0; i < attrKeys.size(); i++)
		{
			xmlOut += " " + attrKeys[i] + "=\"" + currAttributes->getAttr(attrKeys[i]) + "\"";
		}
	}

	xmlOut += ">\n";

	vector<string> children = currNode->getChildren();
	int reqGrpCount = currNode->getNumReqGrp();

	for(int i = 0; i < children.size(); i++)
	{
		xmlOut += writeXML(currNode->getChild(children[i]), prefix + '\t');
	}

	if(reqGrpCount > 0)
	{
		for(int i = 0; i < reqGrpCount; i++)
		{
			xmlOut += writeXML(currNode->getReqGrp(i), prefix + '\t');
		}
	}

	if(currNode->getName() == "reqgrp")
	{
		vector<string> reqConcept = currNode->getReqConcept();
		vector<string> reqParent = currNode->getReqParent();

		for(int i = 0; i < reqConcept.size(); i++)
		{
			xmlOut += prefix + '\t' + "<req parent=\"" + reqParent[i] + "\">" + reqConcept[i] + "</req>\n";
		}
	}

	xmlOut += prefix + "</" + currNode->getName() + ">\n";
	
	return xmlOut;
}

string Web::writeWordsXML()
{
	wordsBAK = map<string, vector<Word *>>(words);
	string xmlString = "<words>\n";
	vector<string> tenses;
	vector<Word *> meanings;
	map<string, vector<Word *>>::iterator wIT;
	string currPOS, concept, parentConcept;

	for(wIT = words.begin(); wIT != words.end(); wIT++)
	{
		if((*wIT).second.size() > 0)
		{
			meanings = (*wIT).second;
			for(int i = 0; i < meanings.size(); i++)
			{
				currPOS = meanings[i]->getPOS();
				concept = meanings[i]->getConcept()->getName();
				parentConcept = meanings[i]->getConcept()->getParent()->getName();
				tenses = meanings[i]->getTenses();

				xmlString += "\t<word>\n";
				for(int j = 0; j < tenses.size(); j++)
				{
					xmlString += "\t\t<tense>" + tenses[j] + "</tense>\n";
					int k = 0;
					while(parentConcept != meanings[k]->getConcept()->getParent()->getName()
						&& concept != meanings[k]->getConcept()->getName() && k < meanings.size())
					{
						k++;
					}
					if(k < meanings.size())
					{
						words[tenses[j]].erase(words[tenses[j]].begin() + k);
					}
				}

				xmlString += "\t\t<concept parent=\"" + parentConcept + "\">" + concept + "</concept>\n";
				xmlString += "\t\t<pos>" + currPOS + "</pos>\n";
				xmlString += "\t</word>\n";
			}
		}
	}

	xmlString += "</words>\n";
	words = map<string, vector<Word *>>(wordsBAK);

	return xmlString;
}

bool Web::writeXML(string xmlFilePath)
{
	if(xmlFilePath[xmlFilePath.length() - 1] != '/')
	{
		xmlFilePath += "/";
	}
	
	ofstream conceptXMLFile, wordXMLFile;
	
	conceptXMLFile.open((xmlFilePath + "concepts.xml").c_str(), ofstream::out);
	wordXMLFile.open((xmlFilePath + "words.xml").c_str(), ofstream::out);

	if(conceptXMLFile.fail() || wordXMLFile.fail())
	{
		printf("conceptFile status: %d\nwordFile status: %d\n", (int) conceptXMLFile.fail(), (int) wordXMLFile.fail());
		return false;
	}

	string conceptXMLOut = writeXML(root, "");
	string wordXMLOut = writeWordsXML();

	conceptXMLFile.write(conceptXMLOut.c_str(), conceptXMLOut.length());
	wordXMLFile.write(wordXMLOut.c_str(), wordXMLOut.length());
	
	conceptXMLFile.close();
	wordXMLFile.close();

	return true;
}

void Web::splitStr(string str, char delim, vector<string> &container)
{
	size_t end;
	string line;
	int times = 0;
	
	while( (end = str.find_first_of(delim)) != string::npos)
	{
		//printf("split loop %d times\n", times);
		times++;
		line = string(str.begin(), str.begin() + int(end) + 1);
		myUtil.strip(line);
		container.push_back(line);
		str = string(str.begin() + int(end) + 1, str.end());
	}
	
	line = string(str.begin(), str.end());
	myUtil.strip(line);
	container.push_back(string(line));
}



void Web::writeGraph(string graphFileName, string graphDef)
{
	ofstream graphFile;
	
	graphFile.open(graphFileName.c_str(), ofstream::out);

	graphFile.write(graphDef.c_str(), graphDef.length());
	
	graphFile.close();
}


void Web::genGraph()
{
	string dotText = "digraph web{\n";

	//dotText += graphNodes(root);
	dotText += graphEdges(root);

	dotText += "}\n";

	writeGraph("web.gv", dotText);
}

/*
"node6" [
label = "<f0> 0xf7fc4380| <f1> | <f2> |1"
shape = "record"
]*/

string Web::graphEdges(Node *currNode)
{
	string graphString = "";
	char strBuild[2048];
	Node *currReq;
	Node *parentNode;
	string currString, reqString, reqPartString;
	
	if(currNode->getNumReqGrp() > 0)
	{
		//nodeName from parentName
		sprintf(strBuild, "\"\\n%s\\n\"", currNode->getName().c_str(),
			currNode->getParent()->getName().c_str());
		currString = strBuild;
		graphString += currString + " [color=blue];\n";
		
		for(int i = 0; i < currNode->getNumReqGrp(); i++)
		{
			currReq = currNode->getReqGrp(i);
			vector<string> reqConcepts, reqParents;
			reqConcepts = currReq->getReqConcept();
			reqParents = currReq->getReqParent();

			//nodeName reqgrp 0
			sprintf(strBuild, "\"%s reqgrp %d\"",
				currNode->getName().c_str(), i);
			reqString = strBuild;

			graphString += reqString + " [color=red];\n";

			graphString += currString + " -> " + reqString + " [color=blue];\n";

			for(int j = 0; j < reqConcepts.size(); j++)
			{
				sprintf(strBuild, "\"%s reqgrp %d req %d\\n\\n%s from %s\"",
					currNode->getName().c_str(), i, j, reqConcepts[j].c_str(),
					reqParents[j].c_str());

				graphString += string(strBuild) + " [color=green];\n";

				graphString += reqString + " -> " + strBuild + " [color=red];\n";
			}
		}

		return graphString;
	}
	else
	{
		vector<string> children = currNode->getChildren();

		//nodeName from parentName
		sprintf(strBuild, "\"\\n%s\\n\"", currNode->getName().c_str(),
			(currNode != root ? currNode->getParent()->getName() : "").c_str());
		currString = strBuild;

		for(int i = 0; i < children.size(); i++)
		{
			//"nodeName from parentName" -> "nodeName from parentName;"
			sprintf(strBuild, "%s -> \"\\n%s\\n\";\n",
				currString.c_str(), currNode->getChild(children[i])->getName().c_str(),
				currNode->getName().c_str());
			graphString += strBuild;
			graphString += graphEdges(currNode->getChild(children[i]));
		}

		return graphString;
	}
}

string Web::graphNodes(Node *currNode)
{
	string graphString = "";
	char strBuild[2048];
	Node *parentNode;
	Node *currReq;

	if(currNode->getNumReqGrp() > 0)
	{
		//nodeName from parentName
		sprintf(strBuild, "\"%s from %s\";\n", currNode->getName().c_str(),
			currNode->getParent()->getName().c_str());
		graphString += strBuild;
		
		vector<string> reqConcepts, reqParents;
		for(int i = 0; i < currNode->getNumReqGrp(); i++)
		{
			currReq = currNode->getReqGrp(i);
			reqConcepts = currReq->getReqConcept();
			reqParents = currReq->getReqParent();

			//nodeName reqgrp 0
			sprintf(strBuild, "\"%s reqgrp %d\";\n",
				currNode->getName().c_str(), i);
			graphString += strBuild;
		}
		return graphString;
	}
	else
	{
		vector<string> children = currNode->getChildren();

		//nodeName from parentName
		sprintf(strBuild, "\"%s from %s\";\n", currNode->getName().c_str(),
			(currNode != root ? currNode->getParent()->getName() : "").c_str());
		graphString += strBuild;

		for(int i = 0; i < children.size(); i++)
		{
			 graphString += graphNodes(currNode->getChild(children[i]));
		}

		return graphString;
	}
}


vector<Word *> Web::getWordList(string word)
{
	return words[word];
}