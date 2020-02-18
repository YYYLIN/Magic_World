#include "MagicUIAPI.h"
#include "System/MagicEngineAPI.h"
#include <tchar.h>

#include "Render/DrawSimpleGraphics.h"
#include "Render/TemplateEffects.h"

#include "container_test.h"
#include <assert.h>

std::string g_Html;
std::wstring g_MasteCSS;

litehtml::document::ptr g_spdoc;

int CALLBACK _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevainstance, LPWSTR lpcmdline, int iCmdshow)
{
	{
		FILE* _pFile;
		fopen_s(&_pFile, "Text.html", "r");
		char _Html[1024];


		while (!feof(_pFile))
		{
			if (fgets(_Html, 1024, _pFile) == NULL) break;
			g_Html += _Html;
		}

		fclose(_pFile);
	}

	{
		FILE* _pFile;
		fopen_s(&_pFile, "master.css", "r");
		wchar_t _Html[1024];


		while (!feof(_pFile))
		{
			if (fgetws(_Html, 1024, _pFile) == NULL) break;
			g_MasteCSS += _Html;
		}

		fclose(_pFile);
	}

	bool _result = Magic::CreateSystemUI(L"WaterMagic", 0, 0, 1024, 768);
	if (!_result)
	{
		MessageBoxA(NULL, Magic::GetEngineErrorMessage(), "´íÎó", MB_OK);
		return false;
	}

	litehtml::context ctx;
	container_test container;
	ctx.load_master_stylesheet(g_MasteCSS.c_str());
	g_spdoc = litehtml::document::createFromUTF8(g_Html.c_str(), &container, &ctx);

	int _width = g_spdoc->width();
	g_spdoc->render(_width);

	HICON _ico = (HICON)::LoadImageA(NULL, 0, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	Magic::SetWindowICO(_ico);

	Magic::CreateFONT("msyh", "simsun.ttc", 0, 13);

	Magic::Engine([]() {
		Magic::TemplateEffects("Main", [](const ::Magic::PTemplate_Effects& _PTE) {

			int _width = g_spdoc->width();
			int _height = g_spdoc->height();
			/*glm::mat4 _WorldMatrix;
			_WorldMatrix = glm::rotate(-180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
			_WorldMatrix[3].y = _height;
			DrawSimpleGraphics::Instance()->SetWorldMatrix(_WorldMatrix);*/
			g_spdoc->draw(0, 0, 0, &litehtml::position(0, 0, _width, _height));

			DrawSimpleGraphics::Instance()->BindFont("msyh");
			DrawSimpleGraphics::Instance()->DrawTEXT(L"ÄãÂðABCDHGJGDfFdfsd154564632", 100.0f, 200.0f, Magic::Color4(1.0f, 1.0f, 1.0f, 1.0f));

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