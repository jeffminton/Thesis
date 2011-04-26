#pragma once

#include <map>

using namespace std;

class myFunctions
{
public:
	myFunctions();
	void travel();
};


class FMap
{
private:
	typedef void (myFunctions::*MemFunc)();

	class FuncMap {
    public:
        FuncMap() {
                descrToFuncMap["travel"]=&myFunctions::travel;
		}
        map<string, MemFunc> descrToFuncMap;
    };


public:
	FMap(void);
	~FMap(void);
};
