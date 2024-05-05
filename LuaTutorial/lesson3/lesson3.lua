

print("hello lua")
function LuaFunction(firstName, FamilyName)
    print("this is lua function")
	--lua 调用C++函数
    local total = add(100,230)
    print("total is " .. total)
    return firstName .. " " .. FamilyName
end

Student = {}
Student.name = "KK"
Student.age = 13