#pragma once

#include "glm.hpp"

#include <gtc/matrix_transform.hpp>
#include <gtx/transform2.hpp>
#include "MagicShader.h"
#include "MagicTexture.h"
#include "MagicVexter.h"

class MagicPen
{
public:
	MagicPen();
	~MagicPen();

	bool Initialize();

	void DrawPicture(const glm::mat4& CameraMatrix, glm::mat4& WorldMatrix, GLuint texture, glm::vec2& Pos,
		glm::vec2& WidthHight, Magic::Color4& Color4, glm::vec2* pUV = 0);

	void ReshapeWinSize(int width, int height);

	inline int GetWinHeight() { return m_WinHeight; }
	inline int GetWinWidth() { return m_WinWidth; }

	MagicVexter* GetQuadVexter() { return &m_MagicPictrueVexter; }

	glm::mat4 GetorthoMatrix() { return orthoMatrix; }

	void AddSpecialEffects(MagicShader* _shader, const string& _Text);

	MagicShader* operator[](const string& _Text);

private:
	int m_WinWidth, m_WinHeight;
	MagicShader m_Picture2D;
	map<string, MagicShader*> Map_SpecialEffectsPanel;

	MagicVexter m_MagicPictrueVexter;

	glm::mat4 orthoMatrix, projectionMatrix;
};

