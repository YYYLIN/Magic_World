#include "MagicUIElemeent.h"
#include "Magic_Macro.h"

SceneExMessage::SceneExMessage()
{
}

SceneExMessage::SceneExMessage(unsigned int _message, unsigned int _parmeter)
{
	Message = _message;
	Parameter = _parmeter;
}


MagicSceneEx::MagicSceneEx()
{

}

void MagicSceneEx::DrawSpirit()
{
	if (pParentScene)
		pParentScene->DrawSpirit();
	m_DrawMessage = true;
}

void MagicSceneEx::SetDisplayState(bool _state)
{
	if (DisplayState != _state)
	{
		DisplayState = _state;
		if (pParentScene)
			pParentScene->DrawSpirit();
	}
}

glm::vec2 MagicSceneEx::GetFrameBufferSize()
{
	return glm::vec2(m_FBOBuffer.GetWidth(), m_FBOBuffer.GetHeight());
}

bool MagicSceneEx::Initialize(MagicScene* _scene, glm::vec4 _PosSize)
{
	bool result;
	result = MagicScene::Initialize(_scene, _PosSize);
	if (!result)
		return false;

	int w = _PosSize.z, h = _PosSize.w;
	result = m_FBOBuffer.Initialize(w, h, MagicFBOTextrue::COLOR4);
	if (!result)
		return false;

	this->DrawSpirit();

	return true;
}

void MagicSceneEx::Render(glm::vec2 _DrawPos)
{
	if (DisplayState)
	{
		if (m_DrawMessage)
		{
			m_FBOBuffer.Use();
			glClear(GL_COLOR_BUFFER_BIT);
			MagicScene::Render(glm::vec2());
			glBindFramebuffer(GL_FRAMEBUFFER, pParentScene->GetFBOTextrue());
			m_DrawMessage = false;
		}
		this->RenderBuffer();
	}
}

void MagicSceneEx::RenderBuffer()
{

	Magic::Pen_Normal* pPen_Normal = MagicEngineContext::pMagicEngineContext->GetPen_Normal();
	pPen_Normal->BindPicture(&m_FBOBuffer);
	pPen_Normal->BindPictureUVPosfault();
	pPen_Normal->DrawPicture(m_PosSize.x, m_PosSize.y, m_PosSize.z, m_PosSize.w);
}

void MagicSceneEx::RenderStart()
{
	Magic::Pen_Normal* pPen_Normal = MagicEngineContext::pMagicEngineContext->GetPen_Normal();
	pPen_Normal->RenderStart();
	pPen_Normal->SetCameraMatrix(CONST_CAMERA);

}

void MagicSceneEx::RenderEnd()
{
	Magic::Pen_Normal* pPen_Normal = MagicEngineContext::pMagicEngineContext->GetPen_Normal();
	glm::vec2 _WH = this->GetFrameBufferSize();
	pPen_Normal->SetDrawWH(_WH.x, _WH.y);
	pPen_Normal->RenderEnd();
}
