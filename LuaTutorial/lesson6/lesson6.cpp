
/*
* 本节课的目标
* 1.C++创建一个lua table，并为table添加函数成员，lua代码调用函数，打印这个table的相关信息
*
* 本节课学习的lua api
* lua_setfield 向栈内的1个table添加k v
* lua_getfield 先栈内的1个table索取一个k，把v入栈
* lua_pushcfunction 向栈内压入一个C Function
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

int PrintVector(lua_State* L)
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

void CreateTable(const char* TableName)
{
    //创建一个table，并入栈
    lua_newtable(LuaState);

    lua_pushnumber(LuaState, 100);
    /*
    * lua_setfield相当于执行一个table[k] = v
    * table为从从栈索引为1的位置取得一个table，这里就是我们新创建的table]
    * k为传递的参数v
    * v为从栈顶拿取元素当做value，即我们最后push的100
    * lua_setfield调用完成后，将栈顶的元素出栈
    */
    lua_setfield(LuaState, 1, "X");

    lua_pushnumber(LuaState, 200);
    lua_setfield(LuaState, 1, "Y");

    lua_pushnumber(LuaState, 300);
    lua_setfield(LuaState, 1, "Z");

    lua_pushcfunction(LuaState, PrintVector);
    lua_setfield(LuaState, 1, "Log");

    //此时栈里面只有一个元素，就是我们新创建的table
    //lua_setglobal执行的操作是从栈中弹出一个元素，并将它设置为name的value
    // 相当于_G[name] = table
    lua_setglobal(LuaState, TableName);
}

int main()
{
    //创建State
    LuaState = luaL_newstate();
    //加载基本库
    luaL_openlibs(LuaState);

    CreateTable("Vector");

    //加载文件
    int result = luaL_dofile(LuaState, "lesson1.lua");
    if (result != LUA_OK)
        return 1;

    lua_close(LuaState);
    return 0;
}
/*
* extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

class MyClass {
public:
    int value;
    MyClass(int v) : value(v) {}
    void setValue(int v) { value = v; }
    int getValue() const { return value; }
};

// 将C++对象指针保存到Lua表中
int registerObjectInLua(lua_State* L, MyClass* obj) {
    // 创建一个表来作为Lua中的对象
    lua_newtable(L);

    // 将C++对象的指针作为userdata存入Lua表
    lua_pushlightuserdata(L, obj);
    lua_setfield(L, -2, "cpp_ptr");

    // 将设置值和获取值的函数引入到Lua表
    lua_pushcfunction(L, setValue);
    lua_setfield(L, -2, "setValue");
    lua_pushcfunction(L, getValue);
    lua_setfield(L, -2, "getValue");

    return 1; // 返回Lua栈顶的元素数量
}

// 从Lua获取对象指针
MyClass* getObjectFromLua(lua_State* L) {
    // 假设Lua栈顶是对象的userdata
    lua_getfield(L, -1, "cpp_ptr");
    MyClass* obj = static_cast<MyClass*>(lua_touserdata(L, -1));
    lua_pop(L, 1); // 弹出userdata
    return obj;
}

// 对应于Lua中的setValue函数
int setValue(lua_State* L) {
    MyClass* obj = getObjectFromLua(L);
    if (obj) {
        int value = luaL_checkinteger(L, 2);
        obj->setValue(value);
    }
    return 0;
}

// 对应于Lua中的getValue函数
int getValue(lua_State* L) {
    MyClass* obj = getObjectFromLua(L);
    if (obj) {
        lua_pushinteger(L, obj->getValue());
        return 1; // 返回获取到的值
    }
    return 0;
}

int main() {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    MyClass myObj(10);
    registerObjectInLua(L, &myObj);

    // 将Lua代码加载到Lua虚拟机中
    const char* code = "obj = ...\nprint(obj:getValue())\nobj:setValue(20)\nprint(obj:getValue())\n";
    if (luaL_dostring(L, code)) {
        lua_error(L);
    }

    lua_close(L);
    return 0;
}
*/