#include "MagicCircle.h"
#include "SpecialEffectsPanel.h"


MagicSceneCircle::MagicSceneCircle()
{
}


MagicSceneCircle::~MagicSceneCircle()
{
}

bool MagicSceneCircle::Initialize(glm::vec4 _PosSize)
{
	bool result;
	result = MagicSceneEx::Initialize(_PosSize);
	if (!result)
		return false;

	result = m_MagicTextrue.Initialize("resources/pictrue/Ä§·¨Õó£ºöÎ_8.png", LOAD_PNG);
	if (!result)
		return false;

	int w = _PosSize.z, h = _PosSize.w;
	result = m_FEPBuffer.Initialize(w, h);
	if (!result)
		return false;

	m_Angle = 0.0f;

	return true;
}

void MagicSceneCircle::Updata()
{
	if (DisplayState)
	{
		m_Angle += MagicEngineContext::pMagicEngineContext->GetDiffTime() / 100;
		if (m_Angle > 360.0f)
			m_Angle = 0.0f;
		this->DrawSpirit();
	}
}

void MagicSceneCircle::RenderReset(glm::mat4& CameraMatrix)
{
	MagicPen* pPen = MagicEngineContext::pMagicEngineContext->GetPen();
	glm::mat4 worldMatrix;
	worldMatrix = glm::rotate(m_Angle, glm::vec3(0.0f, 0.0f, 1.0f));
	worldMatrix[3].x = m_MagicTextrue.GetWidth() / 10;
	worldMatrix[3].y = m_MagicTextrue.GetHeight() / 10;
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_FEPBuffer.GetFBOTextrue());
	glClear(GL_COLOR_BUFFER_BIT);
	pPen->DrawPicture(CameraMatrix, worldMatrix, m_MagicTextrue.GetTextrue(), glm::vec2(-m_MagicTextrue.GetWidth() / 10, -m_MagicTextrue.GetHeight() / 10), glm::vec2(m_MagicTextrue.GetWidth() / 5, m_MagicTextrue.GetHeight() / 5), Magic::Color4());

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, this->GetFBOTextrue());
	worldMatrix = glm::mat4();
	SpecialEffectsPanel::pSpecialEffectsPanel->DrawSpecialEffects(CameraMatrix, worldMatrix, m_FEPBuffer.GetTextrue(), glm::vec2(), glm::vec2(m_PosSize.z, m_PosSize.w), Magic::Color4());
}