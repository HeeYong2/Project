#ifndef Engine_Defines_h__
#define Engine_Defines_h__
#pragma warning (disable : 4251)

#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <time.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <Windows.h>
using namespace std;

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "Engine_Typedef.h"
#include "Engine_Constant.h"
#include "Engine_Struct.h"
#include "Engine_Macro.h"
#include "Engine_Function.h"
#include "Engine_Functor.h"

//Sound
#include <MMSystem.h>
#include <dsound.h>
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dsound.lib")

extern HWND		 g_hWnd;
extern HINSTANCE g_hInstance;

#endif // Engine_Defines_h__


