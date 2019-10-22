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

	for i = 1, numberOfSprite do
		if mouse == 'RPress' then
			local b_collide = inputManager:Collide(SpriteMetaTable[i])
			if b_collide == false then
				currentObject = nil
			end
		end
	end

	for i = 1, numberOfSprite do
		if mouse == "LPress" then
			local b_collide = inputManager:Collide(SpriteMetaTable[i])
			if b_collide == true then
				currentObject = SpriteMetaTable[i]
				break
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

function spawnEnemy()
	sprite1 = Sprite.new()
	sprite1:SetType("Enemy")
	sprite1:CreateEnemy()
	sprite1:SetPosition(-3.5,-6.5)
	sprite1:SetWaypoint()
	SpriteMetaTable[numberOfSprite] = sprite1
	numberOfSprite = numberOfSprite + 1
 end

function destroyEnemy(enemyID)
	enemyID=nil
	--nrOfEnemies=nrOfEnemies-1
end

function gamePhase()
	for i=1, numberOfSprite do
		if SpriteMetaTable[i]:GetType() == "Enemy" then
			SpriteMetaTable[i]:updateEnemy()
		end
		if SpriteMetaTable[i]:GetType()~="Waypoint" then
			SpriteMetaTable[i]:Draw()
		end	
	end
end

