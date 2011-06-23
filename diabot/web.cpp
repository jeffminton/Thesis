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
	func = (FMap::ExecFunc) NULL;
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
	func = (FMap::ExecFunc) NULL;
	//func = functionMap->getFunc(name);
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
	func = n.func;
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

vector<Node *> Node::getReqPtr()
{
	return reqPtr;
}

Node * Node::getReqPtr(int idx)
{
	return reqPtr[idx];
}

FMap::ExecFunc Node::getExecFunc()
{
	return func;
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

void Word::setPos(string posIn)
{
	pos = posIn;
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
	outFile = fopen("out.log", "w");
	functionMap = new FMap();
}

Web::Web(Web &w)
{
	root = w.root;
	curr = w.curr;
	words = w.words;
	myUtil = w.myUtil;
	outFile = fopen("out.log", "w");
	functionMap = new FMap();
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
		fprintf(outFile, "Invalid root name in xml file\n");
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
	vector<Word *> currentWord;
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
			//closeing tag
			if(myUtil.match(xml[i], CLOSE_TAG))
			{
				name = myUtil.getMatch(xml[i], CLOSE_TAG_NAME, 1);
				if(tagFlow.top() != name)
				{
					fprintf(outFile, "Invalid closing tag\n");
					return false;
				}

				if(name == "word")
				{
					tempWord = new Word(nextPOS, nextConcept);

					while(!tenses.empty())
					{
						currWord = tenses.top();
						tempWord->addTense(currWord);
						currentWord = words[currWord];
						currentWord.push_back(tempWord);
						words[currWord] = currentWord;
						tenses.pop();
					}

					nextConcept = (Node *) 0;
					nextPOS = "";
				}
				tagFlow.pop();
				fprintf(outFile, "Close Tag: %s\n", name.c_str());
				//curr = curr->getParent();
			}
			else if(myUtil.match(xml[i], ONE_LINE_TAG))
			{
				fprintf(outFile, "One Line Tag: %s\n", xml[i].c_str());
			}
			else if(myUtil.match(xml[i], ONE_LINE_OPEN_CLOSE))
			{
				fprintf(outFile, "One line open close tag: %s\n", xml[i].c_str());

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
					fprintf(outFile, "invalid close tag\n");
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
						fprintf(outFile, "invalid concept tag, no parent described\n");
						return false;
					}
					nextConcept = search(root, value, attrVal);
				}
				else
				{
					fprintf(outFile, "invalid tag in word group\n");
					return false;
				}
			}
			else
			{
				attributes = myUtil.split(xml[i], ATTRIBUTE);
				name = myUtil.getMatch(xml[i], OPEN_TAG_NAME, 1);
				fprintf(outFile, "Open Tag: %s\n", name.c_str());
				tagFlow.push(name);
				for(int i = 0; i < attributes.size(); i++)
				{
					fprintf(outFile, "Attr %d: %s\n", i, attributes[i].c_str());
				}
			}
		}
	}

	if(!linkReqs(root))
	{
		fprintf(outFile, "Requirment linking failed\n");
		return false;
	}
	
	return true;
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
		//the line has an xml tag and it is not a tag that should be ignored
		if(myUtil.match(xml[i], TAG) && !myUtil.match(xml[i], IGNORE_TAG))
		{
			//closing tag
			if(myUtil.match(xml[i], CLOSE_TAG))
			{
				name = myUtil.getMatch(xml[i], CLOSE_TAG_NAME, 1);
				if(tagFlow.top() != name)
				{
					fprintf(outFile, "Invalid closing tag\n");
					return false;
				}
				tagFlow.pop();
				fprintf(outFile, "Close Tag: %s\n", name.c_str());
				curr = curr->getParent();
			}
			//oneline tage, there is not closing tag, the tag closes itself
			else if(myUtil.match(xml[i], ONE_LINE_TAG))
			{
				fprintf(outFile, "One Line Tag: %s\n", xml[i].c_str());
			}
			//open and close tag all on one line
			else if(myUtil.match(xml[i], ONE_LINE_OPEN_CLOSE))
			{
				fprintf(outFile, "One line open close tag: %s\n", xml[i].c_str());

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
					fprintf(outFile, "invalid close tag\n");
					return false;
				}

				if(name == "req")
				{
					attrKey = myUtil.getMatch(attributes[0], ATTR_SPLIT, 1);
					attrVal = myUtil.getMatch(attributes[0], ATTR_SPLIT, 2);
					if(attrKey != "parent")
					{
						fprintf(outFile, "Invalid attribute in 'req' tag\n");
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
			//must be an open tag
			else
			{
				attributes = myUtil.split(xml[i], ATTRIBUTE);
				name = myUtil.getMatch(xml[i], OPEN_TAG_NAME, 1);
				fprintf(outFile, "Open Tag: %s\n", name.c_str());
				tagFlow.push(name);
				for(int i = 0; i < attributes.size(); i++)
				{
					fprintf(outFile, "Attr %d: %s\n", i, attributes[i].c_str());
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
		//the tag should not be ignored and there is an open tag that has not been closed
		else if(!myUtil.match(xml[i], IGNORE_TAG) && !tagFlow.empty())
		{
			curr->setVal(xml[i]);
		}
	}

	if(!linkReqs(root))
	{
		fprintf(outFile, "Requirment linking failed\n");
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
				fprintf(outFile, "Found link for %s to %s child of %s\n", currNode->getName().c_str(), reqConcepts[i].c_str(), reqParents[i].c_str());
			}
			else
			{
				fprintf(outFile, "couldn't find required node to link\n");
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

bool Web::conceptExists(string nodeName, string nodeParent)
{
	return (search(root, nodeName, nodeParent) != (Node *) 0);
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
		fprintf(outFile, "reading stopped before end of file\n");
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
		fprintf(outFile, "conceptFile status: %d\nwordFile status: %d\n", (int) conceptXMLFile.fail(), (int) wordXMLFile.fail());
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
		//fprintf(outFile, "split loop %d times\n", times);
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


vector<Word *> Web::getWordList(string wordIn)
{
	return words[wordIn];
}


vector<vector<Word *>> Web::getWordList(vector<string> wordsIn)
{
	vector<vector<Word *>> wordsOut;
	for(int i = 0; i < wordsIn.size(); i++)
	{
		wordsOut.push_back(words[wordsIn[i]]);
	}

	return wordsOut;
}


bool Web::addWordNA(string wordIn)
{
	vector<Word *> naWord;

	naWord = words["NA"];
	naWord[0]->addTense(wordIn);
	words[wordIn] = naWord;
	return true;
}


vector<Word *> Web::missingWords(vector<vector<Word *>> meanings)
{
	
	vector<Node *> conceptsNeeded, conceptsPresent, tempConceptsNeeded, currReqPtr;
	vector<int> meaningsMatchedIdx = vector<int>(meanings.size(), -1);
	vector<Word *> wordsNeeded;
	Node *currConcept, *currReqGroup;
	bool reqMatched, haveNulls = false;
	Word *tempWord;
	Node *tempConcept;

	conceptsPresent = getConceptsInWordList(meanings);

	//for each list of meanings in the meaning vecotr
	for(int i = 0; i < meanings.size(); i++)
	{
		//requirments not matched for anything the the vector
		//at the current index
		reqMatched = false;
		//make sure index is not null
		if(meanings[i] != (vector<Word *>) NULL)
		{
			//for each word object in the vector at the current index
			for(int j = 0; j < meanings[i].size(); j++)
			{
				//make sure word is importans
				if(meanings[i][j]->getConcept()->getName() != "na")
				{
					//get pointer to concept in current word object
					currConcept = meanings[i][j]->getConcept();
					//for each requirment group that the current concept has
					for(int k = 0; k < currConcept->getNumReqGrp(); k++)
					{
						//if all the reqs in the current requirment group are satisfied in the
						//set of concepts known to be in the sentence, set req matched to true and break
						if(haveReqs(currConcept->getReqGrp(k)->getReqPtr(), conceptsPresent))
						{
							reqMatched = true;
							break;
						}
					}
					//if reqmatched is true set the current index in the matchedidx array to the
					//index of the word object from the vector that had satisfied requirments
					if(reqMatched == true)
					{
						meaningsMatchedIdx[i] = j;
						break;
					}
				}
				else
				{
					meaningsMatchedIdx[i] = -2;
					break;
				}
			}
		}
		else
		{
			meaningsMatchedIdx[i] = -2;
		}

	}

	for(int i = 0; i < meaningsMatchedIdx.size(); i++)
	{
		if(meaningsMatchedIdx[i] == -1)
		{
			for(int j = 0; j < meanings[i].size(); j++)
			{
				currConcept = meanings[i][j]->getConcept();
				for(int k = 0; k < currConcept->getNumReqGrp(); k++)
				{
					for(int m = 0; m < currConcept->getReqGrp(k)->getReqPtr().size(); m++)
					{
						wordsNeeded.push_back(new Word("", currConcept->getReqGrp(k)->getReqPtr(m)));
					}
				}
			}
		}
	}

	return wordsNeeded;
}


bool Web::haveReqs(vector<Node *> reqPtrs, vector<Node *> conceptsPresent)
{
	vector<bool> haveReqs = vector<bool>(reqPtrs.size(), false);

	for(int i = 0; i < reqPtrs.size(); i++)
	{
		for(int j = 0; j < conceptsPresent.size(); j++)
		{
			if(reqPtrs[i] == conceptsPresent[j])
			{
				haveReqs[i] = true;
				break;
			}
		}
	}

	for(int i = 0; i < haveReqs.size(); i++)
	{
		if(haveReqs[i] == false)
		{
			return false;
		}
	}

	return true;
}


bool Web::addWord(string pos, string wordIn, Word *meaning)
{
	vector<Word *> currWordSet;
	//add the word passed in to the set of tenses
	meaning->addTense(wordIn);
	//set the word objects part of speech
	meaning->setPos(pos);
	//get anything the word currently maps to
	currWordSet = words[wordIn];
	//add new word to set of words
	currWordSet.push_back(meaning);
	//assign word set to word in map
	words[wordIn] = currWordSet;
	return true;
}


vector<vector<Word *>> Web::getRealConcept(vector<string> wordsIn)
{
	int realWordIdx;
	vector<vector<Word *>> realWordSet, possibleWords = getWordList(wordsIn);
	vector<Node *> conceptsPresent = getConceptsInWordList(possibleWords);

	for(int i = 0; i < possibleWords.size(); i++)
	{
		realWordSet.push_back(vector<Word *>());
		for(int j = 0; j < possibleWords[i].size(); j++)
		{
			for(int k = 0; k < possibleWords[i][j]->getConcept()->getNumReqGrp(); k++)
			{
				if(haveReqs(possibleWords[i][j]->getConcept()->getReqGrp(k)->getReqPtr(), conceptsPresent) == true)
				{
					realWordSet[i].push_back(possibleWords[i][j]);
					break;
				}
			}
		}
	}
	return realWordSet;
}


//int Web::getIdxGoodWord(vector<Word *> wordList, vector<Node *> conceptsPresent)
//{
//	//for all items in wordList
//	for(int i = 0; i < wordList.size(); i++)
//	{
//		//if the 
//		if(haveReqs(wordList[i]->getConcept()->getReqPtr(), conceptsPresent) == true)
//		{
//			return i;
//		}
//	}
//	return -1;
//}


vector<Node *> Web::getConceptsInWordList(vector<vector<Word *>> wordList)
{
	vector<Node *> conceptsPresent;

	//for all vectors in meanings vector
	for(int i = 0; i < wordList.size(); i++)
	{
		if(wordList[i] != (vector<Word *>) NULL)
		{
			//for all items in each vector
			for(int j = 0; j < wordList[i].size(); j++)
			{
				//get the concept that the current Word * item points to
				conceptsPresent.push_back(wordList[i][j]->getConcept());
			}
		}
	}

	return conceptsPresent;
}


vector<vector<int>> Web::getIdxValidReqGrp(vector<vector<Word *>> wordList)
{
	vector<Node *> conceptsPresent = getConceptsInWordList(wordList);
	vector<vector<int>> idxs;

	for(int i = 0; i < wordList.size(); i++)
	{
		idxs.push_back(vector<int>());
		for(int j = 0; j < wordList[i].size(); j++)
		{
			for(int idx = 0; idx < wordList[i][j]->getConcept()->getNumReqGrp(); idx++)
			{
				if(haveReqs(wordList[i][j]->getConcept()->getReqGrp(idx)->getReqPtr(), conceptsPresent) == true)
				{
					idxs[i].push_back(idx);
				}
			}
		}
	}

	return idxs;
}