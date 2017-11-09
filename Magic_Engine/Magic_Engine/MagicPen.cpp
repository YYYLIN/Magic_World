#include "MagicPen.h"
#pragma comment(lib,"glew32.lib")
#include <GL/glew.h>  

namespace Magic
{
	Pen_Normal::Pen_Normal()
	{
	}
	Pen_Normal::~Pen_Normal()
	{
	}
	bool Pen_Normal::Initialize()
	{
		return true;
	}
	void Pen_Normal::DrawVertex(Pen_Normal::DRAW_MODE _drawMode, Magic::VERTEX_BUFFER * _VertexVuffer, unsigned int _StartPos, unsigned int _DrawNumber)
	{
	}
	void Pen_Normal::DrawVertex(Pen_Normal::DRAW_MODE _drawMode, const float * _pPos, const Magic::Color4 * _pColor, unsigned int _Number)
	{
	}
	void Pen_Normal::DrawVertex(Pen_Normal::DRAW_MODE _drawMode, const float * _pPos, unsigned int _Number)
	{
	}
	void Pen_Normal::DrawLine(float _x1, float _y1, float _x2, float _y2)
	{
	}
	void Pen_Normal::DrawPicture(float _x, float _y, float _w, float _h)
	{
	}
	void Pen_Normal::DrawTEXT(float _x, float _y, const char * _text, unsigned char _ArrayState)
	{
	}
	void Pen_Normal::DrawRectangle(Pen_Normal::DRAW_MODE _drawMode, float _x, float _y, float _w, float _h)
	{
	}
	void Pen_Normal::BindFonts(Magic_Fonts * _pFonts)
	{
	}
	void Pen_Normal::BindPicture(MagicTexture * _pTexture)
	{
	}
	void Pen_Normal::BindPictureUVPos(glm::vec2 _TictureWH, float _x, float _y, float _w, float _h)
	{
	}
	void Pen_Normal::BindPictureUVPos(float _Left, float _Right, float _Up, float _Down)
	{
	}
	void Pen_Normal::BindPictureUVPosfault()
	{
	}
	void Pen_Normal::SetColor(const Magic::Color4 & _color)
	{
	}
	void Pen_Normal::SetDrawWH(float _w, float _h)
	{
	}
	void Pen_Normal::EnableScissor()
	{
	}
	void Pen_Normal::SetScissor(int _x, int _y, int _w, float _h, int _fboHeight)
	{
	}
	void Pen_Normal::DisableScissor()
	{
	}
	void Pen_Normal::SetPointSize(float _size)
	{
	}
	void Pen_Normal::EnablePointSMOOTH()
	{
	}
	void Pen_Normal::DisablePointSMOOTH()
	{
	}
	void Pen_Normal::SetLineWidth(float _width)
	{
	}
	void Pen_Normal::EnableLinePattern()
	{
	}
	void Pen_Normal::SetLinePattern(int _factor, unsigned short _pattern)
	{
	}
	void Pen_Normal::DisableLinePattern()
	{
	}
	void Pen_Normal::EnableAlpha()
	{
	}
	void Pen_Normal::DisableAlpha()
	{
	}
	void Pen_Normal::EnablePolygonSMOOTH()
	{
	}
	void Pen_Normal::DisablePolygonSMOOTH()
	{
	}
	void Pen_Normal::EnableLineSMOOTH()
	{
	}
	void Pen_Normal::DisableLineSMOOTH()
	{
	}
	void Pen_Normal::SetCameraMatrix(const glm::mat4 & _matrix)
	{
	}
	void Pen_Normal::SetWorldMatrix(const glm::mat4 & _matrix)
	{
	}
	void Pen_Normal::ResetWorldMatrix()
	{
	}
	void Pen_Normal::SetDrawFaceMode(Magic::Pen_Normal::DRAW_FACE_MODE _mode)
	{
	}
	void Pen_Normal::RenderStart()
	{
	}
	void Pen_Normal::RenderEnd()
	{
	}
	void Pen_Normal::SetNowRenderArea(int _pos)
	{
	}
}