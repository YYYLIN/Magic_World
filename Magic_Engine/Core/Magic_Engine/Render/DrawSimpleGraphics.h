#ifndef _DRAWSIMPLEGRAPHICS_H_
#define _DRAWSIMPLEGRAPHICS_H_


#include <vector>

#include "MagicVertex.h"
#include "MagicTexture.h"
#include "MagicShader.h"
#include "Magic_Thread.h"
#include "MagicOpengl.h"
#include "Cross_Platform_Port.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform2.hpp>


class DrawSimpleGraphics {

	struct LINE_VERTEX
	{
		glm::vec3 Position;
		Magic::Color4 Color;
	};

	struct LINE_INSTANCE
	{
		glm::mat4 WorldMatrix;
	};

public:
	DrawSimpleGraphics();
	~DrawSimpleGraphics();

	bool Initialize();

	void DrawLine(float _x1, float _y1, float _x2, float _y2);

	void DrawLine(float _x1, float _y1, float _x2, float _y2, Magic::Color4 _Color);

	void SetColor(Magic::Color4 _Color);

	void SetWorldMatrix(glm::mat4 _WorldMatrix);
private:
	void Render(Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message);
private:
	unsigned int m_LastDrawType;
	glm::mat4 m_WorldMatrix;
	Magic::Color4 m_NowColor;
	//RenderThread ∂‘œÛ
	Magic::VERTEX_BUFFER m_Line_VBO;
	MagicShader m_LineShader;
	unsigned int m_Line_projectionMatrix;
	unsigned int m_Line_PointSize;

	glm::mat4 m_projectionMatrix;

	std::vector<LINE_VERTEX> m_vec_Line_Vertex;
	std::vector<LINE_INSTANCE> m_vec_Instance;
	std::vector<Magic::DrawArraysIndirectCommand> m_vec_DEICommand;
};

#endif