#include "MainScene.h"



MainScene::MainScene()
{
}


MainScene::~MainScene()
{
}

bool MainScene::OnInitialize()
{
	return true;
}

void MainScene::Draw()
{
	Magic::Pen_Normal* pPen_Normal = MagicEngineContext::pMagicEngineContext->GetPen_Normal();
	pPen_Normal->SetColor(Magic::Color4(1.0f, 0.0f, 0.0f, 1.0f));
	pPen_Normal->SetDrawFaceMode(Magic::Pen_Normal::Draw_DoubleFace);
	pPen_Normal->DrawRectangle(Magic::Pen_Normal::TRIANGLES, 0, 0, 100, 100);
}