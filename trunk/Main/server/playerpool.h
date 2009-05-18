#pragma once

#define INVALID_PLAYER_ID 255

#define VALID_KILL		  1
#define TEAM_KILL		  2
#define SELF_KILL		  3

class CPlayerPool
{
private:
	
	BOOL	m_bPlayerSlotState[MAX_PLAYERS];
	CPlayer *m_pPlayers[MAX_PLAYERS];
	CHAR	m_szPlayerName[MAX_PLAYERS][MAX_PLAYER_NAME+1];
	int		m_iPlayerScore[MAX_PLAYERS];
	BOOL	m_bIsAnAdmin[MAX_PLAYERS];
public:
	
	CPlayerPool();
	~CPlayerPool();

	void Process();

	BOOL New(BYTE bytePlayerID, PCHAR szPlayerName);
	BOOL Delete(BYTE bytePlayerID, BYTE byteReason);
		
	// Retrieve a player
	CPlayer* GetAt(BYTE bytePlayerID) {
		if(bytePlayerID > MAX_PLAYERS) { return NULL; }
		return m_pPlayers[bytePlayerID];
	};

	// Find out if the slot is inuse.
	BOOL GetSlotState(BYTE bytePlayerID) {
		if(bytePlayerID > MAX_PLAYERS) { return FALSE; }
		return m_bPlayerSlotState[bytePlayerID];
	};

	PCHAR GetPlayerName(BYTE bytePlayerID) { return m_szPlayerName[bytePlayerID]; };

	BYTE AddResponsibleDeath(BYTE byteWhoKilled, BYTE byteWhoDied);
	float GetDistanceFromPlayerToPlayer(BYTE bytePlayer1, BYTE bytePlayer2);

	void SetAdmin(BYTE bytePlayerID) { m_bIsAnAdmin[bytePlayerID] = TRUE; };
	BOOL IsAdmin(BYTE bytePlayerID) { return m_bIsAnAdmin[bytePlayerID]; };

	int GetScore(BYTE bytePlayerID) { return m_iPlayerScore[bytePlayerID]; };

	BOOL IsNickInUse(PCHAR szNick);

	int GetPlayerCount() {
		BYTE x=0,count=0;
		while(x!=MAX_PLAYERS) {
			if(GetSlotState(x)) count++;
			x++;
		}
		return (int)count;
	};
};
