#include "MagicUIAPI.h"
#include "MainScene.h"
#include "Render/MagicEngineAPI.h"
#include <tchar.h>


int CALLBACK _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevainstance, LPWSTR lpcmdline, int iCmdshow)
{
	bool _result = Magic::CreateSystemUI(L"WaterMagic", 0, 0, 1024, 768);
	if (!_result)
	{
		MessageBoxA(NULL, Magic::GetEngineErrorMessage(), "´íÎó", MB_OK);
		return false;
	}

	HICON _ico = (HICON)::LoadImageA(NULL, 0, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	Magic::SetWindowICO(_ico);

	Magic::RunEngine();

	Magic::ShutdownSystemUI();

	return 0;
}