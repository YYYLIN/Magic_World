/*
*���ܣ�ʹ��LRU�㷨��������ͼ�񻺴���һ�������У��ṩ����ɫ����������Ⱦ
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
		signed   int    left;  //�ַ�������߽߱�ľ���
		signed   int    top;  //��׼�ߵ��ַ���ߵ�ľ���
		signed   int    Ascender;//��׼�ߵ��ַ���������ľ���
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
		//��Ⱦ������λ��
		int16_t x;
		int16_t y;
		uint16_t width;
		uint16_t height;
		//�������UV������
		float top, left, right, bottom;
		uint64_t tick;
	};

	class LRU_Font_Texture
	{
	public:
		LRU_Font_Texture();
		~LRU_Font_Texture();
	public:
		bool Initialize(FT_Font* _pFT_Font, unsigned int _textureWidth = 2048, unsigned int _textureHeight = 2048);//��ʼ��

		MagicTexture* GetTexture() { return &m_MagicTexture; }

		const CHARINFO& GetCHARINFO(const wchar_t& _char) { return U_Char_Info_Map[_char]; }

		void UpdataTexture(const wchar_t* _text);
	private:

	private:
		std::unordered_map<wchar_t, CHARINFO> U_Char_Info_Map;//�Ѿ���Ⱦ���浽�����ϵ��ַ�

		std::list<std::pair<uint32_t, uint32_t>> L_Char_free;//��û�з����ַ���ͼ��Ļ�����

		uint64_t m_Tick;

		unsigned int m_MaxCapacity;//�������
		unsigned int m_X_MaxCapacity;//X���������
		unsigned int m_Y_MaxCapacity;//Y���������
		MagicTexture m_MagicTexture;

		FT_Font* m_pFT_Font;
	};
}