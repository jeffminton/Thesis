#include <string>
#include <engine.h>

using namespace std;

#pragma once
class Matlab
{
private:
	engine *me;
	char matBuf[4096];
public:
	Matlab(void);

	~Matlab(void);
	
	void matExec(string command);
};

