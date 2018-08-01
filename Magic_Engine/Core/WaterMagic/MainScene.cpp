#include "MainScene.h"
#include "Render/MagicEngineAPI.h"

MainScene* MainScene::pMainScene = 0;

MainScene::MainScene() :SceneCommon("MainScene")
{
	m_MagicRotate = 0.0f;
	pMainScene = this;
}


MainScene::~MainScene()
{
}

bool MainScene::OnInitialize()
{
	bool _result = pMagicTexture.Initialize("resources/pictrue/魔法阵：鑫_8.png", LOAD_PNG);
	if (!_result)
		return false;

	_result = m_Magic_Fonts.Initialize("resources/fonts/Fonts1.maigcfonts");
	if (!_result)
		return false;
	Magic::SetSceneSize(this->GetEntity(), glm::vec2(1024.0f, 768.0f));

	/*
		int _w = (int)m_PosSize.z, _h = (int)m_PosSize.w;
		_result = m_FBOBuffer.ResetSize(_w, _h, 16);
		if (!_result)
			return false;*/

	Magic::SetSceneCallUpdata(this->GetEntity(), OnUpdata);
	Magic::SetSceneCallRender(this->GetEntity(), Draw);

	return true;
}

void MainScene::OnUpdata(EntityCommon _Entity)
{
	pMainScene->m_MagicRotate += (float)Magic::GetThreadsDiffTime() * 0.01f;
	if (pMainScene->m_MagicRotate >= 360.0f)
		pMainScene->m_MagicRotate = 0.0f;
}

void MainScene::Draw(EntityCommon _Entity)
{
	glm::mat4 _worldMatrix;

	Magic::Pen_Normal* pPen_Normal = Magic::GetPen_Normal();

	glm::vec2 _Size = Magic::GetSceneSize(_Entity);

	pPen_Normal->SetColor(Magic::Color4(1.0f, 0.0f, 0.0f, 1.0f));
	pPen_Normal->DrawRectangle(Magic::Pen_Normal::TRIANGLES, 30, 30, 100, 100);

	pPen_Normal->DrawRectangle(Magic::Pen_Normal::LINE_STRIP, 30, 160, 100, 100);

	_worldMatrix[3].x = 200.0f;
	_worldMatrix[3].y = 0.0f;
	pPen_Normal->SetWorldMatrix(_worldMatrix);
	pPen_Normal->RepeatDraw();

	pPen_Normal->EnableAlpha();
	pPen_Normal->SetColor(Magic::Color4());
	_worldMatrix = glm::rotate(pMainScene->m_MagicRotate, glm::vec3(0.0f, 0.0f, 1.0f));
	_worldMatrix[3].x = _Size.x * 0.5f;
	_worldMatrix[3].y = _Size.y * 0.5f;
	pPen_Normal->SetWorldMatrix(_worldMatrix);
	pPen_Normal->BindPicture(&pMainScene->pMagicTexture);
	float _w = pMainScene->pMagicTexture.GetWidth() * 0.2f;
	float _h = pMainScene->pMagicTexture.GetHeight() * 0.2f;
	pPen_Normal->DrawPicture(-_w * 0.5f, -_h * 0.5f, _w, _h);

	pPen_Normal->SetColor(Magic::Color4(0.0f, 0.0f, 1.0f, 1.0f));
	_worldMatrix[3].x = _Size.x * 0.5f - 100.0f;
	_worldMatrix[3].y = _Size.y * 0.5f - 100.0f;
	pPen_Normal->SetWorldMatrix(_worldMatrix);
	pPen_Normal->RepeatDraw();

	pPen_Normal->DrawRectangle(Magic::Pen_Normal::TRIANGLES, 0, 0, 100, 100);

	pPen_Normal->DrawLine(150.0f, 20.0f, 300.0f, 20.0f);

	pPen_Normal->ResetWorldMatrix();
	pPen_Normal->RepeatDraw();

	pPen_Normal->SetColor(Magic::Color4(0.0f, 1.0f, 1.0f, 1.0f));
	static float _S_Vertex[] = {
		300.0f,300.0f,
		400.0f,300.0f,
		350.0f,400.0f
	};
	pPen_Normal->DrawVertex(Magic::Pen_Normal::TRIANGLES, _S_Vertex, 3);

	static Magic::Color4 _S_Color[] = {
		{1.0f,1.0f,1.0f,1.0f},
		{1.0f,0.0f,1.0f,1.0f},
		{1.0f,1.0f,0.0f,1.0f}
	};
	_worldMatrix[3].x = _Size.x * 0.5f - 50.0f;
	_worldMatrix[3].y = _Size.y * 0.5f - 50.0f;
	pPen_Normal->SetWorldMatrix(_worldMatrix);
	pPen_Normal->DrawVertex(Magic::Pen_Normal::TRIANGLES, _S_Vertex, _S_Color, 3);

	_worldMatrix[3].x = _Size.x * 0.5f;
	_worldMatrix[3].y = _Size.y * 0.5f;
	pPen_Normal->SetWorldMatrix(_worldMatrix);
	pPen_Normal->RepeatDraw();

	pPen_Normal->ResetWorldMatrix();
	pPen_Normal->BindFonts(&pMainScene->m_Magic_Fonts);
	pPen_Normal->DrawTEXT(0, 200, L"FFF魔法星月：星色 空2333FFFF", 16);
}