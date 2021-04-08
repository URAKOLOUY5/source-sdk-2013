//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "hl1_c_player.h"

ConVar cl_npc_speedmod_intime( "cl_npc_speedmod_intime", "0.25", FCVAR_CLIENTDLL | FCVAR_ARCHIVE );
ConVar cl_npc_speedmod_outtime( "cl_npc_speedmod_outtime", "1.5", FCVAR_CLIENTDLL | FCVAR_ARCHIVE );

#if defined( CHL1_Player )
#undef CHL1_Player	
#endif

IMPLEMENT_CLIENTCLASS_DT( C_HL1_Player, DT_HL1Player, CHL1_Player )
	RecvPropInt( RECVINFO( m_bHasLongJump ) ),
	RecvPropInt( RECVINFO( m_nFlashBattery ) ),
	RecvPropBool( RECVINFO( m_bIsPullingObject ) ),

	RecvPropFloat( RECVINFO( m_flStartCharge ) ),
	RecvPropFloat( RECVINFO( m_flAmmoStartCharge ) ),
	RecvPropFloat( RECVINFO( m_flPlayAftershock ) ),
	RecvPropFloat( RECVINFO( m_flNextAmmoBurn ) )
END_RECV_TABLE()

C_HL1_Player::C_HL1_Player()
{
}

#ifdef DAM
void C_HL1_Player::CalcPlayerView(Vector& eyeOrigin, QAngle& eyeAngles, float& fov)
{
	BaseClass::CalcPlayerView(eyeOrigin, eyeAngles, fov);

	if (GetMoveType() == MOVETYPE_NOCLIP)
		return;

	Vector Velocity;
	EstimateAbsVelocity(Velocity);

	if (Velocity.Length() == 0)
	{
		IdleScale += gpGlobals->frametime * 0.05;
		if (IdleScale > 1.0)
			IdleScale = 1.0;
	}
	else
	{
		IdleScale -= gpGlobals->frametime;
		if (IdleScale < 0.0)
			IdleScale = 0.0;
	}

	CalcViewBob(eyeOrigin);
}

ConVar cl_hl1_rollspeed("cl_hl1_rollspeed", "300.0");
ConVar cl_hl1_rollangle("cl_hl1_rollangle", "0.65");

// Port from HL1 code:
// https://github.com/ValveSoftware/halflife/blob/master/cl_dll/view.cpp
void C_HL1_Player::CalcViewRoll(QAngle& eyeAngles)
{
	if (GetMoveType() == MOVETYPE_NOCLIP)
		return;

	float Side = CalcRoll(GetAbsAngles(), GetAbsVelocity(), cl_hl1_rollangle.GetFloat(), cl_hl1_rollspeed.GetFloat()) * 4.0;
	eyeAngles[ROLL] += Side;

	if (GetHealth() <= 0)
	{
		eyeAngles[ROLL] = 80;
		return;
	}
}

ConVar cl_hl1_bobcycle("cl_hl1_bobcycle", "0.8");
ConVar cl_hl1_bob("cl_hl1_bob", "0.01");
ConVar cl_hl1_bobup("cl_hl1_bobup", "0.5");

// Port from HL1 code:
// https://github.com/ValveSoftware/halflife/blob/master/cl_dll/view.cpp
void C_HL1_Player::CalcViewBob(Vector& eyeOrigin)
{
	float Cycle;
	Vector Velocity;

	if (GetGroundEntity() == nullptr || gpGlobals->curtime == BobLastTime)
	{
		eyeOrigin.z += ViewBob;
		return;
	}

	BobLastTime = gpGlobals->curtime;
	BobTime += gpGlobals->frametime;

	Cycle = BobTime - (int)(BobTime / cl_hl1_bobcycle.GetFloat()) * cl_hl1_bobcycle.GetFloat();
	Cycle /= cl_hl1_bobcycle.GetFloat();

	if (Cycle < cl_hl1_bobup.GetFloat())
		Cycle = M_PI * Cycle / cl_hl1_bobup.GetFloat();
	else
		Cycle = M_PI + M_PI * (Cycle - cl_hl1_bobup.GetFloat()) / (1.0 - cl_hl1_bobup.GetFloat());

	EstimateAbsVelocity(Velocity);
	Velocity.z = 0;

	ViewBob = sqrt(Velocity.x * Velocity.x + Velocity.y * Velocity.y) * cl_hl1_bob.GetFloat();
	ViewBob = ViewBob * 0.3 + ViewBob * 0.7 * sin(Cycle);
	ViewBob = min(ViewBob, 4);
	ViewBob = max(ViewBob, -7);

	eyeOrigin.z += ViewBob;
}

ConVar cl_hl1_iyaw_cycle("cl_hl1_iyaw_cycle", "2.0");
ConVar cl_hl1_iroll_cycle("cl_hl1_iroll_cycle", "0.5");
ConVar cl_hl1_ipitch_cycle("cl_hl1_ipitch_cycle", "1.0");
ConVar cl_hl1_iyaw_level("cl_hl1_iyaw_level", "0.3");
ConVar cl_hl1_iroll_level("cl_hl1_iroll_level", "0.1");
ConVar cl_hl1_ipitch_level("cl_hl1_ipitch_level", "0.3");

// Port from HL1 code:
// https://github.com/ValveSoftware/halflife/blob/master/cl_dll/view.cpp
void C_HL1_Player::CalcViewIdle(QAngle& eyeAngles)
{
	eyeAngles[ROLL] += IdleScale * sin(gpGlobals->curtime * cl_hl1_iroll_cycle.GetFloat()) * cl_hl1_iroll_level.GetFloat();
	eyeAngles[PITCH] += IdleScale * sin(gpGlobals->curtime * cl_hl1_ipitch_cycle.GetFloat()) * cl_hl1_ipitch_level.GetFloat();
	eyeAngles[YAW] += IdleScale * sin(gpGlobals->curtime * cl_hl1_iyaw_cycle.GetFloat()) * cl_hl1_iyaw_level.GetFloat();
}
#endif // HYPERBOREA	