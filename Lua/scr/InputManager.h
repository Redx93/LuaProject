#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include "lua.hpp"
#include "InputHandler.h"

class InputManager
{
private:
	InputHandler* inputhandler;
	MouseClass* mouse;
	KeyboardClass* keyboard;
	Camera* camera;
	EventLua luaEvent;
	int width;
	int height;
public:

	InputManager(MouseClass* mouse, KeyboardClass* keyboard, Camera * camera,
		int width, int height);
	~InputManager();
	static int GetMouseEvent(lua_State *L_state);
	static int GetKeyEvent(lua_State* L_state);
	static int MeshFollow(lua_State* L);
	static int GetMousePosition(lua_State* L);
	static int CollideWith(lua_State* L);

	static int CreateInputHandler(lua_State* L);
	static int DestroyInputHandler(lua_State* L);
	static int InputIndex(lua_State* L);
	static int InputNewIndex(lua_State* L);
	void AddInputManager(InputHandler* input);
	void setValues(); //Camera* camera,
	void AddScript(lua_State* L);
};
#endif // !INPUTMANAGER_H
