/*
*功能：将freetype字体库，进行一次封装，提供更简洁的，字符图像查找->渲染->获取
*/
#ifndef _SYSTEM_FT_FONT_H_
#define _SYSTEM_FT_FONT_H_


#include <vector>

#define FFT_LOAD_FORCE_AUTOHINT               ( 1L << 5 ) //抗锯齿
#define FFT_LOAD_MONOCHROME                   ( 1L << 12 ) //无抗锯齿

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
			FT_PIXEL_MODE_NONE = 0,//值0被保留。
			FT_PIXEL_MODE_MONO,//单色位图，每个像素使用1位。请注意，像素以最高有效顺序（MSB）存储，这意味着字节中最左边的像素的值是128。
			FT_PIXEL_MODE_GRAY,//8位位图，通常用于表示抗锯齿字形图像。每个像素存储在一个字节中。请注意，“灰色”级别的数量存储在结构的num_grays字段中FT_Bitmap（通常为256）。
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