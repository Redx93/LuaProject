--module (..., package.seeall)--enable to  be seen from other module
SpriteMetaTable = { }
numberOfSprite = 1

function CreateMeshFile(x,y,type)
		sprite1  = Sprite.new()
		sprite1:SetPosition(x,y)
		sprite1:SetType(type)
		if type=="Waypoint" then
			sprite1:moveWPtoVector()
		end
		SpriteMetaTable[numberOfSprite] = sprite1
		numberOfSprite = numberOfSprite + 1

end

function WritetoFile()
--[[ That mode string: `r´ for reading, a `w´ for writing (which also erases any previous content of the file),
	or an `a´ for appending,`b´ to open binary files.]]--
	local file = io.open("test2.txt","w")
	for i=1,numberOfSprite do
		local	type = SpriteMetaTable[i]:GetType();
		local x, y = SpriteMetaTable[i]:GetPosition()
		file:write(x .. "\n"..y .. "\n".. type .. "\n")
	end
	file:close()
end

function ReadFile()
	local file = io.open("test2.txt", "r")
	for k in pairs (SpriteMetaTable) do
		SpriteMetaTable[k] = nil
		numberOfSprite = 1;
	end
	--[[
		%a	letters, 
		%d	digits, 
		%s	space characters, 
		%g  float, 
		%l  letter(lower case)
		https://www.lua.org/pil/20.2.html
	]]--

	--data containers
	local lines ={} --raw data
	local xArr = {}	--x value float
	local yArr = {} --y value float 
	local typeArr = {} -- type string

	--brute force data into lines[] array
	for line in file:lines() do
		lines[#lines+1] = line
	end
	--use lines[] array to separate the data into appropriate data-type
	for i=1,#lines do
		if i%3 == 0 then
			local type = string.match(lines[i], "%a+")
			typeArr[#typeArr+1] = type
			--print("type: "..type)
		elseif i%3 == 1 then
			local x = string.match(lines[i], "%g+.%g+")
			xArr[#xArr+1] = x
			--print("x: "..x)
		elseif i%3 == 2 then
			local y = string.match(lines[i], "%g+.%g+")
			yArr[#yArr+1]=y
			--print("y: "..y)
		end
		--print("Table-- " .. lines[i])
	end	
	--this for-loop is only for printing
	for i=1, #typeArr do
		--print("x: "..xArr[i]..", y: "..yArr[i]..", type: "..typeArr[i])
		CreateMeshFile(xArr[i],yArr[i],typeArr[i])
	end
	file:close()
end

