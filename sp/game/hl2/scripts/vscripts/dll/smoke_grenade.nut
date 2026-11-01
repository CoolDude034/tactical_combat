// This script is ran by smoke clouds created by smoke grenades, this is run by the server.dll

EntFireByHandle(self, "TurnOn", "", 0.00, null, null)
EntFireByHandle(self.GetOwner(), "Kill", "", Convars.GetFloat("sk_smoke_grenade_smoke_despawn_time"), null, null)
