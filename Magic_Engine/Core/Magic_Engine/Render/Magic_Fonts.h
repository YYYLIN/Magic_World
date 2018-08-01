/***********************************************************
头文件：Magic_Fonts.h
说明：对于文字的处理显示库

借物说明：此功能代码实现来源KlayGE的Font
************************************************************/

#ifndef _MAGIC_FONTS_H_
#define _MAGIC_FONTS_H_

#include "MagicTexture.h"
#include "Define/Magic_Macro.h"
#include <vector>
#include <unordered_map>
#include <stdint.h>

#pragma warning(push)
#pragma warning(disable:4251)

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
		int16_t top;
		int16_t left;
		uint16_t width;
		uint16_t height;
	};

	struct CHARINFO
	{
		float top, left, right, bottom;
		uint64_t tick;
	};

#define  XY_FONTS_CharNumber  99

	class DLL_MAGIC_ENGINE_OUTPUT_INPUT Fonts
	{
	public:
		Fonts();
		~Fonts();

	public:
		bool Initialize(const char* _path);//初始化
		void Shutdown();    //关闭 结束函数

		int GetTextAllWidth(const char*);

		uint32_t GetCharSize() { return m_header.char_size; }

		MagicTexture* GetTexture() { return &m_MagicTexture; }
		const std::pair<int32_t, uint32_t>& GetCharIndexAdvance(const int32_t& _index) { return V_char_index_advance[_index]; }
		FONT_INFO GetFONT_INFO(const int32_t& _offset) { return V_char_info[_offset]; }
		const CHARINFO& GetCHARINFO(const wchar_t& _char) { return U_Char_Info_Map[_char]; }

		inline float GetBase() { return m_Base; }
		inline float GetScale() { return m_Scale; }

		void UpdataTexture(const wchar_t* _text);
	private:
		void GetCharDistanceData(uint8_t* _pdata, int32_t _index) const;
		std::pair<int32_t, uint32_t> const & CharIndexAdvance(wchar_t _char) const;

	private:
		FILE* pFile;
		std::unordered_map<int32_t, std::pair<int32_t, uint32_t>> V_char_index_advance;
		std::vector<FONT_INFO> V_char_info;
		std::unordered_map<wchar_t, CHARINFO> U_Char_Info_Map;

		std::list<std::pair<uint32_t, uint32_t>> L_Char_free;

		std::vector<uint8_t> V_Char_data;

		int64_t m_Data_Start_Pos;
		uint64_t m_Tick;

		float m_Base;
		float m_Scale;
		FONT_HEADER m_header;
		MagicTexture m_MagicTexture;
	};
}

#pragma warning(pop)

#endif