#include "InputManager.h"

InputManager::InputManager(MouseClass* mouse, KeyboardClass* keyboard, int width, int height)
{
	this->mouse = mouse;
	this->keyboard = keyboard;
}

InputManager::~InputManager()
{
}

int InputManager::GetMouseEvent(lua_State* L_state)
{
	InputHandler* ih = (InputHandler*)lua_touserdata(L_state, -1);
	EventLua &luaEvent= ih->GetEvent();
	if (ih->mouse->EventBufferIsEmpty() == true)
	{
		
		lua_pushstring(L_state, luaEvent.mouse.c_str());
	}
	else
	{
		MouseEvent e  = ih->mouse->ReadEvent();
		luaEvent.mouse = e.GetType();
		lua_pushstring(L_state, luaEvent.mouse.c_str());
	}
	return 1;
}

int InputManager::GetKeyEvent(lua_State* L_state)
{	
	InputHandler* ih = (InputHandler*)lua_touserdata(L_state, -1);
	EventLua &luaEvent = ih->GetEvent();
	if (ih->keyboard->CharBufferIsEmpty() == true)
	{
		lua_pushstring(L_state, luaEvent.key.c_str());
	}
	else
	{
		luaEvent.key = ih->GetKeyCode();
		lua_pushstring(L_state, luaEvent.key.c_str());
	}

	return 1;
}

//int InputManager::GetMeshObject(lua_State* L)
//{
//	MeshOb* sprite = (MeshOb*)lua_touserdata(L, -1);
//
//	return 0;
//}

int InputManager::CreateInputHandler(lua_State* L)
{
	InputManager* m = (InputManager*)lua_touserdata(L, lua_upvalueindex(1));
	assert(m);
	
	void* pointerToASprite = lua_newuserdata(L, sizeof(InputHandler));
	new (pointerToASprite) InputHandler();
	luaL_getmetatable(L, "InputMetaTable");
	assert(lua_istable(L, -1));
	lua_setmetatable(L, -2);

	lua_newtable(L);
	lua_setuservalue(L, 1);

	m->AddInputManager((InputHandler*)pointerToASprite);
	return 1;
}

int InputManager::DestroyInputHandler(lua_State* L)
{
	InputManager* im = (InputManager*)lua_touserdata(L, lua_upvalueindex(1));
	assert(im);

	InputHandler* handler = (InputHandler*)lua_touserdata(L, -1);
	//im->RemoveHandler(handler);
	handler->~InputHandler();
	return 0;
}

int InputManager::InputIndex(lua_State* L)
{
	assert(lua_isuserdata(L, -2));	//1
	assert(lua_isstring(L, -1));	//2

	InputHandler* input = (InputHandler*)lua_touserdata(L, -2);
	const char* index = lua_tostring(L, -1);
	
	lua_getuservalue(L, 1);
	lua_pushvalue(L, 2);
	lua_gettable(L, -2);
	if (lua_isnil(L, -1))
	{
		lua_getglobal(L, "InputManager"); //
		lua_pushstring(L, index);
		lua_rawget(L, -2);
	}
	return 1;

}

int InputManager::InputNewIndex(lua_State* L)
{

	assert(lua_isuserdata(L, -3));  //1
	assert(lua_isstring(L, -2));	//2
									// -1 - value we want to set	//3

	InputHandler* input = (InputHandler*)lua_touserdata(L, -3);
	const char* index = lua_tostring(L, -2);

	lua_getuservalue(L, 1);	//1
	lua_pushvalue(L, 2);	//2
	lua_pushvalue(L, 3);	//3
	lua_settable(L, -3);	//1[2] = 3


	return 0;
	
}

void InputManager::AddInputManager(InputHandler* input)
{
	this->inputhandler = input;
}

void InputManager::setValues()
{
	this->inputhandler->setValues(this->mouse,this->keyboard);

}

void InputManager::AddScript(lua_State* L)
{
	lua_newtable(L);
	int spriteTableIdx = lua_gettop(L);
	lua_pushvalue(L, spriteTableIdx);
	lua_setglobal(L, "InputManager");

	constexpr int NUMBER_OF_UPVALUES = 1;
	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, CreateInputHandler, NUMBER_OF_UPVALUES);
	lua_setfield(L, -2, "new");

	lua_pushcfunction(L, GetKeyEvent);
	lua_setfield(L, -2, "GetKeyEvent");
	lua_pushcfunction(L, GetMouseEvent);
	lua_setfield(L, -2, "GetMouseEvent");
	//lua_pushcfunction(L, GetMeshObject);
	//lua_setfield(L, -2, "GetMesh");

	luaL_newmetatable(L, "InputMetaTable"); //
	lua_pushstring(L, "__gc");
	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, DestroyInputHandler, NUMBER_OF_UPVALUES);
	lua_settable(L, -3);

	lua_pushstring(L, "__index");
	lua_pushcfunction(L, InputIndex);
	lua_settable(L, -3);

	lua_pushstring(L, "__newindex");
	lua_pushcfunction(L, InputNewIndex);
	lua_settable(L, -3);
}
