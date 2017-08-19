#pragma once

#include "glm.hpp"

#include <gtc/matrix_transform.hpp>
#include <gtx/transform2.hpp>
#include "MagicShader.h"
#include "MagicTexture.h"
#include "MagicVertex.h"



class MagicPen
{
public:
	enum StateType {
		BLEND
	};

	void Enable(StateType _StateType);

	void Disable(StateType _StateType);

	MagicPen();
	~MagicPen();

	bool Initialize();

	void DrawPicture(const glm::mat4& CameraMatrix,const glm::mat4& WorldMatrix, GLuint texture,const glm::vec2& Pos,
		const glm::vec2& WidthHight, glm::vec2* pUV = 0);

	void ReshapeWinSize(int width, int height);

	void SetAmbientColor(const Magic::Color4& Color4);

	inline int GetWinHeight() { return m_WinHeight; }
	inline int GetWinWidth() { return m_WinWidth; }

	glm::mat4 GetorthoMatrix() { return orthoMatrix; }

	void AddSpecialEffects(MagicShader* _shader, const string& _Text);

	MagicShader* operator[](const string& _Text);

private:
	int m_WinWidth, m_WinHeight;
	MagicShader m_Picture2D;
	MagicShader m_LightShader;
	MagicShader m_DepthShader;
	map<string, MagicShader*> Map_SpecialEffectsPanel;

	glm::mat4 orthoMatrix, projectionMatrix;
};

