/*
* 本节课的目标
* C++创建一个全局的table，并且可被lua使用
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

void CreateTable(const char* TableName)
{
	//创建一个table，并入栈
	lua_newtable(LuaState);

	lua_pushstring(LuaState, "name");
	lua_pushstring(LuaState, "Jim");

	/*
	* lua_settable相当于执行一个table[k] = v
	* table为从从栈索引为1的位置取得一个table，这里就是我们新创建的table
	* v为从栈顶拿取元素当做value，即我们最后push的Jim
	* k为栈顶下面一个元素，即我们倒数第一次push的name
	* lua_settable调用完成后，将栈顶的两个元素出栈
	*/
	lua_settable(LuaState, 1);

	//同理再次添加一个table k v
	lua_pushstring(LuaState, "age");
	lua_pushnumber(LuaState, 20);
	lua_settable(LuaState, 1);

	//此时栈里面只有一个元素，就是我们新创建的table

	//lua_setglobal执行的操作是从栈中弹出一个元素，并将它设置为name的value
	// 相当于_G[name] = table
	lua_setglobal(LuaState, TableName);
}


int main()
{
	LuaState = initLuaEnv();
	CreateTable("Student");
	loadLuaFile("main.lua");

	lua_close(LuaState);
	return 0;
}

/*
*

struct FVector
{
	FVector(float _X, float _Y, float _Z):X(_X), Y(_Y),Z(_Z){}
	float X = 0.0f;
	float Y = 0.0f;
	float Z = 0.0f;
};

// C++函数注册给lua调用
static int NewFVector(lua_State * L)
{
	double X = lua_tonumber(L, 1);
	double Y = lua_tonumber(L, 2);
	double Z = lua_tonumber(L, 2);
	FVector* ret = new FVector(X, Y, Z);
	lua_pushlightuserdata(L, ret);
	return 1;
}

void RegisterCPPFunction()
{
	lua_register(LuaState, "FVector", NewFVector);
}
class ILuaInterface
{
public:
	virtual const char* GetLuaFileName() = 0;
};

class A :public ILuaInterface
{
public:
	virtual const char* GetLuaFileName() override { return "A";};

	virtual void PrintHello();
};

ILuaInterface* CreateA()
{
	A* a = new A();
	const char* LuaFile = a->GetLuaFileName();


}

*/