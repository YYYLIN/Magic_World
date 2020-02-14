/*
*功能：使用LRU算法，将字体图像缓存在一张纹理中，提供给着色器做快速渲染
*/

#include <stdint.h>
#include "Render/MagicTexture.h"
#include <vector>
#include <unordered_map>

#include "FT_Font.h"

namespace Magic
{
	typedef struct FontsTexture
	{
		unsigned int    rows;
		unsigned int    width;
		signed   int    left;  //字符到最左边边界的距离
		signed   int    top;  //基准线到字符最高点的距离
		signed   int    Ascender;//基准线到字符最高轮廓的距离
		int X, Y;

		FontsTexture();
	} FontsTexture;

	struct FONT_HEADER
	{
		char FileType[8];
		uint32_t version;
		uint32_t validate_chars;
		uint32_t non_empty_chars;
		uint32_t char_size;

		int16_t base;
		int16_t scale;
	};

	struct FONT_INFO
	{
		int16_t x;
		int16_t y;
		uint16_t width;
		uint16_t height;
	};

	struct CHARINFO
	{
		//渲染的像素位置
		int16_t x;
		int16_t y;
		uint16_t width;
		uint16_t height;
		//在纹理的UV坐标上
		float top, left, right, bottom;
		uint64_t tick;
	};

	class LRU_Font_Texture
	{
	public:
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
		MagicTexture m_MagicTexture;

		FT_Font* m_pFT_Font;
	};
}