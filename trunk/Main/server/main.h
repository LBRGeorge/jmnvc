#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

#define MAX_PLAYER_NAME			24
#define MAX_PLAYERS			50
#define MAX_VEHICLES			200
#define MAX_PICKUPS			200
#define MAX_PEDS			200

#define _CRT_SECURE_NO_WARNINGS

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

enum eEntity {PLAYER,PED,OBJECT,VEHICLE,PICKUP};
void logprintf(char * format, ...);
