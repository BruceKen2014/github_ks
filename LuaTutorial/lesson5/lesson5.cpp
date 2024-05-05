/*
* 本节课的目标
* 1.遍历lua 全局table
*
* 本节课学习的lua api
* lua_pushnil 向栈压入一个nil
* lua_gettop 获取栈顶元素索引，可以当做是栈内元素数量，如果栈为空，则返回0
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
void PrintG()
{
    lua_getglobal(LuaState, "_G");

    if (lua_istable(LuaState, -1))
    {
        /* first key */
        lua_pushnil(LuaState);

        /* table is in the stack at index 'tableIndex' */
        while (lua_next(LuaState, -2) != 0)
        {
            // 现在栈顶是值，-1是键
            if (lua_isstring(LuaState, -2))
            {
                std::cout << lua_tostring(LuaState, -2) << " = ";
                switch (lua_type(LuaState, -1))
                {
                case LUA_TSTRING: std::cout << lua_tostring(LuaState, -1); break;
                case LUA_TBOOLEAN: std::cout << (lua_toboolean(LuaState, -1) ? "true" : "false"); break;
                case LUA_TNUMBER: std::cout << lua_tonumber(LuaState, -1); break;
                default: std::cout << "(" << lua_typename(LuaState, lua_type(LuaState, -1)) << ")"; break;
                }
            }
            std::cout << std::endl;
            lua_pop(LuaState, 1);  // 弹出值，保留键继续下一次迭代
        }
    }
    lua_pop(LuaState, 1);
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

    PrintG();
    lua_close(LuaState);
    return 0;
}