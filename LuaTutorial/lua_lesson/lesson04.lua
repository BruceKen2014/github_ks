
--[[
lesson04 函数可变参数的处理

--]]

--方法1，使用select函数
local function PrintArgs(...)
	local number = select('#', ...)
	print("argus num:" .. number)
	local str = ""
	for i = 1, number do
		str = str .. " " .. select(i, ...) --这里会取N个元素，但只有第一个被保留赋值给了args
	end
	print("args is: " .. str)
end
--方法2，使用一个中间table
local function PrintArgs2(...)
	local array = {...}
	print("argus num:" .. #array)
	local str = ""
	for k,v in ipairs(array) do
		str = str .. " " .. tostring(v)
	end
	print("args is: " .. str)
end

PrintArgs(2,1,"43", "ds")
PrintArgs2(2,1,"43", "ds")


