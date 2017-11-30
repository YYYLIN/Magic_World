#include "MagicUIAPI.h"
#include "MagicWindows.h"


namespace Magic
{
	bool CreateSystemUI(const wchar_t* _name, int _x, int _y, int _w, int _h)
	{
		if (MagicWindows::pMagicWindows)
			return false;
		MagicWindows::pMagicWindows = new MagicWindows;

		return MagicWindows::pMagicWindows->Initialize(_name, _x, _y, _w, _h);
	}

	void ShutdownSystemUI()
	{
		if (MagicWindows::pMagicWindows)
		{
			delete MagicWindows::pMagicWindows;
			MagicWindows::pMagicWindows = 0;
		}
	}

	void RunSystem()
	{
		MagicWindows::pMagicWindows->Run();
	}

	void RequestOuitSystem()
	{
		MagicWindows::pMagicWindows->RequestOuitSystem();
	}

	MagicScene* GetSystemScenes(SYSTEMSCENES _SYSTEMSCENES)
	{
		return MagicWindows::pMagicWindows->GetSystemScenes(_SYSTEMSCENES);
	}

	void SetCallbackMessage_WIN32(CallbackMessage_WIN32 _CallbackMessage_WIN32)
	{
		MagicWindows::pMagicWindows->SetCallbackMessage_WIN32(_CallbackMessage_WIN32);
	}

	HWND GetWindowHWND()
	{
		return MagicWindows::pMagicWindows->GetHWND();
	}

	HINSTANCE GetWindowHINSTANCE()
	{
		return MagicWindows::pMagicWindows->GetHINSTANCE();
	}
}