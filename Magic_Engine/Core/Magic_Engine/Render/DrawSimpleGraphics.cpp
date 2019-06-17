#include "DrawSimpleGraphics.h"
#include "Render_thread.h"
#include "Define/Magic_Macro.h"
#include "Define/MagicType.h"
#include <GL/glew.h>  
#include "MagicEngineContext.h"

#include "MagicEngineAPI.h"

static const char* S_Pure_Color_Vertex =
"#version 400\r\n"
"layout(location = 0) in vec3 Position;"
"layout(location = 1) in vec4 Color;"
"layout(location = 2) in mat4 CameraMatrix;"

"out vec4 Out_Color;"

"uniform mat4 projectionMatrix;"
"uniform float PointSize;"

"void main()"
"{"
"	gl_Position = projectionMatrix * CameraMatrix * vec4(Position.x, Position.y, 0.0, 1.0);"
"	Out_Color = Color;"
"	gl_PointSize = PointSize;"
"}";

static const char* S_Pure_Color_Frag =
"#version 400\r\n"

"in vec4 Out_Color;"
"out vec4 gl_FragColor;"

"void main()"
"{"
"	gl_FragColor = Out_Color;"
"}";

#define DRAW_TYPE_RESET							0x00
#define DRAW_TYPE_PICTURE_TEXT					0x01
#define DRAW_TYPE_POINTS						0x02
#define DRAW_TYPE_LINES							0x03
#define DRAW_TYPE_LINE_STRIP					0x04
#define DRAW_TYPE_LINE_LOOP						0x05
#define DRAW_TYPE_TRIANGLES						0x06
#define DRAW_TYPE_TRIANGLE_STRIP				0x07
#define DRAW_TYPE_TRIANGLE_FAN					0x08
#define DRAW_TYPE_DISTANCE_TEXT					0x09

DrawSimpleGraphics* DrawSimpleGraphics::m_S_pDrawSimpleGraphics = nullptr;

DrawSimpleGraphics::DrawSimpleGraphics() {
	m_S_pDrawSimpleGraphics = this;
}

DrawSimpleGraphics::~DrawSimpleGraphics() {
	m_S_pDrawSimpleGraphics = nullptr;
}

bool DrawSimpleGraphics::Initialize() {

	Magic::RenderThread([this](Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message) {
		//初始话渲染对象
		//-------------------------------------------------------------------------------
		bool result = m_LineShader.LoadFromString(GL_VERTEX_SHADER, S_Pure_Color_Vertex);
		if (!result) {
			Magic::ShutdownEngine(1);
			return;
		}

		result = m_LineShader.LoadFromString(GL_FRAGMENT_SHADER, S_Pure_Color_Frag);
		if (!result) {
			Magic::ShutdownEngine(1);
			return;
		}

		result = m_LineShader.CreateAndLinkProgram();
		if (!result) {
			Magic::ShutdownEngine(1);
			return;
		}

		m_LineShader.Use();

		m_LineShader.AddUniform("projectionMatrix");
		m_LineShader.AddUniform("PointSize");

		m_LineShader.UnUse();

		m_Line_projectionMatrix = m_LineShader("projectionMatrix");
		m_Line_PointSize = m_LineShader("PointSize");

		m_Line_VBO.CreateBuffer(3);
		unsigned int _Line_Array_Size[] = {
			sizeof(glm::vec3) / sizeof(float),
			sizeof(Magic::Color4) / sizeof(float),

			sizeof(glm::vec4) / sizeof(float),
			sizeof(glm::vec4) / sizeof(float),
			sizeof(glm::vec4) / sizeof(float),
			sizeof(glm::vec4) / sizeof(float)
		};
		m_Line_VBO.SetBuffer(0, Magic::VERTEX_BUFFER::DYNAMIC_DRAW, 2, _Line_Array_Size);
		m_Line_VBO.SetBuffer(1, Magic::VERTEX_BUFFER::DYNAMIC_DRAW, 4, &_Line_Array_Size[2], 1);
		m_Line_VBO.SetDrawIndirectBuffer(2, Magic::VERTEX_BUFFER::DYNAMIC_DRAW);

		Magic::MonitorRenderThread(Magic::RENDER, BindClassFunctionToMessage(&DrawSimpleGraphics::Render));
	});

	Magic::MonitorRenderThread(Magic::RENDER_SET_RECT, BindClassFunctionToMessage(&DrawSimpleGraphics::Event_Rect));

	return true;
}

void DrawSimpleGraphics::DrawLine(float _x1, float _y1, float _x2, float _y2) {

	DrawLine(_x1, _y1, _x2, _y2, m_NowColor);
}

void DrawSimpleGraphics::DrawLine(float _x1, float _y1, float _x2, float _y2, Magic::Color4 _Color) {
	unsigned int _LastDrawType = m_LastDrawType;
	glm::mat4 _WorldMatrix = m_WorldMatrix;

	Magic::RenderThread([this, _LastDrawType, _Color, _WorldMatrix, _x1, _y1, _x2, _y2](Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message) {
		LINE_VERTEX _Vertex;

		if (_LastDrawType != DRAW_TYPE_LINES || !m_this->m_vec_Instance.size())
		{
			LINE_INSTANCE _Instance;
			_Instance.CameraMatrix = CONST_CAMERA * _WorldMatrix;
			m_this->m_vec_Instance.push_back(_Instance);
		}

		if (!m_this->m_vec_DEICommand.size() || _LastDrawType != DRAW_TYPE_LINES)
		{
			Magic::DrawArraysIndirectCommand _DEICommand;

			_DEICommand.count = 2;
			_DEICommand.instanceCount = 1;
			_DEICommand.first = m_this->m_vec_Line_Vertex.size();
			_DEICommand.baseInstance = m_this->m_vec_Instance.size() - 1;

			m_this->m_vec_DEICommand.push_back(_DEICommand);
		}
		else
			m_this->m_vec_DEICommand.back().count += 2;

		_Vertex.Color = _Color;

		_Vertex.Position.x = _x1;
		_Vertex.Position.y = _y1;
		m_this->m_vec_Line_Vertex.push_back(_Vertex);
		_Vertex.Position.x = _x2;
		_Vertex.Position.y = _y2;
		m_this->m_vec_Line_Vertex.push_back(_Vertex);
	});

	m_LastDrawType = DRAW_TYPE_LINES;
}

void DrawSimpleGraphics::SetColor(Magic::Color4 _Color) {
	m_NowColor = _Color;
}

void DrawSimpleGraphics::SetWorldMatrix(glm::mat4 _WorldMatrix) {
	m_WorldMatrix = _WorldMatrix;
}

void DrawSimpleGraphics::Event_Rect(Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message) {
	Magic::Screen_Rect _Screen_Rect = MESSAGE_TO_SCREEN_RECT(_Message);
	m_this->m_projectionMatrix = glm::ortho(0.0f, (float)_Screen_Rect.w, 0.0f, (float)_Screen_Rect.h, 0.1f, 100.0f);
}

void DrawSimpleGraphics::Render(Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message)
{
	if (m_this->m_vec_Line_Vertex.size())
	{
		m_LineShader.Use();
		glUniformMatrix4fv(m_Line_projectionMatrix, 1, GL_FALSE, &m_this->m_projectionMatrix[0][0]);

		m_Line_VBO.DynamicWrite(0, m_this-> m_vec_Line_Vertex.size() * sizeof(LINE_VERTEX), &m_this->m_vec_Line_Vertex[0]);
		m_Line_VBO.DynamicWrite(1, m_this->m_vec_Instance.size() * sizeof(LINE_INSTANCE), &m_this->m_vec_Instance[0]);
		m_Line_VBO.DynamicWrite(2, m_this->m_vec_DEICommand.size() * sizeof(Magic::DrawArraysIndirectCommand), &m_this->m_vec_DEICommand[0]);

		glLineWidth(1.0f);
		//glPointSize(1.0f);
		glUniform1f(m_Line_PointSize, 1.0f);

		m_Line_VBO.BindBuffer(2);
		m_Line_VBO.Bind();
		glMultiDrawArraysIndirect(GL_LINES, (GLvoid*)(sizeof(Magic::DrawArraysIndirectCommand) * 0), m_this->m_vec_DEICommand.size(), 0);
	}

	m_this->m_vec_Line_Vertex.clear();
	m_this->m_vec_Instance.clear();
	m_this->m_vec_DEICommand.clear();
}