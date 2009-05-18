#pragma once

#include "address.h"
#include "common.h"
#include "vehicle.h"
#include "playerped.h"
#include "camera.h"
#include "scripting.h"

//-----------------------------------------------------------

class CGame
{
public:
	

	CPlayerPed *NewPlayer(int iPlayerID, int iModel,float fPosX,float fPosY,float fPosZ,float fRotation);
	CVehicle *NewVehicle(int iType,float fPosX,float fPosY,float fPosZ,float fRotation);
		
	int		GetWeaponModelFromWeapon(int iWeaponID);
	void	ToggleKeyInputsDisabled(BOOL bDisable);
	void	StartGame(BOOL NewspapersDisabled);
	BOOL	IsMenuActive();
	void	RequestModel(int iModelID);
	void	LoadRequestedModels();
	BOOL	IsModelLoaded(int iModelID);
	void	DisplayHud(BOOL bSwitch);
	void	ToggleFrameLimiterState(BOOL bState);
		
	DWORD	GetD3DDevice() { return *(DWORD *)ADDR_ID3D8DEVICE; };
	void	SetD3DDevice(DWORD pD3DDevice) { *(DWORD *)ADDR_ID3D8DEVICE = pD3DDevice; };
	DWORD	GetD3D() { return *(DWORD *)ADDR_ID3D8DEVICE; };
	void	SetD3D(DWORD pD3D) {	*(DWORD *)ADDR_ID3D8 = pD3D; };
	HWND	GetMainWindowHwnd() { return *(HWND *)ADDR_HWND; };
	void	DisplayTextMessage(PCHAR szText);

	void	ChangeTime(int iHour, int iMinute);
	void	SelectInterior(int iInterior);

	//-----------------------------------------------------------

	CPlayerPed  *FindPlayerPed() {
		if(m_pInternalPlayer==NULL)	m_pInternalPlayer = new CPlayerPed();
		return m_pInternalPlayer;
	};
	
	CCamera     *GetCamera() {	return m_pInternalCamera; };

	//-----------------------------------------------------------

	void EnablePassengerEngineAudio() {
		*(BYTE *)0x5F2175 = 0x3B;
		*(BYTE *)0x5F2176 = 0xC2;
	};

	void DisablePassengerEngineAudio() {
		*(BYTE *)0x5F2175 = 0x39;
		*(BYTE *)0x5F2176 = 0xF0;
	};

	//-----------------------------------------------------------

	CGame();
	~CGame() {};

private:

	CCamera			*m_pInternalCamera;
	CPlayerPed		*m_pInternalPlayer;
};

//-----------------------------------------------------------
