#include "cbase.h"
#include "eventqueue.h"
#include "vscript/ivscript.h"
#include "vscript_server.h"

class CLogicObjectiveManager : public CLogicalEntity
{
	DECLARE_CLASS(CLogicObjectiveManager, CLogicalEntity);
public:
	void Spawn();
	void OnObjectiveUpdated();

	const char* ScriptGetObjective();
private:
	// Inputs
	void InputChangeObjective(inputdata_t& inputdata);

	// Outputs
	COutputEvent m_OnObjectiveCompleted;

	// KVs
	string_t m_sObjective;

	DECLARE_DATADESC();
};

LINK_ENTITY_TO_CLASS(logic_objectivemanager, CLogicObjectiveManager);

BEGIN_DATADESC(CLogicObjectiveManager)

DEFINE_KEYFIELD(m_sObjective, FIELD_STRING, "objective"),

DEFINE_OUTPUT(m_OnObjectiveCompleted, "OnObjectiveCompleted"),

END_DATADESC()

BEGIN_SCRIPTDESC_ROOT(CLogicObjectiveManager, "Objective manager entity")
DEFINE_SCRIPTFUNC(ScriptGetObjective, "Returns the current objective")
END_SCRIPTDESC()

void CLogicObjectiveManager::Spawn()
{
	BaseClass::Spawn();

	if (m_sObjective != NULL_STRING)
	{
		OnObjectiveUpdated();
	}
}

//-----------------------------------------------------------------------------
// Purpose: Updates any objective
//-----------------------------------------------------------------------------
void CLogicObjectiveManager::OnObjectiveUpdated()
{
	// Complete the objective if there was one active
	if (m_sObjective != NULL_STRING)
	{
		m_sObjective = NULL_STRING;
		m_OnObjectiveCompleted.FireOutput(this, this);
	}

	IGameEvent* pEvent = gameeventmanager->CreateEvent("sync_objective");
	if (pEvent)
	{
		pEvent->SetString("objective", m_sObjective.ToCStr());
		gameeventmanager->FireEvent(pEvent);
	}
}

//-----------------------------------------------------------------------------
// Purpose: Returns the current objective in VScript
//-----------------------------------------------------------------------------
const char* CLogicObjectiveManager::ScriptGetObjective()
{
	return STRING(m_sObjective);
}

//-----------------------------------------------------------------------------
// Purpose: Change the current objective
//-----------------------------------------------------------------------------
void CLogicObjectiveManager::InputChangeObjective(inputdata_t& inputdata)
{
	// Complete the objective if there was one active
	if (m_sObjective != NULL_STRING)
	{
		m_sObjective = NULL_STRING;
		m_OnObjectiveCompleted.FireOutput(this, this);
	}

	m_sObjective = inputdata.value.StringID();
	OnObjectiveUpdated();
}

