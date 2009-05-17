
class CFunctions;

#ifndef __CFUNCTIONS_H
#define __CFUNCTIONS_H

#include <stdio.h>

#include "ILuaModuleManager.h"
extern ILuaModuleManager10 *pModuleManager;

class CFunctions
{
public:

		static int		outputServerLog			( lua_State* luaVM );
		static int		outputChatBox			( lua_State* luaVM );
		static int		createVehicle			( lua_State* luaVM );
		static int		spawnVehicle			( lua_State* luaVM );
		static int		despawnVehicle			( lua_State* luaVM );
		static int		isVehicleBlownUp		( lua_State* luaVM );
		static int		destroyElement			( lua_State* luaVM );

};
#endif
