const MEDIC_HEAL_RANGE = 128.0;

m_propaneTank <- null
m_bPropaneHit <- false
m_bHasThrownSmokeGrenade <- false

if ( self.GetName().len() == 0 )
{
	self.SetName( UniqueString() )
}

function SpawnPropaneTank()
{
	local config =
	{
		targetname = self.GetName() + "_propane",
		model = "models/props_junk/propane_tank001a.mdl",
		origin = self.GetLocalOrigin(),
		health = "10",
		max_health = "10",
		physdamagescale = "0",
		spawnflags = 4 + 512 + 8388608
	}
	
	m_propaneTank = SpawnEntityFromTable("prop_physics", config)
	
	if (m_propaneTank) {
		m_propaneTank.SetParent(self, "eyes")
		m_propaneTank.SetLocalOrigin(Vector(-16, 0, -18))
		m_propaneTank.AddOutput("OnTakeDamage", "!self", "Ignite", "", 0.0, -1)
		m_propaneTank.AddOutput("OnTakeDamage", self.GetName(), "CallScriptFunction", "GruntPropanePanic", 0.0, -1)
		m_propaneTank.AddOutput("OnBreak", self.GetName(), "SetHealth", "0", 0.0, -1)
	}
}

function GruntPropanePanic()
{
	if (m_bPropaneHit) return
	m_bPropaneHit = true
}

function NPC_TranslateActivity()
{
	if ( IsGrunt() && m_bPropaneHit )
	{
		return "ACT_COMBINE_BUGBAIT"
	}
	
	return -1
}

function NPC_TranslateSchedule()
{
	if ( IsGrunt() && m_bPropaneHit )
	{
		return "SCHED_NONE"
	}
	
	return -1
}

function Precache()
{
	if ( startswith(GetMapName(), "d2_coast_") )
	{
		PrecacheModel("models/combine_soldier_wasteland.mdl", true)
		self.SetModel("models/combine_soldier_wasteland.mdl")
	}
}

function OnPostSpawn()
{
	if ( IsMedic() )
	{
		self.AddSpawnFlags(131072)
	}
	else if ( IsGrunt() )
	{
		SpawnPropaneTank()
	}
	
	if (::m_bSpeedyCombine) {
		EntFireByHandle(self, "SetSpeedModifier", "1.25", 0.00, null, null)
	}
	
	if ( RandomInt(0,1) == 0 && self.GetActiveWeapon() != null && self.GetActiveWeapon().GetClassname() == "weapon_smg1" )
	{
		//EntFireByHandle(self, "ChangeWeapon", "weapon_mp5", 0.00, null, null)
	}
	
	if ( startswith(self.GetName(), "soldier_harrasser_") )
	{
		self.CapabilitiesRemove(bits_CAP_MOVE_GROUND)
	}
}

function OnDeath()
{
	if ( IsGrunt() )
	{
		EntFire(self.GetName() + "_propane", "Kill")
	}
	
	if ( IsMedic() )
	{
		local ent = null
		while((ent = Entities.FindByClassnameWithin(ent,"npc_combine_s",self.GetOrigin(),MEDIC_HEAL_RANGE)) != null)
		{
			if ( ent == self ) continue;
			local otherScope = ent.GetScriptScope();
			if (otherScope == null) continue;
			if ("HasIncreasedHealth" in otherScope)
			{
				ent.SetMaxHealth(otherScope.OriginalHealth);
				if (ent.GetHealth() > ent.GetMaxHealth()) {
					ent.SetHealth(ent.GetMaxHealth());
				}
			}
		}
		
		SpawnEntityFromTable("item_healthkit", {origin = self.GetOrigin()})
	}
	
	return ::OnKilledEnemy(self)
}

function InputStartGlowing()
{
	local glowEnt = SpawnEntityFromTable("point_glow", {target = self.GetName(), GlowColor = "0 255 0 255"})
	EntFireByHandle(glowEnt, "Enable", "", 0.1)
	EntFireByHandle(glowEnt, "Disable", "", 0.5)
	EntFireByHandle(glowEnt, "Enable", "", 1.0)
	EntFireByHandle(glowEnt, "Disable", "", 1.5)
	EntFireByHandle(glowEnt, "Kill", "", 5.0)
}

// Skin Ranges
// Skin 0 corresponds to regular soldier
// Skin 1 corresponds to shotgun soldier
// Skin 2 corresponds to medic soldier
// Skin 3 corresponds to HLA styled grunt
function IsMedic()
{
	return ( self.GetSkin() == 2 )
}

function IsGrunt()
{
	return ( self.GetSkin() == 3 )
}

function MedicCheckForAllies()
{
    local ent = null
    while((ent = Entities.FindByClassnameWithin(ent,"npc_combine_s",self.GetOrigin(),MEDIC_HEAL_RANGE)) != null)
    {
		if ( ent == self ) continue;
		local myScope = self.GetScriptScope();
		if (myScope == null) continue;
		if ("IsMedic" in myScope && !myScope.IsMedic()) continue;
		local otherScope = ent.GetScriptScope();
		if (otherScope == null) continue;
		if ("HasIncreasedHealth" in otherScope) continue;
		if (ent.GetHealth() < ent.GetMaxHealth()) {
			otherScope.OriginalHealth <- ent.GetMaxHealth();
			otherScope.HasIncreasedHealth <- true;
			
			ent.SetMaxHealth(ent.GetMaxHealth() * 2);
			ent.SetHealth(ent.GetMaxHealth());
			EntFireByHandle(ent, "StartGlowing", "", 0.00, self, self);
		}
    }
}

function Update()
{
	if (IsMedic()) {
		MedicCheckForAllies()
	}
	
	return -1
}