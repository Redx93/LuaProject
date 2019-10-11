--package.path = '/usr/LibLua.lua;'
require("LibLua")
function update()
	for i=1, numberOfSprite do
			SpriteMetaTable[i]:Draw()
	end
	 --print("number Of sprites : ".. numberOfSprite)
 end

--[[
-- create a namespace
Mesh = {}
-- create the prototype with default values
Mesh.prototype = { MeshType = "non", Index = -1 , Draw = "non" , drawcall = HostDrawFunction(self), }
-- create a metatable
Mesh.mt = {}
-- declare the constructor function
function Mesh.new (object)
    setmetatable(object, Mesh.mt)
    return object
end

Mesh.mt.__index = function (table, key)
    return Mesh.prototype[key]
end

meshArray= { }
-- Send the inputs from Engine to lua
State = { MouseState = "null" , KeyInput = "null", }

local NrOfElement = 4;
function CreateMesh(meshtype, index, draw)
	m = Mesh.new{ MeshType = meshtype, Index = index , Draw = draw, drawcall = HostDrawFunction(self)  }
	table.insert(meshArray, m);
end

function RemoveMesh(n)
	print("Remove Mesh Type: ".. meshArray[n].MeshType .. " At Index "..n)
	HostRemoveMesh(meshArray[n]);
	table.remove(meshArray, n);
end

function update()
	for k,v in pairs(meshArray) do
		local CurrentMesh  = meshArray[k]
		if CurrentMesh.MeshType ~= "temp" then
			c =	HostDrawFunction(CurrentMesh)
		end
	end
	--iterating from back to front -- becomes tricky when trying to remove when you do front to back
	for i =# meshArray, 1, -1 do
		if meshArray[i].MeshType == "Enemy" then
			RemoveMesh(i)
		elseif meshArray[i].MeshType == "Player" then
			RemoveMesh(i)
		end
	end
end


for k,v in pairs(meshArray) do
		if( meshArray[k].MeshType == "Player" )
		then
			RemoveMesh(k)
		elseif( meshArray[k].MeshType == "Enemy" )
		then
			RemoveMesh(k)
		end
	end

		]]--
--[[
function update()

	if MouseState == "LeftMouse" and KeyInput == "D" then
		MouseState = "LM is down"
		KeyInput = "D" ;
		--x = Draw()
	end
	if MouseState == "RightMouse" and KeyInput == "D" then
		MouseState = "RM is down"
		KeyInput = "D" ;
	end
	if(MouseState == "non")then
		MouseState = "non"
		KeyInput = "error" ;
	end

	return MouseState,KeyInput
end 

	]]--
-- ~= ( != c++)
