--cant use yet, playerpos is in floats, can only use doubles.
--function updatePlayerPos(x, y, z)
--	playerPosX = x
--	playerPosY = y
--	playerPosZ = z
--end

function ranged(enemyPos, playerPos)
	if enemyPos > playerPos then
		enemyPos = enemyPos - 10
		if enemyPos < playerPos then
			return 0
		end
		return -1.0
	elseif enemyPos < playerPos then
		enemyPos = enemyPos + 10
		if enemyPos > playerPos then
			return 0
		end
		return 1.0
	end
	return 0
end

function melee(enemyPos, playerPos)
	
	if enemyPos > playerPos then
		return -1.0
	elseif enemyPos < playerPos then
		return 1.0
	end
	return 0
end

