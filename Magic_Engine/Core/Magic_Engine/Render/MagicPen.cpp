#include "MagicPen.h"
#include "MagicEngineContext.h"
#include "MagicEngineAPI.h"

#include <GL/glew.h>  

namespace Magic
{
	static const char* S_Picture_Vertex =
		"#version 400\r\n"
		"layout(location = 0) in vec3 Position;"
		"layout(location = 1) in vec2 UV;"
		"layout(location = 2) in vec4 Color;"
		"layout(location = 3) in mat4 CameraMatrix;"
		"out vec2 TexCoord;"
		"out vec4 Out_Color;"

		"uniform mat4 projectionMatrix;"

		"void main()"
		"{"
		"	gl_Position = projectionMatrix * CameraMatrix * vec4(Position.x, Position.y, 0.0, 1.0);"
		"	TexCoord = UV;"
		"	Out_Color = Color;"
		"}";

	static const char* S_Picture_Frag =
		"#version 400\r\n"

		"in vec2 TexCoord;"
		"in vec4 Out_Color;"
		"out vec4 gl_FragColor;"

		"uniform sampler2D sampler0;"

		"void main()"
		"{"
		"	gl_FragColor = texture2D(sampler0, TexCoord) * Out_Color;"
		"}";


	static const char* S_DistanceText_Frag =
		"#version 400\r\n"

		"in vec2 TexCoord;"
		"in vec4 Out_Color;"
		"out vec4 gl_FragColor;"

		"uniform sampler2D sampler0;"
		"uniform vec2 distance_base_scale;"

		"void main()"
		"{"
		"	gl_FragColor = vec4(Out_Color.rgb ,(texture2D(sampler0, TexCoord).a * distance_base_scale.y + distance_base_scale.x) * Out_Color.a);"
		"}";

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

	CALLBACK_COMMON::CALLBACK_COMMON() :pCallBack_Common(NULL), m_Data(0)
	{}

	CALLBACK_COMMON::CALLBACK_COMMON(CallBack_Common _pCallBack, void* _data) : pCallBack_Common(_pCallBack), m_Data(_data)
	{}

	CALLBACK_COMMON::CALLBACK_COMMON(CallBack_Common _pCallBack) : pCallBack_Common(_pCallBack), m_Data(0)
	{}

	void CALLBACK_COMMON::operator()()
	{
		pCallBack_Common(m_Data);
	}

	Pen_Normal::MESSAGE_STATE::MESSAGE_STATE()
	{
		DrawFaceMode = No_Draw_Face;
		LineSMOOTH = 0;
		PolygonSMOOTH = 0;
		PointsSMOOTH = 0;
		DrawNumber = 0;
		pTexture = 0;
		AlphaState = 0;
		OverallMessage = 0;
	}

	Pen_Normal::PICTURE_DRAW::PICTURE_DRAW()
	{
		pFonts = 0;
		pNowTexture = 0;
		NewInstanceState = false;
		PitureUV[0].x = 0.0f;
		PitureUV[0].y = 0.0f;
		PitureUV[1].x = 1.0f;
		PitureUV[1].y = 0.0f;
		PitureUV[2].x = 1.0f;
		PitureUV[2].y = 1.0f;
		PitureUV[3].x = 0.0f;
		PitureUV[3].y = 1.0f;
	}

	void Pen_Normal::PICTURE_DRAW::Clear()
	{
		pFonts = 0;
		pNowTexture = 0;
		NewInstanceState = false;
		PitureUV[0].x = 0.0f;
		PitureUV[0].y = 0.0f;
		PitureUV[1].x = 1.0f;
		PitureUV[1].y = 0.0f;
		PitureUV[2].x = 1.0f;
		PitureUV[2].y = 1.0f;
		PitureUV[3].x = 0.0f;
		PitureUV[3].y = 1.0f;
		V_Vertex.clear();
		V_Instance.clear();
		V_DEICommand.clear();
		V_Index.clear();
	}

	Pen_Normal::LINE_DRAW::LINE_DRAW()
	{
		NewInstanceState = false;
	}

	void Pen_Normal::LINE_DRAW::Clear()
	{
		NewInstanceState = false;
		V_Vertex.clear();
		V_Instance.clear();
		V_DEICommand.clear();
	}

	Pen_Normal::SCISSOR_MESSAGE::SCISSOR_MESSAGE()
	{
		state = 0;
	}

	Pen_Normal::SCISSOR_MESSAGE::SCISSOR_MESSAGE(const bool & _state, const glm::vec4 & _scissorPosWH)
		:state(_state), ScissorPosWH(_scissorPosWH)
	{
	}

	Pen_Normal::LINEPATIERN_MESSAGE::LINEPATIERN_MESSAGE()
	{
		state = 0;
		factor = 0;
		pattern = 0;
	}

	Pen_Normal::DRAW_BOX::DRAW_BOX()
	{
		Create_Scissor_Message = false;
		Create_LineWitdh_Message = false;
		Create_LinePattern_Message = false;
		Create_PointSize_Message = false;
		LastCount = LastFirstIndex = Last_Draw_Type = 0;
		Draw_Number = 0;
		Draw_Number_Bk = 0;
		NowShader = 0;
	}

	void Pen_Normal::DRAW_BOX::Clear()
	{
		Create_Scissor_Message = false;
		Create_LineWitdh_Message = false;
		Create_LinePattern_Message = false;
		Create_PointSize_Message = false;
		LastCount = LastFirstIndex = Last_Draw_Type = 0;

		projectionMatrix = CameraMatrix = WorldMatrix = glm::mat4();
		LinePattern_Message.state = false;
		LinePattern_Message.factor = 0;
		LinePattern_Message.pattern = 0;
		Draw_Number = 0;
		Draw_Number_Bk = 0;
		NowShader = 0;
		Picture_Draw.Clear();
		Line_Draw.Clear();
		V_Scissor_Message.clear();
		V_Shader_Message.clear();
		V_Message.clear();
		V_LineWitdh_Message.clear();
		V_LinePattern_Message.clear();
		V_PointSize_Message.clear();
		V_CallBack_Message.clear();
		V_Fonts_Message.clear();
	}

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

#define SHADER_PICTURE							0x01
#define SHADER_PURE_COLOR						0x02
#define SHADER_DISTANCE_TEXT					0x03

#define MESSAGE_SCISSOR							1
#define MESSAGE_SHADER							1 << 1
#define MESSAGE_LINEWIDTH						1 << 2
#define MESSAGE_LINEPATTERN						1 << 3
#define MESSAGE_POINTSSIZE						1 << 4
#define MESSAGE_CALLBACK						1 << 5
#define MESSAGE_FONTS							1 << 6

	Pen_Normal::Pen_Normal()
	{
		m_Draw_Box_Number = -1;
		pNowDRAW_BOX = 0;
		m_DrawMessageNumber = 0;
		m_NowDraw_Box = 0;
	}

	Pen_Normal::~Pen_Normal()
	{
	}

	bool Pen_Normal::Initialize()
	{
		bool result;

		result = m_PictureShader.LoadFromString(GL_VERTEX_SHADER, S_Picture_Vertex);
		if (!result)
			return false;
		result = m_PictureShader.LoadFromString(GL_FRAGMENT_SHADER, S_Picture_Frag);
		if (!result)
			return false;
		result = m_PictureShader.CreateAndLinkProgram();
		if (!result)
			return false;

		m_PictureShader.Use();

		m_PictureShader.AddUniform("projectionMatrix");

		m_PictureShader.AddUniform("sampler0");

		glUniform1i(m_PictureShader("sampler0"), 0);

		m_PictureShader.UnUse();

		m_Picture2D_projectrionMatrix = m_PictureShader("projectionMatrix");
		//---------------------------------------------------------------------------
		result = m_TextShader.LoadFromString(GL_VERTEX_SHADER, S_Picture_Vertex);
		if (!result)
			return false;
		result = m_TextShader.LoadFromString(GL_FRAGMENT_SHADER, S_DistanceText_Frag);
		if (!result)
			return false;
		result = m_TextShader.CreateAndLinkProgram();
		if (!result)
			return false;

		m_TextShader.Use();

		m_TextShader.AddUniform("projectionMatrix");

		m_TextShader.AddUniform("sampler0");
		m_TextShader.AddUniform("distance_base_scale");

		glUniform1i(m_TextShader("sampler0"), 0);

		m_TextShader.UnUse();

		m_Text_distance_base_scale = m_TextShader("distance_base_scale");
		m_Text_projectrionMatrix = m_TextShader("projectionMatrix");
		//-------------------------------------------------------------------------------
		result = m_LineShader.LoadFromString(GL_VERTEX_SHADER, S_Pure_Color_Vertex);
		if (!result)
			return false;
		result = m_LineShader.LoadFromString(GL_FRAGMENT_SHADER, S_Pure_Color_Frag);
		if (!result)
			return false;
		result = m_LineShader.CreateAndLinkProgram();
		if (!result)
			return false;

		m_LineShader.Use();

		m_LineShader.AddUniform("CameraMatrix");
		m_LineShader.AddUniform("projectionMatrix");
		m_LineShader.AddUniform("PointSize");

		m_LineShader.UnUse();

		m_Line_projectionMatrix = m_LineShader("projectionMatrix");
		m_Line_PointSize = m_LineShader("PointSize");

		m_Picture_VBO.CreateBuffer(4);
		unsigned int _Array_Size[] = {
			sizeof(glm::vec3) / sizeof(float),
			sizeof(glm::vec2) / sizeof(float),

			sizeof(Magic::Color4) / sizeof(float),
			sizeof(glm::vec4) / sizeof(float),
			sizeof(glm::vec4) / sizeof(float),
			sizeof(glm::vec4) / sizeof(float),
			sizeof(glm::vec4) / sizeof(float) };
		m_Picture_VBO.SetBuffer(0, Magic::VERTEX_BUFFER::DYNAMIC_DRAW, 2, _Array_Size);
		m_Picture_VBO.SetBuffer(1, Magic::VERTEX_BUFFER::DYNAMIC_DRAW, 5, &_Array_Size[2], 1);
		m_Picture_VBO.SetIndexBuffer(2, Magic::VERTEX_BUFFER::DYNAMIC_DRAW);
		m_Picture_VBO.SetDrawIndirectBuffer(3, Magic::VERTEX_BUFFER::DYNAMIC_DRAW);

		m_Line_VBO.CreateBuffer(3);
		unsigned int _Line_Array_Size[] = {
			sizeof(glm::vec3) / sizeof(float),
			sizeof(Magic::Color4) / sizeof(float),

			sizeof(glm::vec4) / sizeof(float),
			sizeof(glm::vec4) / sizeof(float),
			sizeof(glm::vec4) / sizeof(float),
			sizeof(glm::vec4) / sizeof(float) };
		m_Line_VBO.SetBuffer(0, Magic::VERTEX_BUFFER::DYNAMIC_DRAW, 2, _Line_Array_Size);
		m_Line_VBO.SetBuffer(1, Magic::VERTEX_BUFFER::DYNAMIC_DRAW, 4, &_Line_Array_Size[1], 1);
		m_Line_VBO.SetDrawIndirectBuffer(2, Magic::VERTEX_BUFFER::DYNAMIC_DRAW);

		return true;
	}

	void Pen_Normal::DrawVertex(Pen_Normal::DRAW_MODE _drawMode, const float * _pPos, const Magic::Color4 * _pColor, unsigned int _Number)
	{
		switch (_drawMode)
		{
		case Magic::Pen_Normal::POINTS:
			AddShaderMessage(DRAW_TYPE_POINTS);
			break;
		case Magic::Pen_Normal::LINES:
			AddShaderMessage(DRAW_TYPE_LINES);
			break;
		case Magic::Pen_Normal::LINE_STRIP:
			AddShaderMessage(DRAW_TYPE_LINE_STRIP, true);
			break;
		case Magic::Pen_Normal::LINE_LOOP:
			AddShaderMessage(DRAW_TYPE_LINE_LOOP, true);
			break;
		case Magic::Pen_Normal::TRIANGLES:
			AddShaderMessage(DRAW_TYPE_TRIANGLES);
			break;
		case Magic::Pen_Normal::TRIANGLE_STRIP:
			AddShaderMessage(DRAW_TYPE_TRIANGLE_STRIP, true);
			break;
		case Magic::Pen_Normal::TRIANGLE_FAN:
			AddShaderMessage(DRAW_TYPE_TRIANGLE_FAN, true);
			break;
		}

		LINE_VERTEX _Vertex;

		if (pNowDRAW_BOX->Line_Draw.NewInstanceState)
		{
			LINE_INSTANCE _Instance;
			_Instance.WorldMatrix = pNowDRAW_BOX->WorldMatrix;
			pNowDRAW_BOX->Line_Draw.V_Instance.push_back(_Instance);
		}

		std::vector<LINE_VERTEX>* _pV_VERTEX = &pNowDRAW_BOX->Line_Draw.V_Vertex;

		if (!pNowDRAW_BOX->V_Message.back().DrawNumber || pNowDRAW_BOX->Line_Draw.NewInstanceState)
		{
			DrawArraysIndirectCommand _DEICommand;

			_DEICommand.count = _Number;
			_DEICommand.instanceCount = 1;
			_DEICommand.first = _pV_VERTEX->size();
			_DEICommand.baseInstance = pNowDRAW_BOX->Line_Draw.V_Instance.size() - 1;

			pNowDRAW_BOX->Line_Draw.V_DEICommand.push_back(_DEICommand);
			pNowDRAW_BOX->V_Message.back().DrawNumber++;

			pNowDRAW_BOX->Line_Draw.NewInstanceState = false;
		}
		else
			pNowDRAW_BOX->Line_Draw.V_DEICommand.back().count += _Number;

		pNowDRAW_BOX->LastCount = _Number;
		pNowDRAW_BOX->LastFirstIndex = _pV_VERTEX->size();
		pNowDRAW_BOX->Last_Draw_Type = SHADER_PURE_COLOR;
		pNowDRAW_BOX->Draw_Number++;

		pNowDRAW_BOX->Line_Draw.V_Vertex.reserve(_Number + pNowDRAW_BOX->Line_Draw.V_Vertex.size());

		_Number *= 2;
		for (unsigned int a = 0, b = 0; a < _Number;)
		{
			_Vertex.Color = _pColor[b++];
			_Vertex.Position.x = _pPos[a++];
			_Vertex.Position.y = _pPos[a++];
			pNowDRAW_BOX->Line_Draw.V_Vertex.push_back(_Vertex);
		}
	}
	void Pen_Normal::DrawVertex(Pen_Normal::DRAW_MODE _drawMode, const float * _pPos, unsigned int _Number)
	{
		switch (_drawMode)
		{
		case Magic::Pen_Normal::POINTS:
			AddShaderMessage(DRAW_TYPE_POINTS);
			break;
		case Magic::Pen_Normal::LINES:
			AddShaderMessage(DRAW_TYPE_LINES);
			break;
		case Magic::Pen_Normal::LINE_STRIP:
			AddShaderMessage(DRAW_TYPE_LINE_STRIP, true);
			break;
		case Magic::Pen_Normal::LINE_LOOP:
			AddShaderMessage(DRAW_TYPE_LINE_LOOP, true);
			break;
		case Magic::Pen_Normal::TRIANGLES:
			AddShaderMessage(DRAW_TYPE_TRIANGLES);
			break;
		case Magic::Pen_Normal::TRIANGLE_STRIP:
			AddShaderMessage(DRAW_TYPE_TRIANGLE_STRIP, true);
			break;
		case Magic::Pen_Normal::TRIANGLE_FAN:
			AddShaderMessage(DRAW_TYPE_TRIANGLE_FAN, true);
			break;
		}

		LINE_VERTEX _Vertex;

		if (pNowDRAW_BOX->Line_Draw.NewInstanceState)
		{
			LINE_INSTANCE _Instance;
			_Instance.WorldMatrix = pNowDRAW_BOX->WorldMatrix;
			pNowDRAW_BOX->Line_Draw.V_Instance.push_back(_Instance);
		}

		std::vector<LINE_VERTEX>* _pV_VERTEX = &pNowDRAW_BOX->Line_Draw.V_Vertex;

		if (!pNowDRAW_BOX->V_Message.back().DrawNumber || pNowDRAW_BOX->Line_Draw.NewInstanceState)
		{
			DrawArraysIndirectCommand _DEICommand;

			_DEICommand.count = _Number;
			_DEICommand.instanceCount = 1;
			_DEICommand.first = _pV_VERTEX->size();
			_DEICommand.baseInstance = pNowDRAW_BOX->Line_Draw.V_Instance.size() - 1;

			pNowDRAW_BOX->Line_Draw.V_DEICommand.push_back(_DEICommand);
			pNowDRAW_BOX->V_Message.back().DrawNumber++;

			pNowDRAW_BOX->Line_Draw.NewInstanceState = false;
		}
		else
			pNowDRAW_BOX->Line_Draw.V_DEICommand.back().count += _Number;

		pNowDRAW_BOX->LastCount = _Number;
		pNowDRAW_BOX->LastFirstIndex = _pV_VERTEX->size();
		pNowDRAW_BOX->Last_Draw_Type = SHADER_PURE_COLOR;
		pNowDRAW_BOX->Draw_Number++;

		pNowDRAW_BOX->Line_Draw.V_Vertex.reserve(_Number + pNowDRAW_BOX->Line_Draw.V_Vertex.size());
		_Vertex.Color = pNowDRAW_BOX->NowColor;
		for (unsigned int a = 0, b = _Number << 1; a < b;)
		{
			_Vertex.Position.x = _pPos[a++];
			_Vertex.Position.y = _pPos[a++];
			pNowDRAW_BOX->Line_Draw.V_Vertex.push_back(_Vertex);
		}
	}
	void Pen_Normal::DrawLine(float _x1, float _y1, float _x2, float _y2)
	{
		AddShaderMessage(DRAW_TYPE_LINES);

		LINE_VERTEX _Vertex;

		if (pNowDRAW_BOX->Line_Draw.NewInstanceState)
		{
			LINE_INSTANCE _Instance;
			_Instance.WorldMatrix = pNowDRAW_BOX->WorldMatrix;
			pNowDRAW_BOX->Line_Draw.V_Instance.push_back(_Instance);
		}

		std::vector<LINE_VERTEX>* _pV_VERTEX = &pNowDRAW_BOX->Line_Draw.V_Vertex;

		if (!pNowDRAW_BOX->V_Message.back().DrawNumber || pNowDRAW_BOX->Line_Draw.NewInstanceState)
		{
			DrawArraysIndirectCommand _DEICommand;

			_DEICommand.count = 2;
			_DEICommand.instanceCount = 1;
			_DEICommand.first = _pV_VERTEX->size();
			_DEICommand.baseInstance = pNowDRAW_BOX->Line_Draw.V_Instance.size() - 1;

			pNowDRAW_BOX->Line_Draw.V_DEICommand.push_back(_DEICommand);
			pNowDRAW_BOX->V_Message.back().DrawNumber++;

			pNowDRAW_BOX->Line_Draw.NewInstanceState = false;
		}
		else
			pNowDRAW_BOX->Line_Draw.V_DEICommand.back().count += 2;

		pNowDRAW_BOX->LastCount = 2;
		pNowDRAW_BOX->LastFirstIndex = pNowDRAW_BOX->Line_Draw.V_Vertex.size();
		pNowDRAW_BOX->Last_Draw_Type = SHADER_PURE_COLOR;
		pNowDRAW_BOX->Draw_Number++;

		_Vertex.Color = pNowDRAW_BOX->NowColor;;

		_Vertex.Position.x = _x1;
		_Vertex.Position.y = _y1;
		pNowDRAW_BOX->Line_Draw.V_Vertex.push_back(_Vertex);
		_Vertex.Position.x = _x2;
		_Vertex.Position.y = _y2;
		pNowDRAW_BOX->Line_Draw.V_Vertex.push_back(_Vertex);
	}
	void Pen_Normal::DrawPicture(float _x, float _y, float _w, float _h)
	{
		AddShaderMessage(DRAW_TYPE_PICTURE_TEXT);

		PICTURE_VERTEX _Vertex;

		DrawElementsIndirectCommand _DEICommand;

		PICTURE_DRAW* _pPICTURE_DRAW = &pNowDRAW_BOX->Picture_Draw;
		std::vector<PICTURE_VERTEX>* _pV_VERTEX = &_pPICTURE_DRAW->V_Vertex;
		std::vector<unsigned int>* _pV_Index = &_pPICTURE_DRAW->V_Index;

		if (pNowDRAW_BOX->V_Message.back().pTexture != _pPICTURE_DRAW->pNowTexture)
			BindPicture(_pPICTURE_DRAW->pNowTexture);

		unsigned int _Vertex_Number = _pV_VERTEX->size();

		if (pNowDRAW_BOX->Picture_Draw.NewInstanceState)
		{
			PICTURE_INSTANCE _Instance;
			_Instance.Color = pNowDRAW_BOX->NowColor;
			_Instance.WorldMatrix = pNowDRAW_BOX->WorldMatrix;
			_pPICTURE_DRAW->V_Instance.push_back(_Instance);
		}

		if (!pNowDRAW_BOX->V_Message.back().DrawNumber || pNowDRAW_BOX->Picture_Draw.NewInstanceState)
		{
			_DEICommand.count = 6;
			_DEICommand.instanceCount = 1;
			_DEICommand.firstIndex = _pV_Index->size();
			_DEICommand.baseVertex = 0;
			_DEICommand.baseInstance = _pPICTURE_DRAW->V_Instance.size() - 1;

			_pPICTURE_DRAW->V_DEICommand.push_back(_DEICommand);
			pNowDRAW_BOX->V_Message.back().DrawNumber++;

			pNowDRAW_BOX->Picture_Draw.NewInstanceState = false;
		}
		else
			pNowDRAW_BOX->Picture_Draw.V_DEICommand.back().count += 6;

		pNowDRAW_BOX->LastCount = 6;
		pNowDRAW_BOX->LastFirstIndex = _pV_Index->size();
		pNowDRAW_BOX->Last_Draw_Type = SHADER_PICTURE;
		pNowDRAW_BOX->Draw_Number++;

		_pV_VERTEX->reserve(4 + _pV_VERTEX->size());
		_Vertex.Position.x = _x;
		_Vertex.Position.y = _y;
		_Vertex.UV.x = _pPICTURE_DRAW->PitureUV[0].x;
		_Vertex.UV.y = _pPICTURE_DRAW->PitureUV[0].y;
		_pV_VERTEX->push_back(_Vertex);
		_Vertex.Position.x = _x + _w;
		_Vertex.Position.y = _y;
		_Vertex.UV.x = _pPICTURE_DRAW->PitureUV[1].x;
		_Vertex.UV.y = _pPICTURE_DRAW->PitureUV[1].y;
		_pV_VERTEX->push_back(_Vertex);
		_Vertex.Position.x = _x + _w;
		_Vertex.Position.y = _y + _h;
		_Vertex.UV.x = _pPICTURE_DRAW->PitureUV[2].x;
		_Vertex.UV.y = _pPICTURE_DRAW->PitureUV[2].y;
		_pV_VERTEX->push_back(_Vertex);
		_Vertex.Position.x = _x;
		_Vertex.Position.y = _y + _h;
		_Vertex.UV.x = _pPICTURE_DRAW->PitureUV[3].x;
		_Vertex.UV.y = _pPICTURE_DRAW->PitureUV[3].y;
		_pV_VERTEX->push_back(_Vertex);

		_pV_Index->reserve(6 + _pV_Index->size());
		_pV_Index->push_back(_Vertex_Number);
		_pV_Index->push_back(_Vertex_Number + 1);
		_pV_Index->push_back(_Vertex_Number + 2);
		_pV_Index->push_back(_Vertex_Number);
		_pV_Index->push_back(_Vertex_Number + 2);
		_pV_Index->push_back(_Vertex_Number + 3);
	}

	float Pen_Normal::DrawTEXT(float _x, float _y, const wchar_t * _text, float _fontsize, unsigned char _ArrayState)
	{
		int _textsize = wcslen(_text);
		Magic::Fonts* _pFonts = pNowDRAW_BOX->Picture_Draw.pFonts;
		if (_text && _textsize && _pFonts)
		{
			AddShaderMessage(DRAW_TYPE_DISTANCE_TEXT);

			PICTURE_VERTEX _Vertex;

			DrawElementsIndirectCommand _DEICommand;

			PICTURE_DRAW* _pPICTURE_DRAW = &pNowDRAW_BOX->Picture_Draw;
			std::vector<PICTURE_VERTEX>* _pV_VERTEX = &_pPICTURE_DRAW->V_Vertex;
			std::vector<unsigned int>* _pV_Index = &_pPICTURE_DRAW->V_Index;

			if (pNowDRAW_BOX->V_Message.back().pTexture != _pFonts->GetTexture())
				BindFonts(_pFonts);

			_pFonts->UpdataTexture(_text);

			unsigned int _Vertex_Number = _pV_VERTEX->size();

			if (pNowDRAW_BOX->Picture_Draw.NewInstanceState)
			{
				PICTURE_INSTANCE _Instance;
				_Instance.Color = pNowDRAW_BOX->NowColor;
				_Instance.WorldMatrix = pNowDRAW_BOX->WorldMatrix;
				_pPICTURE_DRAW->V_Instance.push_back(_Instance);
			}
			unsigned int* _pCount = 0;
			if (!pNowDRAW_BOX->V_Message.back().DrawNumber || pNowDRAW_BOX->Picture_Draw.NewInstanceState)
			{
				_DEICommand.count = 0;
				_DEICommand.instanceCount = 1;
				_DEICommand.firstIndex = _pV_Index->size();
				_DEICommand.baseVertex = 0;
				_DEICommand.baseInstance = _pPICTURE_DRAW->V_Instance.size() - 1;

				_pPICTURE_DRAW->V_DEICommand.push_back(_DEICommand);
				_pCount = &pNowDRAW_BOX->Picture_Draw.V_DEICommand.back().count;
				pNowDRAW_BOX->V_Message.back().DrawNumber++;

				pNowDRAW_BOX->Picture_Draw.NewInstanceState = false;
			}
			else
				_pCount = &pNowDRAW_BOX->Picture_Draw.V_DEICommand.back().count;

			pNowDRAW_BOX->LastCount = 0;
			pNowDRAW_BOX->LastFirstIndex = _pV_Index->size();
			pNowDRAW_BOX->Last_Draw_Type = SHADER_DISTANCE_TEXT;
			pNowDRAW_BOX->Draw_Number++;

			float _X = _x, _Y = _y;
			float const _rel_size = _fontsize / _pFonts->GetCharSize();
			for (int a = 0; a < _textsize; a++)
			{
				auto _char_index_advance = _pFonts->GetCharIndexAdvance(_text[a]);

				if (_char_index_advance.first != -1)
				{
					auto _font_info = _pFonts->GetFONT_INFO(_char_index_advance.first);
					float _left = _X + _font_info.left * _rel_size;
					float _height = _font_info.height * _rel_size;
					float _top = _Y - _font_info.top * _rel_size - _height;
					float _width = _font_info.width * _rel_size;


					auto _char_info = _pFonts->GetCHARINFO(_text[a]);

					_pV_VERTEX->reserve(4 + _pV_VERTEX->size());
					_Vertex.Position.x = _left;
					_Vertex.Position.y = _top;
					_Vertex.UV.x = _char_info.left;
					_Vertex.UV.y = _char_info.bottom;
					_pV_VERTEX->push_back(_Vertex);
					_Vertex.Position.x = _left + _width;
					_Vertex.Position.y = _top;
					_Vertex.UV.x = _char_info.right;
					_Vertex.UV.y = _char_info.bottom;
					_pV_VERTEX->push_back(_Vertex);
					_Vertex.Position.x = _left + _width;
					_Vertex.Position.y = _top + _height;
					_Vertex.UV.x = _char_info.right;
					_Vertex.UV.y = _char_info.top;
					_pV_VERTEX->push_back(_Vertex);
					_Vertex.Position.x = _left;
					_Vertex.Position.y = _top + _height;
					_Vertex.UV.x = _char_info.left;
					_Vertex.UV.y = _char_info.top;
					_pV_VERTEX->push_back(_Vertex);

					_pV_Index->reserve(6 + _pV_Index->size());
					_pV_Index->push_back(_Vertex_Number);
					_pV_Index->push_back(_Vertex_Number + 1);
					_pV_Index->push_back(_Vertex_Number + 2);
					_pV_Index->push_back(_Vertex_Number);
					_pV_Index->push_back(_Vertex_Number + 2);
					_pV_Index->push_back(_Vertex_Number + 3);

					_Vertex_Number += 4;
					pNowDRAW_BOX->LastCount += 6;
					*_pCount += 6;
				}

				_X += (_char_index_advance.second & 0xFFFF) * _rel_size;
				_Y += (_char_index_advance.second >> 16) * _rel_size;
			}

			return _X;
		}
		else
			return 0.0f;
	}

	void Pen_Normal::DrawRectangle(Pen_Normal::DRAW_MODE _drawMode, float _x, float _y, float _w, float _h)
	{
		switch (_drawMode)
		{
		case Magic::Pen_Normal::POINTS:
			AddShaderMessage(DRAW_TYPE_POINTS);
			break;
		case Magic::Pen_Normal::LINE_STRIP:
			AddShaderMessage(DRAW_TYPE_LINE_STRIP, true);
			break;
		case Magic::Pen_Normal::LINE_LOOP:
			AddShaderMessage(DRAW_TYPE_LINE_LOOP, true);
			break;
		case Magic::Pen_Normal::TRIANGLES:
			AddShaderMessage(DRAW_TYPE_TRIANGLES);
			break;
		}

		LINE_VERTEX _Vertex;

		if (pNowDRAW_BOX->Line_Draw.NewInstanceState)
		{
			LINE_INSTANCE _Instance;
			_Instance.WorldMatrix = pNowDRAW_BOX->WorldMatrix;
			pNowDRAW_BOX->Line_Draw.V_Instance.push_back(_Instance);
		}

		std::vector<LINE_VERTEX>* _pV_VERTEX = &pNowDRAW_BOX->Line_Draw.V_Vertex;

		if (!pNowDRAW_BOX->V_Message.back().DrawNumber || pNowDRAW_BOX->Line_Draw.NewInstanceState)
		{
			DrawArraysIndirectCommand _DEICommand;

			_DEICommand.count = 6;
			_DEICommand.instanceCount = 1;
			_DEICommand.first = _pV_VERTEX->size();
			_DEICommand.baseInstance = pNowDRAW_BOX->Line_Draw.V_Instance.size() - 1;

			pNowDRAW_BOX->Line_Draw.V_DEICommand.push_back(_DEICommand);
			pNowDRAW_BOX->V_Message.back().DrawNumber++;

			pNowDRAW_BOX->Line_Draw.NewInstanceState = false;
		}
		else
			pNowDRAW_BOX->Line_Draw.V_DEICommand.back().count += 6;

		pNowDRAW_BOX->LastCount = 6;
		pNowDRAW_BOX->LastFirstIndex = _pV_VERTEX->size();
		pNowDRAW_BOX->Last_Draw_Type = SHADER_PURE_COLOR;
		pNowDRAW_BOX->Draw_Number++;

		pNowDRAW_BOX->Line_Draw.V_Vertex.reserve(6 + pNowDRAW_BOX->Line_Draw.V_Vertex.size());

		_Vertex.Color = pNowDRAW_BOX->NowColor;

		_Vertex.Position.x = _x;
		_Vertex.Position.y = _y;
		_pV_VERTEX->push_back(_Vertex);
		_Vertex.Position.x = _x + _w;
		_Vertex.Position.y = _y;
		_pV_VERTEX->push_back(_Vertex);
		_Vertex.Position.x = _x + _w;
		_Vertex.Position.y = _y + _h;
		_pV_VERTEX->push_back(_Vertex);
		_Vertex.Position.x = _x + _w;
		_Vertex.Position.y = _y + _h;
		_pV_VERTEX->push_back(_Vertex);
		_Vertex.Position.x = _x;
		_Vertex.Position.y = _y + _h;
		_pV_VERTEX->push_back(_Vertex);
		_Vertex.Position.x = _x;
		_Vertex.Position.y = _y;
		_pV_VERTEX->push_back(_Vertex);
	}

	void Pen_Normal::RepeatDraw()
	{
		if (pNowDRAW_BOX->LastCount)
		{
			if (pNowDRAW_BOX->Last_Draw_Type == SHADER_PICTURE || pNowDRAW_BOX->Last_Draw_Type == SHADER_DISTANCE_TEXT)
			{
				if (pNowDRAW_BOX->Picture_Draw.NewInstanceState)
				{
					PICTURE_INSTANCE _Instance;
					_Instance.Color = pNowDRAW_BOX->NowColor;
					_Instance.WorldMatrix = pNowDRAW_BOX->WorldMatrix;
					pNowDRAW_BOX->Picture_Draw.V_Instance.push_back(_Instance);
					pNowDRAW_BOX->Picture_Draw.NewInstanceState = false;
				}

				DrawElementsIndirectCommand _DEICommand;

				_DEICommand.count = pNowDRAW_BOX->LastCount;
				_DEICommand.instanceCount = 1;
				_DEICommand.firstIndex = pNowDRAW_BOX->LastFirstIndex;
				_DEICommand.baseVertex = 0;
				_DEICommand.baseInstance = pNowDRAW_BOX->Picture_Draw.V_Instance.size() - 1;

				pNowDRAW_BOX->Picture_Draw.V_DEICommand.push_back(_DEICommand);
				pNowDRAW_BOX->V_Message.back().DrawNumber++;
			}
			else if (pNowDRAW_BOX->Last_Draw_Type == SHADER_PURE_COLOR)
			{
				if (pNowDRAW_BOX->Line_Draw.NewInstanceState)
				{
					LINE_INSTANCE _Instance;
					_Instance.WorldMatrix = pNowDRAW_BOX->WorldMatrix;
					pNowDRAW_BOX->Line_Draw.V_Instance.push_back(_Instance);
					pNowDRAW_BOX->Line_Draw.NewInstanceState = false;
				}

				DrawArraysIndirectCommand _DEICommand;

				_DEICommand.count = pNowDRAW_BOX->LastCount;
				_DEICommand.instanceCount = 1;
				_DEICommand.first = pNowDRAW_BOX->LastFirstIndex;
				_DEICommand.baseInstance = pNowDRAW_BOX->Line_Draw.V_Instance.size() - 1;

				pNowDRAW_BOX->Line_Draw.V_DEICommand.push_back(_DEICommand);
				pNowDRAW_BOX->V_Message.back().DrawNumber++;
			}

			pNowDRAW_BOX->Draw_Number++;
		}
	}

	void Pen_Normal::BindFonts(Magic::Fonts * _pFonts)
	{
		if (pNowDRAW_BOX->V_Message.back().pTexture != _pFonts->GetTexture())
		{
			AddMessage();
			pNowDRAW_BOX->V_Message.back().pTexture = _pFonts->GetTexture();
			if (pNowDRAW_BOX->Picture_Draw.pFonts != _pFonts)
			{
				pNowDRAW_BOX->Picture_Draw.pFonts = _pFonts;
				if (pNowDRAW_BOX->V_Message.back().OverallMessage & MESSAGE_FONTS)
					pNowDRAW_BOX->V_Fonts_Message.back() = _pFonts;
				else
				{
					pNowDRAW_BOX->V_Message.back().OverallMessage |= MESSAGE_FONTS;
					pNowDRAW_BOX->V_Fonts_Message.push_back(_pFonts);
				}
			}
		}
	}

	void Pen_Normal::BindPicture(MagicTexture * _pTexture)
	{
		if (pNowDRAW_BOX->V_Message.back().pTexture != _pTexture)
		{
			AddMessage();
			pNowDRAW_BOX->Picture_Draw.pNowTexture = _pTexture;
			pNowDRAW_BOX->V_Message.back().pTexture = _pTexture;
		}
	}

	void Pen_Normal::BindPictureUVPos(glm::vec2 _TictureWH, float _x, float _y, float _w, float _h)
	{
		/**/
	}

	void Pen_Normal::BindPictureUVPos(float _Left, float _Right, float _Up, float _Down)
	{
		glm::vec2* _pUV = pNowDRAW_BOX->Picture_Draw.PitureUV;
		_pUV[0].x = _Left;
		_pUV[0].y = _Down;
		_pUV[1].x = _Right;
		_pUV[1].y = _Down;
		_pUV[2].x = _Right;
		_pUV[2].y = _Up;
		_pUV[3].x = _Left;
		_pUV[3].y = _Up;
	}

	void Pen_Normal::BindPictureUVPosfault()
	{
		glm::vec2* _pUV = pNowDRAW_BOX->Picture_Draw.PitureUV;
		_pUV[0].x = 0.0f;
		_pUV[0].y = 0.0f;
		_pUV[1].x = 1.0f;
		_pUV[1].y = 0.0f;
		_pUV[2].x = 1.0f;
		_pUV[2].y = 1.0f;
		_pUV[3].x = 0.0f;
		_pUV[3].y = 1.0f;
	}

	void Pen_Normal::SetColor(const Magic::Color4 & _color)
	{
		pNowDRAW_BOX->NowColor = _color;
		pNowDRAW_BOX->Line_Draw.NewInstanceState = true;
		pNowDRAW_BOX->Picture_Draw.NewInstanceState = true;
	}

	void Pen_Normal::SetDrawWH(float _w, float _h)
	{
		pNowDRAW_BOX->CameraMatrix = CONST_CAMERA;
		pNowDRAW_BOX->projectionMatrix = glm::ortho(0.0f, _w, 0.0f, _h, 0.1f, 100.0f);
	}

	void Pen_Normal::EnableScissor()
	{
		if (!pNowDRAW_BOX->V_Scissor_Message.size() || !pNowDRAW_BOX->V_Scissor_Message.back().state)
		{
			AddMessage();
			pNowDRAW_BOX->V_Message.back().OverallMessage |= MESSAGE_SCISSOR;

			if (!pNowDRAW_BOX->Create_Scissor_Message)
			{
				pNowDRAW_BOX->V_Scissor_Message.push_back(SCISSOR_MESSAGE(
					true,
					pNowDRAW_BOX->V_Scissor_Message.size() ? pNowDRAW_BOX->V_Scissor_Message.back().ScissorPosWH : glm::vec4()
				));
				pNowDRAW_BOX->Create_Scissor_Message = true;
			}
			else
				pNowDRAW_BOX->V_Scissor_Message.back().state = true;
		}
	}

	void Pen_Normal::SetScissor(int _x, int _y, int _w, float _h)
	{
		AddMessage();
		pNowDRAW_BOX->V_Message.back().OverallMessage |= MESSAGE_SCISSOR;

		glm::vec4 _poswh(_x + pNowDRAW_BOX->CameraMatrix[3].x, _y + pNowDRAW_BOX->CameraMatrix[3].y, _w, _h);

		if (!pNowDRAW_BOX->Create_Scissor_Message)
		{
			pNowDRAW_BOX->V_Scissor_Message.push_back(SCISSOR_MESSAGE(
				pNowDRAW_BOX->V_Scissor_Message.size() ? pNowDRAW_BOX->V_Scissor_Message.back().state : false,
				_poswh
			));
		}
		else
			pNowDRAW_BOX->V_Scissor_Message.back().ScissorPosWH = _poswh;
	}

	void Pen_Normal::DisableScissor()
	{
		if (!pNowDRAW_BOX->V_Scissor_Message.size() || pNowDRAW_BOX->V_Scissor_Message.back().state)
		{
			AddMessage();
			pNowDRAW_BOX->V_Message.back().OverallMessage |= MESSAGE_SCISSOR;

			if (!pNowDRAW_BOX->Create_Scissor_Message)
			{
				pNowDRAW_BOX->V_Scissor_Message.push_back(SCISSOR_MESSAGE(false, glm::vec4()));
				pNowDRAW_BOX->Create_Scissor_Message = true;
			}
			else
				pNowDRAW_BOX->V_Scissor_Message.back().state = false;
		}
	}

	void Pen_Normal::SetPointSize(float _size)
	{
		if (!pNowDRAW_BOX->V_PointSize_Message.size() || pNowDRAW_BOX->V_PointSize_Message.back() != _size)
		{
			AddMessage();
			pNowDRAW_BOX->V_Message.back().OverallMessage |= MESSAGE_POINTSSIZE;
			if (!pNowDRAW_BOX->Create_PointSize_Message)
			{
				pNowDRAW_BOX->V_PointSize_Message.push_back(_size);
				pNowDRAW_BOX->Create_PointSize_Message = true;
			}
			else
				pNowDRAW_BOX->V_PointSize_Message.back() = _size;
		}
	}

	void Pen_Normal::EnablePointSMOOTH()
	{
		if (pNowDRAW_BOX->V_Message.back().PointsSMOOTH != true)
		{
			AddMessage();
			pNowDRAW_BOX->V_Message.back().PointsSMOOTH = true;
		}
	}

	void Pen_Normal::DisablePointSMOOTH()
	{
		if (pNowDRAW_BOX->V_Message.back().PointsSMOOTH != false)
		{
			AddMessage();
			pNowDRAW_BOX->V_Message.back().PointsSMOOTH = false;
		}
	}

	void Pen_Normal::SetLineWidth(float _width)
	{
		if (!pNowDRAW_BOX->V_LineWitdh_Message.size() || pNowDRAW_BOX->V_LineWitdh_Message.back() != _width)
		{
			AddMessage();
			pNowDRAW_BOX->V_Message.back().OverallMessage |= MESSAGE_LINEWIDTH;
			if (!pNowDRAW_BOX->Create_LineWitdh_Message)
			{
				pNowDRAW_BOX->V_LineWitdh_Message.push_back(_width);
				pNowDRAW_BOX->Create_LineWitdh_Message = true;
			}
			else
				pNowDRAW_BOX->V_LineWitdh_Message.back() = _width;
		}
	}

	void Pen_Normal::EnableLinePattern()
	{
		if (!pNowDRAW_BOX->LinePattern_Message.state)
		{
			AddMessage();
			pNowDRAW_BOX->V_Message.back().OverallMessage |= MESSAGE_LINEPATTERN;
			LINEPATIERN_MESSAGE _LinePattern_Message;
			_LinePattern_Message.state = pNowDRAW_BOX->LinePattern_Message.state = true;
			_LinePattern_Message.factor = pNowDRAW_BOX->LinePattern_Message.factor;
			_LinePattern_Message.pattern = pNowDRAW_BOX->LinePattern_Message.pattern;
			if (!pNowDRAW_BOX->Create_LinePattern_Message)
			{
				pNowDRAW_BOX->V_LinePattern_Message.push_back(_LinePattern_Message);
				pNowDRAW_BOX->Create_LinePattern_Message = true;
			}
			else
				pNowDRAW_BOX->V_LinePattern_Message.back() = _LinePattern_Message;
		}
	}

	void Pen_Normal::SetLinePattern(int _factor, unsigned short _pattern)
	{
		if (pNowDRAW_BOX->LinePattern_Message.factor != _factor ||
			pNowDRAW_BOX->LinePattern_Message.pattern != _pattern)
		{
			AddMessage();
			pNowDRAW_BOX->V_Message.back().OverallMessage |= MESSAGE_LINEPATTERN;
			LINEPATIERN_MESSAGE _LinePattern_Message;
			_LinePattern_Message.state = pNowDRAW_BOX->LinePattern_Message.state;
			_LinePattern_Message.factor = pNowDRAW_BOX->LinePattern_Message.factor = _factor;
			_LinePattern_Message.pattern = pNowDRAW_BOX->LinePattern_Message.pattern = _pattern;
			if (!pNowDRAW_BOX->Create_LinePattern_Message)
			{
				pNowDRAW_BOX->V_LinePattern_Message.push_back(_LinePattern_Message);
				pNowDRAW_BOX->Create_LinePattern_Message = true;
			}
			else
				pNowDRAW_BOX->V_LinePattern_Message.back() = _LinePattern_Message;
		}
	}

	void Pen_Normal::DisableLinePattern()
	{
		if (!pNowDRAW_BOX->LinePattern_Message.state)
		{
			AddMessage();
			pNowDRAW_BOX->V_Message.back().OverallMessage |= MESSAGE_LINEPATTERN;
			LINEPATIERN_MESSAGE _LinePattern_Message;
			_LinePattern_Message.state = pNowDRAW_BOX->LinePattern_Message.state = false;
			_LinePattern_Message.factor = pNowDRAW_BOX->LinePattern_Message.factor;
			_LinePattern_Message.pattern = pNowDRAW_BOX->LinePattern_Message.pattern;
			if (!pNowDRAW_BOX->Create_LinePattern_Message)
			{
				pNowDRAW_BOX->V_LinePattern_Message.push_back(_LinePattern_Message);
				pNowDRAW_BOX->Create_LinePattern_Message = true;
			}
			else
				pNowDRAW_BOX->V_LinePattern_Message.back() = _LinePattern_Message;
		}
	}

	void Pen_Normal::EnableAlpha()
	{
		if (pNowDRAW_BOX->V_Message.back().AlphaState != true)
		{
			AddMessage();
			pNowDRAW_BOX->V_Message.back().AlphaState = true;
		}
	}

	void Pen_Normal::DisableAlpha()
	{
		if (pNowDRAW_BOX->V_Message.back().AlphaState != false)
		{
			AddMessage();
			pNowDRAW_BOX->V_Message.back().AlphaState = false;
		}
	}

	void Pen_Normal::EnablePolygonSMOOTH()
	{
		if (pNowDRAW_BOX->V_Message.back().PolygonSMOOTH != true)
		{
			AddMessage();
			pNowDRAW_BOX->V_Message.back().PolygonSMOOTH = true;
		}
	}

	void Pen_Normal::DisablePolygonSMOOTH()
	{
		if (pNowDRAW_BOX->V_Message.back().PolygonSMOOTH != false)
		{
			AddMessage();
			pNowDRAW_BOX->V_Message.back().PolygonSMOOTH = false;
		}
	}

	void Pen_Normal::EnableLineSMOOTH()
	{
		if (pNowDRAW_BOX->V_Message.back().LineSMOOTH != true)
		{
			AddMessage();
			pNowDRAW_BOX->V_Message.back().LineSMOOTH = true;
		}
	}

	void Pen_Normal::DisableLineSMOOTH()
	{
		if (pNowDRAW_BOX->V_Message.back().LineSMOOTH != false)
		{
			AddMessage();
			pNowDRAW_BOX->V_Message.back().LineSMOOTH = false;
		}
	}

	void Pen_Normal::SetCameraMatrix(const glm::mat4 & _matrix)
	{
		pNowDRAW_BOX->CameraMatrix = pNowDRAW_BOX->WorldMatrix = _matrix;
		pNowDRAW_BOX->Line_Draw.NewInstanceState = true;
		pNowDRAW_BOX->Picture_Draw.NewInstanceState = true;
	}

	void Pen_Normal::SetWorldMatrix(const glm::mat4 & _matrix)
	{
		pNowDRAW_BOX->WorldMatrix = pNowDRAW_BOX->CameraMatrix * _matrix;
		pNowDRAW_BOX->Line_Draw.NewInstanceState = true;
		pNowDRAW_BOX->Picture_Draw.NewInstanceState = true;
	}

	void Pen_Normal::ResetWorldMatrix()
	{
		SetCameraMatrix(pNowDRAW_BOX->CameraMatrix);
	}

	void Pen_Normal::SetDrawFaceMode(Magic::Pen_Normal::DRAW_FACE_MODE _mode)
	{
		if (pNowDRAW_BOX->V_Message.back().DrawFaceMode != _mode)
		{
			AddMessage();
			pNowDRAW_BOX->V_Message.back().DrawFaceMode = _mode;
		}
	}

	void Pen_Normal::AddCallBackMessage(const CALLBACK_COMMON& _CallBack)
	{
		if (!(pNowDRAW_BOX->V_Message.back().OverallMessage & MESSAGE_CALLBACK))
		{
			AddMessage();
			ResetMessage();

			pNowDRAW_BOX->NowShader = DRAW_TYPE_RESET;
			pNowDRAW_BOX->V_Message.back().OverallMessage |= MESSAGE_CALLBACK;
			pNowDRAW_BOX->Draw_Number++;
		}

		pNowDRAW_BOX->V_CallBack_Message.push_back(_CallBack);
		pNowDRAW_BOX->V_Message.back().DrawNumber++;
	}

	void Pen_Normal::RenderStart()
	{
		m_Draw_Box_Number++;
		if (m_Draw_Box_Number >= (int)V_DRAW_BOX.size())
			V_DRAW_BOX.push_back(DRAW_BOX());
		m_NowDraw_Box = m_Draw_Box_Number;
		pNowDRAW_BOX = &V_DRAW_BOX[m_Draw_Box_Number];
		pNowDRAW_BOX->V_Message.push_back(MESSAGE_STATE());
	}

	void Pen_Normal::RenderEnd()
	{
		if (m_NowDraw_Box != m_Draw_Box_Number)
		{
			m_NowDraw_Box = m_Draw_Box_Number;
			pNowDRAW_BOX = &V_DRAW_BOX[m_Draw_Box_Number];
		}

		if (pNowDRAW_BOX->Draw_Number)
		{
			unsigned int _OverallMessage = 0, _ShaderMessage = 0, _LineWidthMessage = 0, _PointsSizeMessage = 0, _LinePatternMessageNumber = 0;

			if (pNowDRAW_BOX->Picture_Draw.V_Index.size())
			{
				m_PictureShader.Use();
				glUniformMatrix4fv(m_Picture2D_projectrionMatrix, 1, GL_FALSE, &pNowDRAW_BOX->projectionMatrix[0][0]);
				m_TextShader.Use();
				glUniformMatrix4fv(m_Text_projectrionMatrix, 1, GL_FALSE, &pNowDRAW_BOX->projectionMatrix[0][0]);

				PICTURE_DRAW* _pPICTURE_DRAW_BOX = &pNowDRAW_BOX->Picture_Draw;
				m_Picture_VBO.DynamicWrite(0, _pPICTURE_DRAW_BOX->V_Vertex.size() * sizeof(PICTURE_VERTEX), &_pPICTURE_DRAW_BOX->V_Vertex[0]);
				m_Picture_VBO.DynamicWrite(1, _pPICTURE_DRAW_BOX->V_Instance.size() * sizeof(PICTURE_INSTANCE), &_pPICTURE_DRAW_BOX->V_Instance[0]);
				m_Picture_VBO.DynamicWrite(2, _pPICTURE_DRAW_BOX->V_Index.size() * sizeof(unsigned int), &_pPICTURE_DRAW_BOX->V_Index[0]);
				m_Picture_VBO.DynamicWrite(3, _pPICTURE_DRAW_BOX->V_DEICommand.size() * sizeof(DrawElementsIndirectCommand), &_pPICTURE_DRAW_BOX->V_DEICommand[0]);
			}

			if (pNowDRAW_BOX->Line_Draw.V_Vertex.size())
			{
				m_LineShader.Use();
				glUniformMatrix4fv(m_Line_projectionMatrix, 1, GL_FALSE, &pNowDRAW_BOX->projectionMatrix[0][0]);

				LINE_DRAW* _pLINE_DRAW = &pNowDRAW_BOX->Line_Draw;
				m_Line_VBO.DynamicWrite(0, _pLINE_DRAW->V_Vertex.size() * sizeof(LINE_VERTEX), &_pLINE_DRAW->V_Vertex[0]);
				m_Line_VBO.DynamicWrite(1, _pLINE_DRAW->V_Instance.size() * sizeof(LINE_INSTANCE), &_pLINE_DRAW->V_Instance[0]);
				m_Line_VBO.DynamicWrite(2, _pLINE_DRAW->V_DEICommand.size() * sizeof(DrawArraysIndirectCommand), &_pLINE_DRAW->V_DEICommand[0]);

				glLineWidth(1.0f);
				//glPointSize(1.0f);
				glUniform1f(m_Line_PointSize, 1.0f);
			}

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0);

			MESSAGE_STATE _MESSAGE_STATEBuffer;
			glDisable(GL_BLEND);
			glBindTexture(GL_TEXTURE_2D, 0);

			unsigned int _Picture_Now_DrawNumber = 0, _Line_Now_DrawNumber = 0, _CallBack_Now_DrawNumber = 0, _Fonts_Now_DrawNumber = 0;
			unsigned int _Now_Shader = 0, _Now_Draw_Type = 0;
			Magic::Fonts* _pBufferFonts = NULL;

			Magic::VERTEX_BUFFER* _LastVertex = 0;
			LINEPATIERN_MESSAGE _LinePattern_Message;
			SCISSOR_MESSAGE _SCISSOR_MESSAGE;

			std::vector<MESSAGE_STATE>* _pV_MESSAGE_STATE = &pNowDRAW_BOX->V_Message;

			for (std::vector<MESSAGE_STATE>::iterator _iterator = _pV_MESSAGE_STATE->begin(); _iterator != _pV_MESSAGE_STATE->end(); _iterator++)
			{
				if (_iterator->AlphaState != _MESSAGE_STATEBuffer.AlphaState)
				{
					_MESSAGE_STATEBuffer.AlphaState = _iterator->AlphaState;
					if (_MESSAGE_STATEBuffer.AlphaState)
						glEnable(GL_BLEND);
					else
						glDisable(GL_BLEND);
				}

				if (_iterator->DrawFaceMode != _MESSAGE_STATEBuffer.DrawFaceMode)
				{
					_MESSAGE_STATEBuffer.DrawFaceMode = _iterator->DrawFaceMode;
					switch (_MESSAGE_STATEBuffer.DrawFaceMode)
					{
					case Magic::Pen_Normal::Draw_FrontFace:
						glEnable(GL_CULL_FACE);
						glCullFace(GL_BACK);
						break;
					case Magic::Pen_Normal::Draw_BackFace:
						glEnable(GL_CULL_FACE);
						glCullFace(GL_FRONT);
						break;
					case Magic::Pen_Normal::Draw_DoubleFace:
						glDisable(GL_CULL_FACE);
						break;
					}
				}

				if (_iterator->LineSMOOTH != _MESSAGE_STATEBuffer.LineSMOOTH)
				{
					_MESSAGE_STATEBuffer.LineSMOOTH = _iterator->LineSMOOTH;
					if (_MESSAGE_STATEBuffer.LineSMOOTH)
					{
						glEnable(GL_LINE_SMOOTH);
						glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
					}
					else
					{
						glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
						glDisable(GL_LINE_SMOOTH);
					}
				}

				if (_iterator->PointsSMOOTH != _MESSAGE_STATEBuffer.PointsSMOOTH)
				{
					_MESSAGE_STATEBuffer.PointsSMOOTH = _iterator->PointsSMOOTH;
					if (_MESSAGE_STATEBuffer.PointsSMOOTH)
					{
						glEnable(GL_POINT_SMOOTH);
						glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
					}
					else
					{
						glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
						glDisable(GL_POINT_SMOOTH);
					}
				}

				if (_iterator->PolygonSMOOTH != _MESSAGE_STATEBuffer.PolygonSMOOTH)
				{
					_MESSAGE_STATEBuffer.PolygonSMOOTH = _iterator->PolygonSMOOTH;
					if (_MESSAGE_STATEBuffer.PolygonSMOOTH)
					{
						glEnable(GL_POLYGON_SMOOTH);
						glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
					}
					else
					{
						glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
						glDisable(GL_POLYGON_SMOOTH);
					}
				}

				if (_iterator->pTexture != _MESSAGE_STATEBuffer.pTexture)
				{
					_MESSAGE_STATEBuffer.pTexture = _iterator->pTexture;
					if (_MESSAGE_STATEBuffer.pTexture)
						glBindTexture(GL_TEXTURE_2D, _MESSAGE_STATEBuffer.pTexture->GetTextrue());
					else
						glBindTexture(GL_TEXTURE_2D, NULL);
				}

				if (_iterator->OverallMessage & MESSAGE_SCISSOR)
				{
					SCISSOR_MESSAGE* _pSCISSOR_MESSAGE = &pNowDRAW_BOX->V_Scissor_Message[_OverallMessage++];
					if (_SCISSOR_MESSAGE.state != _pSCISSOR_MESSAGE->state)
					{
						if (_pSCISSOR_MESSAGE->state)
							glEnable(GL_SCISSOR_TEST);
						else
							glDisable(GL_SCISSOR_TEST);
					}
					if (_SCISSOR_MESSAGE.ScissorPosWH != _pSCISSOR_MESSAGE->ScissorPosWH)
						glScissor((int)_pSCISSOR_MESSAGE->ScissorPosWH.x, (int)_pSCISSOR_MESSAGE->ScissorPosWH.y,
						(int)_pSCISSOR_MESSAGE->ScissorPosWH.z, (int)_pSCISSOR_MESSAGE->ScissorPosWH.w);

					_SCISSOR_MESSAGE = *_pSCISSOR_MESSAGE;
				}

				if (_iterator->OverallMessage & MESSAGE_LINEPATTERN)
				{
					LINEPATIERN_MESSAGE _LINEPATTERN_MESSAGE = pNowDRAW_BOX->V_LinePattern_Message[_LinePatternMessageNumber++];
					if (_LinePattern_Message.state != _LINEPATTERN_MESSAGE.state)
					{
						_LinePattern_Message.state = _LINEPATTERN_MESSAGE.state;
						if (_LinePattern_Message.state)
							glEnable(GL_LINE_STIPPLE);
						else
							glDisable(GL_LINE_STIPPLE);
					}

					if (_LinePattern_Message.factor != _LINEPATTERN_MESSAGE.factor ||
						_LinePattern_Message.pattern != _LINEPATTERN_MESSAGE.pattern)
					{
						_LinePattern_Message.factor = _LINEPATTERN_MESSAGE.factor;
						_LinePattern_Message.pattern = _LINEPATTERN_MESSAGE.pattern;
						glLineStipple(_LinePattern_Message.factor, _LinePattern_Message.pattern);
					}
				}

				if (_iterator->OverallMessage & MESSAGE_FONTS)
				{
					_pBufferFonts = pNowDRAW_BOX->V_Fonts_Message[_Fonts_Now_DrawNumber++];
				}

				if (_iterator->OverallMessage & MESSAGE_SHADER)
				{
					_Now_Draw_Type = pNowDRAW_BOX->V_Shader_Message[_ShaderMessage++];
					switch (_Now_Draw_Type)
					{
					case DRAW_TYPE_PICTURE_TEXT:
						if (_Now_Shader != SHADER_PICTURE)
						{
							_Now_Shader = SHADER_PICTURE;
							m_PictureShader.Use();
						}
						break;
					case DRAW_TYPE_DISTANCE_TEXT:
						if (_Now_Shader != SHADER_DISTANCE_TEXT)
						{
							_Now_Shader = SHADER_DISTANCE_TEXT;
							m_TextShader.Use();
						}
						break;
					case DRAW_TYPE_POINTS:
					case DRAW_TYPE_LINES:
					case DRAW_TYPE_LINE_STRIP:
					case DRAW_TYPE_LINE_LOOP:
					case DRAW_TYPE_TRIANGLES:
					case DRAW_TYPE_TRIANGLE_STRIP:
					case DRAW_TYPE_TRIANGLE_FAN:
						if (_Now_Shader != SHADER_PURE_COLOR)
						{
							_Now_Shader = SHADER_PURE_COLOR;
							m_LineShader.Use();
						}
						break;
					}
				}

				if (_iterator->OverallMessage & MESSAGE_POINTSSIZE)
				{
					glUniform1f(m_Line_PointSize, pNowDRAW_BOX->V_PointSize_Message[_PointsSizeMessage++]);
				}

				if (_iterator->OverallMessage & MESSAGE_CALLBACK)
				{
					for (int c = _CallBack_Now_DrawNumber,
						r = _CallBack_Now_DrawNumber += _iterator->DrawNumber;
						c < r; c++)
						pNowDRAW_BOX->V_CallBack_Message[c]();
				}
				else
				{
					GLenum _Color_DrawMode = GL_POINTS;
					switch (_Now_Draw_Type)
					{
					case DRAW_TYPE_DISTANCE_TEXT:
						if (_pBufferFonts)
						{
							glUniform2f(m_Text_distance_base_scale, _pBufferFonts->GetBase(), _pBufferFonts->GetScale());
							_pBufferFonts = NULL;
						}
						break;
					case DRAW_TYPE_POINTS:
						_Color_DrawMode = GL_POINTS;
						break;
					case DRAW_TYPE_LINE_STRIP:
						_Color_DrawMode = GL_LINE_STRIP;
						break;
					case DRAW_TYPE_LINE_LOOP:
						_Color_DrawMode = GL_LINE_LOOP;
						break;
					case DRAW_TYPE_TRIANGLES:
						_Color_DrawMode = GL_TRIANGLES;
						break;
					case DRAW_TYPE_TRIANGLE_STRIP:
						_Color_DrawMode = GL_TRIANGLE_STRIP;
						break;
					case DRAW_TYPE_TRIANGLE_FAN:
						_Color_DrawMode = GL_TRIANGLE_FAN;
						break;
					case DRAW_TYPE_LINES:
						_Color_DrawMode = GL_LINES;
						break;
					}

					switch (_Now_Draw_Type)
					{
					case DRAW_TYPE_PICTURE_TEXT:
					case DRAW_TYPE_DISTANCE_TEXT:
						if (_LastVertex != &m_Picture_VBO)
						{
							_LastVertex = &m_Picture_VBO;
							m_Picture_VBO.BindBuffer(3);
							m_Picture_VBO.Bind();
						}
						if (_iterator->DrawNumber)
						{
							glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, (GLvoid*)(sizeof(DrawElementsIndirectCommand)* _Picture_Now_DrawNumber), _iterator->DrawNumber, 0);
							_Picture_Now_DrawNumber += _iterator->DrawNumber;
							//DEBUG_AddDrawMessageNumber(1);
						}
						break;
					case DRAW_TYPE_POINTS:
					case DRAW_TYPE_LINE_STRIP:
					case DRAW_TYPE_LINE_LOOP:
					case DRAW_TYPE_TRIANGLES:
					case DRAW_TYPE_TRIANGLE_STRIP:
					case DRAW_TYPE_TRIANGLE_FAN:
					case DRAW_TYPE_LINES:
						if (_LastVertex != &m_Line_VBO)
						{
							_LastVertex = &m_Line_VBO;
							m_Line_VBO.BindBuffer(2);
							m_Line_VBO.Bind();
						}
						if (_iterator->DrawNumber)
						{
							glMultiDrawArraysIndirect(_Color_DrawMode, (GLvoid*)(sizeof(DrawArraysIndirectCommand)* _Line_Now_DrawNumber), _iterator->DrawNumber, 0);
							_Line_Now_DrawNumber += _iterator->DrawNumber;
							//DEBUG_AddDrawMessageNumber(1);
						}
						break;
					}
				}
			}

			m_Picture_VBO.UnBindBuffer(3);
			m_Picture_VBO.UnBind();
			glDisable(GL_TEXTURE_2D);
			m_PictureShader.UnUse();

			if (_MESSAGE_STATEBuffer.PointsSMOOTH)
			{
				glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
				glDisable(GL_POINT_SMOOTH);
			}

			if (_MESSAGE_STATEBuffer.LineSMOOTH)
			{
				glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
				glDisable(GL_LINE_SMOOTH);
			}

			if (_MESSAGE_STATEBuffer.PolygonSMOOTH)
			{
				glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
				glDisable(GL_POLYGON_SMOOTH);
			}

			if (_LinePattern_Message.state)
				glDisable(GL_LINE_STIPPLE);

			if (_SCISSOR_MESSAGE.state)
				glDisable(GL_SCISSOR_TEST);
		}

		pNowDRAW_BOX->Clear();
		m_Draw_Box_Number--;
		pNowDRAW_BOX = m_Draw_Box_Number >= 0 ? &V_DRAW_BOX[m_Draw_Box_Number] : 0;
	}

	void Pen_Normal::SetNowRenderArea(int _pos)
	{
		if (_pos != m_NowDraw_Box && _pos <= m_Draw_Box_Number)
		{
			m_NowDraw_Box = _pos;
			pNowDRAW_BOX = &V_DRAW_BOX[m_NowDraw_Box];
		}
	}

	void Pen_Normal::AddMessage()
	{
		if (pNowDRAW_BOX->Draw_Number_Bk != pNowDRAW_BOX->Draw_Number)
		{
			pNowDRAW_BOX->Draw_Number_Bk = pNowDRAW_BOX->Draw_Number;
			MESSAGE_STATE _MESSAGE_STATE = pNowDRAW_BOX->V_Message.back();
			_MESSAGE_STATE.OverallMessage = 0;
			_MESSAGE_STATE.DrawNumber = 0;
			pNowDRAW_BOX->V_Message.push_back(_MESSAGE_STATE);
		}
	}

	void Pen_Normal::AddShaderMessage(unsigned int _shader, bool _add)
	{
		if (pNowDRAW_BOX->NowShader != _shader || _add)
		{
			pNowDRAW_BOX->NowShader = _shader;
			AddMessage();
			pNowDRAW_BOX->V_Message.back().OverallMessage |= MESSAGE_SHADER;
			pNowDRAW_BOX->V_Shader_Message.push_back(_shader);
		}
		ResetMessage();
	}

	void Pen_Normal::ResetMessage()
	{
		pNowDRAW_BOX->Create_LinePattern_Message = false;
		pNowDRAW_BOX->Create_LineWitdh_Message = false;
		pNowDRAW_BOX->Create_Scissor_Message = false;
		pNowDRAW_BOX->Create_PointSize_Message = false;
	}
}