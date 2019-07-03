#include "MagicUIAPI.h"
#include "System/MagicEngineAPI.h"
#include <tchar.h>

#include "Render/DrawSimpleGraphics.h"
#include "Render/TemplateEffects.h"

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
		Magic::TemplateEffects("Main", [](const ::Magic::PTemplate_Effects& _PTE) {
			DrawSimpleGraphics::Instance()->SetColor(Magic::Color4(0.0f, 1.0f, 0.0f, 1.0f));
			DrawSimpleGraphics::Instance()->DrawLine(0.0f, 10, 100.0f, 10);
			DrawSimpleGraphics::Instance()->SetColor(Magic::Color4(1.0f, 0.0f, 0.0f, 1.0f));
			DrawSimpleGraphics::Instance()->DrawLine(0.0f, 40, 100, 40);
			DrawSimpleGraphics::Instance()->SetColor(Magic::Color4(1.0f, 1.0f, 0.0f, 1.0f));
			DrawSimpleGraphics::Instance()->DrawLine(0.0f, 100, 100, 100.0f);
			DrawSimpleGraphics::Instance()->DrawLine(220.0f, 300.0f, 320.0f, 400.0f);

			Magic::TemplateEffects("Fuzzy", [](const ::Magic::PTemplate_Effects& _PTE) {
				DrawSimpleGraphics::Instance()->SetColor(Magic::Color4(0.0f, 1.0f, 0.0f, 1.0f));
				DrawSimpleGraphics::Instance()->DrawLine(200.0f, 300.0f, 300.0f, 400.0f);
			});

			DrawSimpleGraphics::Instance()->DrawLine(220.0f, 300.0f, 320.0f, 400.0f);

			Magic::TemplateEffects("Fuzzy", [](const ::Magic::PTemplate_Effects& _PTE) {
				DrawSimpleGraphics::Instance()->SetColor(Magic::Color4(0.0f, 1.0f, 1.0f, 1.0f));
				DrawSimpleGraphics::Instance()->DrawLine(230.0f, 300.0f, 330.0f, 400.0f);
			});
		});	
	});

	if (Magic::RunEngine() != 0) {
		MessageBoxA(NULL, Magic::GetEngineErrorMessage(), "´íÎó", MB_OK);
		return false;
	}

	return 0;
}