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
	if (pUpperScene)
		pUpperScene->DrawSpirit();
	m_DrawMessage = true;
}

void MagicSceneEx::SetDisplayState(bool _state)
{
	if (DisplayState != _state)
	{
		DisplayState = _state;
		if (pUpperScene)
			pUpperScene->DrawSpirit();
	}
}

bool MagicSceneEx::Initialize(glm::vec4 _PosSize)
{
	bool result;
	result = MagicScene::Initialize(_PosSize);
	if (!result)
		return false;

	int w = _PosSize.z, h = _PosSize.w;
	result = m_FBOBuffer.Initialize(w, h, MagicFBOTextrue::COLOR4);
	if (!result)
		return false;

	this->DrawSpirit();

	return true;
}

void MagicSceneEx::Render(glm::mat4 CameraMatrix)
{
	if (DisplayState)
	{
		MagicEngineContext* pEngine = MagicEngineContext::pMagicEngineContext;
		MagicPen* pPen = pEngine->GetPen();
		glm::mat4 Camera = CONST_CAMERA;

		if (m_DrawMessage)
		{
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_FBOBuffer.GetFBOTextrue());
			glClear(GL_COLOR_BUFFER_BIT);
			this->RenderReset(Camera);
			MagicScene::Render(Camera);
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
			m_DrawMessage = false;
		}

		if (pUpperScene)
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, pUpperScene->GetFBOTextrue());
		else
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		Camera = CONST_CAMERA;
		Camera[3].x = m_PosSize.x;
		Camera[3].y = m_PosSize.y;
		pPen->DrawPicture(Camera, glm::mat4(), m_FBOBuffer.GetTextrue(),
			glm::vec2(0.0f), glm::vec2(m_FBOBuffer.GetWidth(), m_FBOBuffer.GetHeight()));
	}
}

void MagicSceneEx::RenderReset(glm::mat4& CameraMatrix)
{
}