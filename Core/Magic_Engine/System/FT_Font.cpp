#include "FT_Font.h"
#include "ft2build.h"
#include "ftoutln.h"
#include  FT_FREETYPE_H


//26.6的像素格式来表示(26.6格式:其实32位的数据,高26位表示整数,低6位表示小数,目的是用整数模拟浮点数,以兼容不支持浮点运算的处理器.使用时,右移6位或者乘以1/64即可转换为普通整数)
namespace Magic
{
	FT_Font::FT_Font()
	{
		m_FT_Library = nullptr;
		m_Bold = 0.0f;
	}

	FT_Font::~FT_Font()
	{
		if (m_FT_Library) {
			FT_Done_FreeType((FT_Library)m_FT_Library);
			m_FT_Library = nullptr;
		}
	}

	int FT_Font::Initialize(const char* _Path, unsigned int _w, unsigned int _h, unsigned int _load_flags) {
		// 1. 初始化freetype2库  
		FT_Library _FT_Library;
		int _Error = FT_Init_FreeType(&_FT_Library);
		if (_Error)
			return _Error;
		m_FT_Library = _FT_Library;

		// 2. 创建一个face  
		FT_Face _FT_Face;
		_Error = FT_New_Face((FT_Library)m_FT_Library,
			_Path, 0, &_FT_Face);
		if (_Error == FT_Err_Unknown_File_Format)
		{
			//可以打开和读这个文件，但不支持它的字体格式 
			return _Error;
		}
		else if (_Error)
		{
			//其它的错误码意味着这个字体文件不能打开和读，或者简单的说它损坏了... 
			return _Error;
		}
		m_FT_Face = _FT_Face;

		m_Width = _w;
		m_Height = _h;
		m_load_flags = _load_flags;
		/*
		*char_width 公称宽度，以26.6小数点表示。
		*char_height标称高度，以26.6分数表示。
		*horz_resolution以dpi为单位的水平分辨率。
		*vert_resolution垂直分辨率（以dpi为单位）
		*/
		//设置字体尺寸
		//_Error = FT_Set_Char_Size(_FT_Face, m_Width * 64, m_Height * 64, 96, 96);
		_Error = FT_Set_Pixel_Sizes(_FT_Face, m_Width, m_Height);
		if (_Error) {
			return _Error;
		}

		return 0;
	}

	int FT_Font::GetFontsTexture(wchar_t _wChar, CHAR_INFO* _pCHAR_INFO)
	{
		FT_Face _FT_Face = (FT_Face)m_FT_Face;
		/*//从字符码检索字形索引
		FT_UInt _glyph_index = FT_Get_Char_Index(_FT_Face, _wChar);
		//装载字形图像到字形槽（将会抹掉先前的字形图像）
		int _Error = FT_Load_Glyph(_FT_Face, _glyph_index, FT_LOAD_DEFAULT);
		if (_Error) { return _Error; }*/

		// FT_LOAD_DEFAULT
		// FT_LOAD_MONOCHROME       无抗锯齿
		// FT_LOAD_FORCE_AUTOHINT   抗锯齿
		// FT_LOAD_RENDER 创建一个8位的灰度位图

		int _Error = FT_Load_Char(_FT_Face, _wChar, FT_LOAD_RENDER | m_load_flags); //== FT_Get_Char_Index + FT_Load_Glyph
		if (_Error) { return _Error; }

		if (m_Bold != 0.0f) {
			//加粗勾勒轮廓。新轮廓的strength像素宽度最多为4倍，更高。您可能会认为左边界和底边界没有变化。
			//strength减小轮廓厚度的负值也是可能的。26.6像素格式
			FT_Outline_Embolden(&_FT_Face->glyph->outline, (int)(m_Bold * 64.0f));
		}

		//转换为一个抗锯齿位图
		/*_Error = FT_Render_Glyph(_FT_Face->glyph, FT_RENDER_MODE_NORMAL);
		if (_Error) { return _Error; }*/

		_pCHAR_INFO->drawWidth = _FT_Face->glyph->bitmap.width;
		_pCHAR_INFO->drawHeight = _FT_Face->glyph->bitmap.rows;
		_pCHAR_INFO->left = _FT_Face->glyph->bitmap_left;
		_pCHAR_INFO->top = -_FT_Face->glyph->bitmap_top;
		_pCHAR_INFO->width = _FT_Face->glyph->advance.x / 64;
		//_pCHAR_INFO->ascender = _FT_Face->size->metrics.ascender >> 6;
		_pCHAR_INFO->buffer = _FT_Face->glyph->bitmap.buffer;

		if (_FT_Face->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_MONO) {//MSB
			m_Buffer.resize(_FT_Face->glyph->bitmap.width*_FT_Face->glyph->bitmap.rows);

			for (unsigned int _H = 0, _Pos = 0; _H < _pCHAR_INFO->drawHeight; _H++) {
				for (unsigned int _W = 0; _W < _pCHAR_INFO->drawWidth; _W++) {
					m_Buffer[_Pos++] = ((_FT_Face->glyph->bitmap.buffer[_H * _FT_Face->glyph->bitmap.pitch + (_W >> 3)] >> (7 - _W % 8)) & 0x01) * 255;
				}
			}

			_pCHAR_INFO->buffer = &m_Buffer[0];
		}

		/*if (_wChar == L'你') { //DEBUG
			FILE* _pFile = 0;
			fopen_s(&_pFile, "text.txt", "w");

			for (unsigned int _h = 0; _h < _pCHAR_INFO->drawHeight; _h++) {
				for (unsigned int _w = 0; _w < _pCHAR_INFO->drawWidth; _w++) {
					if (_pCHAR_INFO->buffer[_h* _pCHAR_INFO->drawWidth + _w] > 0) {
						fprintf(_pFile, "8");
					}
					else {
						fprintf(_pFile, " ");
					}
				}
				fprintf(_pFile, "\n");
			}

			fclose(_pFile);
		}*/

		return 0;
	}

	void FT_Font::Bold(float _Bold) {
		m_Bold = _Bold;
	}

	void FT_Font::Set_load_flags(unsigned int _load_flags) {
		m_load_flags = _load_flags;
	}
}