#pragma once

#include "game.h"
#include "aimstuff.h"

//-----------------------------------------------------------

class CPlayerPed
{
public:

	void  SetKeys(WORD wKeys);
	WORD  GetKeys();

	CAMERA_AIM * GetCurrentAim();
	void SetCurrentAim(CAMERA_AIM *pAim);

	void SetBleeding(bool bBleeding);
	void  Destroy();
	void  SetDead();

	void  ShowMarker(int iMarkerColor);
	BYTE  GetCurrentWeapon();
	int   GetCurrentVehicleID();
	void  Say(UINT uiNum);
	void  SetInitialState();
	BOOL  IsOnScreen();
	void SetZAngle(float z);

	float GetHealth();
	void  SetHealth(float fHealth);
	float GetArmour();
	void  SetArmour(float fArmour);
	BOOL  IsDead();
	BOOL  IsInVehicle();
	BYTE  GetAction();
	void  SetAction(BYTE byteTrigger);
	BYTE  GetShootingFlags();
	void  SetShootingFlags(BYTE byteShooting);

	BOOL  EnforceWorldBoundries(float fPX, float fZX, float fPY, float fNY);
	
	void  GetMatrix(PMATRIX4X4 Matrix);
	void  SetMatrix(MATRIX4X4 Matrix);
	void  GetMoveSpeedVector(PVECTOR Vector);
	void  SetMoveSpeedVector(VECTOR Vector);
	void  GetTurnSpeedVector(PVECTOR Vector);
	void  SetTurnSpeedVector(VECTOR Vector);

	float GetRotation();
	void  SetRotation(float fRotation);

	void GiveWeapon(int iWeaponID, int iAmmo);
	void ClearAllWeapons();
	void SetArmedWeapon(int iWeaponType);
	BOOL HasAmmoForCurrentWeapon();

	void SetModel(int iSkin);
	int  GetModel();
	void CheckAndRepairInvisProblems();
	void Teleport(float x, float y, float z);
	void ClearTargetAndVehicle();

	void ClearLastWeaponDamage();
	void SetObjective(PDWORD pObjEntity, int iFunction);
	void SetImmunities(BOOL bBullet, BOOL bFire, BOOL bExplosion, BOOL bDamage, BOOL bUnknown);

	void PutDirectlyInVehicle(int iVehicleID);
	void EnterVehicleAsDriver(int iVehicleID);
	void EnterVehicleAsPassenger(int iVehicleID);
	void ExitCurrentVehicle();
	void ForceIntoPassengerSeat(UINT uiVehicleID, UINT uiSeat);

	BOOL IsAPassenger();

	VEHICLE_TYPE * GetGtaVehicle();
	PED_TYPE * GetGtaActor() { return m_pPed; };

	void RemoveFromVehicleAndPutAt(float fX, float fY, float fZ);
	void TogglePlayerControllable(int iControllable);
	BYTE FindDeathReasonAndResponsiblePlayer(BYTE * nPlayer);
	void RestartIfWastedAt(VECTOR *vecRestart, float fRotation);
	UINT GetPassengerSeat();

	// Constructor/Destructor.	
	CPlayerPed();
	CPlayerPed(int iPlayerNumber, int iSkin, float fPosX, float fPosY, float fPosZ, float fRotation = 0.0f);
	~CPlayerPed();

	PED_TYPE   *m_pPed;
	DWORD		m_dwGTAId;
	BYTE		m_bytePlayerNumber;
};

//-----------------------------------------------------------
