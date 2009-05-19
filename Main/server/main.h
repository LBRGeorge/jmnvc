#pragma once

#ifdef WIN32
#include <stdio.h>
#include <windows.h>
#else
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#endif

#include <math.h>
#include <stdarg.h>

#define MAX_PLAYER_NAME			24
#define MAX_PLAYERS			50
#define MAX_VEHICLES			200
#define MAX_PICKUPS			200
#define MAX_PEDS			200

#define _CRT_SECURE_NO_WARNINGS 1

#include "../lua/CLuaArgument.h"

#include <iostream>
#define  _us2ms(x) (x*1000)

#include "../raknet/RakServer.h"
#include "../raknet/RakNetworkFactory.h"
#include "../raknet/BitStream.h"

#include "system.h"
#include "config.h"

#include "../lua/CLuaArgument.h"
#include "../lua/CLuaArguments.h"

#define DEFAULT_CONFIG_FILE "mods\\server.ini"
#define DEFAULT_MAX_PLAYERS 32
#define DEFAULT_LISTEN_PORT 8192
#define DEFAULT_RCON_PORT	8190
#define DEFAULT_RCON_MAXUSERS	8

#if WIN32
	#define JVCMPEXPORT extern "C" __declspec(dllexport)
#else
	#define JVCMPEXPORT extern "C"
#endif

enum eEntity {PLAYER,PED,OBJECT,VEHICLE,PICKUP};
void logprintf(char * format, ...);
