--package.path = '/usr/LibLua.lua;'
require("LibLua")

inputManager = InputManager.new()
local currentObject = nil
function update()
	mouse = inputManager:GetMouseEvent()
	keychar = inputManager:GetKeyEvent()

	if keychar == '1' then
		currentObject = SpriteMetaTable[1];
	end
	if mouse == '1' then
		currentObject = nil
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
