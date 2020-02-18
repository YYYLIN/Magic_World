/*
*���ܣ���freetype����⣬����һ�η�װ���ṩ�����ģ��ַ�ͼ�����->��Ⱦ->��ȡ
*/
#ifndef _SYSTEM_FT_FONT_H_
#define _SYSTEM_FT_FONT_H_


#include <vector>

#define FFT_LOAD_FORCE_AUTOHINT               ( 1L << 5 ) //�����
#define FFT_LOAD_MONOCHROME                   ( 1L << 12 ) //�޿����

namespace Magic
{
	class FT_Font
	{
	public:
		struct CHAR_INFO
		{
			unsigned int width;
			unsigned int drawHeight;
			unsigned int drawWidth;
			int left;
			int top;
			//signed long ascender;
			unsigned char* buffer;
		};

		/*typedef enum  FT_Pixel_Mode_
		{
			FT_PIXEL_MODE_NONE = 0,//ֵ0��������
			FT_PIXEL_MODE_MONO,//��ɫλͼ��ÿ������ʹ��1λ����ע�⣬�����������Ч˳��MSB���洢������ζ���ֽ�������ߵ����ص�ֵ��128��
			FT_PIXEL_MODE_GRAY,//8λλͼ��ͨ�����ڱ�ʾ���������ͼ��ÿ�����ش洢��һ���ֽ��С���ע�⣬����ɫ������������洢�ڽṹ��num_grays�ֶ���FT_Bitmap��ͨ��Ϊ256����
			FT_PIXEL_MODE_GRAY2,
			FT_PIXEL_MODE_GRAY4,
			FT_PIXEL_MODE_LCD,
			FT_PIXEL_MODE_LCD_V,
			FT_PIXEL_MODE_BGRA,

			FT_PIXEL_MODE_MAX

		} FT_Pixel_Mode;*/

	public:
		FT_Font();
		~FT_Font();

		int Initialize(const char* _Path, unsigned int _w, unsigned int _h, unsigned int _load_flags = FFT_LOAD_MONOCHROME);

		int GetFontsTexture(wchar_t _wChar, CHAR_INFO* _pCHAR_INFO);

		inline const unsigned int& GetWidth() { return m_Width; }

		inline const unsigned int& GetHeight() { return m_Height; }

		void Bold(float _Bold);

		void Set_load_flags(unsigned int _load_flags);

	private:
		void* m_FT_Library;
		void* m_FT_Face;

		unsigned int m_Width;
		unsigned int m_Height;
		unsigned int m_load_flags;

		float m_Bold;

		std::vector<unsigned char> m_Buffer;
	};
}

#endif