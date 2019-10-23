#ifndef MESHMANGER_H
#define MESHMANGER_H
#include "Mesh.h"
#include <lua.hpp>
#include "../Timer.h"
class MeshManger
{
private:
	std::vector<MeshOb*> m_meshes;
	std::vector<XMFLOAT3> waypoints;

	ID3D11Device* Device;
	ID3D11DeviceContext* DeviceContext;
	int numberOfSpritesExisting = 0;
	int numberOfSpritesMade = 0;
	Timer* theTimer;

public:	
	static MeshManger* meshmanger;
	int GetNumberOfMeshses();
	void Init(ID3D11Device* d, ID3D11DeviceContext *dc, Timer* timer)
	{
		this->Device = d; this->DeviceContext = dc; this->theTimer = timer;
		this->meshmanger = this;
	}
	~MeshManger()
	{
	}
	void AddSprite(MeshOb * sprite)
	{
		sprite->Initialize(Device, DeviceContext);
		numberOfSpritesExisting++;
		numberOfSpritesMade++;
		m_meshes.push_back(sprite);
	}
	void RemoveSprite(MeshOb * sprite)
	{
		int i = 0;
		for (auto& s : m_meshes)
		{
			if (s == sprite)
			{
				numberOfSpritesExisting--;
				m_meshes.erase(m_meshes.begin() + i);
				return;
			}
			i++;
		}
	}
	static int CreateMesh(lua_State* L)
	{
		MeshManger* sm = (MeshManger*)lua_touserdata(L, lua_upvalueindex(1));
		assert(sm);

		void* pointerToASprite = lua_newuserdata(L, sizeof(MeshOb));
		new (pointerToASprite) MeshOb();
		luaL_getmetatable(L, "MeshMetaTable");
		assert(lua_istable(L, -1));
		lua_setmetatable(L, -2);

		lua_newtable(L);
		lua_setuservalue(L, 1);

		sm->AddSprite((MeshOb*)pointerToASprite);
		return 1;
		/*
		MeshManger* sm = (MeshManger*)lua_touserdata(L, lua_upvalueindex(1));
		assert(sm);
		std::string type;
		if (lua_isstring(L, -1))
		{
			type = lua_tostring(L, -1);
		}
		else if(lua_isstring(L, -2))
			std::string type = lua_tostring(L, -2);
		if (type == "Tower")
		{
			void* pointerToASprite = lua_newuserdata(L, sizeof(Tower));
			new (pointerToASprite) Tower();
			luaL_getmetatable(L, "MeshMetaTable");
			assert(lua_istable(L, -1));
			lua_setmetatable(L, -2);
			lua_newtable(L);
			lua_setuservalue(L, 1);
			sm->AddSprite((Tower*)pointerToASprite);
		}
		else if (type == "Enemy")
		{
			void* pointerToASprite = lua_newuserdata(L, sizeof(Enemy));
			new (pointerToASprite) Enemy();
			luaL_getmetatable(L, "MeshMetaTable");
			assert(lua_istable(L, -1));
			lua_setmetatable(L, -2);
			lua_newtable(L);
			lua_setuservalue(L, 1);
			sm->AddSprite((Enemy*)pointerToASprite);
		}
		else
		{
			void* pointerToASprite = lua_newuserdata(L, sizeof(MeshOb));
			new (pointerToASprite) MeshOb();
			luaL_getmetatable(L, "MeshMetaTable");
			assert(lua_istable(L, -1));
			lua_setmetatable(L, -2);
			lua_newtable(L);
			lua_setuservalue(L, 1);
			sm->AddSprite((MeshOb*)pointerToASprite);
		}*/
	}
	static int DestroyMesh(lua_State* L)
	{
		MeshManger* sm = (MeshManger*)lua_touserdata(L, lua_upvalueindex(1));
		assert(sm);
		MeshOb* sprite = (MeshOb*)lua_touserdata(L, -1);
		sm->RemoveSprite(sprite);
		sprite->~MeshOb();
		return 0;
	};
	//enemy
	static int CreateEnemy(lua_State* L)
	{
		MeshOb* sprite = (MeshOb*)lua_touserdata(L, -1);
		MeshManger* sm = MeshManger::meshmanger;
		if (sprite->GetType() == "Enemy")
		{
			sprite->InitEnemy(); //fix vector error
		}
		return 0;
	}
	static int setWaypoint(lua_State* L)
	{
		MeshOb* sprite = (MeshOb*)lua_touserdata(L, -1);
		MeshManger* sm = MeshManger::meshmanger;

		if (sprite->GetType() == "Enemy")
		{
			sprite->initWaypoints(sm->waypoints); //fix vector error
		}
		return 0;
	}
	static int updateEnemy(lua_State* L)
	{
		MeshManger* sm = MeshManger::meshmanger;
		float dt = sm->theTimer->GetMilisecondsElapsed();
		MeshOb* sprite = (MeshOb*)lua_touserdata(L, -1);
		bool wpIsEmpty = false;
		if (sprite->GetType() == "Enemy")
		{
			wpIsEmpty = sprite->Update(dt);

		}
		lua_pushboolean(L, wpIsEmpty);
		return 1;
	}
	static int moveWPtoVector(lua_State* L)
	{
		MeshManger* sm = MeshManger::meshmanger;
		MeshOb* sprite = (MeshOb*)lua_touserdata(L, -1);
		if (sprite->GetType() == "Waypoint")
		{
			sm->waypoints.push_back(sprite->GetPositionFloat3());
		}
		return 0;
	}
	//tower
	static int CreateTower(lua_State* L)
	{
		MeshOb* tower = (MeshOb*)lua_touserdata(L, -1);
		MeshManger* sm = MeshManger::meshmanger;
		if (tower->GetType() == "Tower")
		{
			tower->InitTower(); //fix vector error
		}
		return 0;
	}
	static int InRangeTower(lua_State* L)
	{
		MeshManger* sm = MeshManger::meshmanger;
		MeshOb* tower = (MeshOb*)lua_touserdata(L, -2);
		MeshOb* enemy = (MeshOb*)lua_touserdata(L, -1);
		bool isInRange = false;
		if (tower->GetType() == "Tower" && enemy->GetType() == "Enemy")
		{
			isInRange = tower->InRange(enemy);
		}
		lua_pushboolean(L, isInRange);
		return 1;
	}
	static int Shoot(lua_State* L)
	{
		MeshManger* sm = MeshManger::meshmanger;
		MeshOb* tower = (MeshOb*)lua_touserdata(L, -2);
		MeshOb* enemy = (MeshOb*)lua_touserdata(L, -1);
		if (tower->GetType() == "Tower" && enemy->GetType() == "Enemy")
		{
			tower->Shoot(enemy);
		}
		return 0;
	}
	static int GetType(lua_State* L)
	{
		MeshOb* sprite = (MeshOb*)lua_touserdata(L, -1);
		std::string type = sprite->GetType();
		lua_pushstring(L, type.c_str());
		return 1;
	}
	static int SetType(lua_State* L)
	{
		MeshOb* sprite = (MeshOb*)lua_touserdata(L, -2);
		const char*  type = lua_tostring(L, -1);
		sprite->SetType(type);
		return 0;
	}
	static int GetPosition(lua_State* L)
	{
		MeshOb* sprite = (MeshOb*)lua_touserdata(L, -1);
		float x = sprite->GetPositionFloat3().x;
		float y = sprite->GetPositionFloat3().y;
		lua_pushnumber(L, x);
		lua_pushnumber(L, y);
		return 2;
	}
	static int SetPosition(lua_State* L)
	{
		MeshOb* sprite = (MeshOb*)lua_touserdata(L, -3);
		float velX = (float)lua_tonumber(L, -2);
		float velY = (float)lua_tonumber(L, -1);
		sprite->SetPosition(velX, velY,0.0f);
		return 0;
	};
	static int DrawSprite(lua_State* L)
	{
		MeshOb* sprite = (MeshOb*)lua_touserdata(L, -1);
		sprite->Draw();
		return 0;
	};
	static int MeshIndex(lua_State* L)
	{
		assert(lua_isuserdata(L, -2));	//1
		assert(lua_isstring(L, -1));	//2

		MeshOb* sprite = (MeshOb*)lua_touserdata(L, -2);
		const char* index = lua_tostring(L, -1);
		if (strcmp(index, "x") == 0)
		{
			lua_pushnumber(L, sprite->GetPositionFloat3().x);
			return 1;
		}
		else if (strcmp(index, "y") == 0)
		{
			lua_pushnumber(L, sprite->GetPositionFloat3().y);
			return 1;
		}
		else
		{
			lua_getuservalue(L, 1);
			lua_pushvalue(L, 2);
			lua_gettable(L, -2);
			if (lua_isnil(L, -1))
			{
				lua_getglobal(L, "Sprite");
				lua_pushstring(L, index);
				lua_rawget(L, -2);
			}
			return 1;
		}
	};
	static int MeshNewIndex(lua_State* L)
	{
		assert(lua_isuserdata(L, -3));  //1
		assert(lua_isstring(L, -2));	//2
										// -1 - value we want to set	//3

		MeshOb* sprite = (MeshOb*)lua_touserdata(L, -3);
		const char* index = lua_tostring(L, -2);
		if (strcmp(index, "x") == 0)
		{
			float x = (float)lua_tonumber(L, -1);
			
		}
		else if (strcmp(index, "y") == 0)
		{
			float x = (float)lua_tonumber(L, -1);
		}
		else
		{
			lua_getuservalue(L, 1);	//1
			lua_pushvalue(L, 2);	//2
			lua_pushvalue(L, 3);	//3
			lua_settable(L, -3);	//1[2] = 3
		}

		return 0;
	};
	void AddScript(lua_State* L)
	{
		lua_newtable(L);
		int spriteTableIdx = lua_gettop(L);
		lua_pushvalue(L, spriteTableIdx);
		lua_setglobal(L, "Sprite");

		constexpr int NUMBER_OF_UPVALUES = 1;
		lua_pushlightuserdata(L, this);
		lua_pushcclosure(L, CreateMesh, NUMBER_OF_UPVALUES);
		lua_setfield(L, -2, "new");
		lua_pushcfunction(L, SetPosition);
		lua_setfield(L, -2, "SetPosition");
		lua_pushcfunction(L, DrawSprite);
		lua_setfield(L, -2, "Draw");
		lua_pushcfunction(L, SetType);
		lua_setfield(L, -2, "SetType");
		lua_pushcfunction(L, GetType);
		lua_setfield(L, -2, "GetType");
		lua_pushcfunction(L, GetPosition);
		lua_setfield(L, -2, "GetPosition");

		lua_pushcfunction(L, setWaypoint);
		lua_setfield(L, -2, "SetWaypoint");
		lua_pushcfunction(L, updateEnemy);
		lua_setfield(L, -2, "updateEnemy");
		lua_pushcfunction(L, moveWPtoVector);
		lua_setfield(L, -2, "moveWPtoVector");

		lua_pushcfunction(L, CreateEnemy);
		lua_setfield(L, -2, "CreateEnemy");

		lua_pushcfunction(L, InRangeTower);
		lua_setfield(L, -2, "InRangeTower");
		lua_pushcfunction(L, Shoot);
		lua_setfield(L, -2, "Shoot");
		lua_pushcfunction(L, CreateTower);
		lua_setfield(L, -2, "CreateTower");

		

		luaL_newmetatable(L, "MeshMetaTable");
		lua_pushstring(L, "__gc");
		lua_pushlightuserdata(L, this);
		lua_pushcclosure(L, DestroyMesh, NUMBER_OF_UPVALUES);
		lua_settable(L, -3);

		lua_pushstring(L, "__index");
		lua_pushcfunction(L, MeshIndex);
		lua_settable(L, -3);

		lua_pushstring(L, "__newindex");
		lua_pushcfunction(L, MeshNewIndex);
		lua_settable(L, -3);
	}
};
#endif // !MESHMANGER_H