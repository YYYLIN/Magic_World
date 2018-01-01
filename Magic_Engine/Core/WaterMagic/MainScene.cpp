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
	bool _result = pMagicTexture.Initialize("resources/pictrue/ħ������_8.png", LOAD_PNG);
	if (!_result)
		return false;

	int _w = (int)m_PosSize.z, _h = (int)m_PosSize.w;
	_result = m_MSAA_FBOBuffer.Initialize(_w, _h, MagicFBOTexture::COLOR4, 16);
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
	Magic::Pen_Normal* pPen_Normal = MagicEngineContext::pMagicEngineContext->GetPen_Normal();
	pPen_Normal->SetColor(Magic::Color4(1.0f, 0.0f, 0.0f, 1.0f));
	pPen_Normal->DrawRectangle(Magic::Pen_Normal::TRIANGLES, 30, 30, 100, 100);

	pPen_Normal->EnableAlpha();
	pPen_Normal->SetColor(Magic::Color4());
	glm::mat4 _worldMatrix = glm::rotate(m_MagicRotate, glm::vec3(0.0f, 0.0f, 1.0f));
	_worldMatrix[3].x = m_PosSize.z * 0.5f;
	_worldMatrix[3].y = m_PosSize.w * 0.5f;
	pPen_Normal->SetWorldMatrix(_worldMatrix);
	pPen_Normal->BindPicture(&pMagicTexture);
	float _w = pMagicTexture.GetWidth() * 0.2f;
	float _h = pMagicTexture.GetHeight() * 0.2f;
	pPen_Normal->DrawPicture(-_w * 0.5f, -_h * 0.5f, _w, _h);

	pPen_Normal->SetColor(Magic::Color4(0.0f, 1.0f, 0.0f, 1.0f));
	_worldMatrix[3].x = m_PosSize.z * 0.5f - 100.0f;
	_worldMatrix[3].y = m_PosSize.w * 0.5f - 100.0f;
	pPen_Normal->SetWorldMatrix(_worldMatrix);
	pPen_Normal->RepeatDraw();
}

void MainScene::RenderStart()
{
	m_MSAA_FBOBuffer.Use();
	m_MSAA_FBOBuffer.Clear(MagicFBOTexture::B_COLOR);
	MagicScenesEx::RenderStart();
}

void MainScene::RenderEnd()
{
	MagicScenesEx::RenderEnd();
	m_MSAA_FBOBuffer.CopyFBOTO(&m_FBOBuffer, 0, 0, (int)m_PosSize.z, (int)m_PosSize.w, 0, 0, (int)m_PosSize.z, (int)m_PosSize.w);
}