#pragma once

#include "game.h"

class CVehicle
{

public:

	CVehicle(int iType, float fPosX, float fPosY,
			 float fPosZ, float fRotation = 0.0f);

	~CVehicle();

	float GetDistanceFromLocalPlayerPed();
	void  SetLockedState(int iLocked);
	void  UpdateLastDrivenTime();

	int   GetVehicleType();
	BYTE  GetVehicleSubtype();

	float GetHealth();
	void  SetHealth(float fHealth);
	void  SetColor(int iColor1, int iColor2);
	void  VerifyControlState();

	void  GetMatrix(PMATRIX4X4 Matrix);
	void  SetMatrix(MATRIX4X4 Matrix);
	void  GetMoveSpeedVector(PVECTOR Vector);
	void  SetMoveSpeedVector(VECTOR Vector);
	void  GetTurnSpeedVector(PVECTOR Vector);
	void  SetTurnSpeedVector(VECTOR Vector);

	BOOL  HasSunk();
	BOOL  IsDriverLocalPlayer();
	void  DoubleSpeed(void);
	void  VehicleJump(float ForwardDistance, float UpDistance);
	void  EnforceWorldBoundries(float fPX, float fZX, float fPY, float fNY);
	BOOL  HasExceededWorldBoundries(float fPX, float fZX, float fPY, float fNY);

	BOOL  GetHandbrakeState();
	void  SetHandbrakeState(BOOL bState);
	BYTE  GetMaxPassengers();

	BYTE  GetEntityFlags();
	void  SetEnitityFlags(BYTE byteEntityFlags);

	void SetInvulnerable(BOOL bInv);
	BOOL IsInvulnerable() { return m_bIsInvulnerable; };

	BOOL IsOkToRespawn();
		
	VEHICLE_TYPE	*m_pVehicle;
	DWORD		m_dwGTAId;
	BOOL		m_bIsInvulnerable;
	DWORD		m_dwTimeSinceLastDriven;
	BOOL		m_bHasBeenDriven;
};

//-----------------------------------------------------------
