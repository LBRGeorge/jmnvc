#include "CDynamicLibrary.h"
#include <cstdio>
#include <iostream>

using namespace std;

#define LIB_CORE "core.dll"

#ifdef WIN32
    #include <windows.h>
#endif

int main ( int argc, char* argv [] )
{
    CDynamicLibrary Core;
    if (Core.Load(LIB_CORE))
    {
        typedef int(Main_t)(int,char* []);
        Main_t* pfnEntryPoint = reinterpret_cast <Main_t*> ((long long)(Core.GetProcedureAddress("Run")));
        if (pfnEntryPoint)
        {
            return pfnEntryPoint ( argc, argv );
        }
        else
        {
            printf ( "ERROR: Bad file: %s\n", LIB_CORE );
        }
    }
    else
    {
        printf ( "ERROR: Could not load %s\n", LIB_CORE );
        printf ( "* Check installed data files.\n" );
#ifdef WIN32
        printf ( "* Check installed Microsoft Visual C++ 2008 SP1 Redistributable Package (x86).\n" );
#endif
    }

    // Wait for a key then exit
    printf ( "Press enter to continue...\n" );
    cin.get ();
    return 1;
}
