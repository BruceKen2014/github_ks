



#include <iostream>
extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}


#pragma comment(lib, "lua_lib")
using namespace std;

lua_State* LuaState = nullptr;
// ��ʼ��Lua����.  
lua_State* initLuaEnv()
{
	lua_State* luaEnv = luaL_newstate();
	luaL_openlibs(luaEnv);

	return luaEnv;
}

// ����Lua�ļ���Lua����ʱ������
bool loadLuaFile(const string& fileName)
{
	int result = luaL_loadfile(LuaState, fileName.c_str());
	if (result)
	{
		return false;
	}

	result = lua_pcall(LuaState, 0, 0, 0);
	return result == 0;
}

//C++����lua�����void����
void CallLuaFunction(const string& lua_function_name)
{
	lua_getglobal(LuaState, lua_function_name.c_str());
	lua_pushstring(LuaState, "Jim");
	lua_pushstring(LuaState, "Green");
	// ����Lua������3������,һ������ֵ��.  
	lua_pcall(LuaState, 2, 1, 0);

	if (lua_isstring(LuaState, -1))
	{
		const char* FullName = lua_tostring(LuaState, -1);
		cout << "full name is " << FullName << endl;
	}
}

//C++����ע���lua����
static int add(lua_State* L)
{
	double a = lua_tonumber(L, 1);
	double b = lua_tonumber(L, 2);
	double total = a + b;
	lua_pushnumber(L, total);
	return 1;
}

void RegisterCPPFunction()
{
	lua_register(LuaState, "add", add);
}
int main()
{
	LuaState = initLuaEnv();
	RegisterCPPFunction();
	loadLuaFile("main.lua");
	CallLuaFunction("LuaFunction");

	lua_close(LuaState);
	return 0;
}