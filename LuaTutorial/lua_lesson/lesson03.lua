
--[[
lesson03
测试lua table深拷贝
Q：如何进行lua table的深拷贝？
A：遍历table，逐变量拷贝，递归操作
--]]

local function DeepCopy(src)
	if src == nil or type(src) ~= "table" then
		return src
	end
	local ret = setmetatable({}, getmetatable(src))
	for k,v in pairs(src) do
		ret[k] = DeepCopy(v)
	end
	return ret
end

--测试 table
local FVector = 
{ 
x = 0, 
y = 0, 
z = 0
}
FVector.__index = FVector
setmetatable(FVector, FVector)

function FVector.__tostring(self)
	local ret = "x: " .. self.x .. " y: " .. self.y .. " z: " .. self.z
	return ret
end

function FVector.__call(self,...)
	return setmetatable({}, self)
end


local vector1 = FVector()
vector1.x = 10

print("deep copy")
local vector2 = DeepCopy(vector1)
vector2.x = 20
print("vector1 " .. tostring(vector1))
print("vector2 " .. tostring(vector2))

print("shallow copy")
local vector3 = vector1
vector3.x = 30
print("vector1 " .. tostring(vector1))
print("vector2 " .. tostring(vector2))
print("vector3 " .. tostring(vector3))



