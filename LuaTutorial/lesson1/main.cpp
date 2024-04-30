/*
* 本节课的目标
* 1.创建lua_State
* 2.记载一个lua文件并调用
* 3.C++调用lua定义的函数
* 4.lua调用C++定义的函数
*/

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
// 初始化Lua环境.  
lua_State* initLuaEnv()
{
	lua_State* luaEnv = luaL_newstate();
	luaL_openlibs(luaEnv);

	return luaEnv;
}

// 加载Lua文件到Lua运行时环境中
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

//C++调用lua定义的void函数
void CallLuaFunction(const string& lua_function_name)
{
	lua_getglobal(LuaState, lua_function_name.c_str());
	lua_pushstring(LuaState, "Jim");
	lua_pushstring(LuaState, "Green");
	// 调用Lua函数（3个参数,一个返回值）.  
	lua_pcall(LuaState, 2, 1, 0);

	if (lua_isstring(LuaState, -1))
	{
		const char* FullName = lua_tostring(LuaState, -1);
		cout << "full name is " << FullName << endl;
	}
}

//C++函数注册给lua调用
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