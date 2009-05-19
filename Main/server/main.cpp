
#include <winsock.h>
#include "main.h"
#include "netgame.h"
#include "rcon.h"

lua_State *l;
ILuaModuleManager10 *pModuleManager = NULL;

#include "../raknet/BitStream.h"
#include "../raknet/RakPeerInterface.h"
#include "../raknet/RakNetworkFactory.h"

void fatal_exit(char * szError);

int iMaxPlayers=0;
CNetGame	*pNetGame;
CRcon		*pRcon;
CConfig		*pServerConfig;
int			iLogState=1;
char		*szAdminPass;
BOOL		bShutdown=false;

void report_errors(lua_State *L, int status)
{
	if ( status!=0 ) {
		std::cerr << "-- " << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1); // remove error message
	}
}

void RegisterFunction(const char *szFunctionName, lua_CFunction pFunc)
{
	lua_register(l, szFunctionName, pFunc);
}

//int main (int argc, char* argv[])
JVCMPEXPORT int Run ( int iArgumentCount, char* szArguments [] )
{
	CreateDirectory("dumps",0);
	CreateDirectory("mods",0);

	l = lua_open();
	luaopen_base(l);
	luaopen_table(l);
	luaopen_string(l);
	luaopen_math(l);

	RegisterFunction("outputServerLog", CFunctions::outputServerLog);
	RegisterFunction("outputChatBox", CFunctions::outputChatBox);

	RegisterFunction("destroyElement", CFunctions::destroyElement);

	RegisterFunction("createVehicle", CFunctions::createVehicle);

	int iListenPort=0;
	char *szPass=NULL;
	int iCurrentGame=1;
	char szConfigFile[512];
	char szError[256];

	int iFriendlyFireOption;
	BYTE byteFriendlyFire=0;
	int iShowOnRadarOption;
	int iStartInterior=0;
	BYTE byteShowOnRadarOption=1;

	int iRconPort = 0;
	int iRconMaxUsers = 0;

	pServerConfig = new CConfig();

	if(iArgumentCount > 1) {
		strcpy(szConfigFile,szArguments[1]);
	} else {
		strcpy(szConfigFile,DEFAULT_CONFIG_FILE);
	}

	// try and read the config file.
	if(pServerConfig->ReadFile(szConfigFile) != CCONF_ERR_SUCCESS) {
		sprintf(szError,"Can't read the config file %s\n%s\n-- Server shutting down!",
			DEFAULT_CONFIG_FILE,pServerConfig->m_szErrorString);
		fatal_exit(szError);	
	}

	logprintf("Using config file: %s",szConfigFile);

	// get the max players setting
	if((iMaxPlayers=pServerConfig->GetConfigEntryAsInt("MaxPlayers"))==(-1)) {
		iMaxPlayers = DEFAULT_MAX_PLAYERS;
	}

	if(iMaxPlayers > 48) { iMaxPlayers = 48; };

	// get the listen port setting
	if((iListenPort=pServerConfig->GetConfigEntryAsInt("ListenPort"))==(-1)) {
		iListenPort = DEFAULT_LISTEN_PORT;
	}
	if((iStartInterior=pServerConfig->GetConfigEntryAsInt("CamInterior"))==(-1)) {
		iStartInterior = 0;
	}
	

	// set the server pass if one is required.
	if(pServerConfig->GetConfigEntryAsBool("NeedPassword")==1) {
		szPass=pServerConfig->GetConfigEntryAsString("Password");
	}

	// get the admin pass
	szAdminPass = pServerConfig->GetConfigEntryAsString("AdminPassword");
	if(!szAdminPass || !strlen(szAdminPass) || !strcmp(szAdminPass,"invalid")) {
		fatal_exit("I need an AdminPassword in the .ini before I can start the server.\n");
	}

	// get the ff option from config (default is OFF)
	iFriendlyFireOption = pServerConfig->GetConfigEntryAsBool("FriendlyFire");
	if(iFriendlyFireOption == 1) byteFriendlyFire = 1;

	// get the show on radar option (default is ON)
	iShowOnRadarOption = pServerConfig->GetConfigEntryAsBool("ShowOnRadar");
	if(iShowOnRadarOption == -1 || iShowOnRadarOption == 0)	{
		byteShowOnRadarOption = 0;
	}
	
	// create the NetGame.
	pNetGame = new CNetGame(iMaxPlayers,iListenPort,0,szPass,0,byteFriendlyFire,byteShowOnRadarOption,iStartInterior);

	int s = luaL_loadfile(l, "mods\\server.lua");
	if ( s==0 ) {
		s = lua_pcall(l, 0, LUA_MULTRET, 0);
	}
	report_errors(l, s);
	printf("-- JMN Vice City Multiplayer Server Started. Port: %d Max players: %d\n",iListenPort,iMaxPlayers);
	printf("-- Make sure Port: %d can be accessed from the internet UDP and TCP.\n",iListenPort,iMaxPlayers);
	printf("-- Revision 2.\n");
	//printf("This server may crash if there is any errors so you may develop software\n",iListenPort,iMaxPlayers);
	//printf("To detect this program crashing then restart it (This program terminates in the case of an error)\n",iListenPort,iMaxPlayers);
	//printf("NOTE: In this version there is only server side coding.\n",iListenPort,iMaxPlayers);

	// Get the remote console port.
	if (iRconPort = pServerConfig->GetConfigEntryAsInt("RconPort") == -1) {
		iRconPort = DEFAULT_RCON_PORT;
	} else {
		iRconPort = pServerConfig->GetConfigEntryAsInt("RconPort");
	}

	// Get the remote console max users.
	if (iRconMaxUsers = pServerConfig->GetConfigEntryAsInt("RconMaxUsers") <= 0) {
		iRconMaxUsers = DEFAULT_RCON_MAXUSERS;
	} else {
		iRconMaxUsers = pServerConfig->GetConfigEntryAsInt("RconMaxUsers");
	}

	// create rcon
	pRcon = new CRcon(iRconPort, szAdminPass, iRconMaxUsers);

	// Process the network game.
	while(pNetGame->GetGameState() == GAMESTATE_RUNNING)
	{
		pNetGame->Process();
		pRcon->Process();

#ifdef WIN32
		Sleep(5);
#else
		usleep(_us2ms(5));
#endif
		if(bShutdown==TRUE){
			goto ExitApp;
		}
	}
ExitApp:
	lua_close(l);
	delete pRcon;
	delete pNetGame;
	return 0;
}

//----------------------------------------------------

void fatal_exit(char * szError)
{
	#ifdef WIN32
		printf("%s\n\n",szError);
		printf("Press enter to close.");
		getc(stdin);
	#endif
		exit(1);
}

//----------------------------------------------------

void logprintf(char * format, ...)
{
	/*
	char tmp_buf[512];
	if(iLogState) {
		va_list args;
		va_start(args, format);
		vsprintf(tmp_buf, format, args);
		va_end(args);
		puts(tmp_buf);
	}*/
}

//----------------------------------------------------

void FilterInvalidNickChars(PCHAR szString)
{
	while(*szString) {
		if(*szString < ' ' || *szString > 'z') {
			*szString = '_';
		}
		szString++;
	}
}

//----------------------------------------------------

