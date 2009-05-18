#pragma once

typedef struct _PICKUP_SPAWN_INFO
{
	BYTE bytePickupType;
	VECTOR vecPos;
	float fRotation;
	int iColor1;
	int iColor2;
} PICKUP_SPAWN_INFO;

class CPickup
{
public:

	BYTE					m_bytePickupID;
	BYTE					m_byteDriverID;
	BYTE					m_bytePassengers[7];
	BOOL					m_bIsActive;
	BOOL					m_bIsWasted;
	PICKUP_SPAWN_INFO			m_SpawnInfo;
	MATRIX4X4				m_matWorld;
	VECTOR					m_vecMoveSpeed;
	VECTOR					m_vecTurnSpeed;
	float					m_fHealth;
	BOOL					m_bHasHadUpdate;

	CPickup(BYTE byteModel,VECTOR *vecPos,float fRotation,int iColor1,int iColor2);
	~CPickup(){};

	BOOL IsActive() { return m_bIsActive; };
	BOOL IsWasted() { return m_bIsWasted; };

	void SetID(BYTE bytePickupID) { m_bytePickupID = bytePickupID; };
	PICKUP_SPAWN_INFO * GetSpawnInfo() { return &m_SpawnInfo; };

	void SpawnForPlayer(BYTE byteForPlayerID);

	void Update(BYTE bytePlayerID, MATRIX4X4 * matWorld,
		VECTOR * vecMoveSpeed, float fHealth);
};