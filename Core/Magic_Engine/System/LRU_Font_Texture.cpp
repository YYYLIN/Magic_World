#include "LRU_Font_Texture.h"
#include "glm.hpp"


namespace Magic
{
	LRU_Font_Texture::LRU_Font_Texture()
	{
	}

	LRU_Font_Texture::~LRU_Font_Texture()
	{
	}

	bool LRU_Font_Texture::Initialize(FT_Font* _pFT_Font, unsigned int _textureWidth, unsigned int _textureHeight) {
		m_pFT_Font = _pFT_Font;

		//��ʼ�����������ͼ��
		bool _err = m_MagicTexture.Initialize(0, _textureWidth, _textureHeight, MagicTexture::ALPHA, MagicTexture::UNSIGNED_BYTE);
		if (!_err)
			return false;
		m_MagicTexture.SetParameteri(MagicTexture::LINEAR);

		//��ʼ���ܹ����Դ�ŵ��ַ������С����
		m_X_MaxCapacity = m_MagicTexture.GetWidth() / m_pFT_Font->GetWidth();
		m_Y_MaxCapacity = m_MagicTexture.GetHeight() / m_pFT_Font->GetHeight();
		m_MaxCapacity = m_X_MaxCapacity * m_Y_MaxCapacity;

		L_Char_free.emplace_back(0, m_MaxCapacity);

		return true;
	}

	// ��������ʹ��LRU�㷨
	void LRU_Font_Texture::UpdataTexture(const wchar_t* _text) {
		++m_Tick;

		for (int a = 0; _text[a]; a++) {
			wchar_t _wchar = _text[a];
			auto cmiter = U_Char_Info_Map.find(_wchar);
			if (cmiter != U_Char_Info_Map.end()) {
				// �������������ҵ���
				cmiter->second.tick = m_Tick;
			}
			else
			{
				// �������������Ҳ��������Ե��������������������
				glm::ivec2 _char_pos;
				CHARINFO _charInfo;
				if (U_Char_Info_Map.size() < m_MaxCapacity) {
					// �����пռ�
					uint32_t const s = L_Char_free.front().first;

					_char_pos.y = s / m_Y_MaxCapacity;
					_char_pos.x = s - _char_pos.y * m_X_MaxCapacity;

					_char_pos.x *= m_pFT_Font->GetWidth();
					_char_pos.y *= m_pFT_Font->GetHeight();

					_charInfo.left = static_cast<float>(_char_pos.x) / m_MagicTexture.GetWidth();
					_charInfo.top = static_cast<float>(_char_pos.y) / m_MagicTexture.GetHeight();

					++L_Char_free.front().first;
					if (L_Char_free.front().first == L_Char_free.front().second) {
						L_Char_free.pop_front();
					}
				}
				else
				{
					// �ҵ�ʹ���ʱ��û��ʹ�õ���
					// �ҳ��ʱ��û��ʹ�õ��֣�����ֵ
					uint64_t min_tick = U_Char_Info_Map.begin()->second.tick;
					auto min_chiter = U_Char_Info_Map.begin();
					for (auto chiter = U_Char_Info_Map.begin(); chiter != U_Char_Info_Map.end(); ++chiter) {
						if (chiter->second.tick < min_tick) {
							min_tick = chiter->second.tick;
							min_chiter = chiter;
						}
					}

					_char_pos.x = static_cast<int32_t>(min_chiter->second.left * m_MagicTexture.GetWidth());
					_char_pos.y = static_cast<int32_t>(min_chiter->second.top * m_MagicTexture.GetHeight());
					_charInfo.left = min_chiter->second.left;
					_charInfo.top = min_chiter->second.top;

					//ɾ�����к��ʹ��ʱ����ֵͬ����
					for (auto chiter = U_Char_Info_Map.begin(); chiter != U_Char_Info_Map.end(); ++chiter) {
						if (chiter->second.tick == min_tick) {
							uint32_t const x = static_cast<int32_t>(chiter->second.left * m_MagicTexture.GetWidth());
							uint32_t const y = static_cast<int32_t>(chiter->second.top * m_MagicTexture.GetHeight());
							uint32_t const id = y * m_X_MaxCapacity + x;
							//�����ͷų���������
							auto freeiter = L_Char_free.begin();
							while ((freeiter != L_Char_free.end()) && (freeiter->second <= id))
							{
								++freeiter;
							}
							L_Char_free.emplace(freeiter, id, id + 1);
							//�ͷŻ���
							U_Char_Info_Map.erase(chiter);
							//�ÿ�����
							m_MagicTexture.UpdataData(x, y, m_pFT_Font->GetWidth(), m_pFT_Font->GetHeight(), 0);
						}
					}
					//�����������Ŀ�����ϲ�Ϊһ������
					for (auto freeiter = L_Char_free.begin(); freeiter != L_Char_free.end();) {
						auto nextiter = freeiter;
						++nextiter;

						if (nextiter != L_Char_free.end())
						{
							if (freeiter->second == nextiter->first)
							{
								freeiter->second = nextiter->second;
								L_Char_free.erase(nextiter);
							}
							else
							{
								++freeiter;
							}
						}
						else
						{
							break;
						}
					}
				}

				FT_Font::CHAR_INFO _CHAR_INFO;
				if (m_pFT_Font->GetFontsTexture(_wchar, &_CHAR_INFO) == 0) {
					_charInfo.right = _charInfo.left + static_cast<float>(_CHAR_INFO.drawWidth) / m_MagicTexture.GetWidth();
					_charInfo.bottom = _charInfo.top + static_cast<float>(_CHAR_INFO.drawHeight) / m_MagicTexture.GetHeight();
					_charInfo.tick = m_Tick;

					_charInfo.x = _CHAR_INFO.left;
					_charInfo.y = _CHAR_INFO.top;
					_charInfo.width = _CHAR_INFO.drawWidth;
					_charInfo.height = _CHAR_INFO.drawHeight;

					m_MagicTexture.UpdataData(_char_pos.x, _char_pos.y, _CHAR_INFO.drawWidth, _CHAR_INFO.drawHeight, _CHAR_INFO.buffer);

					U_Char_Info_Map.emplace(_wchar, _charInfo);
				}
			}
		}
	}
}