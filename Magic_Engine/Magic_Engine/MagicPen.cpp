#include "MagicPen.h"
#pragma comment(lib,"glew32.lib")
#include <GL/glew.h>  


MagicPen::MagicPen()
{
}


MagicPen::~MagicPen()
{
}

bool MagicPen::Initialize()
{
	bool result;

	//-----------------------------------------------------------------

	result = m_Picture2D.LoadFromFile(GL_VERTEX_SHADER, "shader/Picture2D.vertshader");
	if (!result)
		return false;
	result = m_Picture2D.LoadFromFile(GL_FRAGMENT_SHADER, "shader/Picture2D.fragshader");
	if (!result)
		return false;
	result = m_Picture2D.CreateAndLinkProgram();
	if (!result)
		return false;

	m_Picture2D.Use();

	m_Picture2D.AddUniform("worldMatrix");
	m_Picture2D.AddUniform("CameraMatrix");
	m_Picture2D.AddUniform("projectionMatrix");

	m_Picture2D.AddUniform("sampler0");
	m_Picture2D.AddUniform("AmbientColor");

	m_Picture2D.UnUse();

	AddSpecialEffects(&m_Picture2D, "Picture2D");

	//-------------------------------------------------------------------------
	result = m_LightShader.LoadFromFile(GL_VERTEX_SHADER, "shader/Light.vertshader");
	if (!result)
		return false;
	result = m_LightShader.LoadFromFile(GL_FRAGMENT_SHADER, "shader/Light.fragshader");
	if (!result)
		return false;
	result = m_LightShader.CreateAndLinkProgram();
	if (!result)
		return false;

	m_LightShader.Use();

	m_LightShader.AddUniform("worldMatrix");
	m_LightShader.AddUniform("CameraMatrix");
	m_LightShader.AddUniform("projectionMatrix");
	m_LightShader.AddUniform("LightMatrix");

	m_LightShader.AddUniform("sampler0");
	m_LightShader.AddUniform("shadowMap");
	m_LightShader.AddUniform("AmbientColor");

	glUniform1i((m_LightShader)("sampler0"), 0);
	glUniform1i((m_LightShader)("shadowMap"), 1);

	m_LightShader.UnUse();

	AddSpecialEffects(&m_LightShader, "Light");


	return true;
}

void MagicPen::DrawPicture(glm::mat4 CameraMatrix, glm::mat4& WorldMatrix, GLuint texture, glm::vec2& Pos,
	glm::vec2& WidthHight, Magic::Color4& Color4, glm::vec2* pUV)
{
	float PostionUV[16];

	m_Picture2D.Use();

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

	glUniformMatrix4fv((m_Picture2D)("CameraMatrix"), 1, GL_FALSE, &CameraMatrix[0][0]);
	glUniformMatrix4fv((m_Picture2D)("worldMatrix"), 1, GL_FALSE, &WorldMatrix[0][0]);

	glUniform4f((m_Picture2D)("AmbientColor"), Color4.R, Color4.G, Color4.B, Color4.A);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, PostionUV);
	glDrawArrays(GL_QUADS, 0, 4);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	m_Picture2D.UnUse();
}

void MagicPen::ReshapeWinSize(int width, int height)
{
	m_WinWidth = width;
	m_WinHeight = height;
	orthoMatrix = glm::mat4(1.0f);
	//左下角x坐标，右上角x坐标，左下角y坐标，右上角y坐标 坐标全相对于窗口左下角 原点
	orthoMatrix = glm::ortho(
		0.0f, (float)width,
		0.0f, (float)height, 0.01f, 100.0f);
	projectionMatrix = glm::perspective(-70.0f, -(float)width / (float)height, 0.1f, 1000000.0f);

	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, width, height); //设置视频口

	m_Picture2D.Use();
	glUniformMatrix4fv((m_Picture2D)("projectionMatrix"), 1, GL_FALSE, &orthoMatrix[0][0]);
	m_Picture2D.UnUse();
}

void MagicPen::AddSpecialEffects(MagicShader* _shader, const string& _Text)
{
	Map_SpecialEffectsPanel[_Text] = _shader;
}

MagicShader* MagicPen::operator[](const string& _Text)
{
	return Map_SpecialEffectsPanel[_Text];
}