#include "SpecialEffectsPanel.h"

SpecialEffectsPanel* SpecialEffectsPanel::pSpecialEffectsPanel = 0;

SpecialEffectsPanel::SpecialEffectsPanel()
{
}


SpecialEffectsPanel::~SpecialEffectsPanel()
{
}


bool SpecialEffectsPanel::Initialize(float _w, float _h)
{
	bool result;

	pSpecialEffectsPanel = this;

	//-----------------------------------------------------------------

	result = m_SelfLuminous.LoadFromFile(GL_VERTEX_SHADER, "shader/SelfLuminous.vertshader");
	if (!result)
		return false;
	result = m_SelfLuminous.LoadFromFile(GL_FRAGMENT_SHADER, "shader/SelfLuminous.fragshader");
	if (!result)
		return false;
	result = m_SelfLuminous.CreateAndLinkProgram();
	if (!result)
		return false;

	m_SelfLuminous.Use();

	m_SelfLuminous.AddUniform("PositionUV");
	m_SelfLuminous.AddUniform("worldMatrix");
	m_SelfLuminous.AddUniform("CameraMatrix");
	m_SelfLuminous.AddUniform("projectionMatrix");

	m_SelfLuminous.AddUniform("sampler0");
	m_SelfLuminous.AddUniform("AmbientColor");
	m_SelfLuminous.AddUniform("tc_offset");

	glUniform1i((m_SelfLuminous)("sampler0"), 0);

	SetPanelSize(_w, _h);

	glUniformMatrix4fv((m_SelfLuminous)("projectionMatrix"), 1, GL_FALSE, &MagicEngineContext::pMagicEngineContext->GetPen()->GetorthoMatrix()[0][0]);

	m_SelfLuminous.UnUse();

	MagicEngineContext::pMagicEngineContext->GetPen()->AddSpecialEffects(&m_SelfLuminous, "SelfLuminous");

	return true;
}

void SpecialEffectsPanel::SetPanelSize(float _w, float _h)
{
	float xInc = 1.0f / _w;
	float yInc = 1.0f / _h;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			texCoordOffsets[(((i * 3) + j) * 2) + 0] = (-1.0f * xInc) + ((GLfloat)i * xInc);
			texCoordOffsets[(((i * 3) + j) * 2) + 1] = (-1.0f * yInc) + ((GLfloat)j * yInc);
		}
	}
	m_SelfLuminous.Use();
	glUniform2fv((m_SelfLuminous)("tc_offset"), 9, texCoordOffsets);
}


void SpecialEffectsPanel::DrawSpecialEffects(const glm::mat4& CameraMatrix, glm::mat4& WorldMatrix, GLuint texture, glm::vec2& Pos,
	glm::vec2& WidthHight, Magic::Color4& Color4, glm::vec2* pUV)
{
	float PostionUV[16];

	m_SelfLuminous.Use();

	memset(PostionUV, 0x00, sizeof(float) * 16);
	PostionUV[0] = Pos.x; PostionUV[1] = Pos.y;
	PostionUV[4] = Pos.x + WidthHight.x; PostionUV[5] = Pos.y;
	PostionUV[8] = Pos.x + WidthHight.x; PostionUV[9] = Pos.y + WidthHight.y;
	PostionUV[12] = Pos.x; PostionUV[13] = Pos.y + WidthHight.y;
	if (pUV)
	{
		PostionUV[2] = pUV[0].x; PostionUV[3] = pUV[0].y;
		PostionUV[6] = pUV[1].x; PostionUV[7] = pUV[1].y;
		PostionUV[10] = pUV[2].x; PostionUV[11] = pUV[2].y;
		PostionUV[14] = pUV[3].x; PostionUV[15] = pUV[3].y;
	}
	else
	{
		PostionUV[2] = 0.0f; PostionUV[3] = 0.0f;
		PostionUV[6] = 1.0f; PostionUV[7] = 0.0f;
		PostionUV[10] = 1.0f; PostionUV[11] = 1.0f;
		PostionUV[14] = 0.0f; PostionUV[15] = 1.0f;
	}

	glUniform4fv((m_SelfLuminous)("PositionUV"), 4, PostionUV);
	glUniformMatrix4fv((m_SelfLuminous)("CameraMatrix"), 1, GL_FALSE, &CameraMatrix[0][0]);
	glUniformMatrix4fv((m_SelfLuminous)("worldMatrix"), 1, GL_FALSE, &WorldMatrix[0][0]);

	glUniform4f((m_SelfLuminous)("AmbientColor"), Color4.R, Color4.G, Color4.B, Color4.A);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(0);
	glDrawArrays(GL_QUADS, 0, 4);
	glBindVertexArray(0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	m_SelfLuminous.UnUse();
}