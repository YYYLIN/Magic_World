#ifndef _MAGICENGINEAPI_H_
#define _MAGICENGINEAPI_H_


#include "MagicEngineContext.h"
#include "MagicUIMacro.h"

namespace Magic
{
	DLL_MAGICUI_OUTPUT_INPUT bool CreateSystemUI(const wchar_t* _name, int _x, int _y, int _w, int _h);

	DLL_MAGICUI_OUTPUT_INPUT void ShutdownSystemUI();

	DLL_MAGICUI_OUTPUT_INPUT void RunSystem();

	DLL_MAGICUI_OUTPUT_INPUT void RequestOuitSystem();

	DLL_MAGICUI_OUTPUT_INPUT MagicScene* GetSystemScenes(SYSTEMSCENES _SYSTEMSCENES);

	DLL_MAGICUI_OUTPUT_INPUT void SetCallbackMessage_WIN32(CallbackMessage_WIN32 _CallbackMessage_WIN32);

	DLL_MAGICUI_OUTPUT_INPUT HWND GetWindowHWND();

	DLL_MAGICUI_OUTPUT_INPUT HINSTANCE GetWindowHINSTANCE();
}


#endif