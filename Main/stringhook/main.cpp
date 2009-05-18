
#include "main.h"

JVCMPEXPORT int InitHook(void)
{
	InstallGXTHook();
	return 1;
}