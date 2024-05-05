
/*
* 本节课的目标
* 1.C++创建一个lua table，并设置metatable，添加new接口创建table实例
* 2.C++创建全局函数NewVector，创建table 实例
*
* 本节课学习的lua api
* lua_setmetatable 从栈内弹出一个table，并将它设置为另一个table的metatable
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

static int Vector_New(lua_State* L)
{
    lua_newtable(L);
    lua_getglobal(L, "Vector");
    lua_setmetatable(L, -2);
    return 1;
}

static int PrintVector(lua_State* L)
{
    lua_getfield(L, -1, "X");
    int X = static_cast<int>(lua_tonumber(L, -1));
    lua_pop(L, 1);

    lua_getfield(L, -1, "Y");
    int Y = static_cast<int>(lua_tonumber(L, -1));
    lua_pop(L, 1);

    lua_getfield(L, -1, "Z");
    int Z = static_cast<int>(lua_tonumber(L, -1));
    lua_pop(L, 1);

    cout << " vector info is " << X << " " << Y << " " << Z << endl;
    return 0;
}


void CreateVector()
{
    lua_newtable(LuaState);

    lua_pushnumber(LuaState, 100);
    lua_setfield(LuaState, -2, "X");

    lua_pushnumber(LuaState, 200);
    lua_setfield(LuaState, -2, "Y");

    lua_pushnumber(LuaState, 300);
    lua_setfield(LuaState, -2, "Z");

    lua_pushcfunction(LuaState, PrintVector);
    lua_setfield(LuaState, -2, "Log");

    lua_pushcfunction(LuaState, Vector_New);
    lua_setfield(LuaState, -2, "new");

    //此时栈里面只有一个元素，就是我们新创建的table
    //lua_setglobal执行的操作是从栈中弹出一个元素，并将它设置为name的value
    // 相当于_G[name] = table
    lua_setglobal(LuaState, "Vector");

    //将刚才创建的table入栈
    lua_getglobal(LuaState, "Vector");
    //入栈一个相同的table
    lua_getglobal(LuaState, "Vector");
    //设置table的__index为自身
    lua_setfield(LuaState, -2, "__index");

    lua_pop(LuaState, 1);
}

int main()
{
    //创建State
    LuaState = luaL_newstate();
    //加载基本库
    luaL_openlibs(LuaState);

    CreateVector();

    lua_register(LuaState, "NewVector", Vector_New);


    //加载文件
    int result = luaL_dofile(LuaState, "lesson1.lua");
    if (result != LUA_OK)
        return 1;

    lua_close(LuaState);
    return 0;
}