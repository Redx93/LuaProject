--package.path = '/usr/LibLua.lua;'
require("LibLua")

inputManager = InputManager.new()
local currentObject = nil
local nrOfEnemies = 1
local enemiesTable = { }

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
--[[
	local collide
	if mouse == 'RPress' then
		local i = 1
		while SpriteMetaTable[i] do
			collide = inputManager:Collide(SpriteMetaTable[i])
			if collide == true then
			break end
			i = i + 1 
		end	
		if collide == false then
		currentObject = nil
		end
	end
	]]--

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
	enemy=Sprite.new("Enemy")
	enemy:setWaypoint()
	enemiesTable[nrOfEnemies] = enemy
	nrOfEnemies= nrOfEnemies + 1
	
	--enemy:updateEnemy() 
 end

function destroyEnemy(enemyID)
	enemyID=nil
	--nrOfEnemies=nrOfEnemies-1
end

function gamePhase()
	-- body
	for i=1,nrOfEnemies do
		local wpIsEmpty = enemiesTable[i]:updateEnemy()  
		enemiesTable[i]:Draw()
		if wpIsEmpty == true then
			destroyEnemy(enemiesTable[i])
		end
		

	end
	--Draw meshes
	for i=1, numberOfSprite do
		if SpriteMetaTable[i]:GetType()~="Waypoint" then
			SpriteMetaTable[i]:Draw()
		end
			
	end

end

