#include "netgame.h"

CPickupPool::CPickupPool()
{
	BYTE bytePickupID = 0;
	while(bytePickupID != MAX_PICKUPS) {
		m_bPickupSlotState[bytePickupID] = FALSE;
		m_pPickups[bytePickupID] = NULL;
		bytePickupID++;
	}
}

BOOL CPickupPool::InitialiseFromConfig(CConfig *pConfig)
{
    int iPickupArrayCount;
    BYTE d=1;

    int iModelID;
    VECTOR vecPos;
    char * szConfigLine;

    iPickupArrayCount = pConfig->GetConfigArrayCount("PICKUP");
	iPickupArrayCount++;

    while((int)d < iPickupArrayCount) {

         szConfigLine = pConfig->GetConfigEntryAsString("PICKUP",d);

		 sscanf(szConfigLine,"%d%f%f%f%f%d%d",&iModelID,&vecPos.X,&vecPos.Y,&vecPos.Z);

         New(d,(BYTE)iModelID,&vecPos);
         d++;
    }

    return TRUE;
}

CPickupPool::~CPickupPool()
{	
	BYTE bytePickupID = 0;
	while(bytePickupID != MAX_PICKUPS) {
		Delete(bytePickupID);
		bytePickupID++;
	}
}

BOOL CPickupPool::New(BYTE bytePickupID, BYTE bytePickupType, VECTOR * vecPos)
{
	m_pPickups[bytePickupID] = new CPickup(bytePickupType,vecPos);
	if(m_pPickups[bytePickupID])
	{
		m_pPickups[bytePickupID]->SetID(bytePickupID);
		m_bPickupSlotState[bytePickupID] = TRUE;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CPickupPool::Delete(BYTE bytePickupID)
{
	if(!GetSlotState(bytePickupID) || !m_pPickups[bytePickupID])
	{
		return FALSE;
	}

	m_bPickupSlotState[bytePickupID] = FALSE;
	delete m_pPickups[bytePickupID];
	m_pPickups[bytePickupID] = NULL;

	return TRUE;
}

void CPickupPool::Process()
{
	
}
