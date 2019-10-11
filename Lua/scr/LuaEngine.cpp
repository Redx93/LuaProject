#include "LuaEngine.h"
//executes a lua script file
void LuaEngine::ExecuteFile(const char* file, int& state)
{
	if (file == NULL)
		return;
	//// Load the string and call it with lua_pcall to catch errors. If there is an error, handle it.
	state = luaL_loadfile(m_L, file) || lua_pcall(m_L, 0, 0, 0);
	report_errors(state);
}

std::string LuaEngine::GetString(const char* p)
{
	std::string str;
	lua_getglobal(this->m_L, p);
	if (lua_isstring(this->m_L, -1))
	{
		str = lua_tostring(this->m_L, -1);
	}
	return str;
}

void LuaEngine::SetGlobalVariable(const char* globaName,const char* variable, lua_State* L)
{

	lua_pushstring(L, variable);
	lua_setglobal(L, globaName);
	lua_pop(L, -1);
}

//execute a lua expression contained in string
//
//(could contain a full script with line breaks)
void LuaEngine::ExecuteString(const char* expression)
{
	if (expression == NULL)
	{
		std::cerr << "ERR: null expression passed to script engine!" << std::endl;
		return;
	}

	int state = luaL_dostring(m_L, expression);
	report_errors(state);
}
LuaEngine::LuaEngine() : m_L(luaL_newstate())
{ 
	luaL_openlibs(m_L);
}
LuaEngine::~LuaEngine()
{ 
	 lua_close(m_L);
}
//returns lua state object
lua_State* LuaEngine::L()
{
	return m_L;
}
void LuaEngine::CallGlobalVariable(const char* globalvar)
{
	/*call a globabl function */
	lua_getglobal(m_L, globalvar);
	if (lua_isfunction(m_L, -1))
	{
		lua_pcall(m_L, 0, 0, 0);
	}
}
//helper function to report errors in evaluated lua scripts
void LuaEngine::report_errors(int& state)
{
	if (state != 0)
	{
		std::cerr << "ERR: " << lua_tostring(m_L, state) << std::endl;
		lua_pop(m_L, -1); //remove error
	}
}
