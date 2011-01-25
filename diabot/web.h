//web.h


#include <map>
#include <vector>
#include <string>

using namespace std;

class Attr
{
	private:
		//map containing attribute values
		map<string, string> attrMap;
		//vector containing list of all keys
		vector<string> keyList;
	
	public:
	
		/**
		 * Function:	Attr
		 * Description:	Zero param constructor
		 * Return:		an Attr object
		 */
		Attr();
		
		/**
		 * Function:	Attr
		 * Description:	Parameterized constructor
		 * Parameters:	string key - inital attribute key
		 * 				string value - initial attribute value
		 * Return:		an Attr object
		 */
		 Attr(string key, string value);
		
		/**
		 * Function:	Attr
		 * Description:	copy constructor constructor
		 * Parameters:	Attr &a - the attr object to copy
		 * Return:		an Attr object
		 */
		Attr(const Attr &a);
		
		/**
		 * Function:	~Attr
		 * Description:	destructor, called when destroying object
		 */
		~Attr();
		
		/**
		 * Function:	addAttr
		 * Description:	add an attribute to the map
		 * Parameters:	string key - attribute key, must not alread by a key
		 * 				string value - atribute value
		 * Return:		bool - true success, false failure
		 */
		bool addAttr(string key, string value);
		
		/**
		 * Function:	getKeyList
		 * Description:	return a vector containing the maps keys
		 * Return:		vector<string> - the vector containing the keys
		 */
		vector<string> getKeyList();
		
		/**
		 * Function:	getAttr
		 * Description:	get the attribute value for a key
		 * Parameters:	string key - the key to search the map for
		 * Return:		string - the value the key maps to
		 */
		string getAttr(string key);
};
	
class Node
{
	private:
		
		/**************************************************************/
		/**    Data                                                   */
		/**************************************************************/
		
		//the node attributes 
		Attr attrList;
		//nodes value
		string nodeVal;
		//node name
		string nodeName;
		//map child name to child node
		map<string, Node *> children;
		//list of child nodes
		vector<string> childNames;
		//pointer to parent node
		Node *parent;
		//name of parent node
		string parentName;
		
		/**************************************************************/
		/**     Functions                                             */
		/**************************************************************/
		
		/**
		 * Function: 	setVal
		 * Description:	set the nodes value
		 * Parameters:	string val - the value to set
		 * Return:		bool - true success, false fail
		 */
		bool setVal(string val);
	
	public:
		/**
		 * Function:	Node
		 * Description:	Zero param constructor
		 * Return:		a node
		 */
		Node();
		
		/**
		 * Function:	Node
		 * Description:	Parameterized contructor
		 * Parameters:	string nodeName - the name of the node
		 * 				string nodeVal - the value the node will hold
		 * Return:		Node
		 */
		Node(string nodeName, string nodeVal, Node *nodeParent);
		
		/**
		 * Function:	Node
		 * Description:	Copy constructor
		 * Parameters:	Node *n - node to copy
		 * Return:		Node
		 */
		Node(Node &n);
		
		/**
		 * Function:	~Node
		 * Description:	Destructor, colled when destroying object
		 */
		~Node();
		
		/**
		 * Function:	getAttrList
		 * Description:	get a nodes attribute object
		 * Return:		Attr - the nodes Attr object
		 */
		Attr getAttrList();
		
		/**
		 * Function:	addAttr
		 * Description:	add an attribute to the node in the form of a
		 * 				key/value pair
		 * Parameters:	string key - the key for the attribute, the key
		 * 				cannot already exist
		 * 				string value - the value to store
		 * Return:		bool - true success, false failure
		 */
		bool addAttr(string key, string value);
		
		/**
		 * Function:	addChild
		 * Description:	add a child to the node
		 * Parameters:	string name - the childs name
		 * 				string value - the childs value, if any
		 * Return:		bool - true success, false failure
		 */
		void addChild(string name, string value);
		
		/**
		 * Function:	getVal
		 * Description:	get the nodes value
		 * Return:		string - the nodes value
		 */
		string getVal();
		
		/**
		 * Function:	getName
		 * Description:	get the nodes name
		 * Return:		string - the nodes name
		 */
		string getName();
};

class Web
{
	private:
		//the webs root node
		Node *root;
		//the current node being looked at
		Node *curr;
			
	public:	
	
		/**
		 * Function:	Web
		 * Description:	Zero param constructor
		 * Return:		a Web object
		 */
		Web();
		
		/**
		 * Function:	Web
		 * Description:	Copy constructor
		 * Parameters:	Web &w - a web to copy
		 * Return:		a Web object
		 */
		Web(Web &w);
		
		/**
		 * Function:	~Web
		 * Description:	Destructor, called when destroying object
		 */
		~Web();
		
		/**
		 * Function:	parseXML
		 * Description:	parse a string containing XML data into a web
		 * Parameters:	string xml - string containing xml data
		 * Return:		bool - true success, false failure
		 */
		bool parseXML(string xml);
		
		/**
		 * Function:	parseXML
		 * Description:	parse the contents of a file containing xml data
		 * 				into a web
		 * Parameters:	FILE *xmlFile - pointer to file containing xml
		 * 				data
		 * Return:		bool - true success, false failure
		 */
		bool parseXML(FILE *xmlFile);
		
		/**
		 * Function:	writeXML
		 * Description:	return a string containing xml data that
		 * 				represents a web
		 * Return:		string - string containing xml data
		 */
		string writeXML();
		
		/**
		 * Function:	writeXML
		 * Description:	write xml data representing a web to a file
		 * Parameters:	FILE *xmlFile - file to write to
		 * Return:		bool - true success, false failure
		 */
		bool writeXML(FILE *xmlFile);
};
