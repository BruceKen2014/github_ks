
/*
* 本节课的目标
* 1.C++创建一个Class，创建lua table，table绑定C++实例
* 2.重写__gc元方法，table销毁时，delete C++指针
* 3.重写__tostring元方法，打印C++ 实例的信息
* 4.重写__call元方法，模拟C++创建类实例操作
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

class Student
{
public:
    int  GetAge() { return age; }
    void SetAge(int _age) { age = _age; }
    const string& GetName() { return name; }
    void Print()
    {
        cout << "my name is " << name << " age is " << age << endl;
    }
private:
    string name = "John";
    int age = 16;
};

static int Student_GetAge(lua_State* L)
{
    lua_getfield(L, -1, "cpp_ptr");
    Student* StudentPtr = reinterpret_cast<Student*>(lua_touserdata(L, -1));
    lua_pop(L, 1);

    lua_pushnumber(L, StudentPtr->GetAge());
    return 1;
}

static int Student_SetAge(lua_State* L)
{
    lua_getfield(L, -2, "cpp_ptr");
    Student* StudentPtr = reinterpret_cast<Student*>(lua_touserdata(L, -1));
    lua_pop(L, 1);

    int age = static_cast<int>(lua_tonumber(L,-1));
    StudentPtr->SetAge(age);

    return 0;
}

static int Student_GetName(lua_State* L)
{
    lua_getfield(L, -1, "cpp_ptr");
    Student* StudentPtr = reinterpret_cast<Student*>(lua_touserdata(L, -1));

    lua_pop(L, 1);
    lua_pushstring(L, StudentPtr->GetName().c_str());
    return 1;
}

static int Student_Print(lua_State* L)
{
    lua_getfield(L, -1, "cpp_ptr");
    Student* StudentPtr = reinterpret_cast<Student*>(lua_touserdata(L, -1));

    lua_pop(L, 1);
    StudentPtr->Print();
    return 0;
}

static int Student_ToString(lua_State* L)
{
    lua_getfield(L, -1, "cpp_ptr");
    Student* StudentPtr = reinterpret_cast<Student*>(lua_touserdata(L, -1));
    lua_pop(L, 1);

    stringstream format;
    format << "name:" << StudentPtr->GetName() << " age:" << StudentPtr->GetAge();
    lua_pushstring(L, format.str().c_str());
    return 1;
}

static int Student_GC(lua_State* L)
{
    lua_getfield(L, -1, "cpp_ptr");
    Student* StudentPtr = reinterpret_cast<Student*>(lua_touserdata(L, -1));

    lua_pop(L, 1);
    delete StudentPtr;
    cout << "gc table" << endl;
    return 0;
}

static int Student_New(lua_State* L)
{
    lua_newtable(L);

    Student* newStudent = new Student();
    lua_pushlightuserdata(L, newStudent);
    lua_setfield(L, -2, "cpp_ptr");

    lua_getglobal(L, "Student");
    lua_setmetatable(L, -2);

    return 1;
}

void CreateStudentTable()
{
    lua_State* L = LuaState;
    lua_newtable(L);

    lua_pushcfunction(L, Student_GetAge);
    lua_setfield(L, -2, "GetAge");

    lua_pushcfunction(L, Student_SetAge);
    lua_setfield(L, -2, "SetAge");

    lua_pushcfunction(L, Student_GetName);
    lua_setfield(L, -2, "GetName");

    lua_pushcfunction(L, Student_Print);
    lua_setfield(L, -2, "Print");

    lua_pushcfunction(L, Student_ToString);
    lua_setfield(L, -2, "__tostring");

    lua_pushcfunction(L, Student_New);
    lua_setfield(L, -2, "__call");

    lua_pushcfunction(L, Student_GC);
    lua_setfield(L, -2, "__gc");

    lua_setglobal(L, "Student");

    //将刚才创建的table入栈
    lua_getglobal(L, "Student");

    //入栈一个相同的table
    lua_getglobal(L, "Student");
    //设置table的__index为自身，这里设置为自身的原因是实例找成员找不到会在metatable找，metatable为自身，又添加了GetAge，所以才能找到GetAge
    lua_setfield(L, -2, "__index");

    //入栈一个相同的table
    lua_getglobal(L, "Student");
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

    CreateStudentTable();
    luaL_dostring(LuaState, 
        "local student = Student()\n"
        "student:Print()\n"
        "student:SetAge(18)\n"
        "print(tostring(student))\n"
    );

    lua_close(LuaState);
    return 0;
}