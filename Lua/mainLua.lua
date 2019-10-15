--package.path = '/usr/LibLua.lua;'
require("LibLua")

inputManager = InputManager.new()
local currentObject = nil

function CreateMesh(type)
	local x , y = inputManager:GetPos()
	--create Object
	sprite  = Sprite.new()
	--in c call function for setting it to mouse position
	sprite:SetPosition(x, y)
	sprite:SetType(type)
	SpriteMetaTable[numberOfSprite] = sprite
	numberOfSprite = numberOfSprite + 1
	--return sprite so we can use it to follow mouse
	return sprite
end
function update()
	mouse = inputManager:GetMouseEvent()
	keychar = inputManager:GetKeyEvent()

	if keychar == '1' or keychar == '2' or keychar == '3' or keychar == '4' then
		currentObject = CreateMesh(keychar)
	end
	if mouse == 'RPress' then
		currentObject = nil
	end
	
	for i = 1, numberOfSprite do
		if mouse == "LPress" then
			local b_collide = inputManager:Collide(SpriteMetaTable[i])
			if b_collide == true then
				currentObject = SpriteMetaTable[i]
				do return end
			end
		end
	end
	if currentObject ~= nil then
		inputManager:Follow(currentObject)
	end
	--Draw meshes
	for i=1, numberOfSprite do
			SpriteMetaTable[i]:Draw()
	end
 end

--[[
function EditorUpdate()
	if InputManger:MouseState() == "LeftMouse" and 
	   InputManger:KeyInput() == "D" then
	 DeleteMesh(SpriteMetaTable[i])
	end
end 

]]--
-- ~= ( != c++)
