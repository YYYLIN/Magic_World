#ifndef _DRAWSIMPLEGRAPHICS_H_
#define _DRAWSIMPLEGRAPHICS_H_


#include <vector>

#include "MagicVertex.h"
#include "MagicTexture.h"
#include "MagicShader.h"
#include "Magic_Thread.h"
#include "MagicOpengl.h"
#include "Cross_Platform_Port.h"
#include "DrawTool.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform2.hpp>

#pragma warning(push)
#pragma warning(disable:4251)

class DLL_MAGIC_ENGINE_OUTPUT_INPUT DrawSimpleGraphics {

	struct LINE_VERTEX
	{
		glm::vec3 Position;
		Magic::Color4 Color;
	};

	struct LINE_INSTANCE
	{
		glm::mat4 CameraMatrix;
	};

	struct DRAW_TYPE_QUEUE
	{
		DRAW_TYPE_QUEUE() :Type(0), StartPos(0) {}
		DRAW_TYPE_QUEUE(unsigned int _Type, unsigned int _StartPos) :Type(_Type), StartPos(_StartPos) {}

		unsigned int Type;
		unsigned int StartPos;
	};

public:
	DrawSimpleGraphics();
	~DrawSimpleGraphics();

	static DrawSimpleGraphics* Instance() { return m_S_pDrawSimpleGraphics; }

	bool Initialize();

	void DrawLine(float _x1, float _y1, float _x2, float _y2);

	void DrawLine(float _x1, float _y1, float _x2, float _y2, Magic::Color4 _Color);

	void DrawRectangle(int _x, int _y, int _w, int _h, Magic::Color4 _Color);
	void DrawRectangle(float _x, float _y, float _w, float _h, Magic::Color4 _Color);

	void SetColor(Magic::Color4 _Color);

	void SetWorldMatrix(glm::mat4 _WorldMatrix);


private:
	void Render(Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message);
private:
	unsigned int m_LastDrawType;
	glm::mat4 m_WorldMatrix;
	Magic::Color4 m_NowColor;
	//RenderThread 对象---------->
	Magic::VERTEX_BUFFER m_Line_VBO;
	MagicShader m_LineShader;
	unsigned int m_Line_projectionMatrix;
	unsigned int m_Line_PointSize;
	RT_ENVIRONMENT_START
		glm::mat4 m_projectionMatrix;

	std::vector<LINE_VERTEX> m_vec_Line_Vertex;
	std::vector<LINE_INSTANCE> m_vec_Instance;
	std::vector<Magic::DrawArraysIndirectCommand> m_vec_DEICommand;
	std::vector<DRAW_TYPE_QUEUE> m_Draw_Type_Queue; //渲染类型切换的队列
	RT_ENVIRONMENT_END;
	//---------------------------<

	static DrawSimpleGraphics* m_S_pDrawSimpleGraphics;

};

#pragma warning(pop)

#endif