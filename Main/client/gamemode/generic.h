#pragma once


class CGameModeGeneric
{
private:
	BOOL m_bClearedToSpawn;
	BYTE m_byteSelectedClass;
	DWORD m_dwLastSpawnSelectionTick;// delays left and right selection
	DWORD m_dwInitialSelectionTick;// delays initial selection

public:
	CGameModeGeneric();
	~CGameModeGeneric() {};

	void ProcessLocalPlayer(CLocalPlayer *pLocalPlayer);
	void HandleClassSelection(CLocalPlayer *pLocalPlayer);
	void HandleClassSelectionOutcome(PLAYER_SPAWN_INFO *pSpawnInfo, BOOL bOutcome);
};


//----------------------------------------------------------