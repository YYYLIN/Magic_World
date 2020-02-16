/*
*功能：使用LRU算法，将字体图像缓存在一张纹理中，提供给着色器做快速渲染
*/

#ifndef _SYSTEM_LRU_FONT_TEXTURE_H_
#define _SYSTEM_LRU_FONT_TEXTURE_H_

#include <stdint.h>
#include "Render/MagicTexture.h"
#include <vector>
#include <unordered_map>

#include "FT_Font.h"

namespace Magic
{
	class LRU_Font_Texture
	{
	public:
		struct CHARINFO
		{
			uint16_t TotalWidth;//字符原点到下一个字符原点的宽度
			//渲染的像素位置
			int16_t x;
			int16_t y;
			uint16_t width;
			uint16_t height;
			//在纹理的UV坐标上
			float top, left, right, bottom;
			uint64_t tick;
		};

		LRU_Font_Texture();
		~LRU_Font_Texture();
	public:
		bool Initialize(FT_Font* _pFT_Font, unsigned int _textureWidth = 2048, unsigned int _textureHeight = 2048);//初始化

		MagicTexture* GetTexture() { return &m_MagicTexture; }

		const CHARINFO& GetCHARINFO(const wchar_t& _char) { return U_Char_Info_Map[_char]; }

		void UpdataTexture(const wchar_t* _text);
	private:

	private:
		std::unordered_map<wchar_t, CHARINFO> U_Char_Info_Map;//已经渲染缓存到纹理上的字符

		std::list<std::pair<uint32_t, uint32_t>> L_Char_free;//还没有分配字符的图像的缓存区

		uint64_t m_Tick;

		unsigned int m_MaxCapacity;//最大容量
		unsigned int m_X_MaxCapacity;//X轴最大容量
		unsigned int m_Y_MaxCapacity;//Y轴最大容量
		unsigned int m_Char_Width_Capacity;
		unsigned int m_Char_Height_Capacity;
		MagicTexture m_MagicTexture;

		FT_Font* m_pFT_Font;

		std::vector<unsigned char> m_BufferImage;
	};
}

#endif