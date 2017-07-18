#pragma once
#include "MagicEngineContext.h"

class SpecialEffectsPanel
{
public:
	SpecialEffectsPanel();
	~SpecialEffectsPanel();


	bool Initialize(float _w, float _h);

	void SetPanelSize(float _w, float _h);

	void DrawSpecialEffects(glm::mat4 CameraMatrix, glm::mat4& WorldMatrix, GLuint texture, glm::vec2& Pos,
		glm::vec2& WidthHight, Magic::Color4& Color4, glm::vec2* pUV = 0);
private:
	MagicShader m_SelfLuminous;  //×Ô·¢¹â


	GLfloat texCoordOffsets[18];

public:
	static SpecialEffectsPanel* pSpecialEffectsPanel;
};

