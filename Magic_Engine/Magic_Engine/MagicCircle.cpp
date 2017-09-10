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
	result = m_FEPBuffer.Initialize(w, h, MagicFBOTextrue::COLOR4);
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
}