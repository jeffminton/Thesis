
#include <map>
#include <string>

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
		std::map<std::string, MemFunc> descrToFuncMap;
    };


public:
	FMap(void);
	~FMap(void);
};
