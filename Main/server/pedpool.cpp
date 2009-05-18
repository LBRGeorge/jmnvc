
/*
	TODO: Write code to sync the peds controls and position etc
*/

#include "netgame.h"

CPedPool::CPedPool()
{
	BYTE bytePedID = 0;
	while(bytePedID != MAX_PEDS) {
		m_bPedSlotState[bytePedID] = FALSE;
		m_pPeds[bytePedID] = NULL;
		bytePedID++;
	}
}

BOOL CPedPool::InitialiseFromConfig(CConfig *pConfig)
{
    int iPedArrayCount;
    BYTE d=1;

    int iModelID;
    VECTOR vecPos;
    float fRotation;
    int iColor1, iColor2;
    char * szConfigLine;

    iPedArrayCount = pConfig->GetConfigArrayCount("PED");
	iPedArrayCount++;

    while((int)d < iPedArrayCount) {

         szConfigLine = pConfig->GetConfigEntryAsString("PED",d);

		 sscanf(szConfigLine,"%d%f%f%f%f%d%d",&iModelID,
                   &vecPos.X,&vecPos.Y,&vecPos.Z,&fRotation,&iColor1,&iColor2);

         New(d,(BYTE)iModelID,&vecPos,fRotation,iColor1,iColor2);
         d++;
    }

    return TRUE;
}

CPedPool::~CPedPool()
{	
	BYTE bytePedID = 0;
	while(bytePedID != MAX_PEDS) {
		Delete(bytePedID);
		bytePedID++;
	}
}

BOOL CPedPool::New(BYTE bytePedID, BYTE bytePedType,
					   VECTOR * vecPos, float fRotation,
					   int iColor1, int iColor2)
{
	m_pPeds[bytePedID] = new CPed(bytePedType,vecPos,fRotation,iColor1,iColor2);
	if(m_pPeds[bytePedID])
	{
		m_pPeds[bytePedID]->SetID(bytePedID);
		m_bPedSlotState[bytePedID] = TRUE;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CPedPool::Delete(BYTE bytePedID)
{
	if(!GetSlotState(bytePedID) || !m_pPeds[bytePedID])
	{
		return FALSE;
	}

	m_bPedSlotState[bytePedID] = FALSE;
	delete m_pPeds[bytePedID];
	m_pPeds[bytePedID] = NULL;

	return TRUE;
}

void CPedPool::Process()
{
	
}
