#pragma once

typedef struct _PICKUP_SPAWN_INFO
{
	BYTE bytePickupModel;
	VECTOR vecPos;
} PICKUP_SPAWN_INFO;

class CPickup
{
public:

	BYTE					m_bytePickupID;
	BOOL					m_bIsActive;
	BOOL					m_bPickedUp;
	PICKUP_SPAWN_INFO		m_SpawnInfo;

	CPickup(BYTE byteModel,VECTOR *vecPos);
	~CPickup(){};

	BOOL IsActive() { return m_bIsActive; };
	BOOL IsPickedUp() { return m_bPickedUp; };

	void SetID(BYTE bytePickupID) { m_bytePickupID = bytePickupID; };
	PICKUP_SPAWN_INFO * GetSpawnInfo() { return &m_SpawnInfo; };

	void SpawnForPlayer(BYTE byteForPlayerID);
};