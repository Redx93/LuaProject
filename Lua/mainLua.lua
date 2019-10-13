--package.path = '/usr/LibLua.lua;'
require("LibLua")

inputManager = InputManager.new()
function update()
	theKey=inputManager:GetKey()
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
