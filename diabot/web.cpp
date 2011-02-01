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
{}

Attr::~Attr()
{}

void Attr::addAttr(string key, string value)
{
	attrMap[key] = value;
	keyList.push_back(value);
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
	attrList = Attr();
	nodeVal = "";
	nodeName = "";
	children = map<string, Node *>();
	childNames = vector<string>();
	parent = NULL;
	parentName = "";
}

Node::Node(string nodeName, string nodeVal, Node *nodeParent)
{
	attrList = Attr();
	nodeVal = nodeVal;
	nodeName = nodeName;
	children = map<string, Node *>();
	childNames = vector<string>();
	parent = nodeParent;
	parentName = nodeParent->getName();
}

Node::Node(Node &n)
{
}

Node::~Node()
{
}

Attr Node::getAttrList()
{
	return attrList;
}

void Node::addAttr(string key, string value)
{
	attrList.addAttr(key, value);
}

void Node::addChild(string name, string value)
{
	children[name] = new Node(name, value, this);
	childNames.push_back(name);
}

string Node::getVal()
{
	return nodeVal;
}

string Node::getName()
{
	return nodeName;
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
}

Web::Web(Web &w)
{
}

Web::~Web()
{
}

bool Web::parseXML(string xml)
{
	stack<string> tagFlow;
	string tag = "^<.+>&";
	vector<string> lines;
	
	splitStr(xml, '\n', lines);
	
	for(int i = 0; i < (int) lines.size(); i++)
	{
		printf("%s\n", lines[i].c_str());
	}
	
	return false;
}

bool Web::parseXMLFile(string xmlFileName)
{
	ifstream xmlFile;
	char *xmlString;
	int fileLen;
	
	xmlFile.open(xmlFileName.c_str(), ifstream::in);
	
	xmlFile.seekg (0, ios::end);
	fileLen = (int) xmlFile.tellg();
	xmlFile.seekg (0, ios::beg);
	
	xmlString = new char[fileLen];
	
	xmlFile.read(xmlString, fileLen);
	
	return parseXML(string(xmlString));
}

string Web::writeXML()
{
	return NULL;
}

bool Web::writeXML(FILE *xmlFile)
{
	return false;
}

void Web::splitStr(string str, char delim, vector<string> &container)
{
	size_t end;
	string line;
	
	while( (end = str.find_first_of(delim)) != string::npos)
	{
		line = string(str.begin(), str.begin() + int(end)).c_str();
		container.push_back(line);
		str = string(str.begin() + int(end), str.end());
	}
	
	line = string(str.begin(), str.end()).c_str();
	container.push_back(string(line));
}
