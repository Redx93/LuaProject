//#include <lua.hpp>
#pragma comment(lib, "liblua53.a")

//#include "lua.hpp" //needed for Lua
#include <lua.hpp>
#include <vector>
#include <iostream>
#include <string> 
using namespace std;
class LuaEngine
{
public:
	//Constructor
	LuaEngine();
	//LuaEngine(const LuaEngine& other);  //non-construction copy
	//LuaEngine& operator=(const LuaEngine&); //non-copy
	~LuaEngine();

	//returns lua state object
	lua_State* L();
	void CallGlobalVariable(const char* globalvar);
	//Execute Lua File
	void ExecuteFile(const char* file);
	std::string GetString(const char* p);
	void SetGlobalVariable(const char* globaName, const char* variable, lua_State* L);
	//Execute Lua Expression (contained in string)
	void ExecuteString(const char* expression);
	void report_errors(int& state);
private:
	//member lua state
	lua_State* m_L;
	//handle errors
	std::string value;
};

