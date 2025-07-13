#include "cbase.h"
#include "eventqueue.h"
#include "vscript/ivscript.h"
#include "vscript_server.h"

bool g_isAlarmTriggered;
bool g_InstanceCreated;

class CLogicAlarmManager : public CLogicalEntity
{
	DECLARE_CLASS(CLogicAlarmManager, CLogicalEntity);
public:
	void Spawn();
	bool ScriptIsPoliceCalled();
	void AlarmTriggered();

	CLogicAlarmManager();
	~CLogicAlarmManager();
private:
	// Inputs
	void InputTriggerAlarm(inputdata_t& inputdata);

	// Outputs
	COutputEvent m_OnAlarmTriggered;

	DECLARE_DATADESC();
};

LINK_ENTITY_TO_CLASS(logic_alarm, CLogicAlarmManager);

BEGIN_DATADESC(CLogicAlarmManager)

// Inputs
DEFINE_INPUTFUNC(FIELD_VOID, "TriggerAlarm", InputTriggerAlarm),

// Outputs
DEFINE_OUTPUT(m_OnAlarmTriggered, "OnAlarmTriggered"),

END_DATADESC()

BEGIN_SCRIPTDESC_ROOT(CLogicAlarmManager, "Alarm manager entity")
DEFINE_SCRIPTFUNC(ScriptIsPoliceCalled, "Returns if the police is called.")
DEFINE_SCRIPTFUNC(AlarmTriggered, "Trigger the alarm if it wasn't called before")
END_SCRIPTDESC()

void CLogicAlarmManager::Spawn()
{
	BaseClass::Spawn();

	if (g_InstanceCreated)
	{
		Warning("logic_alarm only allows one instance per-session!\n");
		UTIL_Remove(this);
		return;
	}

	g_InstanceCreated = true;
}

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CLogicAlarmManager::CLogicAlarmManager()
{
	g_isAlarmTriggered = false;
}

//-----------------------------------------------------------------------------
// Purpose: Deconstructor
//-----------------------------------------------------------------------------
CLogicAlarmManager::~CLogicAlarmManager()
{
	g_isAlarmTriggered = false;
	g_InstanceCreated = false;
}

void CLogicAlarmManager::InputTriggerAlarm(inputdata_t& inputdata)
{
	AlarmTriggered();
}

//-----------------------------------------------------------------------------
// Purpose: Returns if the police are called for VScript
//-----------------------------------------------------------------------------
bool CLogicAlarmManager::ScriptIsPoliceCalled()
{
	return g_isAlarmTriggered;
}

void CLogicAlarmManager::AlarmTriggered()
{
	if (g_isAlarmTriggered) return;
	g_isAlarmTriggered = true;

	m_OnAlarmTriggered.FireOutput(this, this);
}
