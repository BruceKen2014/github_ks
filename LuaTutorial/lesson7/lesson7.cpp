

/*
* 本节课的目标
* 1.C++创建一个lua table，为这个table绑定一个C++ Struct，在lua里面使用这个Struct
*
* 本节课学习的lua api
* lua_pushlightuserdata 向栈内压入一个C指针
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

struct Vector
{
    float X = 10.0f;
    float Y = 0.0f;
    float Z = 0.0f;
};

static int Vector_GetX(lua_State* L)
{
    lua_getfield(L, -1, "cpp_ptr");
    Vector* vector = reinterpret_cast<Vector*>(lua_touserdata(L, -1));

    lua_pop(L, 1);
    lua_pushnumber(L, vector->X);
    return 1;
}
void CreateVector()
{
    lua_newtable(LuaState);

    Vector* vector = new Vector;

    // 将C++对象的指针作为userdata存入Lua表
    lua_pushlightuserdata(LuaState, vector);
    lua_setfield(LuaState, -2, "cpp_ptr");

    lua_pushcfunction(LuaState, Vector_GetX);
    lua_setfield(LuaState, -2, "GetX");

    //此时栈里面只有一个元素，就是我们新创建的table
    //lua_setglobal执行的操作是从栈中弹出一个元素，并将它设置为name的value
    // 相当于_G[name] = table
    lua_setglobal(LuaState, "Vector");
}

int main()
{
    //创建State
    LuaState = luaL_newstate();
    //加载基本库
    luaL_openlibs(LuaState);

    CreateVector();

    //加载文件
    int result = luaL_dofile(LuaState, "lesson1.lua");
    if (result != LUA_OK)
        return 1;

    lua_close(LuaState);
    return 0;
}