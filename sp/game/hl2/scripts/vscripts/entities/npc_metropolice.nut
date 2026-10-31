const SF_METROPOLICE_ARREST_ENEMY = 2097152

m_iEliteCopHealth <- 80

function IsEliteCop()
{
	if ( startswith(GetMapName(), "d3_c17_") ) return true
	if ( GetMapName() == "d3_c17_05" && self.GetName() == "garrison_cop_1" ) return false
	if ( GetMapName() == "d3_c17_05" && self.GetName() == "garrison_cop_2" ) return false
	if ( GetMapName() == "d3_c17_05" && startswith(self.GetName(), "garrison_3nd_floor_") && RandomInt(0,1) == 0 ) return false
	return false
}

function IsOfficeCop()
{
	if ( GetMapName() == "d3_c17_05" && self.GetName() == "outpost_room_1_cop_1" ) return true
	if ( GetMapName() == "d3_c17_05" && self.GetName() == "outpost_room_1_cop_2" ) return true
	if ( GetMapName() == "d3_c17_07" && self.GetName() == "first_cop" ) return true
	if ( GetMapName() == "d3_c17_05" && self.GetName() == "garrison_cop_1" ) return false
	if ( GetMapName() == "d3_c17_05" && self.GetName() == "garrison_cop_2" ) return false
	if ( GetMapName() == "d3_c17_05" && startswith(self.GetName(), "garrison_2nd_floor_") ) return true
	return false
}

function IsOTA()
{
	if ( self.GetName() == "arrest_police_1" ) return false
	if ( self.GetName() == "arrest_police_2" ) return false
	if ( self.GetName() == "beat_cop1" ) return false
	if ( self.GetName() == "stairscop" ) return false
	return ::m_bMandetoryAugmentation
}

if ( IsOfficeCop() )
{
	local shouldRandomizeAppearance = true
	if ( GetMapName() == "d3_c17_05" && self.GetName() == "outpost_room_1_cop_1" ) { shouldRandomizeAppearance = false }
	if ( GetMapName() == "d3_c17_05" && self.GetName() == "outpost_room_1_cop_2" ) { shouldRandomizeAppearance = false }
	
	if (shouldRandomizeAppearance) {
		IncludeScript("entities/base_random")
		m_GroupType <- "combine"
	}
	else {
		function Precache()
		{
			switch(self.GetName())
			{
				case "outpost_room_1_cop_1":
					PrecacheModel("models/humans/combine/male_05.mdl", true)
					self.SetModel("models/humans/combine/male_05.mdl")
					break;
				case "outpost_room_1_cop_2":
					PrecacheModel("models/humans/combine/female_01.mdl", true)
					self.SetModel("models/humans/combine/female_01.mdl")
					break;
				default:
					break;
			}
		}
	}
}
else if ( IsEliteCop() )
{

function Precache()
{
	PrecacheModel("models/police_elite.mdl", true)
	self.SetModel("models/police_elite.mdl")
}

}
else if ( IsOTA() )
{

function Precache()
{
	PrecacheModel("models/combine_soldier.mdl", true)
	self.SetModel("models/combine_soldier.mdl")
}

}

function OnPostSpawn()
{
	if ( self.GetName() == "arrest_police_1" || self.GetName() == "arrest_police_2" )
	{
		EntFireByHandle(self, "ChangeWeapon", "weapon_stunstick", 0.00, null, null)
	}
	else if ( self.GetName() == "beat_cop1" )
	{
		self.AddSpawnFlags(SF_METROPOLICE_ARREST_ENEMY)
	}
	
	if ( IsOfficeCop() )
	{
		EntFireByHandle(self, "AddContext", "classname:npc_citizen", 0.00, null, null)
	}
	else if ( IsEliteCop() )
	{
		self.SetMaxHealth(m_iEliteCopHealth)
		self.SetHealth(self.GetMaxHealth())
	}
	else if ( IsOTA() )
	{
		self.SetMaxHealth( Convars.GetInt("sk_combine_s_health") )
		self.SetHealth(self.GetMaxHealth())
		
		EntFireByHandle(self, "AddContext", "classname:npc_combine_s", 0.00, null, null)
		EntFireByHandle(self, "ChangeWeapon", "weapon_smg1", 0.00, null, null)
	}
	
	if ( startswith(self.GetName(),"stationary_cop_") )
	{
		self.CapabilitiesRemove(bits_CAP_MOVE_GROUND)
		NetProps.SetPropInt(self, "m_bKeepFacingPlayer", 1)
	}
	
	if ( RandomInt(0,1) == 0 && self.GetActiveWeapon() != null && self.GetActiveWeapon().GetClassname() == "weapon_smg1" )
	{
		//EntFireByHandle(self, "ChangeWeapon", "weapon_smg2", 0.00, null, null)
	}
	
	if (::m_bSpeedyCombine) {
		EntFireByHandle(self, "SetSpeedModifier", "1.25", 0.00, null, null)
	}
}

function OnDeath()
{
	return ::OnKilledEnemy(self)
}