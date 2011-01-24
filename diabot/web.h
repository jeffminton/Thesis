//web.h


#include <map>
#include <vector>
#include <string>

class Attr
{
	private:
		map<string, string> attrMap;
		vector<string> keyList;
	
	public:
		Attr();
		Attr(Attr &a);
		~Attr();
		bool addAttr(string key, string value);
		vector<string> getKeyList();
		string getAttr(string key);
};
	
class Node
{
	private:
		Attr attrList;
		string val;
		string name;
		vector<Node *> children;
		vector<string> childNames;
		Node *parent;
		string parentName;
	
	public:
		Node();
		Node(string nodeName, string nodeVal);
		Node(Node &n);
		~Node();
		bool setVal(string val);
		Attr getAttrList();
		bool addAttr(string key, string value);
		string getVal();
		string getName();
};

class Web
{
	private:
		Node *root;
		Node *curr;
		bool addNode(string name, string value)
	
	public:	
		Web();
		Web(Web &w);
		~Web();
		bool parseXML(string xml);
		bool parseXML(FILE *xmlFile);
		string writeXML();
		bool writeXML(FILE *xmlFile);
};
