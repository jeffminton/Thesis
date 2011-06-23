#include "fmap.h"
#include "web.h"
#include "matlab.h"

int ExecFunctions::travel(Web *concepts, Matlab *mat, Word * words, int reqIdx, string theWord, vector<string> *dependsMet){
	vector<Node *> reqPtrs = words->getConcept()->getReqGrp(reqIdx)->getReqPtr();
	vector<Node *>::iterator nodeIter = reqPtrs.begin();
	bool dependMet;

	for(nodeIter; nodeIter != reqPtrs.end(); nodeIter++){
		dependMet = false;
		for(int i = 0; i < dependsMet->size(); i++){
			if((*nodeIter)->getName() == dependsMet->at(i)){
				dependMet = true;
			}
		}
		if(dependMet == false){
			return -1;
		}
	}

	string paramString = "";

	reqPtrs = words->getConcept()->getReqGrp(reqIdx)->getReqPtr();
	nodeIter = reqPtrs.begin();
	for(nodeIter; nodeIter != reqPtrs.end(); nodeIter++){
		paramString += "'" + (*nodeIter)->getName() + "',";
	}

	paramString[paramString.length() - 1] = '/0';
	mat->matExec("goto(" + paramString + ")");
	dependsMet->push_back((*nodeIter)->getName());

	return 0;
}



/**
Function:		color
*/
int ExecFunctions::color(Web *concepts, Matlab *mat, Word * words, int reqIdx, string theWord, vector<string> *dependsMet){
	vector<Node *> reqPtrs = words->getConcept()->getReqGrp(reqIdx)->getReqPtr();
	vector<Node *>::iterator nodeIter = reqPtrs.begin();
	bool dependMet;

	for(nodeIter; nodeIter != reqPtrs.end(); nodeIter++){
		dependMet = false;
		for(int i = 0; i < dependsMet->size(); i++){
			if((*nodeIter)->getName() == dependsMet->at(i)){
				dependMet = true;
			}
		}
		if(dependMet == false){
			return -1;
		}
	}

	mat->matExec("i = 0");
	dependsMet->push_back((*nodeIter)->getName());
	return 0;
}
	
/**
Function:		object
*/
int ExecFunctions::object(Web *concepts, Matlab *mat, Word * words, int reqIdx, string theWord, vector<string> *dependsMet){
	vector<Node *> reqPtrs = words->getConcept()->getReqGrp(reqIdx)->getReqPtr();
	vector<Node *>::iterator nodeIter = reqPtrs.begin();
	bool dependMet;

	for(nodeIter; nodeIter != reqPtrs.end(); nodeIter++){
		dependMet = false;
		for(int i = 0; i < dependsMet->size(); i++){
			if((*nodeIter)->getName() == dependsMet->at(i)){
				dependMet = true;
			}
		}
		if(dependMet == false){
			return -1;
		}
	}

	mat->matExec("i = 0");
	dependsMet->push_back((*nodeIter)->getName());
	return 0;
}

/**
Function:		number
*/
int ExecFunctions::number(Web *concepts, Matlab *mat, Word * words, int reqIdx, string theWord, vector<string> *dependsMet){
	vector<Node *> reqPtrs = words->getConcept()->getReqGrp(reqIdx)->getReqPtr();
	vector<Node *>::iterator nodeIter = reqPtrs.begin();
	bool dependMet;

	for(nodeIter; nodeIter != reqPtrs.end(); nodeIter++){
		dependMet = false;
		for(int i = 0; i < dependsMet->size(); i++){
			if((*nodeIter)->getName() == dependsMet->at(i)){
				dependMet = true;
			}
		}
		if(dependMet == false){
			return -1;
		}
	}

	mat->matExec("i = 0");
	dependsMet->push_back((*nodeIter)->getName());
	return 0;
}

/**
Function:		place
*/
int ExecFunctions::place(Web *concepts, Matlab *mat, Word * words, int reqIdx, string theWord, vector<string> *dependsMet){
	vector<Node *> reqPtrs = words->getConcept()->getReqGrp(reqIdx)->getReqPtr();
	vector<Node *>::iterator nodeIter = reqPtrs.begin();
	bool dependMet;

	for(nodeIter; nodeIter != reqPtrs.end(); nodeIter++){
		dependMet = false;
		for(int i = 0; i < dependsMet->size(); i++){
			if((*nodeIter)->getName() == dependsMet->at(i)){
				dependMet = true;
			}
		}
		if(dependMet == false){
			return -1;
		}
	}

	mat->matExec("i = 0");
	dependsMet->push_back((*nodeIter)->getName());
	return 0;
}

/**
Function:		direction
*/
int ExecFunctions::direction(Web *concepts, Matlab *mat, Word * words, int reqIdx, string theWord, vector<string> *dependsMet){
	vector<Node *> reqPtrs = words->getConcept()->getReqGrp(reqIdx)->getReqPtr();
	vector<Node *>::iterator nodeIter = reqPtrs.begin();
	bool dependMet;

	for(nodeIter; nodeIter != reqPtrs.end(); nodeIter++){
		dependMet = false;
		for(int i = 0; i < dependsMet->size(); i++){
			if((*nodeIter)->getName() == dependsMet->at(i)){
				dependMet = true;
			}
		}
		if(dependMet == false){
			return -1;
		}
	}

	mat->matExec("i = 0");
	dependsMet->push_back((*nodeIter)->getName());
	return 0;
}

/**
Function:		distance
*/
int ExecFunctions::distance(Web *concepts, Matlab *mat, Word * words, int reqIdx, string theWord, vector<string> *dependsMet){
	vector<Node *> reqPtrs = words->getConcept()->getReqGrp(reqIdx)->getReqPtr();
	vector<Node *>::iterator nodeIter = reqPtrs.begin();
	bool dependMet;

	for(nodeIter; nodeIter != reqPtrs.end(); nodeIter++){
		dependMet = false;
		for(int i = 0; i < dependsMet->size(); i++){
			if((*nodeIter)->getName() == dependsMet->at(i)){
				dependMet = true;
			}
		}
		if(dependMet == false){
			return -1;
		}
	}

	mat->matExec("i = 0");
	dependsMet->push_back((*nodeIter)->getName());
	return 0;
}

/**
Function:		time
*/
int ExecFunctions::time(Web *concepts, Matlab *mat, Word * words, int reqIdx, string theWord, vector<string> *dependsMet){
	vector<Node *> reqPtrs = words->getConcept()->getReqGrp(reqIdx)->getReqPtr();
	vector<Node *>::iterator nodeIter = reqPtrs.begin();
	bool dependMet;

	for(nodeIter; nodeIter != reqPtrs.end(); nodeIter++){
		dependMet = false;
		for(int i = 0; i < dependsMet->size(); i++){
			if((*nodeIter)->getName() == dependsMet->at(i)){
				dependMet = true;
			}
		}
		if(dependMet == false){
			return -1;
		}
	}

	mat->matExec(string("i = 0"));
	dependsMet->push_back((*nodeIter)->getName());
	return 0;
}