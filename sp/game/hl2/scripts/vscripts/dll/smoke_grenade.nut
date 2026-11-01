// This script is ran by smoke clouds created by smoke grenades, this is run by the server.dll

EntFireByHandle(self, "TurnOn", "", 0.00, null, null)

local despawnTime = Convars.GetFloat("sk_smoke_grenade_smoke_despawn_time")
EntFireByHandle(self, "Kill", "", despawnTime, null, null)
EntFireByHandle(self.GetOwner(), "Kill", "", despawnTime, null, null)
