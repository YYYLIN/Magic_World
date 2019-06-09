#include "MagicUIAPI.h"
#include "MainScene.h"
#include "Render/MagicEngineAPI.h"
#include <tchar.h>

#include "Render/DrawSimpleGraphics.h"

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

	Magic::Engine([]() {
		DrawSimpleGraphics::Instance()->DrawLine(0.0f, 0.0f, 100.0f, 100.0f);
		DrawSimpleGraphics::Instance()->SetColor(Magic::Color4(1.0f, 0.0f, 0.0f, 1.0f));
		DrawSimpleGraphics::Instance()->DrawLine(0.0f, 0.0f, 10, 100.0f);
		DrawSimpleGraphics::Instance()->SetColor(Magic::Color4(1.0f, 1.0f, 0.0f, 1.0f));
		DrawSimpleGraphics::Instance()->DrawLine(0.0f, 0.0f, 30, 100.0f);
	});

	Magic::RunEngine();

	return 0;
}