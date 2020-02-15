/*
*功能：将freetype字体库，进行一次封装，提供更简洁的，字符图像查找->渲染->获取
*/

namespace Magic
{
	class FT_Font
	{
	public:
		struct CHAR_INFO
		{
			unsigned int drawHeight;
			unsigned int drawWidth;
			int left;
			int top;
			signed long ascender;
			unsigned char* buffer;
		};

	public:
		FT_Font();
		~FT_Font();

		int Initialize(const char* _Path, unsigned int _w, unsigned int _h);

		int GetFontsTexture(wchar_t _wChar, CHAR_INFO* _pCHAR_INFO);

		inline const unsigned int& GetWidth() { return m_Width; }

		inline const unsigned int& GetHeight() { return m_Height; }

	private:
		void* m_FT_Library;
		void* m_FT_Face;

		unsigned int m_Width;
		unsigned int m_Height;
	};
}