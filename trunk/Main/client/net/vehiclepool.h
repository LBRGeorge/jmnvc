#pragma once

typedef struct _VEHICLE_SPAWN_INFO
{
	BYTE byteVehicleType;
	VECTOR vecPos;
	float fRotation;
	int iColor1;
	int iColor2;
} VEHICLE_SPAWN_INFO;

//----------------------------------------------------

class CVehiclePool
{
private:
	
	BOOL				m_bVehicleSlotState[MAX_VEHICLES];
	CVehicle			*m_pVehicles[MAX_VEHICLES];
	VEHICLE_TYPE		*m_pGTAVehicles[MAX_VEHICLES];
	VEHICLE_SPAWN_INFO	m_SpawnInfo[MAX_VEHICLES];
	BOOL				m_bIsActive[MAX_VEHICLES];
	BOOL				m_bIsWasted[MAX_VEHICLES];
	int					m_iRespawnDelay[MAX_VEHICLES];

public:
	
	CVehiclePool();
	~CVehiclePool();

	BOOL New(BYTE byteVehicleID, BYTE byteVehicleType,
			 VECTOR * vecPos, float fRotation,
			 int iColor1, int iColor2,
			 VECTOR * vecSpawnPos, float fSpawnRotation);

	BOOL Delete(BYTE byteVehicleID);	

	CVehicle* GetAt(BYTE byteVehicleID) {
		if(byteVehicleID > MAX_VEHICLES) { return NULL; }
		return m_pVehicles[byteVehicleID];
	};

	BOOL GetSlotState(BYTE byteVehicleID) {
		if(byteVehicleID > MAX_VEHICLES) { return FALSE; }
		return m_bVehicleSlotState[byteVehicleID];
	};

	BOOL Spawn( BYTE byteVehicleID, BYTE byteVehicleType,
				VECTOR * vecPos, float fRotation,
				int iColor1, int iColor2 );

	int FindIDFromGtaPtr(VEHICLE_TYPE * pGtaVehicle);
	int FindGtaIDFromID(int iID);
	void SetForRespawn(BYTE byteVehicleID);

	void Process();
};

//----------------------------------------------------