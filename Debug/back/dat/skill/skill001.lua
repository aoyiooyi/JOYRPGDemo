targetIsEnemy = true
skillAnm = "./image/skill/skill001.png"
skillAnmFrame = 17
skillAnmFPS = 7
skillAnmWidth = 254
skillAnmHeight = 178


skillDamage = 170

function UseSkill(sponsor, target)
	target:UnderAttack(skillDamage + sponsor:GetAnima());

end
