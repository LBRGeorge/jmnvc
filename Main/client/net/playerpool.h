#pragma once

#define INVALID_PLAYER_ID 255

//----------------------------------------------------

class CPlayerPool
{
private:
	
	BOOL m_bPlayerSlotState[MAX_PLAYERS];
	CLocalPlayer  *m_pLocalPlayer;
	BYTE	m_byteLocalPlayerID;
	CRemotePlayer *m_pPlayers[MAX_PLAYERS];
	CHAR	m_szLocalPlayerName[MAX_PLAYER_NAME+1];
	CHAR	m_szPlayerNames[MAX_PLAYERS][MAX_PLAYER_NAME+1];
	ULONG	m_ulIPAddress[MAX_PLAYERS]; // used by admin
	
	int		m_iLocalPlayerScore;
	int		m_iLocalPlayerPing;
	int		m_iScore[MAX_PLAYERS];
	int		m_iPing[MAX_PLAYERS];

public:
	BOOL Process();

	void SetLocalPlayerName(PCHAR szName) { strcpy_s(m_szLocalPlayerName,szName); };
	PCHAR GetLocalPlayerName() { return m_szLocalPlayerName; };
	PCHAR GetPlayerName(BYTE bytePlayerID) { return m_szPlayerNames[bytePlayerID]; };

	CLocalPlayer * GetLocalPlayer() { return m_pLocalPlayer; };
	BYTE FindRemotePlayerIDFromGtaPtr(PED_TYPE * pActor);

	BOOL New(BYTE bytePlayerID, PCHAR szPlayerName);
	BOOL Delete(BYTE bytePlayerID, BYTE byteReason);

	CRemotePlayer* GetAt(BYTE bytePlayerID) {
		if(bytePlayerID > MAX_PLAYERS) { return NULL; }
		return m_pPlayers[bytePlayerID];
	};

	BOOL GetSlotState(BYTE bytePlayerID) {
		if(bytePlayerID > MAX_PLAYERS) { return FALSE; }
		return m_bPlayerSlotState[bytePlayerID];
	};
	
	void SetLocalPlayerID(BYTE byteID) {
		strcpy_s(m_szPlayerNames[byteID],m_szLocalPlayerName);
		m_byteLocalPlayerID = byteID;
	};

	void UpdatePing(BYTE bytePlayerID,int iPing) { 
		if(bytePlayerID == m_byteLocalPlayerID) {
			m_iLocalPlayerPing = iPing;
		} else {
			m_iPing[bytePlayerID] = iPing;
		}
	};

	void UpdateScore(BYTE bytePlayerID,int iScore) { 
		if(bytePlayerID == m_byteLocalPlayerID) {
			m_iLocalPlayerScore = iScore;
		} else {
			m_iScore[bytePlayerID] = iScore;
		}
	};

	void UpdateIPAddress(BYTE bytePlayerID, ULONG ip) {
		m_ulIPAddress[bytePlayerID] = ip;
	}

	int GetPing(BYTE bytePlayerID) { return m_iPing[bytePlayerID]; };
	int GetScore(BYTE bytePlayerID) { return m_iScore[bytePlayerID]; };
	int GetLocalPlayerPing() { return m_iLocalPlayerPing; };
	int GetLocalPlayerScore() { return m_iLocalPlayerScore; };
	BYTE GetLocalPlayerID() { return m_byteLocalPlayerID; };
	ULONG GetIPAddress(BYTE bytePlayerID) { return m_ulIPAddress[bytePlayerID]; };

	CPlayerPool();
	~CPlayerPool();
};

//----------------------------------------------------