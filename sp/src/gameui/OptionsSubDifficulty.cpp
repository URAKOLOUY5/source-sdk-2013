//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "OptionsSubDifficulty.h"
#include "tier1/convar.h"
#include "vgui_controls/ComboBox.h"
#include "CvarToggleCheckButton.h"
#include "EngineInterface.h"
#include "tier1/KeyValues.h"

#include "vgui_controls/RadioButton.h"

using namespace vgui;

#ifdef DBR
//-----------------------------------------------------------------------------
// Purpose: list of skills
//-----------------------------------------------------------------------------
enum SkillsLevel_e
{
	EASY,
	MEDIUM,
	HIGH,
};
#endif

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
COptionsSubDifficulty::COptionsSubDifficulty(vgui::Panel *parent) : BaseClass(parent, NULL)
{
#ifndef DBR
	m_pEasyRadio = new RadioButton(this, "Skill1Radio", "#GameUI_SkillEasy");
	m_pNormalRadio = new RadioButton(this, "Skill2Radio", "#GameUI_SkillNormal");
	m_pHardRadio = new RadioButton(this, "Skill3Radio", "#GameUI_SkillHard");
#endif

#ifdef DBR
	m_pSkillsCombo = new ComboBox(this, "SkillsBox", 6, false);
	m_pSkillsCombo->AddItem("#GameUI_SkillEasy", new KeyValues("SkillsBox", "skill", EASY));
	m_pSkillsCombo->AddItem("#GameUI_SkillNormal", new KeyValues("SkillsBox", "skill", MEDIUM));
	m_pSkillsCombo->AddItem("#GameUI_SkillHard", new KeyValues("SkillsBox", "skill", HIGH));

	m_pHudEnableCrosshairCheckBox = new CCvarToggleCheckButton(
		this,
		"ShowSCPHud",
		"#GameUI_HUD_Enable_Crosshair",
		"hud_draw_crosshair");
#endif

	LoadControlSettings("Resource/OptionsSubDifficulty.res");
}

#ifndef DBR
//-----------------------------------------------------------------------------
// Purpose: resets controls
//-----------------------------------------------------------------------------
void COptionsSubDifficulty::OnResetData()
{
	ConVarRef var( "skill" );

	if (var.GetInt() == 1)
	{
		m_pEasyRadio->SetSelected(true);
	}
	else if (var.GetInt() == 3)
	{
		m_pHardRadio->SetSelected(true);
	}
	else
	{
		m_pNormalRadio->SetSelected(true);
	}
}
#endif DBR

#ifdef DBR
void COptionsSubDifficulty::OnResetData()
{
	// Difficulty (dropdown menu)

	ConVarRef skill_level("skill");
	if (skill_level.GetInt() == 3)
	{
		m_pSkillsCombo->ActivateItem(2);
	}
	if (skill_level.GetInt() == 2)
	{
		m_pSkillsCombo->ActivateItem(1);
	}
	if (skill_level.GetInt() == 1)
	{
		m_pSkillsCombo->ActivateItem(0);
	}

	// Enable crosshair
	m_pHudEnableCrosshairCheckBox->Reset();
}
#endif

#ifdef DBR
//-----------------------------------------------------------------------------
// Purpose: sets data based on control settings
//-----------------------------------------------------------------------------
void COptionsSubDifficulty::OnApplyChanges()
{
	// Difficulty (dropdown menu)
	ConVarRef skill_level("skill");
	int skill_level_value = 0;

	switch (m_pSkillsCombo->GetActiveItem())
	{
	default:
	case 0:
		skill_level_value = 0;
		skill_level.SetValue(1);
		break;
	case 1:
		skill_level_value = 1;
		skill_level.SetValue(2);
		break;
	case 2:
		skill_level_value = 1;
		skill_level.SetValue(3);
		break;
	}
	
	// Enable crosshair
	m_pHudEnableCrosshairCheckBox->ApplyChanges();
}
#endif

#ifndef DBR
//-----------------------------------------------------------------------------
// Purpose: sets data based on control settings
//-----------------------------------------------------------------------------
void COptionsSubDifficulty::OnApplyChanges()
{
	ConVarRef var("skill");

	if (m_pEasyRadio->IsSelected())
	{
		var.SetValue(1);
	}
	else if (m_pHardRadio->IsSelected())
	{
		var.SetValue(3);
	}
	else
	{
		var.SetValue(2);
	}
}

#endif

#ifndef DBR
//-----------------------------------------------------------------------------
// Purpose: enables apply button on radio buttons being pressed
//-----------------------------------------------------------------------------
void COptionsSubDifficulty::OnRadioButtonChecked()
{
	PostActionSignal(new KeyValues("ApplyButtonEnable"));
}
#endif
