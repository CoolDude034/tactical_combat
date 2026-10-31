// MODIFIERS
m_bLowViolence <- false
m_bSlowWalker <- false
m_bMandetoryAugmentation <- false
m_bSpeedyCombine <- false

function RandomizeModifiers()
{
	if (RandomInt(0,100) <= 2) {
		m_bSlowWalker = true
	}
	if (RandomInt(0,100) <= 2) {
		m_bLowViolence = true
	}
	if (RandomInt(0,100) <= 5) {
		m_bMandetoryAugmentation = true
	}
	if (RandomInt(0,100) <= 5) {
		m_bSpeedyCombine = true
	}
}

function OnKilledEnemy(victim)
{
	if (m_bLowViolence) {
		EntFireByHandle(victim, "DropWeapon", "", 0.00, null, null)
		EntFireByHandle(victim, "Kill", "", 0.00, null, null)
		return false
	}
	return true
}

if (date().month == 5 || date().month == 6) //0 indexed, so 5
{
	printl("Happy Pride Month!")
	Globals.AddGlobal("is_pride_month", GetMapName(), 1)
}

RandomizeModifiers()