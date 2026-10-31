function OnPostSpawn()
{
	if (self.GetCitizenType() == 3)
	{
		EntFireByHandle(self, "SetSpeedModifier", "1.25", 0.00, null, null)
		
		self.SetCollisionGroup(COLLISION_GROUP_DEBRIS)
	}
}

function Update()
{
	if (self.GetCitizenType() == 3)
	{
		local Dist=(self.GetLocalOrigin()-Entities.GetLocalPlayer().GetLocalOrigin()).Length()/32.0
		if (Dist>=31.25) { //32.0 * 32.0
			self.SetLocalOrigin(player.GetLocalOrigin())
		}
	}
	
	return -1
}

function OnDeath()
{
	return ::OnKilledEnemy(self)
}