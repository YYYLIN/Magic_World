#include "MagicUIAPI.h"
#include "MainScene.h"
#include <tchar.h>


int CALLBACK _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevainstance, LPWSTR lpcmdline, int iCmdshow)
{
	bool result = Magic::CreateSystemUI("WaterMagic", 0, 0, 1024, 768);
	if (!result)
		return false;

	MainScene* pMainScene = new MainScene;

	pMainScene->Initialize(Magic::GetSystemScenes(Magic::USER_SCENES), glm::vec4(0, 0, 1024, 768));

	Magic::RunSystem();

	Magic::ShutdownSystemUI();

	if (pMainScene)
	{
		delete pMainScene;
		pMainScene = 0;
	}

	return 0;
}