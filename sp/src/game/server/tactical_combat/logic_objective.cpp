#include "cbase.h"
#include "eventqueue.h"

class CLogicObjectiveManager : public CLogicalEntity
{
	DECLARE_CLASS(CLogicObjectiveManager, CLogicalEntity);
public:
	void Spawn();
	void OnObjectiveUpdated();
private:
	// Inputs
	void InputNewObjective(inputdata_t& inputdata);

	// Outputs
	COutputString m_OnNewObjective;

	// KVs
	string_t m_sObjective;

	DECLARE_DATADESC();
};

LINK_ENTITY_TO_CLASS(logic_objectivemanager, CLogicObjectiveManager);

BEGIN_DATADESC(CLogicObjectiveManager)

DEFINE_KEYFIELD(m_sObjective, FIELD_STRING, "objective"),

// Inputs
DEFINE_INPUTFUNC(FIELD_STRING, "NewObjective", InputNewObjective),

DEFINE_OUTPUT(m_OnNewObjective, "OnNewObjective"),

END_DATADESC()

void CLogicObjectiveManager::Spawn()
{
	BaseClass::Spawn();

	if (gEntList.FindEntityByClassname(NULL, "logic_objectivemanager") != NULL)
	{
		Warning("[logic_objectivemanager] Only one instance is allowed!\n");
		UTIL_Remove(this);
		return;
	}

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
	variant_t var;
	var.SetString(m_sObjective);
	m_OnNewObjective.FireOutput(var, this, this);

	IGameEvent* pEvent = gameeventmanager->CreateEvent("sync_objective");
	if (pEvent)
	{
		pEvent->SetString("objective", m_sObjective.ToCStr());
		gameeventmanager->FireEvent(pEvent);
	}
}

void CLogicObjectiveManager::InputNewObjective(inputdata_t& inputdata)
{
	m_sObjective = inputdata.value.StringID();
	OnObjectiveUpdated();
}
