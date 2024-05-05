
/*
* 本节课的目标
* 1.C++调用lua定义的函数
* 2.lua调用C++定义的函数
*
* 本节课学习的lua api
* lua_register 向lua注册一个C++函数
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


//C++调用lua定义的函数，本质上是获取一个全局的函数变量到栈上，然后将参数入栈，之后执行call，result会被入栈
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

int main()
{
	//创建State
	LuaState = luaL_newstate();
	//加载基本库
	luaL_openlibs(LuaState);

	lua_register(LuaState, "add", add);

	//加载文件
	int result = luaL_dofile(LuaState, "lesson1.lua");
	if (result != LUA_OK)
		return 1;

	CallLuaFunction("LuaFunction");

	lua_close(LuaState);
	return 0;
}