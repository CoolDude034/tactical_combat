Entities.EnableEntityListening();

// DATA DUMP
// https://gist.githubusercontent.com/samisalreadytaken/7b85fa38a2c9cac9bfefa3440f9efb4f/raw/1bd9010fe3ccdf12ddab7970911f96df5c31ef53/datadump_ai_ally_speech_manager_sv.log

Hooks.Add( this, "OnEntityCreated", function(ent)
{
	if ( ent.GetClassname() == "generic_actor" || ent.GetClassname() == "generic_actor_custom" )
		return
	if ( !ent.IsNPC() )
		return
	
	local newScriptPath = "entities/" + ent.GetClassname()
	local oldScriptPath = NetProps.GetPropString(ent, "m_iszVScripts");
	if ( oldScriptPath.len() == 0 ) {
		NetProps.SetPropString( ent, "m_iszVScripts", newScriptPath );
	}
	else {
		NetProps.SetPropString( ent, "m_iszVScripts", oldScriptPath + " " + newScriptPath );
	}
	NetProps.SetPropString( ent, "m_iszScriptThinkFunction", "Update" );
}, "NPCManager" )

local Init = function(...)
{
	printl("PLAYER SPAWNED SUCCESSFULLY");
	
	EntFireByHandle(player, "RunScriptFile", "player", 0.00, null, null);
}
ListenToGameEvent( "player_spawn", Init, "post_mapspawn" );