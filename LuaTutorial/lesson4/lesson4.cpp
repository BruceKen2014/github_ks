
/*
* 本节课的目标
* 1.遍历一个lua table
*
* 本节课学习的lua api
* lua_pushnil 向栈压入一个nil
* lua_next 从栈内弹出一个元素当做key，然后将table的key的后一个pair以key value的入栈
* lua_type 获取栈内某个index的lua 类型
* lua_typename 获取某个lua类型的const char*
* lua_pop 从栈内弹出N个元素
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

//C++遍历lua table
void PrintLuaTable(const char* tableName)
{
	int type = lua_getglobal(LuaState, tableName);
	if (type != LUA_TTABLE)
		return;

	/* first key */
	lua_pushnil(LuaState); 

	/* table is in the stack at index 'tableIndex' */
	while (lua_next(LuaState, -2) != 0)
	{
		/* uses 'key' (at index -2) and 'value' (at index -1) */
		cout << lua_tostring(LuaState, -2) <<" = " << lua_tostring(LuaState, -1) << ", value type is " << lua_typename(LuaState, lua_type(LuaState, -1)) << endl;
		/* removes 'value'; keeps 'key' for next iteration */
		lua_pop(LuaState, 1);
	}
}
int main()
{
	//创建State
	LuaState = luaL_newstate();
	//加载基本库
	luaL_openlibs(LuaState);

	//加载文件
	int result = luaL_dofile(LuaState, "lesson1.lua");
	if (result != LUA_OK)
		return 1;

	PrintLuaTable("Student");
	lua_close(LuaState);
	return 0;
}