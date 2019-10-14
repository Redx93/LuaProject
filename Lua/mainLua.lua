--package.path = '/usr/LibLua.lua;'
require("LibLua")

inputManager = InputManager.new()

function update()
	mouse = inputManager:GetMouseEvent()
	keychar = inputManager:GetKeyEvent()
	if keychar == '1' then
		for i=1, numberOfSprite do
				SpriteMetaTable[i]:Draw()
		end
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
