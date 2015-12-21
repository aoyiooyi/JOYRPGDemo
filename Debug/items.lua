if (items == nil) then
	items = {}
end


items[1] = {
	name = hoa_system.Translate("Light Healing Potion"),
	description = hoa_system.Translate("Restores a small amount of hit points to a target."),
	icon = "img/icons/items/health_potion_small.png",
	target_type = hoa_global.GameGlobal.GLOBAL_TARGET_ALLY,
	standard_price = 60,

	BattleUse = function(user, target)
		target_actor = target:GetActor();
		target_actor:RegisterHealing(45);
		AudioManager:PlaySound("snd/potion_drink.wav");
	end,

	FieldUse = function(target)
		target:AddHitPoints(45);
		AudioManager:PlaySound("snd/potion_drink.wav");
	end
}

items[2] = {
	name = hoa_system.Translate("Medium Healing Potion"),
	description = hoa_system.Translate("Restores a moderate amount of hit points to a target."),
	icon = "img/icons/items/health_potion_medium.png",
	target_type = hoa_global.GameGlobal.GLOBAL_TARGET_ALLY,
	standard_price = 200,

	BattleUse = function(user, target)
		target_actor = target:GetActor();
		target_actor:RegisterHealing(45);
		AudioManager:PlaySound("snd/potion_drink.wav");
	end,

	FieldUse = function(target)
		target:AddHitPoints(150);
		AudioManager:PlaySound("snd/potion_drink.wav");
	end
}

items[3] = {
	name = hoa_system.Translate("Blessed Healing Potion"),
	description = hoa_system.Translate("Restores a large amount of hit points to a target."),
	icon = "img/icons/items/health_potion_large.png",
	target_type = hoa_global.GameGlobal.GLOBAL_TARGET_ALLY,
	standard_price = 500,

	BattleUse = function(user, target)
		target_actor = target:GetActor();
		target_actor:RegisterHealing(45);
		AudioManager:PlaySound("snd/potion_drink.wav");
	end,

	FieldUse = function(target)
		target:AddHitPoints(250);
		AudioManager:PlaySound("snd/potion_drink.wav");
	end
}

items[4] = {
	name = hoa_system.Translate("Super Healing Potion"),
	description = hoa_system.Translate("Restores a small amount of hit points to all party members."),
	icon = "img/icons/items/health_potion_huge.png",
	target_type = hoa_global.GameGlobal.GLOBAL_TARGET_ALL_ALLIES,
	standard_price = 1000,

	BattleUse = function(user, target)
		-- TODO: no support yet for party targets
		target_party = target:GetParty();
		-- TODO: iterate through each living actor in the party and add healing
		target_actor:RegisterHealing(100);
		AudioManager:PlaySound("snd/potion_drink.wav");
	end,

	FieldUse = function(target)
		target:AddHitPoints(100);
		AudioManager:PlaySound("snd/potion_drink.wav");
	end
}
