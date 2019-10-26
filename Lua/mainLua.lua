--package.path = '/usr/LibLua.lua;'
require("LibLua")

inputManager = InputManager.new()
local currentObject = nil

--EnemyMetaTable = { }
--numberOfEnemies = 1


function CreateMesh(type)
	local x , y = inputManager:GetPos()
	--create Object
	sprite  = Sprite.new()
	--in c call function for setting it to mouse position
	sprite:SetPosition(x, y)
	sprite:SetType(type)  
	if sprite:GetType() == "Tower" then
		sprite:CreateTower()
	end
	SpriteMetaTable[numberOfSprite] = sprite
	numberOfSprite = numberOfSprite + 1
	--return sprite so we can use it to follow mouse
	return sprite
end
function update() --edit phase
	mouse = inputManager:GetMouseEvent()
	keychar = inputManager:GetKeyEvent()

	if keychar == '1' or keychar == '3' or keychar == '4' then
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
	--sprite1:SetPosition(-3.5,-5.5)
	sprite1:SetWaypoint() --init Waypoint here


	--EnemyMetaTable[numberOfEnemies]= sprite1
	--numberOfEnemies = numberOfEnemies + 1 

	SpriteMetaTable[numberOfSprite] = sprite1
	numberOfSprite = numberOfSprite + 1
 end



function destroyEnemy(enemyID)
	enemyID=nil
	--nrOfEnemies=nrOfEnemies-1
end

function gamePhase() 
	--draw
	for i=1, numberOfSprite do
		--if SpriteMetaTable[i]:GetType()~="Waypoint" then
			SpriteMetaTable[i]:Draw()
		--end	

		if SpriteMetaTable[i]:GetType() == "Enemy" then
			local enemyInGoal = SpriteMetaTable[i]:updateEnemy()
			if enemyInGoal == true then
				--SpriteMetaTable[i]:SetPosition(0,0)
				--destroyEnemy(SpriteMetaTable[i])
			end

			for j=1, numberOfSprite do
				if SpriteMetaTable[j]:GetType() == "Tower" then
					if SpriteMetaTable[j]:InRangeTower(SpriteMetaTable[i]) == true then
						SpriteMetaTable[j]:Shoot(SpriteMetaTable[i])
					end
				end
			end

		end




	end


	--physics
	--for i=1, numberOfSprite do
		--if SpriteMetaTable[i]:GetType() == "Enemy" then
		--	local enemyInGoal = SpriteMetaTable[i]:updateEnemy()
		--	if enemyInGoal == true then
				--SpriteMetaTable[i]:SetPosition(0,0)
				--destroyEnemy(SpriteMetaTable[i])
		--	end
		--end

		--fix
		--if SpriteMetaTable[i]:GetType() == "Tower" then
			--local newSpriteTable = {} 
			--newSpriteTable = SpriteMetaTable
			--newSpriteTable:InRangeTower()
			--towerAttack(SpriteMetaTable[i], SpriteMetaTable)
		--end 
	--end



end

function towerAttack(tower, table )

	for j=1, numberOfSprite do
		local enemytype = table[j]
		if table[j]:GetType() == "Enemy" then
			tower:InRangeTower(enemytype)
		end	
	end
end