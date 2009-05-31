
#include "netgame.h"
#include <math.h>

extern CNetGame *pNetGame;

CPickup::CPickup(BYTE byteModel, VECTOR *vecPos)
{
	m_SpawnInfo.bytePickupModel = byteModel;
	m_SpawnInfo.vecPos.X = vecPos->X;
	m_SpawnInfo.vecPos.Y = vecPos->Y;
	m_SpawnInfo.vecPos.Z = vecPos->Z;
	
	m_bIsActive = TRUE;
	m_bPickedUp = FALSE;
}

void CPickup::SpawnForPlayer(BYTE byteForPlayerID)
{
	RakNet::BitStream bsPickupSpawn;
	bsPickupSpawn.Write(m_bytePickupID);
	bsPickupSpawn.Write(m_SpawnInfo.bytePickupModel);
	bsPickupSpawn.Write(m_SpawnInfo.vecPos.X);
	bsPickupSpawn.Write(m_SpawnInfo.vecPos.Y);
	bsPickupSpawn.Write(m_SpawnInfo.vecPos.Z);
	bsPickupSpawn.Write(m_bPickedUp);
	pNetGame->GetNetSends()->ClientCall(true, pNetGame->GetRakServer()->GetPlayerIDFromIndex(byteForPlayerID), "PickupSpawn", bsPickupSpawn);
}
