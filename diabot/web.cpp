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
}

Web::~Web()
{
}

bool Web::parseXML(string xml)
{
	//a stack conataining all open tags, checks that
	//closing tag matches the last opened tag
	stack<string> tagFlow;
	//lines - contains all lines in xml
	//attributes - contains attributes for a tag
	vector<string> lines, attributes;
	//name - tag name
	//openTag - open tag in a one line tag
	//closeTag - close tag in a one line tag
	//value - value in a one line tag
	//attrKey - the key for an attribute
	//attrVal - an attributes value
	string name, openTag, closeTag, value, attrKey, attrVal;
	
	splitStr(xml, '\n', lines);
	
	for(int i = 0; i < (int) lines.size(); i++)
	{
		
		if(myUtil.match(lines[i], TAG) && !myUtil.match(lines[i], IGNORE_TAG))
		{
			if(myUtil.match(lines[i], CLOSE_TAG))
			{
				name = myUtil.getMatch(lines[i], CLOSE_TAG_NAME, 1);
				if(tagFlow.top() != name)
				{
					printf("Invalid closing tag\n");
					return false;
				}
				tagFlow.pop();
				printf("Close Tag: %s\n", name.c_str());
				curr = curr->getParent();
			}
			else if(myUtil.match(lines[i], ONE_LINE_TAG))
			{
				printf("One Line Tag: %s\n", lines[i].c_str());
			}
			else if(myUtil.match(lines[i], ONE_LINE_OPEN_CLOSE))
			{
				printf("One line open close tag: %s\n", lines[i].c_str());

				string currLine = lines[i];
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
				attributes = myUtil.split(lines[i], ATTRIBUTE);
				name = myUtil.getMatch(lines[i], OPEN_TAG_NAME, 1);
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
		else if(!myUtil.match(lines[i], IGNORE_TAG) && !tagFlow.empty())
		{
			curr->setVal(lines[i]);
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
		/*vector<string> children = currNode->getChildren();

		for(int i = 0; i < children.size(); i++)
		{
			if(children[i] == nodeName)
			{
				Node *found = currNode->getChild(children[i]);
				return found;
			}
		}*/

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

bool Web::writeXML(string xmlFileName)
{
	string xmlOut = writeXML(root, "");
	ofstream xmlFile;
	
	xmlFile.open(xmlFileName.c_str(), ofstream::out);

	xmlFile.write(xmlOut.c_str(), xmlOut.length());
	
	xmlFile.close();

	return true;
}

void Web::splitStr(string str, char delim, vector<string> &container)
{
	size_t end;
	string line;
	int times = 0;
	
	while( (end = str.find_first_of(delim)) != string::npos)
	{
		printf("split loop %d times\n", times);
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
