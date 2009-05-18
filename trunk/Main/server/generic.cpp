#include "netgame.h"
#include "player.h"

extern CNetGame *pNetGame;
extern CConfig *pServerConfig;

int CGameModeGeneric::Init()
{
	// Load up all the spawns.
	int iSpawnConfigCount = pServerConfig->GetConfigArrayCount("CLASS");
	int x=1;
	char * szSpawnConf;

	while(x<=iSpawnConfigCount) {
		szSpawnConf = pServerConfig->GetConfigEntryAsString("CLASS",x);
		if(szSpawnConf != NULL) {
			sscanf(szSpawnConf,"%d%u%f%f%f%f%d%d%d%d%d%d",
				&m_AvailableSpawns[x - 1].byteTeam,
				&m_AvailableSpawns[x - 1].byteSkin,
				&m_AvailableSpawns[x - 1].vecPos.X,
				&m_AvailableSpawns[x - 1].vecPos.Y,
				&m_AvailableSpawns[x - 1].vecPos.Z,
				&m_AvailableSpawns[x - 1].fRotation,
				&m_AvailableSpawns[x - 1].iSpawnWeapons[0],
				&m_AvailableSpawns[x - 1].iSpawnWeaponsAmmo[0],
				&m_AvailableSpawns[x - 1].iSpawnWeapons[1],
				&m_AvailableSpawns[x - 1].iSpawnWeaponsAmmo[1],
				&m_AvailableSpawns[x - 1].iSpawnWeapons[2],
				&m_AvailableSpawns[x - 1].iSpawnWeaponsAmmo[2]);
		}
		x++;
	}

	return iSpawnConfigCount;
}

void CGameModeGeneric::HandleClientJoin(BYTE bytePlayerID) 
{
	

}

BOOL CGameModeGeneric::HandleSpawnClassRequest(BYTE bytePlayerID,
											   int iSpawnType)
{
	CPlayer *pPlayer;
	pPlayer = pNetGame->GetPlayerPool()->GetAt(bytePlayerID);

	if(pPlayer && (iSpawnType <= (pNetGame->m_iSpawnsAvailable - 1))) {

		pPlayer->SetSpawnInfo(
			m_AvailableSpawns[iSpawnType].byteTeam,
			m_AvailableSpawns[iSpawnType].byteSkin,
			&m_AvailableSpawns[iSpawnType].vecPos,
			m_AvailableSpawns[iSpawnType].fRotation,
			m_AvailableSpawns[iSpawnType].iSpawnWeapons[0],
			m_AvailableSpawns[iSpawnType].iSpawnWeaponsAmmo[0],
			m_AvailableSpawns[iSpawnType].iSpawnWeapons[1],
			m_AvailableSpawns[iSpawnType].iSpawnWeaponsAmmo[1],
			m_AvailableSpawns[iSpawnType].iSpawnWeapons[2],
			m_AvailableSpawns[iSpawnType].iSpawnWeaponsAmmo[2]
			);

		return TRUE;
	}
 
	return FALSE;
}
