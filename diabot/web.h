//web.h

#include <map>
#include <vector>
#include <stack>
#include <string>
#include <iostream>
#include <fstream>
#include <gvc.h>
#include "lib\util.h"

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
		void addAttr(string key, string value);
		
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
		Attr *attrList;
		//nodes value
		string nodeVal;
		//node name
		string nodeName;
		//map child name to child node
		map<string, Node *> children;
		//list of child nodes
		vector<string> childNames;
		//list of req groups
		vector<Node *> reqs;
		//symmetric vectors contain required concepts, their parents
		//and pointers to the required concept nodes
		vector<string> reqParent;
		vector<string> reqConcept;
		vector<Node *> reqPtr;
		//pointer to parent node
		Node *parent;
		//name of parent node
		string parentName;
		
	
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
		Node(string name, Node *nodeParent);
		
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
		Attr *getAttrList();
		
		/**
		 * Function:	addAttr
		 * Description:	add an attribute to the node in the form of a
		 * 				key/value pair
		 * Parameters:	string key - the key for the attribute, the key
		 * 				cannot already exist
		 * 				string value - the value to store
		 * Return:		bool - true success, false failure
		 */
		void addAttr(string key, string value);
		
		/**
		 * Function:	addChild
		 * Description:	add a child to the node
		 * Parameters:	string name - the childs name
		 * 				string value - the childs value, if any
		 * Return:		bool - true success, false failure
		 */
		void addChild(string name);
		
		/**
		 * Function: 	setVal
		 * Description:	set the nodes value
		 * Parameters:	string val - the value to set
		 * Return:		bool - true success, false fail
		 */
		void setVal(string val);

		/**
		Function:		setReqPtr
		Description:	set the pointer in the symetic array of
						pointers to concepts
		Parameters:		int idx - index to store the pointer in
		*/
		void setReqPtr(int idx, Node *ptr);

		/**
		Function:		addReqGrp
		Description:	add a requirment group to the reqs vector
		Return:			int - the index in the reqs vector that
						the group is stored in
		*/
		int addReqGrp();

		/**
		Function:		addReq
		Description:	add requirments to the reqParent and reqConcept vector
		*/
		void addReq(string reqParentStr, string reqStr);

		/**
		 * Function:	getVal
		 * Description:	get the nodes value
		 * Return:		string - the nodes value
		 */
		string getVal();

		/**
		Function:		getParent
		Description:	return a pointer to the nodes parent
		Return:			Node * - pointer to parent
		*/
		Node * getParent();

		/**
		Function:		getChild
		Description:	return a pointer to a child
		Parameters:		string childName - name of child to return
		Return:			Node * - pointer to child
		*/
		Node * getChild(string childName);

		/**
		Function:		getChildren
		Description:	return a list of child names
		Return:			vector<string> - the names of children
		*/
		vector<string> getChildren();
		
		/**
		 * Function:	getName
		 * Description:	get the nodes name
		 * Return:		string - the nodes name
		 */
		string getName();

		/**
		Function:		getNumReqGrp
		Description:	get the number of reqgroups the node has
		Return:			int - the number of reqgroups
		*/
		int getNumReqGrp();

		/**
		Function:		getReqGrp
		Description:	return a pointer to the reqgrp at idx
		Parameters:		int idx - index of reqgrp in reqs vector
		Return:			Node * - pointer to reqgrp
		*/
		Node * getReqGrp(int idx);

		/**
		Function:		getReqConcept
		Description:	return a list of required concepts
		Return:			vector<string> - vector of required concepts
		*/
		vector<string> getReqConcept();

		/**
		Function:		getReqParent
		Description:	return the list of requried concept's parents
		Return:			vector<string> - vector of required concept's parents
		*/
		vector<string> getReqParent();

		/**
		Function:		getNumChildren
		Description:	returns the number of child nodes
		Return:			int - number of children
		*/
		int getNumChildren();
};



class Word
{
	private:
		//concept the word points to
		Node *concept;
		//part of speech the word represents
		string pos;
		//list of this words other tenses
		vector<string> tenses;

	public:
		/**
		 * Function:	Word
		 * Description:	Zero param constructor
		 * Return:		a Word
		 */
		Word();
		
		/**
		 * Function:	Word
		 * Description:	Parameterized contructor
		 * Parameters:	string pos - the pos of the word
		 * 				Node *concept - the Node holding concept it represents
		 * Return:		Word
		 */
		Word(string wordPOS, Node *wordsConcept);
		
		/**
		 * Function:	Word
		 * Description:	Copy constructor
		 * Parameters:	Word *n - Word to copy
		 * Return:		Word
		 */
		Word(Word &n);
		
		/**
		 * Function:	~Word
		 * Description:	Destructor, colled when destroying object
		 */
		~Word();

		/**
		Function:		getConcept
		Description:	return the pointer to the concept
		Return:			Node * - the concept
		*/
		Node * getConcept();

		/**
		Function:		getPOS
		Description:	return the words POS
		Return:			string - the POS of the word
		*/
		string getPOS();

		/**
		Function:		getTenses
		Description:	return the list of tenses this word has
		Return:			vector<string> - list of tenses
		*/
		vector<string> getTenses();

		/**
		Function:		mergeTenses
		Description:	add tenses in a vector to the tenses vector in object
						do not add repeats
		Parameters:		vector<string> tensesin - the tenses to add
		*/
		void addTense(vector<string> tensesIn);

		/**
		Function:		addTense
		Description:	add a word tense to the list of tenses
		Parameters:		string tense - the tense to add
		*/
		void addTense(string tense);
};

class Web
{
	private:
		//the webs root node
		Node *root;
		//the current node being looked at
		Node *curr;
		//hash table that maps words to concepts
		map<string, vector<Word *>> words, wordsBAK;
		//Util object to use
		Util myUtil;
		
		/**
		 * Function:	splitLines
		 * Description:	Split a string using newline characters, each
		 * 				index will be a line from the string
		 * Parameters:	string str - the string to split
		 * Return:		vector - vector containing lines
		 */
		void splitStr(string str, char delim, vector<string> &container);

			
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
		Function:		parseConcepts
		Description:	parse a vector containing xml data into a web
						of concepts
		Parameters:		vector<string> xml - the string containing xml data
		Return:			bool - true success false failure
		*/
		bool parseConcepts(vector<string> xml);

		/**
		Function:		parseWords
		Description:	parse a vector containing xml data into a hash
						table of words
		Parameters:		vector<string> xml - the string containing xml data
		Return:			bool - true success false failure
		*/
		bool parseWords(vector<string> xml);

		/**
		Function:		addWord
		Description:	add a word to the words map
		Parameters:		vector<string> tenses - list of tenses for the word
						string concept - represented concept
						string parentConcept - concepts parent
						string pos - the part of speach for the word
		*/
		void addWord(vector<string> tenses, string concept, string parentConcept, string pos);

		/**
		Function:		mergeTenses
		Description:	merge two vectors of tenses into one that has no repeats
		Parameters:		vector<string> tenses1 - first set of tenses, stores final merged set
						vector<string> tenses2 - second set of tenses
		*/
		void mergeTenses(vector<string> &tenses1, vector<string> &tenses2);

		/**
		Function:		postLink
		Description:	link requirments to other parts of the web
		Return:			bool - true if success, false if failure
		*/
		bool postLink();
		
		/**
		 * Function:	parseXML
		 * Description:	parse the contents of a file containing xml data
		 * 				into a web
		 * Parameters:	FILE *xmlFile - pointer to file containing xml
		 * 				data
		 * Return:		bool - true success, false failure
		 */
		bool parseXMLFile(string xmlFileName);

		/**
		Function:		writeWordsXML
		Description:	write a string of XML data that defines the words in the
						words map
		Return:			string - the XML data
		*/
		string writeWordsXML();
		
		/**
		 * Function:	writeXML
		 * Description:	return a string containing xml data that
		 * 				represents a web
		 * Return:		string - string containing xml data
		 */
		string writeXML(Node *currNode, string prefix);
		
		/**
		 * Function:	writeXML
		 * Description:	write xml data representing a web to a file
		 * Parameters:	FILE *xmlFile - file to write to
		 * Return:		bool - true success, false failure
		 */
		bool writeXML(string xmlFilePath);

		/**
		Function:		linkReqs
		Description:	Recurse through web to find all reqs
						and link them to the concepts nodes
		Parameters:		Node *currNode - the current Node
		Return:			bool - true success, false failure
		*/
		bool linkReqs(Node *currNode);

		/**
		Function:		search
		Description:	return a node that matches the search terms
		Parameters:		string nodeName - the node to find
						string nodeParent - the name of nodes parent
		Return:			Node * - pointer to the desired node
		*/
		Node * search(Node *currNode, string nodeName, string nodeParent);

		/**
		Function:		writeGraph
		Description:	generate DOT code to represent the graph visualy
		Parameters:		string graphFileName - name of file to store dot code in
						string graphDef - the DOT code
		*/
		void writeGraph(string graphFileName, string graphDef);

		/**
		Function:		genGraph
		Description:	generate a visual representation of the web
		*/
		void genGraph();

		/**
		Function:		graphNodes
		Description:	recurse through the web generating nodes in a graph
						uses breadth-first search
		Parameters:		Node *curr - current node
		*/
		string graphNodes(Node *currNode);

		/**
		Function:		graphEdges
		Description:	recurse through the web generating a graph
		Parameters:		Node *curr - current node
		Return:			string - the string of DOT commands
		*/
		string graphEdges(Node *currNode);

		/**
		Function:		getWordList
		Description:	return the vector of Word * that a word maps to
		Parameters:		string word - the word to search for
		Return:			vector<Word *> - the list of meainings
		*/
		vector<Word *> getWordList(string word);
};
