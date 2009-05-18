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
	"",			L"JACKS MULTIPLAYER!!",
	"!WELC_0",	L"Welcome",
	"LOADCOL",	L"Loading Jack's Multiplayer...",
	"MAP_YAH",	L"LOCAL PLAYER",
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
	for (int i=1; i<array_size(GxtEntries); i++)
	{
		if (strcmpi(GxtLabel, GxtEntries[i].GxtLabel) == 0)
		{
			return GxtEntries[i].GxtText;
		}
	}
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
		cmp   byte ptr [eax], 'F'	// Test first letter of GxtLabel for '!'...
		jnz   nohook				// If it's not, don't hook this call.
		push  ebx					// Reconstructed.
		push  ebp
		mov   ebx, ecx
		sub   esp, 0x40
		jmp   ReadGXTString			// Jump to real ReadGxtString function.
	nohook:
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