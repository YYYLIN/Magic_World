#ifndef _MAGICPEN_H_
#define _MAGICPEN_H_

#include "glm.hpp"

#include <gtc/matrix_transform.hpp>
#include <gtx/transform2.hpp>
#include "MagicShader.h"
#include "MagicTexture.h"
#include "MagicVertex.h"
#include "Magic_Macro.h"
#include "Magic_Fonts.h"
#include "MagicMessage.h"

#include "vector"

#pragma warning(push)
#pragma warning(disable:4251)

namespace Magic
{
	typedef  struct {
		unsigned int count;
		unsigned int instanceCount;
		unsigned int first;
		unsigned int baseInstance;
	} DrawArraysIndirectCommand;

	typedef struct {
		unsigned int count;
		unsigned int instanceCount;
		unsigned int firstIndex;
		unsigned int baseVertex;
		unsigned int baseInstance;
	} DrawElementsIndirectCommand;

	class Pen_Common
	{
	public:
		virtual void RenderStart() = 0;
		virtual void RenderEnd() = 0;
	};

	class DLL_MAGIC_ENGINE_OUTPUT_INPUT Pen_Normal :public Pen_Common
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
		struct PICTURE_VERTEX
		{
			glm::vec3 Position;
			glm::vec2 UV;
		};

		struct PICTURE_INSTANCE
		{
			Magic::Color4 Color;
			glm::mat4 WorldMatrix;
		};

		struct LINE_VERTEX
		{
			glm::vec3 Position;
			Magic::Color4 Color;
		};

		struct LINE_INSTANCE
		{
			glm::mat4 WorldMatrix;
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

		struct PICTURE_DRAW
		{
			glm::vec2 PitureUV[4];
			Magic::Fonts* pFonts;
			MagicTexture* pNowTexture;
			bool NewInstanceState;
			std::vector<PICTURE_VERTEX> V_Vertex;
			std::vector<PICTURE_INSTANCE> V_Instance;
			std::vector<DrawElementsIndirectCommand> V_DEICommand;
			std::vector<unsigned int> V_Index;

			PICTURE_DRAW();

			void Clear();
		};

		struct LINE_DRAW
		{
			bool NewInstanceState;
			std::vector<LINE_VERTEX> V_Vertex;
			std::vector<LINE_INSTANCE> V_Instance;
			std::vector<DrawArraysIndirectCommand> V_DEICommand;

			LINE_DRAW();
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

		struct DRAW_BOX
		{
			unsigned int Draw_Number, Draw_Number_Bk;
			glm::mat4 projectionMatrix;
			glm::mat4 WorldMatrix;
			glm::mat4 CameraMatrix;
			Magic::Color4 NowColor;
			unsigned char NowShader;
			unsigned int LastCount, LastFirstIndex, Last_Draw_Type;

			bool Create_Scissor_Message;
			bool Create_LineWitdh_Message;
			bool Create_PointSize_Message;
			bool Create_LinePattern_Message;

			LINEPATIERN_MESSAGE LinePattern_Message;
			std::vector<MESSAGE_STATE> V_Message;
			std::vector<SCISSOR_MESSAGE> V_Scissor_Message;
			std::vector<unsigned char> V_Shader_Message;
			std::vector<float> V_LineWitdh_Message;
			std::vector<float> V_PointSize_Message;
			std::vector<LINEPATIERN_MESSAGE> V_LinePattern_Message;
			std::vector<CALLBACK_COMMON> V_CallBack_Message;
			PICTURE_DRAW Picture_Draw;
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

		/*
		����:��Ⱦ����
		����:
		_drawMode = (LINES | POINTS | LINE_STRIP | LINE_LOOP | TRIANGLES | TRIANGLE_STRIP | TRIANGLE_FAN);
		_pPos = ָ����ʼλ�õ�ָ�� (struct float x, y;)
		_pColor = ÿ�������ɫָ��
		_number = �������
		����ֵ:��
		*/
		void DrawVertex(Pen_Normal::DRAW_MODE _drawMode, const float* _pPos, const Magic::Color4* _pColor, unsigned int _Number);

		/*
		����:��Ⱦ����
		����:
		_drawMode = (LINES | POINTS | LINE_STRIP | LINE_LOOP | TRIANGLES | TRIANGLE_STRIP | TRIANGLE_FAN);
		_pPos = ָ����ʼλ�õ�ָ�� (struct float x, y;)
		_number = λ�õ������
		����ֵ:��
		*/
		void DrawVertex(Pen_Normal::DRAW_MODE _drawMode, const float* _pPos, unsigned int _Number);

		/*
		����:��Ⱦֱ��
		����:
		_x1 = ��һ����x
		_y1 = ��һ����y
		_x2 = �ڶ�����x
		_y2 = �ڶ�����y
		����ֵ:��
		*/
		void DrawLine(float _x1, float _y1, float _x2, float _y2);

		/*
		����:�����½�Ϊԭ����ȾͼƬ
		����:
		_x = ���½�x
		_y = ���½�y
		_w = ���
		_h = �߶�
		����ֵ:��
		*/
		void DrawPicture(float _x, float _y, float _w, float _h);

		/*
		����:��Ⱦ����
		����:
		_x = ���½�x
		_y = ���½�y
		_text = �ı�
		_ArrayState = ����ģʽ CHARATER_LEFT_ARRAY | CHARATER_RIGHT_ARRAY | CHARATER_MIDDLE_ARRAY
		����ֵ:
		����Ľ�βλ��
		*/
		int DrawTEXT(float _x, float _y, const char* _text, unsigned char _ArrayState = CHARATER_LEFT_ARRAY);

		/*
		����:�����½�Ϊԭ����Ⱦ����
		����:
		_drawMode = POINTS | LINE_LOOP | LINES_STRIP | TRIANGLES
		_x = ���½�x
		_y = ���½�y
		_w = ���
		_h = �߶�
		����ֵ:
		*/
		void DrawRectangle(Pen_Normal::DRAW_MODE  _drawMode, float _x, float _y, float _w, float _h);

		/*
		����:�ظ���һ�ε���Ⱦ������Ⱦ
		(���޸�����DrawPicture  DrawTEXT ��ɫ �� ����
		DrawRectangle DrawLine DrawVertex ����)
		����:��
		����ֵ:��
		*/
		void RepeatDraw();

		/*
		����:������
		����:
		_pFonts = ����
		����ֵ:��
		*/
		void BindFonts(Magic::Fonts* _pFonts);

		/*
		����:��ͼƬ
		����:
		_pTextrue = ����ָ��
		����ֵ:��
		*/
		void BindPicture(MagicTexture* _pTexture);

		/*
		����:��ͼƬ��UV�����ȡ��������
		����:
		_TictureWH = ͼƬ���
		_x = ���½�x
		_y = ���½�y
		_w = Ҫ��ȡ�Ŀ�
		_h = Ҫ��ȡ�ĸ�
		����ֵ:��
		*/
		void BindPictureUVPos(glm::vec2 _TictureWH, float _x, float _y, float _w, float _h);

		/*
		����:��ͼƬ��UV����
		����:
		_Left = ��ߵ�ֵ
		_Right = �ұߵ�ֵ
		_Up = �ϱߵ�ֵ
		_Down = �±ߵ�ֵ
		����ֵ:��
		*/
		void BindPictureUVPos(float _Left, float _Right, float _Up, float _Down);

		/*
		����:��ͼƬ��UV����ΪĬ��
		����:��
		����ֵ:��
		*/
		void BindPictureUVPosfault();

		/*
		����:����һ����ɫ
		����:
		_color = ��ɫ
		����ֵ:��
		*/
		void SetColor(const Magic::Color4& _color);

		/*
		����:������Ⱦ���(�˺�����RenderStart��ʼ���������һ��)
		����:
		_w = ��
		_h = ��
		����ֵ:
		*/
		void SetDrawWH(float _w, float _h);

		/*
		����:������������
		(���Ƶ�ʱ��ֻ���Ƶ�ָ������)
		����:��
		����ֵ:��
		*/
		void EnableScissor();

		/*
		����:���û�������
		�˺������ܵ���һ�ε�CameraMatrixӰ��
		����:
		_x = ���½�x
		_y = ���½�y
		_width = ���
		_height = �߶�
		����ֵ:
		*/
		void SetScissor(int _x, int _y, int _w, float _h);

		/*
		����:�ر���������
		����:��
		����ֵ:��
		*/
		void DisableScissor();

		/*
		����:������Ⱦ��Ĵ�С(Ĭ��Ϊ1.0f)
		����:
		_size = ��С
		����ֵ:��
		*/
		void SetPointSize(float _size);

		/*
		����:��������Ⱦƽ��
		(ע�������˹�����Ҫ����Alphaͨ������Ч)
		����:��
		����ֵ:��
		*/
		void EnablePointSMOOTH();

		/*
		����:�رյ���Ⱦƽ��
		����:��
		����ֵ:��
		*/
		void DisablePointSMOOTH();

		/*
		����:�����߿�(Ĭ��Ϊ1.0f)
		����:
		_width = ���
		����ֵ:��
		*/
		void SetLineWidth(float _width);

		/*
		����:�����߶�����
		����:��
		����ֵ:��
		*/
		void EnableLinePattern();

		/*
		����:�����߶�����
		����:
		factor = �ظ�����(00111 �������1 ���Ȼ�3���㲻��2��.�������2 ���Ȼ�6���ٲ���4��)
		pattern = ÿһλ��Ӧһ������trueΪ��ʾ
		����ֵ:��
		*/
		void SetLinePattern(int _factor, unsigned short _pattern);

		/*
		����:�ر��߶�����
		����:��
		����ֵ:��
		*/
		void DisableLinePattern();

		/*
		����:����͸��ͨ��
		����:��
		����ֵ:��
		*/
		void EnableAlpha();

		/*
		����:�ر�͸��ͨ��
		����:��
		����ֵ:��
		*/
		void DisableAlpha();

		/*
		����:����������ƽ��
		(ע�������˹�����Ҫ����Alphaͨ������Ч)
		����:�ڻ������ֺ�ͼƬʱ��رմ˹���
		����:��
		����ֵ:��
		*/
		void EnablePolygonSMOOTH();

		/*
		����:�ر�������ƽ��
		����:��
		����ֵ:��
		*/
		void DisablePolygonSMOOTH();

		/*
		����:������ƽ��
		(ע�������˹�����Ҫ����Alphaͨ������Ч)
		����:��
		����ֵ:��
		*/
		void EnableLineSMOOTH();

		/*
		����:�ر���ƽ��
		����:��
		����ֵ:��
		*/
		void DisableLineSMOOTH();

		/*
		����:�����������
		(����˹��ܻ�Ӱ��ȫ���������������)
		(���ô����Ի��ʼ��WorldMatrix)
		����:
		_matrix = ����������
		����ֵ:
		*/
		void SetCameraMatrix(const glm::mat4& _matrix);

		/*
		����:������������
		(����˹��ܻ�Ӱ��ȫ���������������)
		����:
		_matrix = �����������
		����ֵ:
		*/
		void SetWorldMatrix(const glm::mat4& _matrix);

		/*
		����:������������
		(����˹��ܻ�Ӱ��ȫ���������������)
		����:��
		����ֵ:��
		*/
		void ResetWorldMatrix();

		/*
		����:������Ⱦ���ģʽ
		(�ú�������RenderEnd���ȫ����Ⱦ����Ӱ��)
		����:
		_mode = No_Draw_Face ����ֵ | Draw_FrontFace ��Ⱦ���� | Draw_BackFace ��Ⱦ���� | Draw_DoubleFace ��Ⱦ˫��
		����ֵ:��
		*/
		void SetDrawFaceMode(Magic::Pen_Normal::DRAW_FACE_MODE _mode);

		/*
		����:���ӻص���Ϣ,����Ϣ����ʵ����Ⱦ��ʱ�����
		(����˳������Ⱦ˳�����)
		����:
		_CallBack = �ص���������
		����ֵ:��
		*/
		void AddCallBackMessage(const CALLBACK_COMMON& _CallBack);

		/*
		����:��ʼ��Ⱦ
		(�˺�����RenderEnd�Ƕ�Ӧ��һ��)
		ע��:����ʹ��RenderStart���κ���ʹ������RenderEnd
		����:��
		����ֵ:��
		*/
		virtual void RenderStart();

		/*
		����:������Ⱦ
		(�˺�����RenderStart�Ƕ�Ӧһ��)
		ע��:����ʹ��RenderStart���κ���ʹ������RenderEnd
		����:��
		����ֵ:��
		*/
		virtual void RenderEnd();

		/*
		����:������ȾRenderStart��RenderEnd������
		���������ʹ�ô˺���
		�˺������������Ⱦ�������Ӱ��
		����:
		_pos = ����ֵ
		����ֵ:��
		*/
		void SetNowRenderArea(int _pos);

		/*
		����:��ȡ��ȾRenderStart��RenderEnd������
		����:��
		����ֵ:
		int = ����
		*/
		inline int GetNowRenderArea() { return m_NowDraw_Box; }

		/*
		����:��ȡ��ǰ��ɫ
		����:��
		����ֵ:
		const Magic::Color4& = ��ɫֵ
		*/
		inline const Magic::Color4& GetNowColor() { return pNowDRAW_BOX->NowColor; }

		/*
		����:��ȡ��ǰ����
		����:��
		����ֵ:
		Magic::Fonts* = ����
		*/
		inline Magic::Fonts* GetNowFonts() { return pNowDRAW_BOX->Picture_Draw.pFonts; }
	private:
		void AddMessage();
		void AddShaderMessage(unsigned int _shader, bool _add = false);

		void ResetMessage();

	private:
		int m_Draw_Box_Number;
		Magic::VERTEX_BUFFER m_Picture_VBO;
		MagicShader m_PictureShader;
		unsigned int m_Picture2D_projectrionMatrix;

		Magic::VERTEX_BUFFER m_Line_VBO;
		MagicShader m_LineShader;
		unsigned int m_Line_projectionMatrix;
		unsigned int m_Line_PointSize;


		int m_NowDraw_Box;
		DRAW_BOX* pNowDRAW_BOX;
		std::vector<DRAW_BOX> V_DRAW_BOX;

		unsigned int m_DrawMessageNumber;
	};

}
#pragma warning(pop)

#endif