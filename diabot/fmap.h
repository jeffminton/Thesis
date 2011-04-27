
#ifndef FMAP_H
#define FMAP_H

#include <map>
#include <string>
//#include "web.h"
//#include "matlab.h"

class ExecFunctions
{
//private:
//	FMap functionMap;

public:
	ExecFunctions(){}
	int travel(){
		return 0;
	}
	int color(){
		return 0;
	}
	int object(){
		return 0;
	}
	int number(){
		return 0;
	}
	int place(){
		return 0;
	}
	int direction(){
		return 0;
	}
	int distance(){
		return 0;
	}
	int time(){
		return 0;
	}
};


class FMap
{

public:
	typedef int (ExecFunctions::*ExecFunc)();
	FMap(void) {
		theFuncMap = FuncMap();
		for(theFuncMap.funcMapIter = theFuncMap.descrToFuncMap.begin();
			theFuncMap.funcMapIter != theFuncMap.descrToFuncMap.end();
			theFuncMap.funcMapIter++) {
				funcCallStatus[(*theFuncMap.funcMapIter).first] = false;
		}
	}
	void setStatusBit(string funcName) {
		funcCallStatus[funcName] = true;
	}
	bool getStatusBit(string funcName) {
		return funcCallStatus[funcName];
	}
	void reset() {
		for(statIter = funcCallStatus.begin();
			statIter != funcCallStatus.end(); statIter++) {
				funcCallStatus[(*statIter).first] = false;
		}
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
		std::map<std::string, ExecFunc>::iterator funcMapIter;
    };
	FuncMap theFuncMap;
	map<string, bool> funcCallStatus;
	map<string, bool>::iterator statIter;

};



#endif