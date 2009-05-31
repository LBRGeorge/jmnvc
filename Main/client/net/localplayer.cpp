#include "../main.h"
#include "../game/util.h"

extern CGame		 *pGame;
extern CChatWindow   *pChatWindow;
extern CCmdWindow	 *pCmdWindow;

using namespace RakNet;
extern CNetGame* pNetGame;

//----------------------------------------------------------

CLocalPlayer::CLocalPlayer()
{
	m_bHasSpawnInfo = FALSE;
	m_pPlayerPed = pGame->FindPlayerPed();
	m_bIsActive = FALSE;
	m_bIsWasted = FALSE;
	m_wLastKeys = 0;
	m_dwLastSendTick = GetTickCount();
	m_dwLastPassengerSendTick = GetTickCount();
}

//----------------------------------------------------------

BOOL CLocalPlayer::DestroyPlayer()
{
	return TRUE;
}

//----------------------------------------------------------

BOOL CLocalPlayer::Process()
{
	CGameModeGeneric *pGameLogic;
	CVehicle *pGameVehicle;
	CVehiclePool *pVehiclePool;

	BYTE byteVehicleID;

	DWORD dwThisTick;

	if(m_bIsActive && !m_bIsWasted && (NULL != m_pPlayerPed))
	{
		if((m_pPlayerPed->GetAction() == ACTION_WASTED) && !m_bIsWasted)
		{
			m_bIsWasted = TRUE;
			m_bIsActive = FALSE;
			SendWastedNotification();
		}
		else
		{
			dwThisTick = GetTickCount();

			if(m_pPlayerPed->IsInVehicle() && !m_pPlayerPed->IsAPassenger())
			{
				// DRIVING VEHICLE

				// VEHICLE WORLD BOUNDS STUFF
				pVehiclePool = pNetGame->GetVehiclePool();
				byteVehicleID = (BYTE)pVehiclePool->FindIDFromGtaPtr(m_pPlayerPed->GetGtaVehicle());
				if(byteVehicleID != 255) {
					pGameVehicle = pVehiclePool->GetAt(byteVehicleID);
					pGameVehicle->EnforceWorldBoundries(
						pNetGame->m_WorldBounds[0],pNetGame->m_WorldBounds[1],
						pNetGame->m_WorldBounds[2],pNetGame->m_WorldBounds[3]);
				}

				if((dwThisTick - m_dwLastSendTick) > (UINT)GetOptimumInCarSendRate()) {
					m_dwLastSendTick = GetTickCount();
				
					// send nothing while we're getting out.
					if(m_pPlayerPed->GetAction() != ACTION_EXITING_VEHICLE) {
						SendInCarFullSyncData(); 
					}
				}
				pGame->DisablePassengerEngineAudio();
			}
			else if(!m_pPlayerPed->IsAPassenger()) 
			{
				// ONFOOT NOT PASSENGER

				if((dwThisTick - m_dwLastSendTick) > (UINT)GetOptimumOnFootSendRate()) {
					m_dwLastSendTick = GetTickCount();

					// send nothing while we're getting in
					if(m_pPlayerPed->GetAction() != ACTION_DRIVING_VEHICLE)
					{
						SendOnFootFullSyncData(); 
					}
				}

				pGame->DisablePassengerEngineAudio();
			}
			else if(m_pPlayerPed->IsAPassenger()) 
			{
				// PASSENGER

				if((dwThisTick - m_dwLastPassengerSendTick) > 1000) {
					m_dwLastPassengerSendTick = GetTickCount();
					SendInCarPassengerData();
				}

				pGame->EnablePassengerEngineAudio();
			}			
		}
	}
	else if ((m_bIsWasted == TRUE) && 
			(m_pPlayerPed->GetAction() != ACTION_WASTED))
	{
		m_bIsWasted = FALSE;
		pNetGame->GetGameLogic()->HandleClassSelection(this);
		return TRUE;
	}

	pGameLogic = pNetGame->GetGameLogic();
	if(pGameLogic) pGameLogic->ProcessLocalPlayer(this);

	return TRUE;
}

//----------------------------------------------------------

void CLocalPlayer::SendOnFootFullSyncData()
{
	RakNet::BitStream bsPlayerSync;
	CPlayerPed *pGamePlayer = pGame->FindPlayerPed();
	MATRIX4X4 matPlayer;
	WORD wKeys = pGamePlayer->GetKeys();
	BYTE bytePlayerHealth;
	CAMERA_AIM * pCameraAim = pGamePlayer->GetCurrentAim();
		
	C_VECTOR1 cvecAimRoll;
	C_VECTOR1 cvecAimDir;

	if(m_pPlayerPed)
	{
		// Don't allow them to send firing actions if
		// they got no ammo.
		if(!m_pPlayerPed->HasAmmoForCurrentWeapon()) {
			wKeys = CEASE_FIRE_CEASE_FIRE(wKeys);
		}

		// packet ID
		bsPlayerSync.Write((BYTE)ID_PLAYER_SYNC);

		// contents
		bsPlayerSync.Write(wKeys);
		m_pPlayerPed->GetMatrix(&matPlayer);

		// PART OF THE ANTI-CHEAT
		if(matPlayer.vPos.Z > 499.1f) exit(1);

		bsPlayerSync.Write(matPlayer.vPos.X);
		bsPlayerSync.Write(matPlayer.vPos.Y);
		bsPlayerSync.Write(matPlayer.vPos.Z);
		bsPlayerSync.Write(m_pPlayerPed->GetRotation());
		bsPlayerSync.Write(m_pPlayerPed->GetShootingFlags());
	
		bytePlayerHealth = (BYTE)m_pPlayerPed->GetHealth();
		bsPlayerSync.Write(bytePlayerHealth);
		bsPlayerSync.Write(m_pPlayerPed->GetCurrentWeapon());

		// send aiming data if the firing button is held
		if(IS_FIRING(wKeys)) {
			// Get vectors into format for compression.
			VECTOR vecAimRoll;
			vecAimRoll.X = pCameraAim->f1x;
			vecAimRoll.Y = pCameraAim->f1y;
			vecAimRoll.Z = pCameraAim->f1z;
			
			VECTOR vecAimDir;
			vecAimDir.X = pCameraAim->f2x;
			vecAimDir.Y = pCameraAim->f2y;
			vecAimDir.Z = pCameraAim->f2z;

			CompressVector1(&vecAimRoll,&cvecAimRoll);
			CompressVector1(&vecAimDir,&cvecAimDir);

			// aiming
			bsPlayerSync.Write(cvecAimRoll.X);
			bsPlayerSync.Write(cvecAimRoll.Y);
			bsPlayerSync.Write(cvecAimRoll.Z);
			bsPlayerSync.Write(cvecAimDir.X);
			bsPlayerSync.Write(cvecAimDir.Y);
			bsPlayerSync.Write(cvecAimDir.Z);
			bsPlayerSync.Write(pCameraAim->pos1x);
			bsPlayerSync.Write(pCameraAim->pos1y);
			bsPlayerSync.Write(pCameraAim->pos1z);
		}

		pNetGame->GetRakClient()->Send(&bsPlayerSync,HIGH_PRIORITY,UNRELIABLE,0);
	}
}

//----------------------------------------------------------

void CLocalPlayer::SendInCarFullSyncData()
{
	RakNet::BitStream bsVehicleSync;
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();

	MATRIX4X4 matPlayer;
	WORD wKeys = m_pPlayerPed->GetKeys();
	CVehicle *pGameVehicle=NULL;
	BYTE byteVehicleID=0;
	
	C_VECTOR1 cvecCompressedRoll;
	C_VECTOR1 cvecCompressedDirection;
	
	VECTOR	vecMoveSpeed;

	BYTE		byteWriteVehicleHealth;
	BYTE		bytePlayerHealth;

	if(m_pPlayerPed)
	{
		byteVehicleID = (BYTE)pVehiclePool->FindIDFromGtaPtr(m_pPlayerPed->GetGtaVehicle());
		if(byteVehicleID == 255) return;

		// Don't allow them to send firing actions if
		// they got no ammo.
		if(!m_pPlayerPed->HasAmmoForCurrentWeapon()) {
			wKeys = CEASE_FIRE_CEASE_FIRE(wKeys);
		}

		// get the vehicle matrix
		pGameVehicle = pVehiclePool->GetAt(byteVehicleID);
		pGameVehicle->GetMatrix(&matPlayer);

		// packing
		CompressVector1(&matPlayer.vLookRight,&cvecCompressedRoll);
		CompressVector1(&matPlayer.vLookUp,&cvecCompressedDirection);
		byteWriteVehicleHealth = PACK_VEHICLE_HEALTH(pGameVehicle->GetHealth());

		// writing
		bsVehicleSync.Write((BYTE)ID_VEHICLE_SYNC);
		bsVehicleSync.Write(byteVehicleID);
		bsVehicleSync.Write(wKeys);

		// matrix stuff
		bsVehicleSync.Write(cvecCompressedRoll.X);
		bsVehicleSync.Write(cvecCompressedRoll.Y);
		bsVehicleSync.Write(cvecCompressedRoll.Z);
		bsVehicleSync.Write(cvecCompressedDirection.X);
		bsVehicleSync.Write(cvecCompressedDirection.Y);
		bsVehicleSync.Write(cvecCompressedDirection.Z);
		bsVehicleSync.Write(matPlayer.vPos.X);
		bsVehicleSync.Write(matPlayer.vPos.Y);
		bsVehicleSync.Write(matPlayer.vPos.Z);

		// speed vectors
		pGameVehicle->GetMoveSpeedVector(&vecMoveSpeed);
		bsVehicleSync.Write(vecMoveSpeed.X);
		bsVehicleSync.Write(vecMoveSpeed.Y);
		
		bsVehicleSync.Write(byteWriteVehicleHealth);

		bytePlayerHealth = (BYTE)m_pPlayerPed->GetHealth();
		bsVehicleSync.Write(bytePlayerHealth);

		// ..sending
		pNetGame->GetRakClient()->Send(&bsVehicleSync,HIGH_PRIORITY,UNRELIABLE,0);
	}
}

//----------------------------------------------------------

void CLocalPlayer::SendInCarPassengerData()
{
	RakNet::BitStream bsPassengerSync;
	MATRIX4X4 matPlayer;
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();

	BYTE byteVehicleID = (BYTE)pVehiclePool->FindIDFromGtaPtr(m_pPlayerPed->GetGtaVehicle());
	if(byteVehicleID == 255) return;

	UINT uiPassengerSeat = m_pPlayerPed->GetPassengerSeat();
	
	m_pPlayerPed->GetMatrix(&matPlayer);
	
	bsPassengerSync.Write((BYTE)ID_PASSENGER_SYNC);
	bsPassengerSync.Write((BYTE)byteVehicleID);
	bsPassengerSync.Write(uiPassengerSeat);
	bsPassengerSync.Write(matPlayer.vPos.X);
	bsPassengerSync.Write(matPlayer.vPos.Y);
	bsPassengerSync.Write(matPlayer.vPos.Z);
	pNetGame->GetRakClient()->Send(&bsPassengerSync,HIGH_PRIORITY,UNRELIABLE,0);
}

//----------------------------------------------------------

int CLocalPlayer::GetOptimumInCarSendRate()
{
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
	CVehicle	 *pGameVehicle=NULL;
	VECTOR	 vecMoveSpeed;
	BYTE		 byteVehicleID=0;

	if(m_pPlayerPed) {

		byteVehicleID = (BYTE)pVehiclePool->FindIDFromGtaPtr(m_pPlayerPed->GetGtaVehicle());
		pGameVehicle = pVehiclePool->GetAt(byteVehicleID);

		if(pGameVehicle) {

			pGameVehicle->GetMoveSpeedVector(&vecMoveSpeed);

			if( (vecMoveSpeed.X == 0.0f) &&
				(vecMoveSpeed.Y == 0.0f) ) {

				return 200;
			}
			else {
				return 66;
			}
		}
	}
	return 200;
}

//----------------------------------------------------------

int CLocalPlayer::GetOptimumOnFootSendRate()
{	
	VECTOR	 vecMoveSpeed;

	if(m_pPlayerPed) {

		m_pPlayerPed->GetMoveSpeedVector(&vecMoveSpeed);

		if( (vecMoveSpeed.X == 0.0f) &&
			(vecMoveSpeed.Y == 0.0f) ) {

			return 60;
		}
		else {
			return 30;
		}
	}

	return 60;
}

//----------------------------------------------------------

void CLocalPlayer::SendWastedNotification()
{
	BYTE byteDeathReason;
	BYTE byteWhoWasResponsible;
	byteDeathReason = m_pPlayerPed->FindDeathReasonAndResponsiblePlayer(&byteWhoWasResponsible);
	pNetGame->GetNetSends()->Death(byteDeathReason, byteWhoWasResponsible);
}

//----------------------------------------------------------

void CLocalPlayer::HandleDeath(BYTE byteReason, BYTE byteWhoKilled, BYTE byteScoringModifier)
{
	char * szPlayerName = pNetGame->GetPlayerPool()->GetLocalPlayerName();
	char * szWhoKilledName;

	if(byteWhoKilled != INVALID_PLAYER_ID) {
		szWhoKilledName = pNetGame->GetPlayerPool()->GetPlayerName(byteWhoKilled);
	}

	switch(byteScoringModifier) {

		case VALID_KILL:
			pChatWindow->AddInfoMessage(">> %s was killed by %s",szPlayerName,szWhoKilledName);
			break;

		case TEAM_KILL:
			pChatWindow->AddInfoMessage(">> %s killed team-mate %s",szWhoKilledName,szPlayerName);
			break;

		case SELF_KILL:
			if(byteReason == WEAPON_DROWN) {
				pChatWindow->AddInfoMessage(">> %s drowned",szPlayerName);
			}
			else {
				pChatWindow->AddInfoMessage(">> %s died",szPlayerName);
			}
			break;
	}
}

//----------------------------------------------------------

void CLocalPlayer::RequestClass(BYTE byteClass)
{
	pNetGame->GetNetSends()->RequestClass(byteClass);
}

//----------------------------------------------------------

void CLocalPlayer::SetSpawnInfo(BYTE byteTeam, BYTE byteSkin, VECTOR * vecPos, float fRotation,
		int iSpawnWeapon1, int iSpawnWeapon1Ammo, int iSpawnWeapon2, int iSpawnWeapon2Ammo,
		int iSpawnWeapon3, int iSpawnWeapon3Ammo)
{
	m_SpawnInfo.byteTeam = byteTeam;
	m_SpawnInfo.byteSkin = byteSkin;
	m_SpawnInfo.vecPos.X = vecPos->X;
	m_SpawnInfo.vecPos.Y = vecPos->Y;
	m_SpawnInfo.vecPos.Z = vecPos->Z;
	m_SpawnInfo.fRotation = fRotation;
	m_SpawnInfo.iSpawnWeapons[0] = iSpawnWeapon1;
	m_SpawnInfo.iSpawnWeapons[1] = iSpawnWeapon2;
	m_SpawnInfo.iSpawnWeapons[2] = iSpawnWeapon3;
	m_SpawnInfo.iSpawnWeaponsAmmo[0] = iSpawnWeapon1Ammo;
	m_SpawnInfo.iSpawnWeaponsAmmo[1] = iSpawnWeapon2Ammo;
	m_SpawnInfo.iSpawnWeaponsAmmo[2] = iSpawnWeapon3Ammo;
	m_bHasSpawnInfo = TRUE;
}

//----------------------------------------------------------

BOOL CLocalPlayer::SpawnPlayer()
{
	return SpawnPlayer(m_SpawnInfo.byteTeam,m_SpawnInfo.byteSkin,
		&m_SpawnInfo.vecPos,m_SpawnInfo.fRotation,
		m_SpawnInfo.iSpawnWeapons[0],
		m_SpawnInfo.iSpawnWeaponsAmmo[0],
		m_SpawnInfo.iSpawnWeapons[1],
		m_SpawnInfo.iSpawnWeaponsAmmo[1],
		m_SpawnInfo.iSpawnWeapons[2],
		m_SpawnInfo.iSpawnWeaponsAmmo[2]);
}

//----------------------------------------------------------

BOOL CLocalPlayer::SpawnPlayer( BYTE byteTeam,
							    BYTE byteSkin, 
							    VECTOR * vecPos,
							    float fRotation,int iSpawnWeapon1,
								int iSpawnWeapon1Ammo, int iSpawnWeapon2, 
								int iSpawnWeapon2Ammo, int iSpawnWeapon3,
								int iSpawnWeapon3Ammo )
{
	CPlayerPed *pGamePlayer = pGame->FindPlayerPed();

	if(!pGamePlayer) return FALSE;	

	if(pGamePlayer)
	{
		pGamePlayer->RestartIfWastedAt(vecPos, fRotation);

		m_pPlayerPed = pGamePlayer;

		// Set skin stuff.. logic is because of temperament
		if(pGamePlayer->GetModel() != byteSkin && (byteSkin < 107)) {
			if(!pGame->IsModelLoaded(byteSkin)) {
				pGame->RequestModel(byteSkin);
				while(!pGame->IsModelLoaded(byteSkin)) { Sleep(1); }
			}
			pGamePlayer->SetModel(byteSkin);
		}

		pGamePlayer->ClearAllWeapons();

		if(iSpawnWeapon3 != (-1)) {
			pGamePlayer->GiveWeapon(iSpawnWeapon3,iSpawnWeapon3Ammo);
		}
		if(iSpawnWeapon2 != (-1)) {
			pGamePlayer->GiveWeapon(iSpawnWeapon2,iSpawnWeapon2Ammo);
		}		
		if(iSpawnWeapon1 != (-1)) {
			pGamePlayer->GiveWeapon(iSpawnWeapon1,iSpawnWeapon1Ammo);
		}

		pGamePlayer->Teleport(vecPos->X,vecPos->Y,vecPos->Z);
		pGamePlayer->SetZAngle(fRotation);
		pGamePlayer->SetInitialState();

		m_bIsWasted = FALSE;
		m_bIsActive = TRUE;
	}	

	// Let the rest of the network know we're spawning.
	pNetGame->GetNetSends()->Spawn();
	return TRUE;
}

//----------------------------------------------------------

void CLocalPlayer::Say(PCHAR szText)
{
	if(pNetGame->GetGameState() != GAMESTATE_CONNECTED) {
		pChatWindow->AddInfoMessage("Not connected");
		return;
	}

	pNetGame->GetNetSends()->Chat(szText);
	
	// Process chat message to chat window.
	pChatWindow->AddChatMessage(pNetGame->GetPlayerPool()->GetLocalPlayerName(),
		GetTeamColorAsARGB(),szText);
}

//----------------------------------------------------------

void CLocalPlayer::SendEnterVehicleNotification(BYTE byteVehicleID, BOOL bPassenger)
{
	BYTE bytePassenger=0;
	if(bPassenger) bytePassenger=1;
	pNetGame->GetNetSends()->EnterVehicle(byteVehicleID,bytePassenger);
}

//----------------------------------------------------------

void CLocalPlayer::SendExitVehicleNotification(BYTE byteVehicleID)
{
	pNetGame->GetNetSends()->ExitVehicle(byteVehicleID);
}

//----------------------------------------------------

DWORD CLocalPlayer::GetTeamColorAsRGBA()
{
	return TranslateColorCodeToRGBA(m_SpawnInfo.byteTeam);
}

//----------------------------------------------------

DWORD CLocalPlayer::GetTeamColorAsARGB()
{
	return (TranslateColorCodeToRGBA(m_SpawnInfo.byteTeam) >> 8) | 0xFF000000;	
}

//----------------------------------------------------

