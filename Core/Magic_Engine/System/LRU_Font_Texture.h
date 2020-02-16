/*
*���ܣ�ʹ��LRU�㷨��������ͼ�񻺴���һ�������У��ṩ����ɫ����������Ⱦ
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
			uint16_t TotalWidth;//�ַ�ԭ�㵽��һ���ַ�ԭ��Ŀ��
			//��Ⱦ������λ��
			int16_t x;
			int16_t y;
			uint16_t width;
			uint16_t height;
			//�������UV������
			float top, left, right, bottom;
			uint64_t tick;
		};

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
		unsigned int m_Char_Width_Capacity;
		unsigned int m_Char_Height_Capacity;
		MagicTexture m_MagicTexture;

		FT_Font* m_pFT_Font;

		std::vector<unsigned char> m_BufferImage;
	};
}

#endif