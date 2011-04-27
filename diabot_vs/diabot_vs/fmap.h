
#include <map>
#include <string>

class myFunctions
{
public:
	myFunctions();
	int travel();
	int color();
	int object();
	int number();
	int place();
	int direction();
	int distance();
	int time();
};


static class FMap
{
private:
	typedef int (myFunctions::*MemFunc)();

	class FuncMap {
    public:
        FuncMap() {
                descrToFuncMap["travel"]=&myFunctions::travel;
		}
		std::map<std::string, MemFunc> descrToFuncMap;
    };


public:
	FMap(void);
	~FMap(void);
};
