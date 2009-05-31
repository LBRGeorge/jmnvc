#include "net/netgame.h"

using namespace RakNet;
extern CNetGame* pNetGame;

void CNetSends::ServerCall( BOOL bSequenced, char* szCallName, RakNet::BitStream bsSend )
{
	if (bSequenced)
	{
		pNetGame->GetRakClient()->RPC(szCallName,&bsSend,HIGH_PRIORITY,RELIABLE_SEQUENCED,0,FALSE);
	}else{
		pNetGame->GetRakClient()->RPC(szCallName,&bsSend,HIGH_PRIORITY,RELIABLE,0,FALSE);
	}
}

void CNetSends::Death( BYTE byteDeathReason, BYTE byteWhoWasResponsible )
{
	RakNet::BitStream bsPlayerDeath;
	bsPlayerDeath.Write(byteDeathReason);
	bsPlayerDeath.Write(byteWhoWasResponsible);
	ServerCall(true, "Death",bsPlayerDeath);
}

void CNetSends::RequestClass( BYTE byteClass )
{
	RakNet::BitStream bsSpawnRequest;
	bsSpawnRequest.Write(byteClass);
	ServerCall(false, "RequestClass",bsSpawnRequest);
}

void CNetSends::Spawn( void )
{
	RakNet::BitStream bsSendSpawn;
	ServerCall(true, "Spawn",bsSendSpawn);
}

void CNetSends::Chat( PCHAR szText )
{
	BYTE byteTextLen = strlen(szText);
	RakNet::BitStream bsSend;
	bsSend.Write(byteTextLen);
	bsSend.Write(szText,byteTextLen);
	ServerCall(false, "Chat",bsSend);
}

void CNetSends::EnterVehicle( BYTE byteVehicleID, BYTE bytePassenger )
{
	RakNet::BitStream bsSend;
	bsSend.Write(byteVehicleID);
	bsSend.Write(bytePassenger);
	ServerCall(true, "EnterVehicle",bsSend);
}

void CNetSends::ExitVehicle( BYTE byteVehicleID )
{
	RakNet::BitStream bsSend;
	bsSend.Write(byteVehicleID);
	ServerCall(true, "ExitVehicle",bsSend);
}

void CNetSends::KickPlayer( BYTE bytePlayerID )
{
	RakNet::BitStream bsSend;
	bsSend.Write(bytePlayerID);
	ServerCall(false, "KickPlayer",bsSend);
}

void CNetSends::BanIP( PCHAR szIP )
{
	int iIPLen=0;
	RakNet::BitStream bsSend;
	iIPLen = strlen(szIP);
	if(iIPLen > 64) return;
	bsSend.Write(iIPLen);
	bsSend.Write(szIP,iIPLen);
	ServerCall(false, "BanIPAddress",bsSend);
}

void CNetSends::AdminLogin( PCHAR szPassword )
{
	int iPassLen=0;
	RakNet::BitStream bsSend;
	iPassLen = strlen(szPassword);
	if(iPassLen > 64) return;
	bsSend.Write(iPassLen);
	bsSend.Write(szPassword,iPassLen);
	ServerCall(false, "Admin",bsSend);
}
