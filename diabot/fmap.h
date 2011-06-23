
#ifndef FMAP_H
#define FMAP_H

#include <map>
#include <string>
#include <vector>
//#include "web.h"
//#include "matlab.h"

using namespace std;

class Web;
class Matlab;
class Word;
class Node;

class ExecFunctions
{
public:
	ExecFunctions(){}
	/**
	Function:		travel
	*/
	int travel(Web *concepts, Matlab *mat, Word * words, int reqIdx, string theWord, vector<string> *dependsMet);
	/**
	Function:		color
	*/
	int color(Web *concepts, Matlab *mat, Word * words, int reqIdx, string theWord, vector<string> *dependsMet);
	/**
	Function:		object
	*/
	int object(Web *concepts, Matlab *mat, Word * words, int reqIdx, string theWord, vector<string> *dependsMet);
	/**
	Function:		number
	*/
	int number(Web *concepts, Matlab *mat, Word * words, int reqIdx, string theWord, vector<string> *dependsMet);
	/**
	Function:		place
	*/
	int place(Web *concepts, Matlab *mat, Word * words, int reqIdx, string theWord, vector<string> *dependsMet);
	/**
	Function:		direction
	*/
	int direction(Web *concepts, Matlab *mat, Word * words, int reqIdx, string theWord, vector<string> *dependsMet);
	/**
	Function:		distance
	*/
	int distance(Web *concepts, Matlab *mat, Word * words, int reqIdx, string theWord, vector<string> *dependsMet);
	/**
	Function:		time
	*/
	int time(Web *concepts, Matlab *mat, Word * words, int reqIdx, string theWord, vector<string> *dependsMet);
};


class FMap
{
public:
	typedef int (ExecFunctions::*ExecFunc)(Web *, Matlab *, Word *, int, string, vector<string> *);
	FMap(void) {
		theFuncMap = FuncMap();
	}
	~FMap(void){}
	ExecFunc getFunc(string funcName){
		return theFuncMap.descrToFuncMap[funcName];
	}

private:
	class FuncMap {
    public:
		FuncMap() {
                descrToFuncMap["travel"] = &ExecFunctions::travel;
				descrToFuncMap["color"] = &ExecFunctions::color;
				descrToFuncMap["object"] = &ExecFunctions::object;
				descrToFuncMap["number"] = &ExecFunctions::number;
				descrToFuncMap["place"] = &ExecFunctions::place;
				descrToFuncMap["direction"] = &ExecFunctions::direction;
				descrToFuncMap["distance"] = &ExecFunctions::distance;
				descrToFuncMap["time"] = &ExecFunctions::time;
		}
		std::map<std::string, ExecFunc> descrToFuncMap;
    };

public:
	FuncMap theFuncMap;
};



#endif