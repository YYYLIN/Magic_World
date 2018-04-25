#include "MagicUIAPI.h"
#include "MainScene.h"
#include <tchar.h>


int CALLBACK _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevainstance, LPWSTR lpcmdline, int iCmdshow)
{
	bool _result = Magic::CreateSystemUI(L"WaterMagic", 0, 0, 1024, 768);
	if (!_result)
		return false;

	HICON _ico = (HICON)::LoadImageA(NULL, 0, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	Magic::SetWindowICO(_ico);

	MainScene* pMainScene = new MainScene;

	EntityCommon _EntityCommon;
	_result = Magic::CreateScene(Magic::GetSceneCommon("UserScene"), &_EntityCommon);
	if (!_result)
		return false;

	Magic::RunSystem();

	Magic::ShutdownSystemUI();

	if (pMainScene)
	{
		delete pMainScene;
		pMainScene = 0;
	}

	return 0;
}