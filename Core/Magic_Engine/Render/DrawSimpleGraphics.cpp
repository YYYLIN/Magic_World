#include "DrawSimpleGraphics.h"
#include "Render_thread.h"
#include "Define/Magic_Macro.h"
#include "Define/MagicType.h"
#include <GL/glew.h>  
#include "System/MagicEngineContext.h"

#include "System/MagicEngineAPI.h"

static const char* S_Pure_Color_Vertex =
"#version 400\r\n"
"layout(location = 0) in vec3 Position;"
"layout(location = 1) in vec2 UV;"
"layout(location = 2) in vec4 Color;"
"layout(location = 3) in mat4 CameraMatrix;"

"out vec4 gl_TexCoord[];"

"uniform mat4 projectionMatrix;"
"uniform float PointSize;"

"void main()"
"{"
"	gl_Position = projectionMatrix * CameraMatrix * vec4(Position.x, Position.y, 0.0, 1.0);"
"	gl_TexCoord[0] = vec4(UV,0.0,0.0);"
"	gl_PointSize = PointSize;"
"   gl_FrontColor = Color;"//正面颜色
//gl_BackColor = Color 背面颜色
"}";

static const char* S_Pure_Color_Frag =
"#version 400\r\n"

"in vec4 gl_Color;"
"in vec4 gl_TexCoord[];"
"out vec4 gl_FragColor;"

"uniform int DrawType;"
"uniform sampler2D sampler0;"

"void main()"
"{"
"	vec4 _texColor = texture2D(sampler0, gl_TexCoord[0].xy);"
"	if(DrawType == 1){"
"		gl_FragColor = _texColor * gl_Color;"
"	}else if(DrawType == 2){"
"		gl_FragColor = _texColor.a * gl_Color;"
"	}else{"
"		gl_FragColor = gl_Color;"
"	}"
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

enum SHADER_DRAW_TYPE {
	SHADER_DRAW_PICTURE = 1,
	SHADER_DRAW_TEXT = 2
};

DrawSimpleGraphics * DrawSimpleGraphics::m_S_pDrawSimpleGraphics = nullptr;

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
		m_DrawType = m_LineShader("DrawType");

		m_Line_VBO.CreateBuffer(3);
		unsigned int _Line_Array_Size[] = {
			sizeof(glm::vec3) / sizeof(float),
			sizeof(glm::vec2) / sizeof(float),
			sizeof(Magic::Color4) / sizeof(float),

			sizeof(glm::vec4) / sizeof(float),
			sizeof(glm::vec4) / sizeof(float),
			sizeof(glm::vec4) / sizeof(float),
			sizeof(glm::vec4) / sizeof(float)
		};
		m_Line_VBO.SetBuffer(0, Magic::VERTEX_BUFFER::DYNAMIC_DRAW, 3, _Line_Array_Size);
		m_Line_VBO.SetBuffer(1, Magic::VERTEX_BUFFER::DYNAMIC_DRAW, 4, &_Line_Array_Size[2], 1);
		m_Line_VBO.SetDrawIndirectBuffer(2, Magic::VERTEX_BUFFER::DYNAMIC_DRAW);

		Magic::MonitorRenderThread(Magic::RENDER, BindClassFunctionToMessage(&DrawSimpleGraphics::Render));
	}, true);

	return true;
}

void DrawSimpleGraphics::BindFont(const char* _Name) {
	Magic::RenderThread([this, _Name](MM_MESS) {
		auto _Font = MagicEngineContext::Instance()->GetFont_Texture(_Name);
		if (_Font) {
			m_this->m_Bind_LRU_Font_Texture = _Font;
		}
	});
}

void DrawSimpleGraphics::DrawTEXT(const std::wstring _wText, float _x, float _y, Magic::Color4 _Color) {
	unsigned int _LastDrawType = m_LastDrawType;
	glm::mat4 _WorldMatrix = m_WorldMatrix;

	Magic::RenderThread([this, _wText, _LastDrawType, _Color, _WorldMatrix, _x, _y](MM_MESS) {
		if (m_this->m_Bind_LRU_Font_Texture) {
			LINE_VERTEX _Vertex;

			if (_LastDrawType != DRAW_TYPE_PICTURE_TEXT || !m_this->m_vec_Instance.size())
			{
				LINE_INSTANCE _Instance;
				_Instance.CameraMatrix = CONST_CAMERA * _WorldMatrix;
				m_this->m_vec_Instance.push_back(_Instance);
			}

			if (_LastDrawType != DRAW_TYPE_PICTURE_TEXT || !m_this->m_vec_DEICommand.size())
			{
				Magic::DrawArraysIndirectCommand _DEICommand;

				_DEICommand.count = 0;
				_DEICommand.instanceCount = 1;
				_DEICommand.first = (unsigned int)m_this->m_vec_Line_Vertex.size();
				_DEICommand.baseInstance = (unsigned int)m_this->m_vec_Instance.size() - 1;

				m_this->m_Draw_Type_Queue.push_back(DRAW_TYPE_QUEUE(DRAW_TYPE_PICTURE_TEXT, m_this->m_vec_DEICommand.size(), m_this->m_Bind_LRU_Font_Texture->GetTexture()));

				m_this->m_vec_DEICommand.push_back(_DEICommand);
			}

			auto _count = &m_this->m_vec_DEICommand.back().count;

			_Vertex.Color = _Color;

			float _X = _x;
			float _Y = _y;

			for (auto _wchar : _wText) {
				auto _CharInfo = m_this->m_Bind_LRU_Font_Texture->GetCHARINFO(_wchar);
				_Vertex.Position.x = _X + _CharInfo.x;
				_Vertex.Position.y = _Y + _CharInfo.y;
				_Vertex.uv.x = _CharInfo.left;
				_Vertex.uv.y = _CharInfo.top;
				m_this->m_vec_Line_Vertex.push_back(_Vertex);
				_Vertex.Position.x = _X + _CharInfo.x;
				_Vertex.Position.y = _Y + _CharInfo.y + _CharInfo.height;
				_Vertex.uv.x = _CharInfo.left;
				_Vertex.uv.y = _CharInfo.bottom;
				m_this->m_vec_Line_Vertex.push_back(_Vertex);
				_Vertex.Position.x = _X + _CharInfo.x + _CharInfo.width;
				_Vertex.Position.y = _Y + _CharInfo.y + _CharInfo.height;
				_Vertex.uv.x = _CharInfo.right;
				_Vertex.uv.y = _CharInfo.bottom;
				m_this->m_vec_Line_Vertex.push_back(_Vertex);
				_Vertex.Position.x = _X + _CharInfo.x;
				_Vertex.Position.y = _Y + _CharInfo.y;
				_Vertex.uv.x = _CharInfo.left;
				_Vertex.uv.y = _CharInfo.top;
				m_this->m_vec_Line_Vertex.push_back(_Vertex);
				_Vertex.Position.x = _X + _CharInfo.x + _CharInfo.width;
				_Vertex.Position.y = _Y + _CharInfo.y + _CharInfo.height;
				_Vertex.uv.x = _CharInfo.right;
				_Vertex.uv.y = _CharInfo.bottom;
				m_this->m_vec_Line_Vertex.push_back(_Vertex);
				_Vertex.Position.x = _X + _CharInfo.x + _CharInfo.width;
				_Vertex.Position.y = _Y + _CharInfo.y;
				_Vertex.uv.x = _CharInfo.right;
				_Vertex.uv.y = _CharInfo.top;
				m_this->m_vec_Line_Vertex.push_back(_Vertex);

				_X += _CharInfo.width;
			}
		}
	});

	m_LastDrawType = DRAW_TYPE_PICTURE_TEXT;
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

		if (_LastDrawType != DRAW_TYPE_LINES || !m_this->m_vec_DEICommand.size())
		{
			Magic::DrawArraysIndirectCommand _DEICommand;

			_DEICommand.count = 2;
			_DEICommand.instanceCount = 1;
			_DEICommand.first = (unsigned int)m_this->m_vec_Line_Vertex.size();
			_DEICommand.baseInstance = (unsigned int)m_this->m_vec_Instance.size() - 1;

			m_this->m_Draw_Type_Queue.push_back(DRAW_TYPE_QUEUE(DRAW_TYPE_LINES, m_this->m_vec_DEICommand.size()));

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

void DrawSimpleGraphics::DrawRectangle(int _x, int _y, int _w, int _h, Magic::Color4 _Color) {
	DrawRectangle((float)_x, (float)_y, (float)_w, (float)_h, _Color);
}

void DrawSimpleGraphics::DrawRectangle(float _x, float _y, float _w, float _h, Magic::Color4 _Color) {
	unsigned int _LastDrawType = m_LastDrawType;
	glm::mat4 _WorldMatrix = m_WorldMatrix;

	Magic::RenderThread([this, _LastDrawType, _Color, _WorldMatrix, _x, _y, _w, _h](Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message) {
		LINE_VERTEX _Vertex;

		if (_LastDrawType != DRAW_TYPE_TRIANGLES || !m_this->m_vec_Instance.size())
		{
			LINE_INSTANCE _Instance;
			_Instance.CameraMatrix = CONST_CAMERA * _WorldMatrix;
			m_this->m_vec_Instance.push_back(_Instance);
		}

		if (_LastDrawType != DRAW_TYPE_TRIANGLES || !m_this->m_vec_DEICommand.size())
		{
			Magic::DrawArraysIndirectCommand _DEICommand;

			_DEICommand.count = 6;
			_DEICommand.instanceCount = 1;
			_DEICommand.first = (unsigned int)m_this->m_vec_Line_Vertex.size();
			_DEICommand.baseInstance = (unsigned int)m_this->m_vec_Instance.size() - 1;

			m_this->m_Draw_Type_Queue.push_back(DRAW_TYPE_QUEUE(DRAW_TYPE_TRIANGLES, m_this->m_vec_DEICommand.size()));

			m_this->m_vec_DEICommand.push_back(_DEICommand);
		}
		else
			m_this->m_vec_DEICommand.back().count += 6;

		_Vertex.Color = _Color;

		_Vertex.Position.x = _x;
		_Vertex.Position.y = _y;
		m_this->m_vec_Line_Vertex.push_back(_Vertex);
		_Vertex.Position.x = _x;
		_Vertex.Position.y = _y + _h;
		m_this->m_vec_Line_Vertex.push_back(_Vertex);
		_Vertex.Position.x = _x + _w;
		_Vertex.Position.y = _y + _h;
		m_this->m_vec_Line_Vertex.push_back(_Vertex);
		_Vertex.Position.x = _x;
		_Vertex.Position.y = _y;
		m_this->m_vec_Line_Vertex.push_back(_Vertex);
		_Vertex.Position.x = _x + _w;
		_Vertex.Position.y = _y + _h;
		m_this->m_vec_Line_Vertex.push_back(_Vertex);
		_Vertex.Position.x = _x + _w;
		_Vertex.Position.y = _y;
		m_this->m_vec_Line_Vertex.push_back(_Vertex);
	});

	m_LastDrawType = DRAW_TYPE_TRIANGLES;
}

void DrawSimpleGraphics::SetColor(Magic::Color4 _Color) {
	m_NowColor = _Color;
}

void DrawSimpleGraphics::SetWorldMatrix(glm::mat4 _WorldMatrix) {
	m_WorldMatrix = _WorldMatrix;
}

void DrawSimpleGraphics::Render(Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message)
{
	if (m_this->m_vec_Line_Vertex.size())
	{
		Magic::Screen_Rect _Screen_Rect = m_this->Now_PTE->Rect();
		m_this->m_projectionMatrix = glm::ortho(0.0f, (float)_Screen_Rect.w, (float)_Screen_Rect.h, 0.0f, 0.1f, 100.0f);

		m_LineShader.Use();
		glUniformMatrix4fv(m_Line_projectionMatrix, 1, GL_FALSE, &m_this->m_projectionMatrix[0][0]);

		m_Line_VBO.DynamicWrite(0, (unsigned int)m_this->m_vec_Line_Vertex.size() * sizeof(LINE_VERTEX), &m_this->m_vec_Line_Vertex[0]);
		m_Line_VBO.DynamicWrite(1, (unsigned int)m_this->m_vec_Instance.size() * sizeof(LINE_INSTANCE), &m_this->m_vec_Instance[0]);
		m_Line_VBO.DynamicWrite(2, (unsigned int)m_this->m_vec_DEICommand.size() * sizeof(Magic::DrawArraysIndirectCommand), &m_this->m_vec_DEICommand[0]);


		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glLineWidth(1.0f);
		//glPointSize(1.0f);
		glUniform1f(m_Line_PointSize, 1.0f);

		m_Line_VBO.BindBuffer(2);
		m_Line_VBO.Bind();
		for (auto _auto = m_this->m_Draw_Type_Queue.begin(); _auto != m_this->m_Draw_Type_Queue.end(); _auto++) {
			GLenum _DrawType = GL_LINES;
			switch (_auto->Type)
			{
			case DRAW_TYPE_LINES:
				_DrawType = GL_LINES;
				glUniform1i(m_DrawType, 0);
				break;
			case DRAW_TYPE_TRIANGLES:
				_DrawType = GL_TRIANGLES;
				glUniform1i(m_DrawType, 0);
				break;
			case DRAW_TYPE_PICTURE_TEXT:
				_DrawType = GL_TRIANGLES;
				glUniform1i(m_DrawType, SHADER_DRAW_TEXT);
				glBindTexture(GL_TEXTURE_2D, _auto->m_Texture->GetTextrue());
				break;
			}

			GLsizei _DrawSize = 0;
			auto _Next = _auto + 1;
			if (_Next != m_this->m_Draw_Type_Queue.end()) {
				_DrawSize = _Next->StartPos - _auto->StartPos;
			}
			else {
				_DrawSize = m_this->m_vec_DEICommand.size() - _auto->StartPos;
			}

			glMultiDrawArraysIndirect(_DrawType, (GLvoid*)(sizeof(Magic::DrawArraysIndirectCommand) * _auto->StartPos), _DrawSize, 0);
		}

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	}

	m_this->m_vec_Line_Vertex.clear();
	m_this->m_vec_Instance.clear();
	m_this->m_vec_DEICommand.clear();
	m_this->m_Draw_Type_Queue.clear();
}