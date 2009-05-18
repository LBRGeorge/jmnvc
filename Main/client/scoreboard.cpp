#include "main.h"
#include <stdio.h>

extern CNetGame* pNetGame;
extern GAME_SETTINGS tSettings;
extern IDirect3DDevice8 *pD3DDevice;
D3DDISPLAYMODE DisplayMode;
extern CChatWindow * pChatWindow;


void CScoreBoard::Draw()
{
	CPlayerPool* pPlayerPool = pNetGame->GetPlayerPool();
	pNetGame->UpdatePlayerScoresAndPings();	

	char szScoreBuf[8192] = {0};
	char szScoreWrite[1024] = {0};
	char szColorCodedNick[256] = {0};
	DWORD dwNickColor;
	DWORD dwChatColor = 0xFFFFFFFF;
	char * t;
	char * szNick;

	strcpy_s(szScoreWrite,"Jack's VC Multiplayer");
	//sprintf(szScoreWrite,"VC-MP dev-shot release 0.1 - %s:%s\n\n",
	//	    tSettings.szConnectHost,tSettings.szConnectPort);
	strcat_s(szScoreBuf,szScoreWrite);

	sprintf_s(szScoreWrite,"Player\t\t\tScore\t\tPing\n\n");
	strcat_s(szScoreBuf,szScoreWrite);

	// write local player nick with color
	t = szColorCodedNick;
	
	dwNickColor = pPlayerPool->GetLocalPlayer()->GetTeamColorAsARGB();
	szNick = pPlayerPool->GetLocalPlayerName();

	*t = '\01'; t++; // color start
	memcpy(t,&dwNickColor,sizeof(DWORD)); t+=4; // nick color
	memcpy(t,szNick,strlen(szNick)); t+=strlen(szNick); // nick string
	*t = '\01'; t++; // color start
	memcpy(t,&dwChatColor,sizeof(DWORD)); t+=4; // chat color
	*t = '\0';
	
	sprintf_s(szScoreWrite,"%s\t\t\t%d\t\t%d\n",
		szColorCodedNick,pPlayerPool->GetLocalPlayerScore(),
		pPlayerPool->GetLocalPlayerPing());
	strcat_s(szScoreBuf,szScoreWrite);

	int x=0;

	while(x!=MAX_PLAYERS) {
		if(pPlayerPool->GetSlotState(x) == TRUE) {

				// write local player nick with color
				t = szColorCodedNick;
	
				dwNickColor = pPlayerPool->GetAt(x)->GetTeamColorAsARGB();
				szNick = pPlayerPool->GetPlayerName(x);

				*t = '\01'; t++; // color start
				memcpy(t,&dwNickColor,sizeof(DWORD)); t+=4; // nick color
				memcpy(t,szNick,strlen(szNick)); t+=strlen(szNick); // nick string
				*t = '\01'; t++; // color start
				memcpy(t,&dwChatColor,sizeof(DWORD)); t+=4; // chat color
				*t = '\0';

				sprintf_s(szScoreWrite,"%u: %s\t\t\t%d\t\t%d\n",
						x,szColorCodedNick,pPlayerPool->GetScore(x),
						pPlayerPool->GetPing(x));

				strcat_s(szScoreBuf,szScoreWrite);			
		}
		x++;
	}

	pD3DDevice->GetDisplayMode(&DisplayMode);
	pChatWindow->m_pD3DFont->DrawTextBox((float)((DisplayMode.Width / 2) - 100),50.0f,50.0f,0x50000000,0xFFFFFFFF,szScoreBuf,0L);
}
