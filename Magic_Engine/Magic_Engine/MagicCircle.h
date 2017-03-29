#pragma once
#include "MagicUIElemeent.h"

class MagicSceneCircle :public MagicSceneEx
{
public:
	MagicSceneCircle();
	~MagicSceneCircle();

protected:
	bool Initialize(glm::vec4 _PosSize);
	void Updata();
	void RenderReset(glm::mat4& CameraMatrix);

private:
	MagicTexture m_MagicTextrue;

	MagicFBOTextrue m_FEPBuffer;

	float m_Angle;
};

