#include "MagicUIAPI.h"
#include "MagicWindows.h"
#include "Include/MagicEngineAPI.h"

namespace Magic
{
	bool CreateSystemUI(const wchar_t* _name, int _x, int _y, int _w, int _h)
	{
		if (MagicWindows::Instance())
		{
			SetEngineErrorMessage("ÒÑ´´½¨\n");
			return false;
		}

		new MagicWindows;

		return MagicWindows::Instance()->Initialize(_name, _x, _y, _w, _h);
	}

	void ShutdownSystemUI()
	{
		if (MagicWindows::Instance())
			delete MagicWindows::Instance();
	}

	void RequestOuitSystem()
	{
		MagicWindows::Instance()->RequestOuitSystem();
	}

	void SetCallbackMessage_WIN32(CallbackMessage_WIN32 _CallbackMessage_WIN32)
	{
		MagicWindows::Instance()->SetCallbackMessage_WIN32(_CallbackMessage_WIN32);
	}

	void SetWindowICO(HICON _ico)
	{
		::SendMessage(MagicWindows::Instance()->GetHWND(), WM_SETICON, TRUE, (LPARAM)(_ico));
	}

	HWND GetWindowHWND()
	{
		return MagicWindows::Instance()->GetHWND();
	}

	HINSTANCE GetWindowHINSTANCE()
	{
		return MagicWindows::Instance()->GetHINSTANCE();
	}
}