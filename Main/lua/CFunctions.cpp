
#include "CFunctions.h"
#include "CLuaArguments.h"
//#include "../server/main.h"
#include "../server/netgame.h"

//RakServerInterface		*pRak=0;
extern CNetGame			*pNetGame;

int CFunctions::outputServerLog ( lua_State* luaVM )
{
    if ( luaVM )
    {
        CLuaArguments args;
		char* text = (char*) lua_tostring(luaVM,1);
		strcat(text,"\n");
		printf(text);
        args.PushBoolean(true);
        args.PushArguments(luaVM);
        return 1;
    }
    return 0;
}

int CFunctions::outputChatBox ( lua_State* luaVM )
{
    if ( luaVM )
    {
        CLuaArguments args;
		char* text = (char*) lua_tostring(luaVM,1);
		char* consoleText = (char*) lua_tostring(luaVM,1);
		strcat(consoleText,"\n");
		printf(consoleText);
        args.PushBoolean(true);
        args.PushArguments(luaVM);
        return 1;
    }
    return 0;
}

int CFunctions::spawnVehicle ( lua_State* luaVM )
{
    if ( luaVM )
    {
        CLuaArguments args;
		bool success = false;
		if (args.Count() >= 1){
			CVehicle* pVehicle = (CVehicle*) lua_touserdata(luaVM,1);
			if (pVehicle->IsDestroyed()){
				pVehicle->SpawnForEveryPlayer();
				success = true;
			}
		}
        args.PushBoolean(success);
        args.PushArguments(luaVM);
        return 1;
    }
    return 0;
}

int CFunctions::isVehicleBlownUp ( lua_State* luaVM )
{
    if ( luaVM )
    {
        CLuaArguments args;
		bool success = false;
		if (args.Count() >= 1){
			CVehicle* pVehicle = (CVehicle*) lua_touserdata(luaVM,1);
			if (pVehicle->IsWasted()){
				success = true;
			}
		}
        args.PushBoolean(success);
        args.PushArguments(luaVM);
        return 1;
    }
    return 0;
}

int CFunctions::despawnVehicle ( lua_State* luaVM )
{
    if ( luaVM )
    {
        CLuaArguments args;
		bool success = false;
		if (args.Count() >= 1){
			CVehicle* pVehicle = (CVehicle*) lua_touserdata(luaVM,1);
			if (!pVehicle->IsDestroyed()){
				pVehicle->Destroy();
				success = true;
			}
		}
        args.PushBoolean(success);
        args.PushArguments(luaVM);
        return 1;
    }
    return 0;
}

int CFunctions::createVehicle ( lua_State* luaVM )
{
    if ( luaVM )
    {
        CLuaArguments args;
		bool success = false;
		if (args.Count() >= 7){
			int model = (int) lua_tonumber(luaVM,1);
			float x = (float) lua_tonumber(luaVM,2);
			float y = (float) lua_tonumber(luaVM,3);
			float z = (float) lua_tonumber(luaVM,4);
			float rotZ = (float) lua_tonumber(luaVM,5);
			int colour1 = (int) lua_tonumber(luaVM,6);
			int colour2 = (int) lua_tonumber(luaVM,7);
			VECTOR *pos = new VECTOR();
			pos->X = x;
			pos->Y = y;
			pos->Z = z;
			CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
			success = pVehiclePool->New(pVehiclePool->GetTotal(), model, pos, rotZ, colour1, colour2);
			if (success){
				BYTE id = pVehiclePool->GetTotal();
				CVehicle *pVehicle = pVehiclePool->GetAt(id);
				//This code will return an element insted of a vehicle
				//Not finished yet
				//CElement* e = new CElement(1,(void*)pVehicle);
				//pVehicle->SpawnForEveryPlayer();
				args.PushUserData(pVehicle);
			}else{
		        args.PushBoolean(false);
			}
		}else{
	        args.PushBoolean(false);
		}
        args.PushArguments(luaVM);
        return 1;
    }
    return 0;
}

int CFunctions::destroyElement ( lua_State* luaVM )
{
    if ( luaVM )
    {
        CLuaArguments args;
		bool success = false;
		if (args.Count() >= 1){
			CVehicle* pVehicle = (CVehicle*) lua_touserdata(luaVM,1);
			if (!pVehicle->IsDestroyed()){ //Check its not destroyed
				pVehicle->Destroy(); //This tells all the clients to destroy the vehicle
				delete pVehicle; //This destroys the vehicle class
				success = true; //Make sure the user knows it was destroyed
			}
		}
		args.PushBoolean(success);
        args.PushArguments(luaVM);
        return 1;
    }
    return 0;
}
