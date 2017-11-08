#pragma once

#include "glm.hpp"

#include <gtc/matrix_transform.hpp>
#include <gtx/transform2.hpp>
#include "MagicShader.h"
#include "MagicTexture.h"
#include "MagicVertex.h"

#include "vector"


namespace Magic
{
	class Pen_Common
	{
	public:
		virtual void RenderStart() = 0;
		virtual void RenderEnd() = 0;
	};

	class Pen_Normal :public Pen_Common
	{
	public:
		enum DRAW_FACE_MODE
		{
			No_Draw_Face = 0x00,
			Draw_FrontFace = 0x01,
			Draw_BackFace,
			Draw_DoubleFace
		};
	private:
		struct PICTRUE_VERTEX
		{
			glm::vec3 Position;
			glm::vec2 UV;
			Magic::Color4 Color;
		};

		struct LINE_VERTEX
		{
			glm::vec2 Position;
			Magic::Color4 Color;
		};

		struct MESSAGE_STATE
		{
			unsigned int DrawNumber;
			MagicTexture* pTexture;
			bool AlphaState;
			bool LineSMOOTH;
			bool PolygonSMOOTH;
			bool PointsSMOOTH;
			DRAW_FACE_MODE DrawFaceMode;
			unsigned int OverallMessage;
			MESSAGE_STATE();
		};

		struct PICTRUE_DRAW
		{
			glm::vec2 m_PitrueUV[4];
			MagicTexture* pNowTexture;
			std::vector<PICTRUE_VERTEX> V_Vertex;
			std::vector<unsigned int> V_Index;

			PICTRUE_DRAW();

			void Clear();
		};

		struct LINE_DRAW
		{
			std::vector<LINE_VERTEX> V_Vertex;
			void Clear();
		};

		struct SCISSOR_MESSAGE
		{
			bool state;
			glm::vec4 ScissorPosWH;
			SCISSOR_MESSAGE();
			SCISSOR_MESSAGE(const bool& _state, const glm::vec4& _scissorPosWH);
		};

		struct LINEPATIERN_MESSAGE
		{
			bool state;
			int factor;
			unsigned short pattern;
			LINEPATIERN_MESSAGE();
		};

		struct VBO_VERTEX
		{
			Magic::VERTEX_BUFFER* VertexBuffer;
			unsigned int StartPos;
			unsigned int DrawNumber;
			VBO_VERTEX();
		};

		struct DRAW_BOX
		{
			unsigned int Draw_Number, Draw_Number_Bk;
			glm::mat4 projectionMatrix;
			glm::mat4 CameraMatrix;
			Magic::Color4 NowColor;
			unsigned char NowShader;

			bool Create_Scissor_Message;
			bool Create_LineWitdh_Message;
			bool Create_PointSize_Message;
			bool Create_LinePattern_Message;
			bool Create_CameraMatrix_Message;

			LINEPATIERN_MESSAGE LinePattern_Message;
			std::vector<MESSAGE_STATE> V_Message;
			std::vector<SCISSOR_MESSAGE> V_Scissor_Message;
			std::vector<unsigned char> V_Shader_Message;
			std::vector<float> V_LineWitdh_Message;
			std::vector<float> V_PointSize_Message;
			std::vector<LINEPATIERN_MESSAGE> V_LinePattern_Message;
			std::vector<VBO_VERTEX> V_VBO_VERTEX;
			std::vector<glm::mat4> V_CameraMatrix;
			PICTRUE_VERTEX Pictrue_Draw;
			LINE_DRAW Line_Draw;

			DRAW_BOX();
			void Clear();
		};
	public:
		enum DRAW_MODE {
			POINTS,
			LINES,
			LINE_STRIP,
			LINE_LOOP,
			TRIANGLES,
			TRIANGLE_STRIP,
			TRIANGLE_FAN
		};

		Pen_Normal();
		virtual ~Pen_Normal();

		bool Initialize();

	private:
		int m_Draw_Box_Number;
		Magic::VERTEX_BUFFER m_Pitrue_VBO;
		MagicShader m_PitrueShader;
		unsigned int m_Picture2D_projectrionMatrix;
		unsigned int m_Picture2D_CameraMatrix;

		Magic::VERTEX_BUFFER m_Line_VBO;
		MagicShader m_LineShader;
		unsigned int m_Line_projectionMatrix;
		unsigned int m_Line_CameraMatrix;
		unsigned int m_Line_PointSize;


		int m_NowDraw_Box;
		DRAW_BOX* pNowDRAW_BOX;
		std::vector<DRAW_BOX> V_DRAW_BOX;

		unsigned int m_DrawMessageNumber;
	};

}

