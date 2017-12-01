#include "MainScene.h"



MainScene::MainScene()
{
}


MainScene::~MainScene()
{
}

bool MainScene::OnInitialize()
{
	bool _result = pMagicTexture.Initialize("resources/pictrue/Ä§·¨Õó£ºöÎ_8.png", LOAD_PNG);
	if (!_result)
		return false;

	return true;
}

void MainScene::Draw()
{
	Magic::Pen_Normal* pPen_Normal = MagicEngineContext::pMagicEngineContext->GetPen_Normal();
	pPen_Normal->SetColor(Magic::Color4(1.0f, 0.0f, 0.0f, 1.0f));
	pPen_Normal->DrawRectangle(Magic::Pen_Normal::TRIANGLES, 30, 30, 100, 100);

	pPen_Normal->EnableAlpha();
	pPen_Normal->BindPicture(&pMagicTexture);
	pPen_Normal->DrawPicture(300, 200, (int)pMagicTexture.GetHeight() * 0.2f, (int)pMagicTexture.GetWidth() * 0.2f);
}