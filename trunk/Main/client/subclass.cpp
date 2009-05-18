#include "main.h"
#include "screenshot.h"
#include <string>

extern CNetGame			*pNetGame;
extern CGame			*pGame;
extern CChatWindow		*pChatWindow;
extern CCmdWindow		*pCmdWindow;
extern IDirect3DDevice8 *pD3DDevice;
extern BOOL				bShowNameTags;

WNDPROC hOldProc;
LRESULT APIENTRY NewWndProc(HWND,UINT,WPARAM,LPARAM);
void GetScreenshotFileName(std::string& FileName);

//----------------------------------------------------

BOOL HandleKeyPress(DWORD vKey) 
{
	switch(vKey) {

		case VK_F4:
			{
				if(bShowNameTags)
				{
					bShowNameTags = FALSE;
					break;
				}
				else
				{
					bShowNameTags = TRUE;
					break;
				}
			}

		case VK_F7:			
			pChatWindow->ToggleEnabled();
			break;

		case VK_F8:
			{
				CScreenshot ScreenShot(pD3DDevice);
				std::string sFileName;
				GetScreenshotFileName(sFileName);
				if(ScreenShot.TakeScreenShot((PCHAR)sFileName.c_str())) {
					pChatWindow->AddInfoMessage("Screenshot Taken - %s",sFileName.c_str());
				} else {
					pChatWindow->AddInfoMessage("Unable to take a screenshot");
				}
			}
			break;

		case VK_RETURN:
			pCmdWindow->ProcessInput();
			break;
	}

	return FALSE;
}

//----------------------------------------------------

BOOL HandleCharacterInput(DWORD dwChar)
{
	if(pCmdWindow->isEnabled()) {
		if(dwChar == 8) { // backspace
			pCmdWindow->BackSpace();
			return TRUE;
		}
		else if(dwChar == '`') {
			pCmdWindow->Disable();
			return TRUE;
		}
		pCmdWindow->AddChar((char)dwChar);
		return TRUE;
	}
	else {
		switch(dwChar) {
			case '`':
			case 't':
			case 'T':
				pCmdWindow->Enable();
				return TRUE;
		}
	}
	return FALSE;
}

//----------------------------------------------------

BOOL SubclassGameWindow()
{
	HWND hwndGameWnd = pGame->GetMainWindowHwnd();
	
	if(hwndGameWnd) {
		hOldProc = (WNDPROC)GetWindowLong(hwndGameWnd,GWL_WNDPROC);
		SetWindowLong(hwndGameWnd,GWL_WNDPROC,(LONG)NewWndProc);
		return TRUE;
	}
	return FALSE;
}

static void DoubleCarSpeed(void)
{
	CPlayerPed* pLocalPlayer = pGame->FindPlayerPed();
	if (pLocalPlayer->IsInVehicle())
	{
		CVehiclePool* pVehiclePool = pNetGame->GetVehiclePool();
		CVehicle* pVehicle = pVehiclePool->GetAt(pVehiclePool->FindIDFromGtaPtr(pLocalPlayer->GetGtaVehicle()));
		pVehicle->DoubleSpeed();
	}
}

static void VehicleJump(void)
{
	CPlayerPed* pLocalPlayer = pGame->FindPlayerPed();
	if (pLocalPlayer->IsInVehicle())
	{
		CVehiclePool* pVehiclePool = pNetGame->GetVehiclePool();
		CVehicle* pVehicle = pVehiclePool->GetAt(pVehiclePool->FindIDFromGtaPtr(pLocalPlayer->GetGtaVehicle()));
		VECTOR vecMoveSpeed;
		vecMoveSpeed.X = 0;
		vecMoveSpeed.Y = 0;
		vecMoveSpeed.Z = 0;
		pVehicle->SetTurnSpeedVector(vecMoveSpeed);
		pVehicle->VehicleJump(0.25, 0.5);
	}
}

static void InstantStop(void)
{
	CPlayerPed* pLocalPlayer = pGame->FindPlayerPed();
	if (pLocalPlayer->IsInVehicle())
	{
		CVehiclePool* pVehiclePool = pNetGame->GetVehiclePool();
		CVehicle* pVehicle = pVehiclePool->GetAt(pVehiclePool->FindIDFromGtaPtr(pLocalPlayer->GetGtaVehicle()));
		VECTOR vecMoveSpeed;
		vecMoveSpeed.X = 0;
		vecMoveSpeed.Y = 0;
		vecMoveSpeed.Z = 0;
		pVehicle->SetMoveSpeedVector(vecMoveSpeed);
		pVehicle->SetTurnSpeedVector(vecMoveSpeed);
	}
}

static void InvertSpeed(void)
{
	CPlayerPed* pLocalPlayer = pGame->FindPlayerPed();
	if (pLocalPlayer->IsInVehicle())
	{
		CVehiclePool* pVehiclePool = pNetGame->GetVehiclePool();
		CVehicle* pVehicle = pVehiclePool->GetAt(pVehiclePool->FindIDFromGtaPtr(pLocalPlayer->GetGtaVehicle()));
		VECTOR vecMoveSpeed;
		pVehicle->GetMoveSpeedVector(&vecMoveSpeed);
		vecMoveSpeed.X *= -1;
		vecMoveSpeed.Y *= -1;
		vecMoveSpeed.Z *= -1;
		pVehicle->SetMoveSpeedVector(vecMoveSpeed);
	}
}

static void Fix(void)
{
	CPlayerPed* pLocalPlayer = pGame->FindPlayerPed();
	if (pLocalPlayer->IsInVehicle())
	{
		CVehiclePool* pVehiclePool = pNetGame->GetVehiclePool();
		CVehicle* pVehicle = pVehiclePool->GetAt(pVehiclePool->FindIDFromGtaPtr(pLocalPlayer->GetGtaVehicle()));
		pVehicle->SetHealth(5000);
	}
}

//----------------------------------------------------

LRESULT APIENTRY NewWndProc( HWND hwnd,UINT uMsg,
							 WPARAM wParam,LPARAM lParam ) 
{ 
	switch(uMsg) {
		case WM_KEYDOWN:
			switch (wParam) {
				case 50: //2
				DoubleCarSpeed();
				return 0;
				break;
				case 51: //3
				InstantStop();
				return 0;
				break;
				case 53: //5
				InvertSpeed();
				return 0;
				break;
				case 54: //6
				Fix();
				return 0;
				break;
				case 55: //7
				VehicleJump();
				return 0;
				break;
			}
			break;
		case WM_KEYUP:
			if(HandleKeyPress((DWORD)wParam)) { // 'I' handled it.
				return 0;
			}
			break;
		case WM_CHAR:
			if(HandleCharacterInput((DWORD)wParam)) { // 'I' handled it.
				return 0;
			}
			break;
	}
	return CallWindowProc(hOldProc,hwnd,uMsg,wParam,lParam);
}

//----------------------------------------------------

void GetScreenshotFileName(std::string & FileName)
{
	std::string ModuleFileName;
    ModuleFileName.reserve(MAX_PATH);
    GetModuleFileName(NULL,(char *)(ModuleFileName.data()),MAX_PATH);
    FileName = ModuleFileName.substr(0, ModuleFileName.find_last_of(":\\"));

    char Buf[MAX_PATH];
    WIN32_FIND_DATA ffd;
    HANDLE h;
    for (int i = 0; i < 1000; i++)
    {
        wsprintf(Buf, (FileName + "vc-mp-%03i.jpg").c_str(), i);
        h = FindFirstFile(Buf, &ffd);
        if(h != INVALID_HANDLE_VALUE) {   
			FindClose(h);
		}
        else {   
			break;
		}
    }
    FileName = Buf;
}

//----------------------------------------------------
// EOF