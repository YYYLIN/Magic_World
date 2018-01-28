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
		功能:渲染顶点
		参数:
		_drawMode = (LINES | POINTS | LINE_STRIP | LINE_LOOP | TRIANGLES | TRIANGLE_STRIP | TRIANGLE_FAN);
		_pPos = 指向起始位置的指针 (struct float x, y;)
		_pColor = 每个点的颜色指针
		_number = 点的数量
		返回值:空
		*/
		void DrawVertex(Pen_Normal::DRAW_MODE _drawMode, const float* _pPos, const Magic::Color4* _pColor, unsigned int _Number);

		/*
		功能:渲染顶点
		参数:
		_drawMode = (LINES | POINTS | LINE_STRIP | LINE_LOOP | TRIANGLES | TRIANGLE_STRIP | TRIANGLE_FAN);
		_pPos = 指向起始位置的指针 (struct float x, y;)
		_number = 位置点的数量
		返回值:空
		*/
		void DrawVertex(Pen_Normal::DRAW_MODE _drawMode, const float* _pPos, unsigned int _Number);

		/*
		功能:渲染直线
		参数:
		_x1 = 第一个点x
		_y1 = 第一个点y
		_x2 = 第二个点x
		_y2 = 第二个点y
		返回值:空
		*/
		void DrawLine(float _x1, float _y1, float _x2, float _y2);

		/*
		功能:以左下角为原点渲染图片
		参数:
		_x = 左下角x
		_y = 左下角y
		_w = 宽度
		_h = 高度
		返回值:空
		*/
		void DrawPicture(float _x, float _y, float _w, float _h);

		/*
		功能:渲染文字
		参数:
		_x = 左下角x
		_y = 左下角y
		_text = 文本
		_ArrayState = 排列模式 CHARATER_LEFT_ARRAY | CHARATER_RIGHT_ARRAY | CHARATER_MIDDLE_ARRAY
		返回值:
		字体的结尾位置
		*/
		int DrawTEXT(float _x, float _y, const char* _text, unsigned char _ArrayState = CHARATER_LEFT_ARRAY);

		/*
		功能:以左下角为原点渲染矩形
		参数:
		_drawMode = POINTS | LINE_LOOP | LINES_STRIP | TRIANGLES
		_x = 左下角x
		_y = 左下角y
		_w = 宽度
		_h = 高度
		返回值:
		*/
		void DrawRectangle(Pen_Normal::DRAW_MODE  _drawMode, float _x, float _y, float _w, float _h);

		/*
		功能:重复上一次的渲染参数渲染
		(可修改内容DrawPicture  DrawTEXT 颜色 和 矩阵
		DrawRectangle DrawLine DrawVertex 矩阵)
		参数:空
		返回值:空
		*/
		void RepeatDraw();

		/*
		功能:绑定字体
		参数:
		_pFonts = 字体
		返回值:空
		*/
		void BindFonts(Magic::Fonts* _pFonts);

		/*
		功能:绑定图片
		参数:
		_pTextrue = 纹理指针
		返回值:空
		*/
		void BindPicture(MagicTexture* _pTexture);

		/*
		功能:绑定图片的UV坐标截取部分区域
		参数:
		_TictureWH = 图片宽高
		_x = 左下角x
		_y = 左下角y
		_w = 要截取的宽
		_h = 要截取的高
		返回值:空
		*/
		void BindPictureUVPos(glm::vec2 _TictureWH, float _x, float _y, float _w, float _h);

		/*
		功能:绑定图片的UV坐标
		参数:
		_Left = 左边的值
		_Right = 右边的值
		_Up = 上边的值
		_Down = 下边的值
		返回值:空
		*/
		void BindPictureUVPos(float _Left, float _Right, float _Up, float _Down);

		/*
		功能:绑定图片的UV坐标为默认
		参数:空
		返回值:空
		*/
		void BindPictureUVPosfault();

		/*
		功能:设置一种颜色
		参数:
		_color = 颜色
		返回值:空
		*/
		void SetColor(const Magic::Color4& _color);

		/*
		功能:设置渲染宽高(此函数在RenderStart开始后必须设置一次)
		参数:
		_w = 宽
		_h = 高
		返回值:
		*/
		void SetDrawWH(float _w, float _h);

		/*
		功能:启动屏蔽区域
		(绘制的时候只绘制到指定区域)
		参数:空
		返回值:空
		*/
		void EnableScissor();

		/*
		功能:设置绘制区域
		此函数会受到上一次的CameraMatrix影响
		参数:
		_x = 左下角x
		_y = 左下角y
		_width = 宽度
		_height = 高度
		返回值:
		*/
		void SetScissor(int _x, int _y, int _w, float _h);

		/*
		功能:关闭屏蔽区域
		参数:空
		返回值:空
		*/
		void DisableScissor();

		/*
		功能:设置渲染点的大小(默认为1.0f)
		参数:
		_size = 大小
		返回值:空
		*/
		void SetPointSize(float _size);

		/*
		功能:开启点渲染平滑
		(注意启动此功能需要启动Alpha通道才有效)
		参数:空
		返回值:空
		*/
		void EnablePointSMOOTH();

		/*
		功能:关闭点渲染平滑
		参数:空
		返回值:空
		*/
		void DisablePointSMOOTH();

		/*
		功能:设置线宽(默认为1.0f)
		参数:
		_width = 宽度
		返回值:空
		*/
		void SetLineWidth(float _width);

		/*
		功能:启动线段类型
		参数:空
		返回值:空
		*/
		void EnableLinePattern();

		/*
		功能:设置线段类型
		参数:
		factor = 重复次数(00111 如果等于1 就先画3个点不画2个.如果等于2 就先画6个再不画4个)
		pattern = 每一位对应一个像素true为显示
		返回值:空
		*/
		void SetLinePattern(int _factor, unsigned short _pattern);

		/*
		功能:关闭线段类型
		参数:空
		返回值:空
		*/
		void DisableLinePattern();

		/*
		功能:启动透明通道
		参数:空
		返回值:空
		*/
		void EnableAlpha();

		/*
		功能:关闭透明通道
		参数:空
		返回值:空
		*/
		void DisableAlpha();

		/*
		功能:启动三角面平滑
		(注意启动此功能需要启动Alpha通道才有效)
		警告:在绘制文字和图片时请关闭此功能
		参数:空
		返回值:空
		*/
		void EnablePolygonSMOOTH();

		/*
		功能:关闭三角面平滑
		参数:空
		返回值:空
		*/
		void DisablePolygonSMOOTH();

		/*
		功能:启动线平滑
		(注意启动此功能需要启动Alpha通道才有效)
		参数:空
		返回值:空
		*/
		void EnableLineSMOOTH();

		/*
		功能:关闭线平滑
		参数:空
		返回值:空
		*/
		void DisableLineSMOOTH();

		/*
		功能:设置相机坐标
		(警告此功能会影响全局坐标请谨慎调用)
		(设置此属性会初始化WorldMatrix)
		参数:
		_matrix = 相机坐标矩阵
		返回值:
		*/
		void SetCameraMatrix(const glm::mat4& _matrix);

		/*
		功能:设置世界坐标
		(警告此功能会影响全局坐标请谨慎调用)
		参数:
		_matrix = 世界坐标矩阵
		返回值:
		*/
		void SetWorldMatrix(const glm::mat4& _matrix);

		/*
		功能:重置世界坐标
		(警告此功能会影响全局坐标请谨慎调用)
		参数:空
		返回值:空
		*/
		void ResetWorldMatrix();

		/*
		功能:设置渲染面的模式
		(该函数会在RenderEnd后对全局渲染进行影响)
		参数:
		_mode = No_Draw_Face 错误值 | Draw_FrontFace 渲染正面 | Draw_BackFace 渲染背面 | Draw_DoubleFace 渲染双面
		返回值:空
		*/
		void SetDrawFaceMode(Magic::Pen_Normal::DRAW_FACE_MODE _mode);

		/*
		功能:增加回调消息,此消息会在实际渲染的时候调用
		(调用顺序由渲染顺序决定)
		参数:
		_CallBack = 回调函数类型
		返回值:空
		*/
		void AddCallBackMessage(const CALLBACK_COMMON& _CallBack);

		/*
		功能:开始渲染
		(此函数和RenderEnd是对应的一对)
		注意:可以使用RenderStart两次后再使用两次RenderEnd
		参数:空
		返回值:空
		*/
		virtual void RenderStart();

		/*
		功能:结束渲染
		(此函数和RenderStart是对应一对)
		注意:可以使用RenderStart两次后再使用两次RenderEnd
		参数:空
		返回值:空
		*/
		virtual void RenderEnd();

		/*
		功能:设置渲染RenderStart到RenderEnd的区域
		警告请谨慎使用此函数
		此函数会对整个渲染队列造成影响
		参数:
		_pos = 区域值
		返回值:空
		*/
		void SetNowRenderArea(int _pos);

		/*
		功能:获取渲染RenderStart到RenderEnd的区域
		参数:空
		返回值:
		int = 区域
		*/
		inline int GetNowRenderArea() { return m_NowDraw_Box; }

		/*
		功能:获取当前颜色
		参数:空
		返回值:
		const Magic::Color4& = 颜色值
		*/
		inline const Magic::Color4& GetNowColor() { return pNowDRAW_BOX->NowColor; }

		/*
		功能:获取当前字体
		参数:空
		返回值:
		Magic::Fonts* = 字体
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