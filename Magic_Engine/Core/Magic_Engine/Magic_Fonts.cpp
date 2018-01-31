#include "Magic_Fonts.h"
#include <stdio.h>

#include "glm.hpp"

namespace Magic
{

	FontsTexture::FontsTexture()
	{
		rows = 0;
		width = 0;
		X = 0;
		Y = 0;
	}

	Fonts::Fonts()
	{
		pFile = NULL;
	}

	Fonts::~Fonts()
	{
		Shutdown();
	}

	bool Fonts::Initialize(const char*  _filepathname)
	{
		errno_t _err = fopen_s(&pFile, _filepathname, "rb");
		if (_err != 0)
			return false;

		fread(&m_header, sizeof(FONT_HEADER), 1, pFile);

		m_Base = m_header.base / 32768.0f * 32 + 1;
		m_Scale = (m_header.scale / 32768.0f + 1.0f) * 32;
		std::vector<std::pair<int32_t, int32_t>> temp_char_index(m_header.non_empty_chars);
		fread(&temp_char_index[0], sizeof(std::pair<int32_t, int32_t>), temp_char_index.size(), pFile);

		std::vector<std::pair<int32_t, uint32_t>> temp_char_advance(m_header.validate_chars);
		fread(&temp_char_advance[0], sizeof(std::pair<int32_t, uint32_t>), temp_char_advance.size(), pFile);

		for (auto& ci : temp_char_index)
		{
			V_char_index_advance.emplace(ci.first, std::make_pair(ci.second, 0));
		}
		for (auto& ca : temp_char_advance)
		{
			auto iter = V_char_index_advance.find(ca.first);
			if (iter != V_char_index_advance.end())
			{
				iter->second.second = ca.second;
			}
			else
			{
				V_char_index_advance[ca.first] = std::make_pair(-1, ca.second);
			}
		}

		V_char_info.resize(m_header.non_empty_chars);
		fread(&V_char_info[0], sizeof(FONT_INFO), V_char_info.size(), pFile);

		m_Data_Start_Pos = ftell(pFile);

		V_Char_data.resize(m_header.char_size * m_header.char_size);

		_err = m_MagicTexture.Initialize(0, 2048, 2048, MagicTexture::ALPHA, MagicTexture::UNSIGNED_BYTE);
		if (!_err)
			return false;
		m_MagicTexture.SetParameteri(MagicTexture::LINEAR);

		L_Char_free.emplace_back(0, m_MagicTexture.GetWidth() * m_MagicTexture.GetHeight() / m_header.char_size / m_header.char_size);

		return true;
	}

	void Fonts::Shutdown()
	{
		if (pFile)
			fclose(pFile);
	}

	int Fonts::GetTextAllWidth(const char* _text)
	{
		int _width = 0;
		return _width;
	}

	// 更新纹理，使用LRU算法
	void Fonts::UpdataTexture(const wchar_t* _text)
	{
		++m_Tick;

		uint32_t const tex_size = m_MagicTexture.GetWidth();

		uint32_t const kfont_char_size = m_header.char_size;

		uint32_t const num_chars_a_row = tex_size / kfont_char_size;
		uint32_t const num_total_chars = num_chars_a_row * num_chars_a_row;

		for (int a = 0; _text[a]; a++)
		{
			wchar_t _char = _text[a];
			int32_t offset = CharIndexAdvance(_char).first;
			if (offset != -1)
			{
				auto cmiter = U_Char_Info_Map.find(_char);
				if (cmiter != U_Char_Info_Map.end())
				{
					// 在现有纹理中找到了

					cmiter->second.tick = m_Tick;
				}
				else
				{
					// 在现有纹理中找不到，所以得在现有纹理中添加新字

					FONT_INFO const & ci = V_char_info[offset];

					uint32_t width = ci.width;
					uint32_t height = ci.height;

					glm::ivec2 _char_pos;
					CHARINFO _charInfo;
					if (U_Char_Info_Map.size() < num_total_chars)
					{
						// 纹理还有空间

						uint32_t const s = L_Char_free.front().first;

						_char_pos.y = s / num_chars_a_row;
						_char_pos.x = s - _char_pos.y * num_chars_a_row;

						_char_pos.x *= kfont_char_size;
						_char_pos.y *= kfont_char_size;

						_charInfo.left = static_cast<float>(_char_pos.x) / tex_size;
						_charInfo.top = static_cast<float>(_char_pos.y) / tex_size;

						++L_Char_free.front().first;
						if (L_Char_free.front().first == L_Char_free.front().second)
						{
							L_Char_free.pop_front();
						}
					}
					else
					{
						// 找到使用最长时间没有使用的字

						uint64_t min_tick = U_Char_Info_Map.begin()->second.tick;
						auto min_chiter = U_Char_Info_Map.begin();
						for (auto chiter = U_Char_Info_Map.begin(); chiter != U_Char_Info_Map.end(); ++chiter)
						{
							if (chiter->second.tick < min_tick)
							{
								min_tick = chiter->second.tick;
								min_chiter = chiter;
							}
						}

						_char_pos.x = static_cast<int32_t>(min_chiter->second.left * tex_size);
						_char_pos.y = static_cast<int32_t>(min_chiter->second.top * tex_size);
						_charInfo.left = min_chiter->second.left;
						_charInfo.top = min_chiter->second.top;

						for (auto chiter = U_Char_Info_Map.begin(); chiter != U_Char_Info_Map.end(); ++chiter)
						{
							if (chiter->second.tick == min_tick)
							{
								uint32_t const x = static_cast<int32_t>(chiter->second.left * tex_size);
								uint32_t const y = static_cast<int32_t>(chiter->second.top * tex_size);
								uint32_t const id = y * num_chars_a_row + x;
								auto freeiter = L_Char_free.begin();
								while ((freeiter != L_Char_free.end()) && (freeiter->second <= id))
								{
									++freeiter;
								}
								L_Char_free.emplace(freeiter, id, id + 1);

								U_Char_Info_Map.erase(chiter);
								break;
							}
						}
						for (auto freeiter = L_Char_free.begin(); freeiter != L_Char_free.end();)
						{
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

					_charInfo.right = _charInfo.left + static_cast<float>(width) / tex_size;
					_charInfo.bottom = _charInfo.top + static_cast<float>(height) / tex_size;
					_charInfo.tick = m_Tick;

					GetCharDistanceData(&V_Char_data[0], offset);

					m_MagicTexture.UpdataData(_char_pos.x, _char_pos.y, kfont_char_size, kfont_char_size, &V_Char_data[0]);

					U_Char_Info_Map.emplace(_char, _charInfo);
				}
			}
		}
	}

	void Fonts::GetCharDistanceData(uint8_t* _pdata, int32_t _index) const
	{
		_fseeki64(pFile, m_Data_Start_Pos + 1024 * _index, SEEK_SET);
		fread(_pdata, 1024, 1, pFile);
	}

	std::pair<int32_t, uint32_t> const & Fonts::CharIndexAdvance(wchar_t _char) const
	{
		auto iter = V_char_index_advance.find(_char);
		if (iter != V_char_index_advance.end())
		{
			return iter->second;
		}
		else
		{
			static std::pair<int32_t, uint32_t> ret(-1, 0);
			return ret;
		}
	}
}