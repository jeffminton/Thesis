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

bool Attr::addAttr(string key, string value)
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

bool Node::addAttr(string key, string value)
{
	return attrList.addAttr(key, value);
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
	return false;
}

bool Web::parseXML(FILE *xmlFile)
{
	return false;
}

string Web::writeXML()
{
	return NULL;
}

bool Web::writeXML(FILE *xmlFile)
{
	return false;
}


