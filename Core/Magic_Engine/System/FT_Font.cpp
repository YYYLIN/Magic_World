#include "FT_Font.h"

namespace Magic
{
	FT_Font::FT_Font()
	{
	}

	FT_Font::~FT_Font()
	{
		FT_Done_FreeType(m_FT_Library);
	}

	int FT_Font::Initialize(const char* _Path, unsigned int _w, unsigned int _h) {
		// 1. 初始化freetype2库  
		int _Error = FT_Init_FreeType(&m_FT_Library);
		if (_Error)
			return _Error;

		// 2. 创建一个face  
		_Error = FT_New_Face(m_FT_Library,
			_Path, 0, &m_FT_Face);
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

		//设置字体尺寸
		/*_Error = FT_Set_Char_Size(m_FT_Face, 0, 16 << 6, width * 8, high * 8);
		if (_Error)
		{
			return _Error;
		}*/
		m_Width = _w;
		m_Height = _h;
		_Error = FT_Set_Pixel_Sizes(m_FT_Face, _w, _h);
		if (_Error)
		{
			return _Error;
		}

		return 0;
	}

	int FT_Font::GetFontsTexture(wchar_t _wChar, CHAR_INFO* _pCHAR_INFO)
	{
		//从字符码检索字形索引
		FT_UInt _glyph_index = FT_Get_Char_Index(m_FT_Face, _wChar);
		//装载字形图像到字形槽（将会抹掉先前的字形图像）
		int _Error = FT_Load_Glyph(m_FT_Face, _glyph_index, FT_LOAD_DEFAULT);
		if (_Error) { return _Error; }

		//转换为一个抗锯齿位图
		_Error = FT_Render_Glyph(m_FT_Face->glyph, FT_RENDER_MODE_NORMAL);
		if (_Error) { return _Error; }

		_pCHAR_INFO->drawWidth = m_FT_Face->glyph->bitmap.width;
		_pCHAR_INFO->drawHeight = m_FT_Face->glyph->bitmap.rows;
		_pCHAR_INFO->left = m_FT_Face->glyph->bitmap_left;
		_pCHAR_INFO->top = m_FT_Face->glyph->bitmap_top;
		_pCHAR_INFO->ascender = m_FT_Face->size->metrics.ascender >> 6;
		_pCHAR_INFO->buffer = m_FT_Face->glyph->bitmap.buffer;

		return true;
	}
}