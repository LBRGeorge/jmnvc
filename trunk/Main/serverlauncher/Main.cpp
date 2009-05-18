#include "Main.h"

using namespace std;

static int StartRcon(int argc,char* argv [])
{
    CDynamicLibrary RCon;
    if (RCon.Load(LIB_RCON))
    {
        typedef int(Main_t)(int,char* []);
        Main_t* pfnEntryPoint = reinterpret_cast <Main_t*> ((long long)(RCon.GetProcedureAddress("RunRcon")));
        if (pfnEntryPoint)
        {
            return pfnEntryPoint(argc,argv);
        }
        else
        {
            printf("ERROR: Bad file: %s\n",LIB_RCON);
        }
    }
    else
    {
        printf("ERROR: Could not load %s\n",LIB_RCON);
        printf("* Check installed data files.\n");
#ifdef WIN32
        printf("* Check installed Microsoft Visual C++ 2008 SP1 Redistributable Package (x86).\n");
#endif
    }
	return -1;
}

static int StartServer(int argc,char* argv [])
{
    CDynamicLibrary Core;
    if (Core.Load(LIB_CORE))
    {
        typedef int(Main_t)(int,char* []);
        Main_t* pfnEntryPoint = reinterpret_cast <Main_t*> ((long long)(Core.GetProcedureAddress("Run")));
        if (pfnEntryPoint)
        {
            return pfnEntryPoint(argc,argv);
        }
        else
        {
            printf("ERROR: Bad file: %s\n",LIB_CORE);
        }
    }
    else
    {
        printf("ERROR: Could not load %s\n",LIB_CORE);
        printf("* Check installed data files.\n");
#ifdef WIN32
        printf("* Check installed Microsoft Visual C++ 2008 SP1 Redistributable Package (x86).\n");
#endif
    }
	return -1;
}

int main(int argc,char* argv [])
{
	if (argc > 1){
		if (!strcmp(argv[1],"rcon") || !strcmp(argv[1],"RCON")){
			if (StartRcon(argc,argv)==-1){
				goto EndLaunchKey;
			}
			goto EndLaunch;
		}else{
			printf("Invalid Arguments!\n");
			goto EndLaunchKey;
		}
	}else{
		if (StartServer(argc,argv)==-1){
			goto EndLaunchKey;
		}
		goto EndLaunch;
	}
EndLaunchKey:
    printf ( "Press enter to continue...\n" );
    cin.get();
EndLaunch:
    return 1;
}
