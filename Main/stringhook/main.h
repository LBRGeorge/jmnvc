#pragma once

#ifdef WIN32
#include <stdio.h>
#include <windows.h>
#else
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include "GxtHook.h"

#if WIN32
	#define JVCMPEXPORT extern "C" __declspec(dllexport)
#else
	#define JVCMPEXPORT extern "C"
#endif

#include <iostream>
#define  _us2ms(x) (x*1000)
