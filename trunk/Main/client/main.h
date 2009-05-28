#pragma once

#include <stdio.h>
#include <windows.h>

#define MAX_PLAYER_NAME		24
#define MAX_NETWORK_PLAYERS	50
#define MAX_SETTINGS_STRING 128
#define PI 3.14159265

typedef struct _GAME_SETTINGS {
	BOOL bDisableNewspapers;
	BOOL bDebug;
	BOOL bPlayOnline;
	CHAR szConnectHost[MAX_SETTINGS_STRING+1];
	CHAR szConnectPort[MAX_SETTINGS_STRING+1];
	CHAR szConnectPass[MAX_SETTINGS_STRING+1];
	CHAR szNickName[MAX_SETTINGS_STRING+1];
} GAME_SETTINGS;

#include "d3d8/include/d3d8.h"
#include "d3d8/include/d3dfont.h"
#include "d3d8/include/d3dx8tex.h"
#include "d3d8/include/DXUtil.h"

#include "game/game.h"

#include "d3dhook/IDirect3D8Hook.h"
#include "d3dhook/IDirect3DDevice8Hook.h"

#include "../raknet/RakClient.h"
#include "../raknet/RakNetworkFactory.h"
#include "../raknet/BitStream.h"
#include "../raknet/PacketEnumerations.h"

#include "net/localplayer.h"
#include "net/remoteplayer.h"
#include "gamemode/generic.h"
#include "net/netrpc.h"
#include "net/playerpool.h"
#include "net/vehiclepool.h"
//#include "net/pickuppool.h"
#include "net/netgame.h"

#include "chatwindow.h"
#include "cmdwindow.h"
#include "bbfont.h"
#include "scoreboard.h"
#include "netstats.h"
#include "anticheat.h"

#include "game/scripting.h"

//----------------------------------------------------

void SetStringFromCommandLine(char *szCmdLine, char *szString);
void InitSettings();

//----------------------------------------------------
// EOF

