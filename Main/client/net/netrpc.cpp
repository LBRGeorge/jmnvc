#include "../main.h"

extern CGame		 *pGame;
extern CChatWindow   *pChatWindow;
extern CCmdWindow	 *pCmdWindow;

using namespace RakNet;
extern CNetGame* pNetGame;

#define REJECT_REASON_BAD_VERSION   1
#define REJECT_REASON_BAD_NICKNAME  2

//----------------------------------------------------
// This gets sent when we first join the game to let us
// know how many players there are and add them to the pool

void FirstJoin(PCHAR Data, int iBitLength, PlayerID sender)
{
	BYTE x = 0;
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	BYTE byteTotalPlayers;
	bsData.Read(byteTotalPlayers);

	while(x<byteTotalPlayers) {
		CHAR szPlayerName[MAX_PLAYER_NAME];
		BYTE bytePlayerID;
		UINT uiNameLength;
		memset(szPlayerName,0,MAX_PLAYER_NAME);
		bsData.Read(bytePlayerID);
		bsData.Read(uiNameLength);
		bsData.Read(szPlayerName,uiNameLength);
		szPlayerName[uiNameLength] = '\0';

		// Add this client to the player pool.
		pPlayerPool->New(bytePlayerID, szPlayerName, false);

		x++;
	}
}

//----------------------------------------------------
// Sent when a client joins the server we're
// currently connected to.

void ServerJoin(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	CHAR szPlayerName[MAX_PLAYER_NAME];
	BYTE bytePlayerID;
	UINT uiNameLength;
	BYTE byteSendDuringConnect;

	memset(szPlayerName,0,MAX_PLAYER_NAME);

	bsData.Read(bytePlayerID);
	bsData.Read(uiNameLength);
	bsData.Read(szPlayerName,uiNameLength);
	bsData.Read(byteSendDuringConnect);
	szPlayerName[uiNameLength] = '\0';

	BOOL bAlert = FALSE;
	if (byteSendDuringConnect==1)
	{
		bAlert = TRUE;
	}

	// Add this client to the player pool.
	pPlayerPool->New(bytePlayerID, szPlayerName, bAlert);
}

//----------------------------------------------------
// Sent when a client joins the server we're
// currently connected to.

void ServerQuit(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	BYTE bytePlayerID;
	BYTE byteReason;

	bsData.Read(bytePlayerID);
	bsData.Read(byteReason);

	// Delete this client from the player pool.
	pPlayerPool->Delete(bytePlayerID,byteReason);
}

//----------------------------------------------------
// Server is giving us basic init information.

void InitGame(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsInitGame(Data,iBitLength/8,FALSE);
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	BYTE byteMyPlayerID;

	bsInitGame.Read((float)pNetGame->m_vecInitPlayerPos.X);
	bsInitGame.Read((float)pNetGame->m_vecInitPlayerPos.Y);
	bsInitGame.Read((float)pNetGame->m_vecInitPlayerPos.Z);
	bsInitGame.Read((float)pNetGame->m_vecInitCameraPos.X);
	bsInitGame.Read((float)pNetGame->m_vecInitCameraPos.Y);
	bsInitGame.Read((float)pNetGame->m_vecInitCameraPos.Z);
	bsInitGame.Read((float)pNetGame->m_vecInitCameraLook.X);
	bsInitGame.Read((float)pNetGame->m_vecInitCameraLook.Y);
	bsInitGame.Read((float)pNetGame->m_vecInitCameraLook.Z);
	bsInitGame.Read((float)pNetGame->m_WorldBounds[0]);
	bsInitGame.Read((float)pNetGame->m_WorldBounds[1]);
	bsInitGame.Read((float)pNetGame->m_WorldBounds[2]);
	bsInitGame.Read((float)pNetGame->m_WorldBounds[3]);
	bsInitGame.Read(pNetGame->m_iSpawnsAvailable);
	bsInitGame.Read(pNetGame->m_byteFriendlyFire);
	bsInitGame.Read(pNetGame->m_byteShowOnRadar);
	bsInitGame.Read((int)pNetGame->m_startInterior);
	bsInitGame.Read(byteMyPlayerID);

	pPlayerPool->SetLocalPlayerID(byteMyPlayerID);

	pNetGame->InitGameLogic();
	pNetGame->SetGameState(GAMESTATE_CONNECTED);
}

//----------------------------------------------------
// Remote player has sent a chat message.

void Chat(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	BYTE bytePlayerID;
	BYTE byteTextLen;
	CHAR szText[256];

	bsData.Read(bytePlayerID);
	bsData.Read(byteTextLen);
	bsData.Read(szText,byteTextLen);

	szText[byteTextLen] = '\0';

	CRemotePlayer *pRemotePlayer = pNetGame->GetPlayerPool()->GetAt(bytePlayerID);
	if(pRemotePlayer) {
		pRemotePlayer->Say(szText);	
	}
}

//----------------------------------------------------
// This should be rewritten as a packet instead of
// an RPC.

void Passenger(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	
	BYTE bytePlayerID;
	BYTE byteVehicleID;
	UINT uiSeat;

	bsData.Read(bytePlayerID);
	bsData.Read(byteVehicleID);
	bsData.Read(uiSeat);
	
	CRemotePlayer * pRemotePlayer = pNetGame->GetPlayerPool()->GetAt(bytePlayerID);

	if(pRemotePlayer) {
		pRemotePlayer->StorePassengerData(byteVehicleID,uiSeat);
	}	
}

//----------------------------------------------------
// Reply to our class request from the server.

void RequestClass(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	BYTE byteRequestOutcome=0;
	PLAYER_SPAWN_INFO SpawnInfo;
	CLocalPlayer *pPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();

	bsData.Read(byteRequestOutcome);
	bsData.Read(SpawnInfo.byteTeam);
	bsData.Read(SpawnInfo.byteSkin);
	bsData.Read((float)SpawnInfo.vecPos.X);
	bsData.Read((float)SpawnInfo.vecPos.Y);
	bsData.Read((float)SpawnInfo.vecPos.Z);
	bsData.Read((float)SpawnInfo.fRotation);
	bsData.Read(SpawnInfo.iSpawnWeapons[0]);
	bsData.Read(SpawnInfo.iSpawnWeaponsAmmo[0]);
	bsData.Read(SpawnInfo.iSpawnWeapons[1]);
	bsData.Read(SpawnInfo.iSpawnWeaponsAmmo[1]);
	bsData.Read(SpawnInfo.iSpawnWeapons[2]);
	bsData.Read(SpawnInfo.iSpawnWeaponsAmmo[2]);


	if(byteRequestOutcome) {
		pPlayer->SetSpawnInfo(SpawnInfo.byteTeam,SpawnInfo.byteSkin,&SpawnInfo.vecPos,
			SpawnInfo.fRotation,
			SpawnInfo.iSpawnWeapons[0],SpawnInfo.iSpawnWeaponsAmmo[0],
			SpawnInfo.iSpawnWeapons[1],SpawnInfo.iSpawnWeaponsAmmo[1],
			SpawnInfo.iSpawnWeapons[2],SpawnInfo.iSpawnWeaponsAmmo[2]);

		pNetGame->GetGameLogic()->HandleClassSelectionOutcome(&SpawnInfo, TRUE);
	}
	else {
		pNetGame->GetGameLogic()->HandleClassSelectionOutcome(NULL,FALSE);
	}
}

//----------------------------------------------------
// Remote client is spawning.

void Spawn(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	CRemotePlayer *pRemotePlayer;

	BYTE bytePlayerID=0;
	BYTE byteTeam=0;
	BYTE byteSkin=0;
	VECTOR vecPos;
	float fRotation=0;
	BYTE byteOnGround=0;
	int iSpawnWeapons1,iSpawnWeapons2,iSpawnWeapons3;
	int iSpawnWeaponsAmmo1,iSpawnWeaponsAmmo2,iSpawnWeaponsAmmo3;

	bsData.Read(bytePlayerID);
	bsData.Read(byteTeam);
	bsData.Read(byteSkin);
	bsData.Read(vecPos.X);
	bsData.Read(vecPos.Y);
	bsData.Read(vecPos.Z);
	bsData.Read(fRotation);
	bsData.Read(iSpawnWeapons1);
	bsData.Read(iSpawnWeaponsAmmo1);
	bsData.Read(iSpawnWeapons2);
	bsData.Read(iSpawnWeaponsAmmo2);
	bsData.Read(iSpawnWeapons3);
	bsData.Read(iSpawnWeaponsAmmo3);

	pRemotePlayer = pNetGame->GetPlayerPool()->GetAt(bytePlayerID);

	if(pRemotePlayer) {
		pRemotePlayer->SpawnPlayer(byteTeam,byteSkin,&vecPos,fRotation,
			iSpawnWeapons1,iSpawnWeaponsAmmo1,
			iSpawnWeapons2,iSpawnWeaponsAmmo2,
			iSpawnWeapons3,iSpawnWeaponsAmmo3);
	}		
}

//----------------------------------------------------
// Remote client is dead.

void Death(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	BYTE bytePlayerID=0;
	BYTE byteReason;
	BYTE byteWhoKilled;
	BYTE byteScoringModifier;

	bsData.Read(bytePlayerID);
	bsData.Read(byteReason);
	bsData.Read(byteWhoKilled);
	bsData.Read(byteScoringModifier);

	CRemotePlayer *pRemotePlayer = pNetGame->GetPlayerPool()->GetAt(bytePlayerID);
	if(pRemotePlayer) {
		pRemotePlayer->HandleDeath(byteReason,byteWhoKilled,byteScoringModifier);
	}
}

//----------------------------------------------------
// Server responding to our Death message.

void OwnDeath(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	BYTE bytePlayerID=0;
	BYTE byteReason;
	BYTE byteWhoKilled;
	BYTE byteScoringModifier;

	bsData.Read(bytePlayerID);
	bsData.Read(byteReason);
	bsData.Read(byteWhoKilled);
	bsData.Read(byteScoringModifier);

	CLocalPlayer *pLocalPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();
	pLocalPlayer->HandleDeath(byteReason,byteWhoKilled,byteScoringModifier);
}

//----------------------------------------------------
// Remote client is trying to enter vehicle gracefully.

void EnterVehicle(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	BYTE bytePlayerID=0;
	BYTE byteVehicleID=0;
	BYTE bytePassenger=0;

	bsData.Read(bytePlayerID);
	bsData.Read(byteVehicleID);
	bsData.Read(bytePassenger);

	CRemotePlayer *pRemotePlayer = pNetGame->GetPlayerPool()->GetAt(bytePlayerID);
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();

	if(pRemotePlayer) {
		if(!bytePassenger) {
			pRemotePlayer->GetPlayerPed()->EnterVehicleAsDriver(pVehiclePool->FindGtaIDFromID(byteVehicleID));
		} else {
			pRemotePlayer->GetPlayerPed()->EnterVehicleAsPassenger(pVehiclePool->FindGtaIDFromID(byteVehicleID));
		}
	}
}

//----------------------------------------------------
// Remote client is trying to enter vehicle gracefully.

void ExitVehicle(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	BYTE bytePlayerID=0;
	BYTE byteVehicleID=0;

	bsData.Read(bytePlayerID);
	bsData.Read(byteVehicleID);

	CRemotePlayer *pRemotePlayer = pNetGame->GetPlayerPool()->GetAt(bytePlayerID);
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();

	if(pRemotePlayer) {
		pRemotePlayer->GetPlayerPed()->ExitCurrentVehicle();
	}
}

//----------------------------------------------------

void PedSpawn(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	BYTE bytePedID=0;
	BYTE byteSkin;
	VECTOR vecPos;
	VECTOR vecSpawnPos;
	float fRotation;
	float fSpawnRotation;
	float fHealth;
	int iColor1, iColor2;

	bsData.Read(bytePedID);
	bsData.Read(byteSkin);
	bsData.Read(vecPos.X);
	bsData.Read(vecPos.Y);
	bsData.Read(vecPos.Z);
	bsData.Read(fRotation);
	bsData.Read(iColor1);
	bsData.Read(iColor2);
	bsData.Read(fHealth);
	bsData.Read(vecSpawnPos.X);
	bsData.Read(vecSpawnPos.Y);
	bsData.Read(vecSpawnPos.Z);
	bsData.Read(fSpawnRotation);

	if((byteSkin < 107) && !pGame->IsModelLoaded(byteSkin)) {
		pGame->RequestModel(byteSkin);
		while(!pGame->IsModelLoaded(byteSkin)) { Sleep(1); }
	}

	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	pPlayerPool->New(bytePedID+20, "sinorpedped", FALSE);
	CRemotePlayer* Player = pPlayerPool->GetAt(bytePedID+20);
	Player->SpawnPlayer(1,byteSkin,&vecPos,fRotation,-1,-1,-1,-1,-1,-1);
	//Player->GetPlayerPed()->SetImmunities(0,0,0,0,0);
	Player->GetPlayerPed()->ShowMarker(2);
	Player->GetPlayerPed()->SetKeys(15);

	//CPlayerPed *pPed = pGame->NewPlayer(bytePedID+10,byteSkin,vecPos.X,vecPos.Y,vecPos.Z,fRotation);
	//pPed->TogglePlayerControllable(0);
	//pPed->GiveWeapon(WEAPON_MINIGUN,9999);
	//pPed->ShowMarker(1);
	//pNetGame->GetPlayerPool()->GetAt(bytePedID+10)->SetReportedHealth(1000);
	//pNetGame->GetPlayerPool()->GetAt(bytePedID+10)->;

	//DWORD myPed=0;
	//ScriptCommand(&create_actor,4,byteSkin,vecPos.X,vecPos.Y,vecPos.Z,&myPed);
	//ScriptCommand(&reset_actor_flags,myPed);
	//ScriptCommand(&toggle_actor_wander,myPed,0);

	//DWORD hndMarker;
	//float f=0.0f;

	/*_asm push 2
	_asm push 4
	_asm mov eax, myPed
	_asm push eax
	_asm push 2
	_asm mov edx, ADDR_TIE_MARKER_TO_ACTOR
	_asm call edx
	_asm mov hndMarker, eax
	_asm pop ecx
	_asm pop ecx
	_asm pop ecx
	_asm pop ecx*/

	/*_asm push 1 //Color not needed yet
	_asm push hndMarker
	_asm mov edx, ADDR_SET_MARKER_COLOR
	_asm call edx
	_asm pop ecx
	_asm pop ecx*/

	/*_asm push 2
	_asm push hndMarker
	_asm mov edx, ADDR_SHOW_ON_RADAR1
	_asm call edx
	_asm pop ecx
	_asm pop ecx*/
}

void PickupSpawn(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	BYTE bytePickupID=0;
	BYTE bytePickupModel;
	BOOL bytePickedUp;
	VECTOR vecPos;

	bsData.Read(bytePickupID);
	bsData.Read(bytePickupModel);
	bsData.Read(vecPos.X);
	bsData.Read(vecPos.Y);
	bsData.Read(vecPos.Z);
	bsData.Read(bytePickedUp);

	//if (bytePickedUp==FALSE)
	//{
		DWORD myPickup=0;
		ScriptCommand(&create_pickup,bytePickupModel,15,vecPos.X,vecPos.Y,vecPos.Z,&myPickup);
	//}
}

void VehicleSpawn(PCHAR Data, int iBitLength, PlayerID sender)
{
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	BYTE byteVehicleID=0;
	BYTE byteVehicleType;
	VECTOR vecPos;
	VECTOR vecSpawnPos;
	float fRotation;
	float fSpawnRotation;
	float fHealth;
	int iColor1, iColor2;

	bsData.Read(byteVehicleID);
	bsData.Read(byteVehicleType);
	bsData.Read(vecPos.X);
	bsData.Read(vecPos.Y);
	bsData.Read(vecPos.Z);
	bsData.Read(fRotation);
	bsData.Read(iColor1);
	bsData.Read(iColor2);
	bsData.Read(fHealth);
	bsData.Read(vecSpawnPos.X);
	bsData.Read(vecSpawnPos.Y);
	bsData.Read(vecSpawnPos.Z);
	bsData.Read(fSpawnRotation);

	pVehiclePool->New(byteVehicleID,byteVehicleType,
		&vecPos,fRotation,iColor1,iColor2,&vecSpawnPos,fSpawnRotation);
	//pVehiclePool->GetAt(byteVehicleID)->SetHealth(fHealth);
}

//----------------------------------------------------

void UpdateScPing(PCHAR Data, int iBitLength, PlayerID sender)
{	
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();

	int iPlayers = (iBitLength/8) / 9;
	int x=0;

	BYTE bytePlayerID;
	int iPlayerScore;
	int iPlayerPing;
	ULONG ip;

	while(x!=iPlayers) {
		bsData.Read(bytePlayerID);
		bsData.Read(iPlayerScore);
		bsData.Read(iPlayerPing);
		bsData.Read(ip);

		if( pPlayerPool->GetSlotState(bytePlayerID) == TRUE ||
			bytePlayerID == pPlayerPool->GetLocalPlayerID() ) {

			pPlayerPool->UpdateScore(bytePlayerID,iPlayerScore);
			pPlayerPool->UpdatePing(bytePlayerID,iPlayerPing);
			pPlayerPool->UpdateIPAddress(bytePlayerID,ip);
		}

		x++;
	}
}

//----------------------------------------------------

void ConnectionRejected(PCHAR Data, int iBitLength, PlayerID sender)
{
	RakNet::BitStream bsData(Data,iBitLength/8,FALSE);
	BYTE byteRejectReason;

	bsData.Read(byteRejectReason);

	if(byteRejectReason==REJECT_REASON_BAD_VERSION) {
		pChatWindow->AddInfoMessage("CONNECTION REJECTED");
		pChatWindow->AddInfoMessage("YOU'RE USING AN INCORRECT VERSION!");
	}
	else if(byteRejectReason==REJECT_REASON_BAD_NICKNAME)
	{
		pChatWindow->AddInfoMessage("CONNECTION REJECTED");
		pChatWindow->AddInfoMessage("YOUR NICKNAME IS INVALID");
	}

	pNetGame->GetRakClient()->Disconnect(0);
}

//----------------------------------------------------

void RegisterRPCs(RakClientInterface * pRakClient)
{
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,FirstJoin);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,ServerJoin);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,ServerQuit);	
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,InitGame);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,Chat);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,RequestClass);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,Spawn);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,Death);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,OwnDeath);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,EnterVehicle);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,ExitVehicle);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,VehicleSpawn);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,PickupSpawn);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,PedSpawn);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,UpdateScPing);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,ConnectionRejected);
	REGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,Passenger);
}

//----------------------------------------------------

void UnRegisterRPCs(RakClientInterface * pRakClient)
{
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,FirstJoin);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,ServerJoin);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,ServerQuit);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,InitGame);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,Chat);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,RequestClass);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,Spawn);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,Death);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,OwnDeath);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,EnterVehicle);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,ExitVehicle);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,VehicleSpawn);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,PickupSpawn);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,PedSpawn);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,UpdateScPing);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,ConnectionRejected);
	UNREGISTER_AS_REMOTE_PROCEDURE_CALL(pRakClient,Passenger);
}
