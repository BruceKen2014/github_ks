/*
* 本节课的目标
* 1.创建lua_State
* 2.加载一个lua文件并调用
* 3.C++获取lua里面创建的全局变量
* 
* 本节课学习的lua api
* luaL_newstate 创建一个lua state
* luaL_openlibs 记载常用的lua 库
* luaL_loadfile 加载一个lua文件
* lua_pcall 执行一个lua函数
* lua_getglobal 获取一个全局变量，并将值入栈
* lua_isstring 判断某个栈元素是否是string
* lua_tostring 获取栈元素，作为字符串返回
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


int main()
{
	//创建State
	LuaState = luaL_newstate();
	//加载基本库
	luaL_openlibs(LuaState);

	//加载文件


	int result = luaL_loadfile(LuaState, "lesson1.lua");
	if (result != LUA_OK)
		return 1;

	result = lua_pcall(LuaState, 0, 0, 0);
	if (result != LUA_OK)
		return 1;

	//传递一个全局变量的名字，将这个全局变量的值入栈
	lua_getglobal(LuaState, "name");

	//这里是获取的字符串变量，其他类型同理处置
	if (lua_isstring(LuaState, 1) == 1)
		cout << "value is " << lua_tostring(LuaState, 1) << endl;
	else
		cout << "not find variable" << endl;

	lua_close(LuaState);
	return 0;
}
