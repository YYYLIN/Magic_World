#pragma once
#include "MagicUIElemeent.h"

class MagicSceneCircle :public MagicSceneEx
{
public:
	MagicSceneCircle();
	~MagicSceneCircle();

protected:
	bool OnInitialize();
	void Updata();
	void RenderReset(glm::mat4& CameraMatrix);

private:
	MagicTexture m_MagicTextrue;

	MagicFBOTextrue m_FEPBuffer;

	float m_Angle;
};

