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
	m_Font = surface()->CreateFont();
	surface()->AddBitmapFontFile("sourcetest");
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
	int x = 10;
	int y = 10;
	int w = 300;
	int h = 80;

	surface()->DrawSetColor(Color(111, 120, 133, 255));
	surface()->DrawFilledRect(x, y, x + w, y + h);

	const char* pszText = STRING(g_syncedObjective);
	wchar_t wszText[128];
	g_pVGuiLocalize->ConvertANSIToUnicode(pszText, wszText, sizeof(wszText));

	surface()->DrawSetTextFont(m_Font);
	surface()->DrawSetTextColor(Color(255, 255, 255, 255));
	surface()->DrawSetTextPos(x, y);
	surface()->SetFontGlyphSet(m_Font, "Tahoma", 18, 500, 0, 0, 0);
	surface()->DrawPrintText(wszText, wcslen(wszText));
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
