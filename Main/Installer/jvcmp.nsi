!include MUI.nsh
!include LogicLib.nsh
!include WinMessages.nsh
!include FileFunc.nsh

Name "Jack's Vice City Multiplayer - Beta"
OutFile "jvcmp.beta.r37.exe"

XPStyle on
SetCompressor /SOLID /FINAL lzma
Page custom nsDialogsWelcome
Page custom nsDialogsDirectory
InstallDir "$PROGRAMFILES\Rockstar Games\Grand Theft Auto Vice City"
InstallDirRegKey HKLM "Software\JacksMiniNetwork\jvcmp" "GTAVCPath"
RequestExecutionLevel user
ShowInstDetails show

!define MUI_ABORTWARNING
!define MUI_ICON		"app.ico"
!define MUI_UNICON		"bin.ico"

!insertmacro MUI_PAGE_INSTFILES

!insertmacro MUI_LANGUAGE English

!define WS_EX_CLIENTEDGE    0x00000200

!define WS_CHILD            0x40000000
!define WS_VISIBLE          0x10000000
!define WS_DISABLED         0x08000000
!define WS_CLIPSIBLINGS     0x04000000
!define WS_MAXIMIZE         0x01000000
!define WS_VSCROLL          0x00200000
!define WS_HSCROLL          0x00100000
!define WS_GROUP            0x00020000
!define WS_TABSTOP          0x00010000

!define ES_LEFT             0x00000000
!define ES_CENTER           0x00000001
!define ES_RIGHT            0x00000002
!define ES_MULTILINE        0x00000004
!define ES_UPPERCASE        0x00000008
!define ES_LOWERCASE        0x00000010
!define ES_PASSWORD         0x00000020
!define ES_AUTOVSCROLL      0x00000040
!define ES_AUTOHSCROLL      0x00000080
!define ES_NOHIDESEL        0x00000100
!define ES_OEMCONVERT       0x00000400
!define ES_READONLY         0x00000800
!define ES_WANTRETURN       0x00001000
!define ES_NUMBER           0x00002000

!define SS_LEFT             0x00000000
!define SS_CENTER           0x00000001
!define SS_RIGHT            0x00000002
!define SS_ICON             0x00000003
!define SS_BLACKRECT        0x00000004
!define SS_GRAYRECT         0x00000005
!define SS_WHITERECT        0x00000006
!define SS_BLACKFRAME       0x00000007
!define SS_GRAYFRAME        0x00000008
!define SS_WHITEFRAME       0x00000009
!define SS_USERITEM         0x0000000A
!define SS_SIMPLE           0x0000000B
!define SS_LEFTNOWORDWRAP   0x0000000C
!define SS_OWNERDRAW        0x0000000D
!define SS_BITMAP           0x0000000E
!define SS_ENHMETAFILE      0x0000000F
!define SS_ETCHEDHORZ       0x00000010
!define SS_ETCHEDVERT       0x00000011
!define SS_ETCHEDFRAME      0x00000012
!define SS_TYPEMASK         0x0000001F
!define SS_REALSIZECONTROL  0x00000040
!define SS_NOPREFIX         0x00000080
!define SS_NOTIFY           0x00000100
!define SS_CENTERIMAGE      0x00000200
!define SS_RIGHTJUST        0x00000400
!define SS_REALSIZEIMAGE    0x00000800
!define SS_SUNKEN           0x00001000
!define SS_EDITCONTROL      0x00002000
!define SS_ENDELLIPSIS      0x00004000
!define SS_PATHELLIPSIS     0x00008000
!define SS_WORDELLIPSIS     0x0000C000
!define SS_ELLIPSISMASK     0x0000C000

!define BS_PUSHBUTTON       0x00000000
!define BS_DEFPUSHBUTTON    0x00000001
!define BS_CHECKBOX         0x00000002
!define BS_AUTOCHECKBOX     0x00000003
!define BS_RADIOBUTTON      0x00000004
!define BS_3STATE           0x00000005
!define BS_AUTO3STATE       0x00000006
!define BS_GROUPBOX         0x00000007
!define BS_USERBUTTON       0x00000008
!define BS_AUTORADIOBUTTON  0x00000009
!define BS_PUSHBOX          0x0000000A
!define BS_OWNERDRAW        0x0000000B
!define BS_TYPEMASK         0x0000000F
!define BS_LEFTTEXT         0x00000020
!define BS_TEXT             0x00000000
!define BS_ICON             0x00000040
!define BS_BITMAP           0x00000080
!define BS_LEFT             0x00000100
!define BS_RIGHT            0x00000200
!define BS_CENTER           0x00000300
!define BS_TOP              0x00000400
!define BS_BOTTOM           0x00000800
!define BS_VCENTER          0x00000C00
!define BS_PUSHLIKE         0x00001000
!define BS_MULTILINE        0x00002000
!define BS_NOTIFY           0x00004000
!define BS_FLAT             0x00008000
!define BS_RIGHTBUTTON      ${BS_LEFTTEXT}

!define LR_DEFAULTCOLOR     0x0000
!define LR_MONOCHROME       0x0001
!define LR_COLOR            0x0002
!define LR_COPYRETURNORG    0x0004
!define LR_COPYDELETEORG    0x0008
!define LR_LOADFROMFILE     0x0010
!define LR_LOADTRANSPARENT  0x0020
!define LR_DEFAULTSIZE      0x0040
!define LR_VGACOLOR         0x0080
!define LR_LOADMAP3DCOLORS  0x1000
!define LR_CREATEDIBSECTION 0x2000
!define LR_COPYFROMRESOURCE 0x4000
!define LR_SHARED           0x8000

!define IMAGE_BITMAP        0
!define IMAGE_ICON          1
!define IMAGE_CURSOR        2
!define IMAGE_ENHMETAFILE   3

Var DIALOG
Var HEADLINE
Var TEXT
Var IMAGECTL
Var IMAGE
Var DIRECTORY
Var FREESPACE

Var HEADLINE_FONT

Function .onInit

	CreateFont $HEADLINE_FONT "$(^Font)" "14" "700"

	InitPluginsDir
	File /oname=$PLUGINSDIR\welcome.bmp "${NSISDIR}\Contrib\Graphics\Wizard\gtavc.bmp"

FunctionEnd

Function HideControls

    LockWindow on
    GetDlgItem $0 $HWNDPARENT 1028
    ShowWindow $0 ${SW_HIDE}

    GetDlgItem $0 $HWNDPARENT 1256
    ShowWindow $0 ${SW_HIDE}

    GetDlgItem $0 $HWNDPARENT 1035
    ShowWindow $0 ${SW_HIDE}

    GetDlgItem $0 $HWNDPARENT 1037
    ShowWindow $0 ${SW_HIDE}

    GetDlgItem $0 $HWNDPARENT 1038
    ShowWindow $0 ${SW_HIDE}

    GetDlgItem $0 $HWNDPARENT 1039
    ShowWindow $0 ${SW_HIDE}

    GetDlgItem $0 $HWNDPARENT 1045
    ShowWindow $0 ${SW_NORMAL}
    LockWindow off

FunctionEnd

Function ShowControls

    LockWindow on
    GetDlgItem $0 $HWNDPARENT 1028
    ShowWindow $0 ${SW_NORMAL}

    GetDlgItem $0 $HWNDPARENT 1256
    ShowWindow $0 ${SW_NORMAL}

    GetDlgItem $0 $HWNDPARENT 1035
    ShowWindow $0 ${SW_NORMAL}

    GetDlgItem $0 $HWNDPARENT 1037
    ShowWindow $0 ${SW_NORMAL}

    GetDlgItem $0 $HWNDPARENT 1038
    ShowWindow $0 ${SW_NORMAL}

    GetDlgItem $0 $HWNDPARENT 1039
    ShowWindow $0 ${SW_NORMAL}

    GetDlgItem $0 $HWNDPARENT 1045
    ShowWindow $0 ${SW_HIDE}
    LockWindow off

FunctionEnd

Function nsDialogsWelcome

	nsDialogs::Create 1044
	Pop $DIALOG

	nsDialogs::CreateControl STATIC ${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS}|${SS_BITMAP} 0 0 0 109u 193u ""
	Pop $IMAGECTL

	StrCpy $0 $PLUGINSDIR\welcome.bmp
	System::Call 'user32::LoadImage(i 0, t r0, i ${IMAGE_BITMAP}, i 0, i 0, i ${LR_LOADFROMFILE}) i.s'
	Pop $IMAGE
	
	SendMessage $IMAGECTL ${STM_SETIMAGE} ${IMAGE_BITMAP} $IMAGE

	nsDialogs::CreateControl STATIC ${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS} 0 120u 10u -130u 20u "Welcome to JVCMP!"
	Pop $HEADLINE

	SendMessage $HEADLINE ${WM_SETFONT} $HEADLINE_FONT 0

	nsDialogs::CreateControl STATIC ${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS} 0 120u 32u -130u -32u "This installation wizard will install JVCMP to your computer please make sure there are no GTA Vice City mods installed or they may conflict and cause the game to crash, it is recomended that you quit this setup and re-install GTA Vice City if you have modifyed it in any way."
	Pop $TEXT

	SetCtlColors $DIALOG "" 0xffffff
	SetCtlColors $HEADLINE "" 0xffffff
	SetCtlColors $TEXT "" 0xffffff

	Call HideControls

	nsDialogs::Show

	Call ShowControls

	System::Call gdi32::DeleteObject(i$IMAGE)

FunctionEnd

!define SHACF_FILESYSTEM 1

Function nsDialogsDirectory

	!insertmacro MUI_HEADER_TEXT "Choose Install Location" "Choose the folder in which to install $(^NameDA)."

	GetDlgItem $0 $HWNDPARENT 1
	EnableWindow $0 0

	nsDialogs::Create 1018
	Pop $DIALOG

	nsDialogs::CreateControl STATIC ${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS}|${SS_CENTER} 0 0 0 100% 30 "Directory page"
	Pop $HEADLINE

	SendMessage $HEADLINE ${WM_SETFONT} $HEADLINE_FONT 0

	nsDialogs::CreateControl STATIC ${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS} 0 0 30 100% 40 "Select the installation directory of a clean GTA Vice City to continue. The 'Install' button wont work until the installation is passed by the file check. $_CLICK"
	Pop $TEXT

	nsDialogs::CreateControl EDIT ${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS}|${ES_AUTOHSCROLL}|${WS_TABSTOP} ${WS_EX_CLIENTEDGE} 0 75 100% 12u "$PROGRAMFILES\Rockstar Games\Grand Theft Auto Vice City"
	Pop $DIRECTORY

	SendMessage $HWNDPARENT ${WM_NEXTDLGCTL} $DIRECTORY 1

	GetFunctionAddress $0 DirChange
	nsDialogs::OnChange $DIRECTORY $0

	System::Call shlwapi::SHAutoComplete(i$DIRECTORY,i${SHACF_FILESYSTEM})

	nsDialogs::CreateControl STATIC ${WS_VISIBLE}|${WS_CHILD}|${WS_CLIPSIBLINGS} 0 0 -10u 100% 10u ""
	Pop $FREESPACE

	Call UpdateFreeSpace
	Call DirChange

	nsDialogs::Show

FunctionEnd

Function UpdateFreeSpace

	${GetRoot} $INSTDIR $0
	StrCpy $1 " bytes"

	System::Call kernel32::GetDiskFreeSpaceEx(tr0,*l,*l,*l.r0)

	${If} $0 > 1024
	${OrIf} $0 < 0
		System::Int64Op $0 / 1024
		Pop $0
		StrCpy $1 "kb"
		${If} $0 > 1024
		${OrIf} $0 < 0
			System::Int64Op $0 / 1024
			Pop $0
			StrCpy $1 "mb"
			${If} $0 > 1024
			${OrIf} $0 < 0
				System::Int64Op $0 / 1024
				Pop $0
				StrCpy $1 "gb"
			${EndIf}
		${EndIf}
	${EndIf}

	SendMessage $FREESPACE ${WM_SETTEXT} 0 "STR:Free space: $0$1"

FunctionEnd

Function DirChange

	Pop $0 # dir hwnd

	GetDlgItem $0 $HWNDPARENT 1

	System::Call user32::GetWindowText(i$DIRECTORY,t.d,i${NSIS_MAX_STRLEN})

	${If} ${FileExists} $INSTDIR\gta-vc.exe
		${If} ${FileExists} $INSTDIR\mss32.dll
			EnableWindow $0 1
		${Else}
			EnableWindow $0 0
		${EndIf}
	${Else}
		EnableWindow $0 0
	${EndIf}

	Call UpdateFreeSpace

FunctionEnd

Section "Uninstall"
  Delete "$INSTDIR\jmnvc.asi"
  Delete "$INSTDIR\lua5.1.dll"
  Delete "$INSTDIR\vcmpfnt.dat"
  Delete "$INSTDIR\vcmpfnt.png"
  Delete "$INSTDIR\data\jmnvc.scm"
  Delete "$INSTDIR\JvcmpClientUninstall.exe"
  DeleteRegKey /ifempty HKLM "Software\JacksMiniNetwork"
  DeleteRegKey /ifempty HKLM "Software\JacksMiniNetwork\jvcmp"
SectionEnd

Section "Client and Data Files" SecClient

  ;Remove reg keys if empty
  DeleteRegKey /ifempty HKCU "Software\JacksMiniNetwork"
  DeleteRegKey /ifempty HKCU "Software\JacksMiniNetwork\jvcmp"

  ;Store installation folder
  WriteRegStr HKLM "Software\JacksMiniNetwork\jvcmp" "GTAVCPath" $INSTDIR
  WriteRegStr HKLM "Software\JacksMiniNetwork\jvcmp" "InstallFolder" $INSTDIR

  ;Install Client
  SetOutPath "$INSTDIR"
  File "..\output\jmnvc.asi"
  File "..\output\lua5.1.dll"
  File "..\output\vcmpfnt.png"
  File "..\output\vcmpfnt.dat"
  SetOutPath "$INSTDIR\data"
  File "..\output\data\jmnvc.scm"
  ;End of Client

  ;Install Server
  SetOutPath "$INSTDIR\server"
  File "..\output\Server.exe"
  File "..\output\core.dll"
  File "..\output\rcon.dll"
  File "..\output\lua5.1.dll"
  File "..\output\remoteconsole.bat"
  SetOutPath "$INSTDIR\server\mods"
  File "..\output\mods\server.ini"
  File "..\output\mods\server.lua"
  ;End of Server
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\JvcmpClientUninstall.exe"
SectionEnd
