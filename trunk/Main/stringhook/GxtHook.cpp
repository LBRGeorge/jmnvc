/*
	file:
		GxtHook.cpp
	desc:
		This file contains the code for hooking the Vice City game-text
		reading funtion so we can optionally pass back full game-strings
		of our own for use in the missions.
		Made by spookie.
*/

// Includes
#include "main.h"

// Defines
FARPROC ReadGXTString		= (FARPROC)0x584F37;	// 0x584F57
#define array_size(a) (sizeof((a))/sizeof(*(a)))

// GxtEntries is an array of custom GXT file entries.
// They MUST start with an '!' because the hook doesn't
// test GxtLabels that don't start with it.
spookie_dot_gxt GxtEntries[] =
{
	"",		L"Jack's Multiplayer",
	"!WELC_0",	L"Welcome",
	"MAP_YAH",	L"LOCAL PLAYER",
	"BUSTED",	L"BUSTED!", //Should not be able to be busted since disabled
	"DEAD",		L"YOU DIED!",
	"FEC_SM3",	L"Disconnect (R3 button)",
	"FEC_SMT",	L"Disconnect",
	"FEC_SUB",	L"Disconnect",
	"FEH_SGA",	L"CRASH GAME", //START NEW GAME
	"FEN_STA",	L"CRASH GAME", //START GAME
	"FEN_STG",	L"CRASH GAME", //Start Game
	"FEP_QUI",	L"CRASH GAME", //Quit Game
	"FEP_RES",	L"Stay Connected", //Resume
	"LOADCOL",	L"Loading Jack's Multiplayer...",
	"FELD_WR",	L"Loading Jack's Multiplayer...",
	"FEM_MM",	L"Jack's Multiplayer",
	"FET_PAU",	L"Jack's Multiplayer",
	"FEQ_SRE",	L"This will crash your client and cause a dont send error report if you have them, it is better to type '/quit' in the chat box. Proceed?", //Are you sure you want to quit? All progress since the last save game will be lost. Proceed?
	"FEQ_SRW",	L"This will crash your client and cause a dont send error report if you have them, it is better to type '/quit' in the chat box. Proceed?", //Are you sure you want to quit? All progress since the last save game will be lost. Proceed?
	"GA_1",		L"You cannot repair emergency vehicles!",
	"GA_10",	L"Reward $~1~",
	"GA_12",	L"Bomb Activated",
	"GA_14",	L"Whoa your client crashed!", //Not needed really :P
	"GA_15",	L"Nice Colour!", //TODO: Hook this and send message to resync colour
	"GA_16",	L"You didnt need to repair your car!",
	"GA_1A",	L"This server blocks the Pay And Spray.",
	"GA_2",		L"Engine repaired, tires fixed!",
	"GA_21",	L"Someone is already inside the hideout!",
	"GA_21",	L"Someone is already inside the hideout!",
	"GA_3",		L"$100 to respray!",
	"GA_4",		L"$500 for a car bomb!",
	"GA_5",		L"You can only have 1 bomb per vehicle!",
	"GOODBOY",	L"Stop memory hacking me!", //Raised when you killed people police chase
	"HEY",		L"Stop memory hacking me!", //Raised when you lost your gang members
	"WIN_95",	L"Jack's Multiplayer does not work on Windows 95.",
	"WIN_DX",	L"Jack's Multiplayer requires at least DirectX version 8.1",
	"WIN_VDM",	L"Jack's Multiplayer cannot find enough available video memory",
	"WIN_TTL",	L"Jack's Multiplayer",
	"WANT_L",	L"", //Prevent wanted level message (Blank prevents the info box displaying)

	"CHEAT1",	L"Cheat activated", //Should not be able to cheat since disabled
	"CHEAT2",	L"Cheat activated", //Should not be able to cheat since disabled
	"CHEAT3",	L"Cheat activated", //Should not be able to cheat since disabled
	"CHEAT4",	L"Cheat activated", //Should not be able to cheat since disabled
	"CHEAT5",	L"Cheat activated", //Should not be able to cheat since disabled
	"CHEAT6",	L"Cheat activated", //Should not be able to cheat since disabled
	"CHEAT7",	L"Cheat activated", //Should not be able to cheat since disabled
};

// Functions

//--------------------------------------------------------------------------------------------
//	func:
//		ReadGXTStringHooked()
//
//	desc:
//		This function gets called only from ReadGXTStringHook() - it returns a widechar
//		string relating to the passed label. See spookie_dot_gxt structure.
//
//	params:
//		GxtLabel
//			Pointer to a string containing the short version of the game-text string.
//
wchar_t* ReadGXTStringHooked(char* GxtLabel)
{
	//Check we want to handle it
	for (int i=1; i<array_size(GxtEntries); i++)
	{
		if (strcmpi(GxtLabel, GxtEntries[i].GxtLabel) == 0)
		{
			return GxtEntries[i].GxtText;
		}
	}

	//Let the origional game handle it
	__asm
	{
		push  ebx
		push  ebp
		mov   ebx, ecx
		sub   esp, 0x40
		jmp   ReadGXTString
	}

	//Otherwise if we fail read the default text
	return GxtEntries[0].GxtText;
}

//--------------------------------------------------------------------------------------------
//	func:
//		ReadGXTStringHook()
//
//	desc:
//		This function gets called when the game tries to read a game-text string.
//
//		We have the opportunity to pass back a string of our own or let the game search for
//		the string in the loaded *.gxt file at this point. I decided to prefix all my text
//		with an exclamation mark - this way we know the string is ours before looking it
//		up in the internal array of game strings.
//
//	params:
//		n/a - ish
//		We access the passed GxtLabel via [esp+4] (StackPtr+4), also note the return
//		of this function must balance the stack (retn 4) if we don't pass the call
//		on to the real game-string reading function.
//
void __declspec(naked) ReadGXTStringHook()
{
	__asm
	{
		mov   eax, [esp+0x4]		// Move GxtLabel pointer into eax.
		push  dword ptr [esp+0x4]	// Push the GxtLabel.
		call  ReadGXTStringHooked	// Call hooked ReadGxtString function.
		add   esp, 0x4				// Balance the stack.
		retn  0x4					// Return.
	}
}

int InstallGXTHook(void)
{
	DWORD ReadGXTStringHook_addr = 0x584F35;	// 0x584F55
	BYTE* VerCheck = (BYTE*)0x608578;
	if (*VerCheck == 0x81)	// If this is Vice City version 1.1...
	{
				ReadGXTString			= (FARPROC)0x584F57;
				ReadGXTStringHook_addr	= 0x584F55;
	}
	DWORD dwProt = 0;
	DWORD dwOffset = (DWORD)&ReadGXTStringHook - ReadGXTStringHook_addr;
	if (!VirtualProtect((void*)(ReadGXTStringHook_addr-5), 7, PAGE_EXECUTE_READWRITE, &dwProt)) { return 0; }
	*(BYTE*)(ReadGXTStringHook_addr-5) = 0xE9;
	*(DWORD*)(ReadGXTStringHook_addr-4) = dwOffset;
	*(WORD*)ReadGXTStringHook_addr = 0x9090;
	if (!VirtualProtect((void*)(ReadGXTStringHook_addr-5), 7, dwProt, &dwProt)) { return 0; }
	return 1;
}