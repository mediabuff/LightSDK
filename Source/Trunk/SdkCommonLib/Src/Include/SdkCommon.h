/*!
* @file SdkCommon.h 
* 
* @brief This file defines the common module header file,
*        which shall be used for the applications.
* 
* Copyright (C) 2010, LZT Corporation.
* 
* @author Li Hong
* @date 2010/12/20
*/

#ifdef __cplusplus
#ifndef _SDKCOMMON_H_
#define _SDKCOMMON_H_


//////////////////////////////////////////////////////////////////////////
//
// Include section
//
//////////////////////////////////////////////////////////////////////////

#include <Windows.h>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <tchar.h>
#include <objbase.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <UIAnimation.h>
#include <wincodec.h>
#include <dwrite.h>
#include <shobjidl.h>
#include <oleidl.h>
#include <strsafe.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <propvarutil.h>
#include <structuredquery.h>
#include <knownfolders.h>
#include <propkey.h>
#include <MMSystem.h>
#include <WindowsX.h>
#include <GdiPlus.h>

using namespace std;
using namespace D2D1;
using namespace Gdiplus;


//////////////////////////////////////////////////////////////////////////
//
// Library section
//
//////////////////////////////////////////////////////////////////////////


#pragma comment(lib, "windowscodecs.lib")   // For Windows Imaging Component.
#pragma comment(lib, "strsafe.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "structuredquery.lib")
#pragma comment(lib, "propsys.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Netapi32.lib")
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "comctl32.lib")


#pragma warning(disable : 4251)
#pragma warning(disable : 4275)
#pragma warning(disable : 4995)     // Functions were marked as #pragma deprecated.



//////////////////////////////////////////////////////////////////////////
//
// Inline functions section
//
//////////////////////////////////////////////////////////////////////////


// Retrieve the HINSTANCE for the current DLL or EXE using this symbol that
// the linker provides for every module, avoids the need for a global HINSTANCE variable
// and provides access to this value for static libraries
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
__inline HINSTANCE GetModuleHINSTANCE()
{
    return (HINSTANCE)&__ImageBase;
} // GetModuleHINSTANCE



//////////////////////////////////////////////////////////////////////////
// 
// The defines switch of flags dllexport and dllimport
//
//////////////////////////////////////////////////////////////////////////

// Defines the switch of DLL export and import

#ifdef COMMON_EXPORT
#   define CLASS_DECLSPEC __declspec(dllexport)
#   define EXPIMP_TEMPLATE
#else
#   define CLASS_DECLSPEC __declspec(dllimport)
#   define EXPIMP_TEMPLATE extern
#endif



//////////////////////////////////////////////////////////////////////////
//
// The linker version.
//
//////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


#endif // _SDKCOMMON_H_
#endif // __cplusplus
