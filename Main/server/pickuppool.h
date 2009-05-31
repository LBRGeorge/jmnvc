
#pragma once

class CPickupPool
{
private:
	
	BOOL m_bPickupSlotState[MAX_PICKUPS];
	CPickup *m_pPickups[MAX_PICKUPS];

public:
	
	CPickupPool();
	~CPickupPool();

	BOOL New(BYTE bytePickupID, BYTE bytePickupType, VECTOR * vecPos);
	BOOL Delete(BYTE bytePickupID);	
		
	// Retrieve a Pickup by id
	CPickup* GetAt(BYTE bytePickupID) {
		if(bytePickupID > MAX_PICKUPS) { return NULL; }
		return m_pPickups[bytePickupID];
	};

	// Find out if the slot is inuse.
	BOOL GetSlotState(BYTE bytePickupID) {
		if(bytePickupID > MAX_PICKUPS) { return FALSE; }
		return m_bPickupSlotState[bytePickupID];
	};

	BOOL InitialiseFromConfig(CConfig *pConfig);
	void Process();
};
