#include "cbase.h"
#include "hud.h"
#include "hudelement.h"
#include "hud_numericdisplay.h"
#include "hud_macros.h"
#include "c_basehlplayer.h"
#include "iclientmode.h"
#include <vgui_controls/AnimationController.h>
#include <vgui/ISurface.h>
#include <vgui/ILocalize.h>
#include <vgui_controls/Panel.h>

using namespace vgui;

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

string_t g_syncedObjective;

//-----------------------------------------------------------------------------
// Purpose: Shows the objective
//-----------------------------------------------------------------------------
class CHudObjective : public CHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE(CHudObjective, vgui::Panel);

public:
	CHudObjective(const char* pElementName);
	virtual void	Init(void);
	virtual void	Reset(void);
	virtual void	OnThink(void);
	bool			ShouldDraw(void);

	void			ApplySchemeSettings(vgui::IScheme* pScheme);

protected:
	virtual void	Paint();

private:
	HFont m_Font;
};

DECLARE_HUDELEMENT(CHudObjective);

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CHudObjective::CHudObjective(const char* pElementName) : CHudElement(pElementName), BaseClass(NULL, "HudObjective")
{
	vgui::Panel* pParent = g_pClientMode->GetViewport();
	SetParent(pParent);

	SetHiddenBits(HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudObjective::Init(void)
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudObjective::Reset(void)
{
	Init();
	g_syncedObjective = NULL_STRING;
}

//-----------------------------------------------------------------------------
// Purpose: Save CPU cycles by letting the HUD system early cull
// costly traversal.  Called per frame, return true if thinking and 
// painting need to occur.
//-----------------------------------------------------------------------------
bool CHudObjective::ShouldDraw()
{
	return CHudElement::ShouldDraw();
}

//-----------------------------------------------------------------------------
// Purpose: Apply settings from .res file
//-----------------------------------------------------------------------------
void CHudObjective::ApplySchemeSettings(vgui::IScheme* pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);
	m_Font = pScheme->GetFont("ObjectiveFont", true);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudObjective::OnThink(void)
{
}

//-----------------------------------------------------------------------------
// Purpose: draws the objective box
//-----------------------------------------------------------------------------
void CHudObjective::Paint()
{
	int x,y,w,h;
	GetPos(x, y);
	GetSize(w, h);

	surface()->DrawSetColor(GetBgColor());
	surface()->DrawFilledRect(x, y, x + w, y + h);

	const char* pszText = STRING(g_syncedObjective);
//	wchar_t wszText[128];
	wchar_t* wasLocalized = NULL;
	char szToken[256];
	Q_snprintf(szToken, sizeof(szToken), "#%s", pszText);
	wasLocalized = g_pVGuiLocalize->Find(szToken);

	if (!wasLocalized)
	{
		static wchar_t wszFallback[256];
		g_pVGuiLocalize->ConvertANSIToUnicode(pszText, wszFallback, ARRAYSIZE(wszFallback));
		wasLocalized = wszFallback;
	}

	surface()->DrawSetTextFont(m_Font);
	surface()->DrawSetTextColor(GetFgColor());
	surface()->DrawSetTextPos(x, y);
	surface()->DrawPrintText(wasLocalized, wcslen(wasLocalized));
}

class ObjectiveListener : public IGameEventListener2
{
	ObjectiveListener()
	{
		gameeventmanager->AddListener(this, "sync_objective", false);
	}

	void FireGameEvent(IGameEvent* pEvent)
	{
		// check event type and print message
		if (!strcmp("sync_objective", pEvent->GetName()))
		{
			Msg("New Objective: %s\n",pEvent->GetString("objective"));

			g_syncedObjective = pEvent->GetString("objective");
		}
	}
};
