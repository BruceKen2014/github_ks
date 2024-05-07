
/*
* 本节课的目标
* 1.C++创建Struct，创建lua table，table绑定C++实例
* 2.重写__index元方法，直接获取Struct成员
* 3.重写__newindex元方法，直接设置Struct成员
* 
* 本节课学习的lua api
* lua_pop 从栈内弹出n个元素
* luaL_dostring 执行一段lua代码
* 元方法必须写在metatable中
*/

#include <iostream>
#include <string>
#include <sstream>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}


#pragma comment(lib, "lua_lib")
using namespace std;

lua_State* LuaState = nullptr;

struct FVector
{
    float X = 10.0f;
    float Y = 20.0f;
    float Z = 30.0f;
};

static int Vector_ToString(lua_State* L)
{
    lua_getfield(L, -1, "cpp_ptr");
    FVector* vector = reinterpret_cast<FVector*>(lua_touserdata(L, -1));
    lua_pop(L, 1);

    stringstream format;
    format << "X:" << vector->X << " Y:" << vector->Y << " Z:" << vector->Z;
    lua_pushstring(L, format.str().c_str());
    return 1;
}

static int Vector_GC(lua_State* L)
{
    lua_getfield(L, -1, "cpp_ptr");
    FVector* vector = reinterpret_cast<FVector*>(lua_touserdata(L, -1));

    lua_pop(L, 1);
    delete vector;
    cout << "gc table" << endl;
    return 0;
}

static int Vector_New(lua_State* L)
{
    lua_newtable(L);

    FVector* vector = new FVector();
    lua_pushlightuserdata(L, vector);
    lua_setfield(L, -2, "cpp_ptr");

    lua_getglobal(L, "Vector");
    lua_setmetatable(L, -2);

    return 1;
}

static int Vector_Index(lua_State* L)
{
    lua_getfield(L, -2, "cpp_ptr");
    FVector* vector = reinterpret_cast<FVector*>(lua_touserdata(L, -1));
    lua_pop(L, 1);

    const char* fieldName = lua_tostring(L, -1);
    if (strcmp(fieldName, "X") == 0)
        lua_pushnumber(L, vector->X);
    else if (strcmp(fieldName, "Y") == 0)
        lua_pushnumber(L, vector->Y);
    else if (strcmp(fieldName, "Z") == 0)
        lua_pushnumber(L, vector->Z);
    else
    {
        cout << "get unknown filed " << fieldName << endl;
        lua_pushnil(L);
    }
    return 1;
}

static int Vector_NewIndex(lua_State* L)
{
    lua_getfield(L, -3, "cpp_ptr");
    FVector* vector = reinterpret_cast<FVector*>(lua_touserdata(L, -1));
    lua_pop(L, 1);

    const char* fieldName = lua_tostring(L, -2);
    float value = lua_tonumber(L, -1);
    if (strcmp(fieldName, "X") == 0)
        vector->X = value;
    else if (strcmp(fieldName, "Y") == 0)
        vector->Y = value;
    else if (strcmp(fieldName, "Z") == 0)
        vector->Z = value;
    else
    {
        cout << "set unknown filed " << fieldName << endl;
    }
    return 0;
}

void CreateVectorTable()
{
    lua_State* L = LuaState;
    lua_newtable(L);

    lua_pushcfunction(L, Vector_ToString);
    lua_setfield(L, -2, "__tostring");

    lua_pushcfunction(L, Vector_New);
    lua_setfield(L, -2, "__call");

    lua_pushcfunction(L, Vector_GC);
    lua_setfield(L, -2, "__gc");
    
    lua_pushcfunction(L, Vector_Index);
    lua_setfield(L, -2, "__index");

    lua_pushcfunction(L, Vector_NewIndex);
    lua_setfield(L, -2, "__newindex");

    lua_setglobal(L, "Vector");

    //将刚才创建的table入栈
    lua_getglobal(L, "Vector");

    //入栈一个相同的table
    lua_getglobal(L, "Vector");
    //设置table的metatable为自身
    lua_setmetatable(L, -2);

    lua_pop(L, 1);
}


int main()
{
    //创建State
    LuaState = luaL_newstate();
    //加载基本库
    luaL_openlibs(LuaState);

    CreateVectorTable();
    luaL_dostring(LuaState, 
        "local vector = Vector()\n"
        "print(tostring(vector))\n"
        "print(vector.X)\n"
        "print(vector.Y)\n"
        "print(vector.Z)\n"
        "print(vector.W)\n"
        "vector.X = 1\n"
        "vector.Y = 2\n"
        "vector.Z = 3\n"
        "vector.W = 4\n"
        "print(tostring(vector))"
    );

    lua_close(LuaState);
    return 0;
}