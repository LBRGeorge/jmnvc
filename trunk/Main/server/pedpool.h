
#pragma once

class CPedPool
{
private:
	
	BOOL m_bPedSlotState[MAX_PEDS];
	CPed *m_pPeds[MAX_PEDS];

public:
	
	CPedPool();
	~CPedPool();

	BOOL New(BYTE bytePedID, BYTE bytePedType, VECTOR * vecPos, float fRotation, int iColor1, int iColor2);
	BOOL Delete(BYTE bytePedID);	
		
	// Retrieve a Ped by id
	CPed* GetAt(BYTE bytePedID) {
		if(bytePedID > MAX_PEDS) { return NULL; }
		return m_pPeds[bytePedID];
	};

	// Find out if the slot is inuse.
	BOOL GetSlotState(BYTE bytePedID) {
		if(bytePedID > MAX_PEDS) { return FALSE; }
		return m_bPedSlotState[bytePedID];
	};

	BOOL InitialiseFromConfig(CConfig *pConfig);
	void Process();
};
