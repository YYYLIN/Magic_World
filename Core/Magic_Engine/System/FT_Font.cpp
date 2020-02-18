#include "FT_Font.h"
#include "ft2build.h"
#include "ftoutln.h"
#include  FT_FREETYPE_H


//26.6�����ظ�ʽ����ʾ(26.6��ʽ:��ʵ32λ������,��26λ��ʾ����,��6λ��ʾС��,Ŀ����������ģ�⸡����,�Լ��ݲ�֧�ָ�������Ĵ�����.ʹ��ʱ,����6λ���߳���1/64����ת��Ϊ��ͨ����)
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
		// 1. ��ʼ��freetype2��  
		FT_Library _FT_Library;
		int _Error = FT_Init_FreeType(&_FT_Library);
		if (_Error)
			return _Error;
		m_FT_Library = _FT_Library;

		// 2. ����һ��face  
		FT_Face _FT_Face;
		_Error = FT_New_Face((FT_Library)m_FT_Library,
			_Path, 0, &_FT_Face);
		if (_Error == FT_Err_Unknown_File_Format)
		{
			//���Դ򿪺Ͷ�����ļ�������֧�����������ʽ 
			return _Error;
		}
		else if (_Error)
		{
			//�����Ĵ�������ζ����������ļ����ܴ򿪺Ͷ������߼򵥵�˵������... 
			return _Error;
		}
		m_FT_Face = _FT_Face;

		m_Width = _w;
		m_Height = _h;
		m_load_flags = _load_flags;
		/*
		*char_width ���ƿ�ȣ���26.6С�����ʾ��
		*char_height��Ƹ߶ȣ���26.6������ʾ��
		*horz_resolution��dpiΪ��λ��ˮƽ�ֱ��ʡ�
		*vert_resolution��ֱ�ֱ��ʣ���dpiΪ��λ��
		*/
		//��������ߴ�
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
		/*//���ַ��������������
		FT_UInt _glyph_index = FT_Get_Char_Index(_FT_Face, _wChar);
		//װ������ͼ�����βۣ�����Ĩ����ǰ������ͼ��
		int _Error = FT_Load_Glyph(_FT_Face, _glyph_index, FT_LOAD_DEFAULT);
		if (_Error) { return _Error; }*/

		// FT_LOAD_DEFAULT
		// FT_LOAD_MONOCHROME       �޿����
		// FT_LOAD_FORCE_AUTOHINT   �����
		// FT_LOAD_RENDER ����һ��8λ�ĻҶ�λͼ

		int _Error = FT_Load_Char(_FT_Face, _wChar, FT_LOAD_RENDER | m_load_flags); //== FT_Get_Char_Index + FT_Load_Glyph
		if (_Error) { return _Error; }

		if (m_Bold != 0.0f) {
			//�Ӵֹ�����������������strength���ؿ�����Ϊ4�������ߡ������ܻ���Ϊ��߽�͵ױ߽�û�б仯��
			//strength��С������ȵĸ�ֵҲ�ǿ��ܵġ�26.6���ظ�ʽ
			FT_Outline_Embolden(&_FT_Face->glyph->outline, (int)(m_Bold * 64.0f));
		}

		//ת��Ϊһ�������λͼ
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

		/*if (_wChar == L'��') { //DEBUG
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