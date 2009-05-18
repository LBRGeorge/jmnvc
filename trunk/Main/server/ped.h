#pragma once

typedef struct _PED_SPAWN_INFO
{
	BYTE bytePedType;
	VECTOR vecPos;
	float fRotation;
	int iColor1;
	int iColor2;
} PED_SPAWN_INFO;

class CPed
{
public:

	BYTE					m_bytePedID;
	BYTE					m_byteDriverID;
	BYTE					m_bytePassengers[7];
	BOOL					m_bIsActive;
	BOOL					m_bIsWasted;
	PED_SPAWN_INFO			m_SpawnInfo;
	MATRIX4X4				m_matWorld;
	VECTOR					m_vecMoveSpeed;
	VECTOR					m_vecTurnSpeed;
	float					m_fHealth;
	BOOL					m_bHasHadUpdate;

	CPed(BYTE byteModel,VECTOR *vecPos,float fRotation,int iColor1,int iColor2);
	~CPed(){};

	BOOL IsActive() { return m_bIsActive; };
	BOOL IsWasted() { return m_bIsWasted; };

	void SetID(BYTE bytePedID) { m_bytePedID = bytePedID; };
	PED_SPAWN_INFO * GetSpawnInfo() { return &m_SpawnInfo; };

	void SpawnForPlayer(BYTE byteForPlayerID);

	void Update(BYTE bytePlayerID, MATRIX4X4 * matWorld,
		VECTOR * vecMoveSpeed, float fHealth);
};