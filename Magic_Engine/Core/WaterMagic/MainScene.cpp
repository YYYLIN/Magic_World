#include "MainScene.h"



MainScene::MainScene()
{
	m_MagicRotate = 0.0f;
}


MainScene::~MainScene()
{
}

bool MainScene::OnInitialize()
{
	bool _result = pMagicTexture.Initialize("resources/pictrue/Ä§·¨Õó£ºöÎ_8.png", LOAD_PNG);
	if (!_result)
		return false;

	int _w = (int)m_PosSize.z, _h = (int)m_PosSize.w;
	_result = m_FBOBuffer.ResetSize(_w, _h, 16);
	if (!_result)
		return false;

	return true;
}

void MainScene::OnUpdata()
{
	m_MagicRotate += (float)MagicEngineContext::pMagicEngineContext->GetDiffTime() * 0.01f;
	if (m_MagicRotate >= 360.0f)
		m_MagicRotate = 0.0f;
	this->DrawSpirit();
}

void MainScene::Draw()
{
	glm::mat4 _worldMatrix;

	Magic::Pen_Normal* pPen_Normal = MagicEngineContext::pMagicEngineContext->GetPen_Normal();
	pPen_Normal->SetColor(Magic::Color4(1.0f, 0.0f, 0.0f, 1.0f));
	pPen_Normal->DrawRectangle(Magic::Pen_Normal::TRIANGLES, 30, 30, 100, 100);

	pPen_Normal->DrawRectangle(Magic::Pen_Normal::LINE_STRIP, 30, 160, 100, 100);

	pPen_Normal->SetColor(Magic::Color4(0.0f, 1.0f, 0.0f, 1.0f));

	_worldMatrix[3].x = 200.0f;
	_worldMatrix[3].y = 0.0f;
	pPen_Normal->SetWorldMatrix(_worldMatrix);
	pPen_Normal->RepeatDraw();

	pPen_Normal->EnableAlpha();
	pPen_Normal->SetColor(Magic::Color4());
	_worldMatrix = glm::rotate(m_MagicRotate, glm::vec3(0.0f, 0.0f, 1.0f));
	_worldMatrix[3].x = m_PosSize.z * 0.5f;
	_worldMatrix[3].y = m_PosSize.w * 0.5f;
	pPen_Normal->SetWorldMatrix(_worldMatrix);
	pPen_Normal->BindPicture(&pMagicTexture);
	float _w = pMagicTexture.GetWidth() * 0.2f;
	float _h = pMagicTexture.GetHeight() * 0.2f;
	pPen_Normal->DrawPicture(-_w * 0.5f, -_h * 0.5f, _w, _h);

	pPen_Normal->SetColor(Magic::Color4(0.0f, 0.0f, 1.0f, 1.0f));
	_worldMatrix[3].x = m_PosSize.z * 0.5f - 100.0f;
	_worldMatrix[3].y = m_PosSize.w * 0.5f - 100.0f;
	pPen_Normal->SetWorldMatrix(_worldMatrix);
	pPen_Normal->RepeatDraw();

	pPen_Normal->DrawRectangle(Magic::Pen_Normal::TRIANGLES, 0, 0, 100, 100);

	pPen_Normal->DrawLine(150.0f, 20.0f, 300.0f, 20.0f);

	pPen_Normal->ResetWorldMatrix();
	pPen_Normal->RepeatDraw();
}