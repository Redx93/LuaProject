--package.path = '/usr/LibLua.lua;'
require("LibLua")

inputManager = InputManager.new()
local currentObject = nil

function CreateMesh(type)
		--create Object
		sprite  = Sprite.new()
		--in c call function for setting it to mouse position
		sprite:setType(type)
		SpriteMetaTable[numberOfSprite] = sprite
		numberOfSprite = numberOfSprite + 1
end
function update()
	mouse = inputManager:GetMouseEvent()
	keychar = inputManager:GetKeyEvent()

	if keychar == '1' then
		currentObject = SpriteMetaTable[1];
	end
	if mouse == 'RPress' then
		currentObject = nil
	end
	--[[
	if mouse "leftclick"
		for i =1, numberOfSprite do
		inputManager:CheckCollision(SpriteMetaTable[i])
		end
	]]--
	--[[
	if mouse "rightClick"
		currentObject = nil
	end
	]]--
	--[[
	if currentObject ~= nil then
		inputManager:Follow(currentObject)
	end
	]]--
	--[[
		if keychar == '1' or keychar =='2' or keychar == '3' or keychar =='4' then
		CreateMesh(keychar)
	end
	]]--

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
