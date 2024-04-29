

print("hello lua")
function LuaFunction(firstName, FamilyName)
    print("this is lua function")
    local total = add(100,230)
    print("total is " .. total)
    return firstName .. " " .. FamilyName
end