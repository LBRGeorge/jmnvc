#pragma once

#define UPDATE_TYPE_NONE		0
#define UPDATE_TYPE_FULL_ONFOOT	1
#define UPDATE_TYPE_MINI_ONFOOT 2
#define UPDATE_TYPE_FULL_INCAR	3
#define UPDATE_TYPE_MINI_INCAR  4
#define UPDATE_TYPE_PASSENGER	5

#define VALID_KILL		  1
#define TEAM_KILL		  2
#define SELF_KILL		  3

#define NO_TEAM			  255

//----------------------------------------------------

class CRemotePlayer
{
private:
	CPlayerPed			*m_pPlayerPed;

	BYTE				m_bytePlayerID;
	BYTE				m_byteUpdateFromNetwork;
	BOOL				m_bIsActive;
	BOOL				m_bIsWasted;
	BYTE				m_byteTeam;

	// Information that is synced.
	WORD				m_wKeys;
	MATRIX4X4			m_matWorld;

	VECTOR				m_vecMoveSpeed;
	float				m_fRotation;
	float				m_fVehicleHealth;
	BYTE				m_byteHealth; // health float casted to a byte.
	BYTE				m_byteCurrentWeapon;

	BOOL				m_bIsInVehicle;
	BOOL				m_bIsAPassenger;
	BYTE				m_byteVehicleID;
	UINT				m_uiPassengerSeat;
	BYTE				m_byteAction;

	CHAR				m_szPlayerName[256]; // Note to self: use a fucking constant
											 // for the size - dickhead.

	BYTE				m_iJustSpawned; // Indicates that we shouldn't process
										// this player straight away.

	CAMERA_AIM			m_Aim;

public:
	CRemotePlayer();
	~CRemotePlayer();

	BOOL IsActive() { return m_bIsActive; };
	BOOL IsWasted() { return m_bIsWasted; };

	void Process();
	void HandleVehicleEntryExit();
	void Say(PCHAR szText);

	void SetID(BYTE bytePlayerID) { m_bytePlayerID = bytePlayerID; };
	BYTE GetID() { return m_bytePlayerID; };
	BYTE GetTeam() { return m_byteTeam; };	

	CPlayerPed * GetPlayerPed() { return m_pPlayerPed; };

	void SetReportedHealth(BYTE byteReportHealth) { 
		m_byteHealth = byteReportHealth;
	}

	float GetReportedHealth() { return (float)m_byteHealth; };

	void UpdateOnFootMatrix(MATRIX4X4 * matWorld);

	void UpdateInCarMatrixAndSpeed(MATRIX4X4 * matWorld,
								   VECTOR * vecMoveSpeed);

	void StoreOnFootFullSyncData(WORD wKeys, MATRIX4X4 * matWorld,
								 float fRotation, BYTE byteCurrentWeapon, BYTE byteAction);

	void StoreAimSyncData(CAMERA_AIM * pAim) { 
		memcpy(&m_Aim,pAim,sizeof(CAMERA_AIM)); 
	};
	
	void StoreInCarFullSyncData(BYTE byteVehicleID,WORD wKeys,MATRIX4X4 * matWorld,
		VECTOR *vecMoveSpeed, float fVehicleHealth);

	void StorePassengerData(BYTE byteVehicleID, UINT uiSeat);

	BOOL DestroyPlayer();

	BOOL SpawnPlayer( BYTE byteTeam, BYTE byteSkin, 
					  VECTOR * vecPos, float fRotation, int iSpawnWeapon1,
					  int iSpawnWeapon1Ammo, int iSpawnWeapon2, 
					  int iSpawnWeapon2Ammo, int iSpawnWeapon3,
					  int iSpawnWeapon3Ammo );

	void HandleDeath(BYTE byteReason, BYTE byteWhoKilled, BYTE byteScoringModifier);

	float GetDistanceFromRemotePlayer(CRemotePlayer *pFromPlayer);
	float GetDistanceFromLocalPlayer();

	DWORD GetTeamColorAsRGBA();
	DWORD GetTeamColorAsARGB();
};

//----------------------------------------------------
