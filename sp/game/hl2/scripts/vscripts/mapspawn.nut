if (SERVER_DLL) {
	IncludeScript("globals")
	IncludeScript("global_entities")
	IncludeScript("hooks")
}
if (CLIENT_DLL) {
	getconsttable().PROP_COMBINEBALL_CLASSNAME <- IsWindows() ? "class C_PropCombineBall" : "17C_PropCombineBall";
	IncludeScript("combineball_dlight")
}