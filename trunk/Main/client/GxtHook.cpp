
// Includes
#include <windows.h>
#include <process.h>
#include "GxtHook.h"
#include "CDynamicLibrary.h"

bool InstallGXTHook(void)
{
    /*CDynamicLibrary Strings;
    if (Strings.Load(LIB_STRINGS))
    {
        typedef int(Main_t)(void);
        Main_t* pfnEntryPoint = reinterpret_cast <Main_t*> ((long long)(Strings.GetProcedureAddress("InitHook")));
        if (pfnEntryPoint)
        {
			if (pfnEntryPoint()==1){
	            return true;
			}
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }*/
	return true;
}