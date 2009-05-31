#include "net/netgame.h"

using namespace RakNet;
extern CNetGame* pNetGame;

void CNetSends::Death( BYTE byteDeathReason, BYTE byteWhoWasResponsible )
{
	RakNet::BitStream bsPlayerDeath;
	bsPlayerDeath.Write(byteDeathReason);
	bsPlayerDeath.Write(byteWhoWasResponsible);
	pNetGame->GetRakClient()->RPC("Death",&bsPlayerDeath,HIGH_PRIORITY,RELIABLE_SEQUENCED,0,FALSE);
}

void CNetSends::RequestClass( BYTE byteClass )
{
	RakNet::BitStream bsSpawnRequest;
	bsSpawnRequest.Write(byteClass);
	pNetGame->GetRakClient()->RPC("RequestClass",&bsSpawnRequest,HIGH_PRIORITY,RELIABLE,0,FALSE);
}

void CNetSends::Spawn( void )
{
	RakNet::BitStream bsSendSpawn;
	pNetGame->GetRakClient()->RPC("Spawn",&bsSendSpawn,HIGH_PRIORITY,RELIABLE_SEQUENCED,0,FALSE);
}

void CNetSends::Chat( PCHAR szText )
{
	BYTE byteTextLen = strlen(szText);
	RakNet::BitStream bsSend;
	bsSend.Write(byteTextLen);
	bsSend.Write(szText,byteTextLen);
	pNetGame->GetRakClient()->RPC("Chat",&bsSend,HIGH_PRIORITY,RELIABLE,0,FALSE);
}

void CNetSends::EnterVehicle( BYTE byteVehicleID, BYTE bytePassenger )
{
	RakNet::BitStream bsSend;
	bsSend.Write(byteVehicleID);
	bsSend.Write(bytePassenger);
	pNetGame->GetRakClient()->RPC("EnterVehicle",&bsSend,HIGH_PRIORITY,RELIABLE_SEQUENCED,0,FALSE);
}

void CNetSends::ExitVehicle( BYTE byteVehicleID )
{
	RakNet::BitStream bsSend;
	bsSend.Write(byteVehicleID);
	pNetGame->GetRakClient()->RPC("ExitVehicle",&bsSend,HIGH_PRIORITY,RELIABLE_SEQUENCED,0,FALSE);
}

void CNetSends::KickPlayer( BYTE bytePlayerID )
{
	RakNet::BitStream bsSend;
	bsSend.Write(bytePlayerID);
	pNetGame->GetRakClient()->RPC("KickPlayer",&bsSend,HIGH_PRIORITY,RELIABLE,0,FALSE);			
}

void CNetSends::BanIP( PCHAR szIP )
{
	int iIPLen=0;
	RakNet::BitStream bsSend;
	iIPLen = strlen(szIP);
	if(iIPLen > 64) return;
	bsSend.Write(iIPLen);
	bsSend.Write(szIP,iIPLen);
	pNetGame->GetRakClient()->RPC("BanIPAddress",&bsSend,HIGH_PRIORITY,RELIABLE,0,FALSE);			
}

void CNetSends::AdminLogin( PCHAR szPassword )
{
	int iPassLen=0;
	RakNet::BitStream bsSend;
	iPassLen = strlen(szPassword);
	if(iPassLen > 64) return;
	bsSend.Write(iPassLen);
	bsSend.Write(szPassword,iPassLen);
	pNetGame->GetRakClient()->RPC("Admin",&bsSend,HIGH_PRIORITY,RELIABLE,0,FALSE);			
}
