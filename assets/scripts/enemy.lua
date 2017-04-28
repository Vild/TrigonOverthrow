--cant use yet, playerpos is in floats, can only use doubles.
function updatePlayerPos(x, y, z)
	playerPosX = x
	playerPosY = y
	playerPosZ = z
end
	
function update(delta, enemyPos, playerPos)
	if enemyPos > playerPos then
		enemyPos = enemyPos - delta
	elseif enemyPos < playerPos then
		enemyPos = enemyPos + delta
	end
	return enemyPos
end

