#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "netgame.h"
#include "../raknet/RakNetworkFactory.h"

extern CConfig *pServerConfig;

CNetGame::CNetGame(int iMaxPlayers, int iPort, int iGameType, 
				   char * szPassword, char * szGameFile,
				   BYTE byteFriendlyFire, BYTE byteShowOnRadar, INT startInterior)
{
	// Setup raknet
	m_pRak = RakNetworkFactory::GetRakServerInterface();

	m_pRak->InitializeSecurity(0,0);
	m_pRak->Start(iMaxPlayers,0,5,iPort);
	m_pRak->StartOccasionalPing();
	m_pRak->StartSynchronizedRandomInteger();

	LoadBanList();
	
	if(szPassword != NULL) {
		m_pRak->SetPassword(szPassword);
	}

	// Register our RPC handlers
	RegisterRPCs(m_pRak);

	// Setup player pool
	m_pPlayerPool = new CPlayerPool();

	// Setup vehicle pool
	m_pVehiclePool = new CVehiclePool();
	m_pVehiclePool->InitialiseFromConfig(pServerConfig);

	// Setup pickup pool
	m_pPickupPool = new CPickupPool();
	m_pPickupPool->InitialiseFromConfig(pServerConfig);

	// Setup ped pool
	m_pPedPool = new CPedPool();
	m_pPedPool->InitialiseFromConfig(pServerConfig);

	// Setup the init positions.
	SetupInitPositions();

	// Init the game logic.
	m_pGameLogic = new CGameModeGeneric();
	m_iSpawnsAvailable = m_pGameLogic->Init();
		
	// Flag we're in a running state.
	m_iGameState = GAMESTATE_RUNNING;

	// Set the ff option
	m_byteFriendlyFire = byteFriendlyFire;

	// Set the show player on radar option
	m_byteShowOnRadar = byteShowOnRadar;

	// Set the camera selection interior
	m_startInterior = startInterior;

	srand(time(NULL));
}

CNetGame::~CNetGame()
{
	logprintf("--- Server Shutting Down.");
	UnRegisterRPCs(m_pRak);
	RakNetworkFactory::DestroyRakServerInterface(m_pRak);
	delete m_pPlayerPool;
}

//----------------------------------------------------

void CNetGame::Process()
{
	UpdateNetwork();

	// Process the Players.
	m_pPlayerPool->Process();

	// Process the Vehicles.
	m_pVehiclePool->Process();
}

void CNetGame::UpdateNetwork()
{
	Packet* p;
	unsigned char packetIdentifier;

	while(p=m_pRak->Receive())
	{
		packetIdentifier = GetPacketID(p);
		
		switch(packetIdentifier) {

		case ID_DISCONNECTION_NOTIFICATION:
			m_pPlayerPool->Delete((BYTE)p->playerIndex,1);
			break;
		case ID_CONNECTION_LOST:
			m_pPlayerPool->Delete((BYTE)p->playerIndex,0);
			break;
		case ID_PLAYER_SYNC:
			PlayerSync(p);
			break;
		case ID_VEHICLE_SYNC:
			VehicleSync(p);
			break;
		case ID_PASSENGER_SYNC:
			PassengerSync(p);
			break;
		}

		m_pRak->DeallocatePacket(p);		
	}
}

void CNetGame::BroadcastData( RakNet::BitStream *bitStream,
							  PacketPriority priority,
							  PacketReliability reliability,
							  char orderingStream,
							  BYTE byteExcludedPlayer )
{
	BYTE x=0;
	float fDistance;
	BOOL bShouldSend;
	int r=0;

	while(x!=MAX_PLAYERS) {
		if( (m_pPlayerPool->GetSlotState(x) == TRUE) && 
			(x != byteExcludedPlayer) ) {

			 bShouldSend = FALSE;
			 fDistance = m_pPlayerPool->GetDistanceFromPlayerToPlayer(byteExcludedPlayer,x);
			 
			 if(fDistance < 250.0f) {
				bShouldSend = TRUE;
			 } else {
				 // If not within said distance, broadcast
				 // randomly once in ten.
				 r = (int)(rand() % 10);
				 if(!r) bShouldSend = TRUE;
			 }
			
			 if(bShouldSend) {
			 	 m_pRak->Send(bitStream,priority,reliability,
					orderingStream,m_pRak->GetPlayerIDFromIndex(x),FALSE);
			 }
		}
		x++;
	}			
}

void CNetGame::PlayerSync(Packet *p)
{
	CPlayer * pPlayer = GetPlayerPool()->GetAt((BYTE)p->playerIndex);
	RakNet::BitStream bsPlayerSync((PCHAR)p->data, p->length, FALSE);

	BYTE		bytePacketID=0;

	WORD		wKeys;
	VECTOR		vecWorldPos;
	float		fRotation;
	BYTE		bytePlayerHealth;
	BYTE		byteCurrentWeapon;
	BYTE		byteAction;

	S_CAMERA_AIM	caAiming;

	bsPlayerSync.Read(bytePacketID);
	bsPlayerSync.Read(wKeys);
	bsPlayerSync.Read(vecWorldPos.X);
	bsPlayerSync.Read(vecWorldPos.Y);
	bsPlayerSync.Read(vecWorldPos.Z);
	bsPlayerSync.Read(fRotation);
	bsPlayerSync.Read(byteAction);
	bsPlayerSync.Read(bytePlayerHealth);
	bsPlayerSync.Read(byteCurrentWeapon);

	if(IS_FIRING(wKeys)) {
		bsPlayerSync.Read(caAiming.f1x);
		bsPlayerSync.Read(caAiming.f1y);
		bsPlayerSync.Read(caAiming.f1z);
		bsPlayerSync.Read(caAiming.f2x);
		bsPlayerSync.Read(caAiming.f2y);
		bsPlayerSync.Read(caAiming.f2z);
		bsPlayerSync.Read(caAiming.pos1x);
		bsPlayerSync.Read(caAiming.pos1y);
		bsPlayerSync.Read(caAiming.pos1z);
	}
		
	if(pPlayer)	{
		pPlayer->StoreOnFootFullSyncData(wKeys,&vecWorldPos,fRotation,byteCurrentWeapon,byteAction);
		if(IS_FIRING(wKeys)) pPlayer->StoreAimSyncData(&caAiming);		
		pPlayer->SetReportedHealth(bytePlayerHealth);
	}
}

void CNetGame::VehicleSync(Packet *p)
{
	CPlayer * pPlayer = GetPlayerPool()->GetAt((BYTE)p->playerIndex);
	RakNet::BitStream bsVehicleSync((PCHAR)p->data, p->length, FALSE);

	BYTE		bytePacketID=0;
	BYTE		byteVehicleID=0;

	WORD		wKeys;

	C_VECTOR1 cvecRoll;
	C_VECTOR1 cvecDirection;
	VECTOR	vecWorldPos;
	VECTOR	vecMoveSpeed;

	BYTE		byteReadVehicleHealth;
	float		fHealth;
	BYTE		bytePlayerHealth;

	bsVehicleSync.Read(bytePacketID);
	bsVehicleSync.Read(byteVehicleID);
	bsVehicleSync.Read(wKeys);
	bsVehicleSync.Read(cvecRoll.X);
	bsVehicleSync.Read(cvecRoll.Y);
	bsVehicleSync.Read(cvecRoll.Z);
	bsVehicleSync.Read(cvecDirection.X);
	bsVehicleSync.Read(cvecDirection.Y);
	bsVehicleSync.Read(cvecDirection.Z);
	bsVehicleSync.Read(vecWorldPos.X);
	bsVehicleSync.Read(vecWorldPos.Y);
	bsVehicleSync.Read(vecWorldPos.Z);

	// move and turn speed vectors
	bsVehicleSync.Read(vecMoveSpeed.X);
	bsVehicleSync.Read(vecMoveSpeed.Y);
	bsVehicleSync.Read(byteReadVehicleHealth);
	bsVehicleSync.Read(bytePlayerHealth);

	fHealth = UNPACK_VEHICLE_HEALTH(byteReadVehicleHealth);

	if(pPlayer)	{
		pPlayer->StoreInCarFullSyncData(byteVehicleID,wKeys,&cvecRoll,
			&cvecDirection,&vecWorldPos,&vecMoveSpeed,fHealth);
		pPlayer->SetReportedHealth(bytePlayerHealth);
	}
}

void CNetGame::PassengerSync(Packet *p)
{
	CPlayer * pPlayer = GetPlayerPool()->GetAt((BYTE)p->playerIndex);
	RakNet::BitStream bsPassengerSync((PCHAR)p->data, p->length, FALSE);
	RakNet::BitStream bsPassengerSend;

	BYTE		bytePacketID=0;
	BYTE		byteVehicleID=0;
	UINT		uiPassengerSeat;
	float		x,y,z;

	bsPassengerSync.Read(bytePacketID);
	bsPassengerSync.Read(byteVehicleID);
	bsPassengerSync.Read(uiPassengerSeat);
	bsPassengerSync.Read(x);
	bsPassengerSync.Read(y);
	bsPassengerSync.Read(z);

	pPlayer->UpdatePosition(x,y,z);

	// Now broadcast it.
	bsPassengerSend.Write((BYTE)p->playerIndex);
	bsPassengerSend.Write(byteVehicleID);
	bsPassengerSend.Write(uiPassengerSeat);
	m_pRak->RPC("Passenger",&bsPassengerSend,
		HIGH_PRIORITY,RELIABLE,0,p->playerId,TRUE,FALSE);

}

void CNetGame::SetupInitPositions()
{	
	char *szParseMe;

	// Init Player Pos
	szParseMe = pServerConfig->GetConfigEntryAsString("PLAYERPOS");
	if(!szParseMe) { 
		logprintf("I can't init a network game because it's missing a PLAYERPOS setting.");
		exit(1);
	}
	sscanf(szParseMe,"%f%f%f",&m_vecInitPlayerPos.X,&m_vecInitPlayerPos.Y,&m_vecInitPlayerPos.Z);

	// Init Camera Pos
	szParseMe = pServerConfig->GetConfigEntryAsString("CAMPOS");
	if(!szParseMe) { 
		logprintf("I can't init a network game because it's missing a CAMPOS setting.");
		exit(1);
	}
	sscanf(szParseMe,"%f%f%f",&m_vecInitCameraPos.X,&m_vecInitCameraPos.Y,&m_vecInitCameraPos.Z);

	// Init Camera Look
	szParseMe = pServerConfig->GetConfigEntryAsString("CAMLOOK");
	if(!szParseMe) { 
		logprintf("I can't init a network game because it's missing a CAMLOOK setting.");
		exit(1);
	}
	sscanf(szParseMe,"%f%f%f",&m_vecInitCameraLook.X,&m_vecInitCameraLook.Y,&m_vecInitCameraLook.Z);

	// Init World Bounds
	szParseMe = pServerConfig->GetConfigEntryAsString("WORLDBOUNDS");
	if(!szParseMe) { 
		logprintf("I can't init a network game because it's missing a WORLDBOUNDS setting.");
		exit(1);
	}
	sscanf(szParseMe,"%f%f%f%f",&m_WorldBounds[0],&m_WorldBounds[1],&m_WorldBounds[2],&m_WorldBounds[3]);
}

void CNetGame::KickPlayer(BYTE byteKickPlayer)
{
	if (byteKickPlayer < MAX_PLAYERS)
	{
		if (m_pPlayerPool->GetSlotState(byteKickPlayer))
		{
			m_pPlayerPool->Delete(byteKickPlayer,2);
		}
	}
}

void CNetGame::AddBan(char * ip_mask)
{
	m_pRak->AddToBanList(ip_mask);
	
	FILE * fileBanList = fopen("jmnvc.banlist","a");
	if(!fileBanList) return;

	fprintf(fileBanList,"%s\n",ip_mask);

	fclose(fileBanList);
}

void CNetGame::LoadBanList()
{
	FILE * fileBanList = fopen("jmnvc.banlist","r");
	if(!fileBanList) return;

	char ban_ip[256];

	while(!feof(fileBanList)) {
		fgets(ban_ip,256,fileBanList);
		ban_ip[strlen(ban_ip) - 1] = '\0';
		m_pRak->AddToBanList(ban_ip);
	}

	fclose(fileBanList);
}

