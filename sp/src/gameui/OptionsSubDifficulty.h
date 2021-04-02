//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef OPTIONS_SUB_DIFFICULTY_H
#define OPTIONS_SUB_DIFFICULTY_H
#ifdef _WIN32
#pragma once
#endif

#include "vgui_controls/PropertyPage.h"

class CLabeledCommandComboBox;
class CCvarToggleCheckButton;
class CKeyToggleCheckButton;

//-----------------------------------------------------------------------------
// Purpose: Difficulty selection options
//-----------------------------------------------------------------------------
class COptionsSubDifficulty : public vgui::PropertyPage
{
	DECLARE_CLASS_SIMPLE( COptionsSubDifficulty, vgui::PropertyPage );

public:
	COptionsSubDifficulty(vgui::Panel *parent);

	virtual void OnResetData();
	virtual void OnApplyChanges();

#ifndef DBR
	MESSAGE_FUNC( OnRadioButtonChecked, "RadioButtonChecked" );
#endif

private:
#ifndef DBR
	vgui::RadioButton *m_pEasyRadio;
	vgui::RadioButton *m_pNormalRadio;
	vgui::RadioButton *m_pHardRadio;
#endif

#ifdef DBR
	vgui::ComboBox				*m_pSkillsCombo;
	CCvarToggleCheckButton		*m_pHudEnableCrosshairCheckBox;
#endif
};


#endif // OPTIONS_SUB_DIFFICULTY_H