#include "TemplateEffects.h"
#include "System/MagicEngineAPI.h"
#include "Cross_Platform_Port.h"
#include "Render_thread.h"
#include "MainTemplateEffects.h"

namespace Magic {

	static VariableRecord<Template_Effects*> m_S_Now_Template_Effects;
	static std::unordered_map<std::string, Template_Effects*> m_vec_Template_Effects;

	bool TemplateEffects(const char* _Name, const Fun_Template_Effects& _Fun) {
		auto _TE = m_vec_Template_Effects.find(_Name);
		if (_TE != m_vec_Template_Effects.end()) {
			m_S_Now_Template_Effects = _TE->second;

			_Fun(_TE->second);

			Template_Effects* _pTemplate_Effects = _TE->second;
			if (Main_Template_Effects::Instance() == _pTemplate_Effects) {
				_pTemplate_Effects->Render();
			}
			else {
				Magic::RenderThread([_pTemplate_Effects](Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message) {
					_pTemplate_Effects->Render();
				});
			}
			m_S_Now_Template_Effects.back();

			return true;
		}
		else {
			return false;
		}
	}

	bool TemplateEffects(Template_Effects* _pTemplate_Effects, const Fun_Template_Effects& _Fun) {
		if (_pTemplate_Effects) {
			m_S_Now_Template_Effects = _pTemplate_Effects;

			_Fun(_pTemplate_Effects);

			if (Main_Template_Effects::Instance() == _pTemplate_Effects) {
				_pTemplate_Effects->Render();
			}
			else {
				Magic::RenderThread([_pTemplate_Effects](Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message) {
					_pTemplate_Effects->Render();
				});
			}

			m_S_Now_Template_Effects.back();

			return true;
		}
		else {
			return false;
		}
	}

	bool EnableTemplateEffects(const char* _Name) {
		auto _TE = m_vec_Template_Effects.find(_Name);
		if (_TE != m_vec_Template_Effects.end()) {
			m_S_Now_Template_Effects = _TE->second;

			return true;
		}
		else {
			return false;
		}
	}

	bool EnableTemplateEffects(Template_Effects* _pTemplate_Effects) {
		if (_pTemplate_Effects) {
			m_S_Now_Template_Effects = _pTemplate_Effects;

			return true;
		}
		else {
			return false;
		}
	}

	bool DisableTemplateEffects(const char* _Name) {
		auto _TE = m_vec_Template_Effects.find(_Name);
		if (_TE != m_vec_Template_Effects.end()) {

			Template_Effects* _pTemplate_Effects = _TE->second;
			if (Main_Template_Effects::Instance() == _pTemplate_Effects) {
				_pTemplate_Effects->Render();
			}
			else {
				Magic::RenderThread([_pTemplate_Effects](Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message) {
					_pTemplate_Effects->Render();
				});
			}

			m_S_Now_Template_Effects.back();

			return true;
		}
		else {
			return false;
		}
	}

	bool DisableTemplateEffects(Template_Effects* _pTemplate_Effects) {
		if (_pTemplate_Effects) {

			if (Main_Template_Effects::Instance() == _pTemplate_Effects) {
				_pTemplate_Effects->Render();
			}
			else {
				Magic::RenderThread([_pTemplate_Effects](Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message) {
					_pTemplate_Effects->Render();
				});
			}

			m_S_Now_Template_Effects.back();

			return true;
		}
		else {
			return false;
		}
	}

	void MonitorTemplateEffects(Fun_Template_Effects* _Fun) {
		m_S_Now_Template_Effects.Monitor(_Fun);
	}

	void RemoveMonitorTemplateEffects(Fun_Template_Effects* _Fun) {
		m_S_Now_Template_Effects.RemoveMonitor(_Fun);
	}

	Template_Effects::Template_Effects(const char* _Name) {
		auto _TE = m_vec_Template_Effects.find(_Name);
		if (_TE != m_vec_Template_Effects.end()) {
			char _text[256];
			Magic_Sprintf_s(_text, 256, "m_vec_Template_Effects:Name conflict£¡%s", _Name);
			ShutdownEngine(1, _text);
			return;
		}
		else {
			m_Name = _Name;
			m_vec_Template_Effects.insert(std::make_pair(m_Name, this));
		}
	}

	Template_Effects::~Template_Effects() {
		m_vec_Template_Effects.erase(m_Name);
	}

	void Template_Effects::Rect(const Magic::Screen_Rect& _Rect) {
		m_Rect = _Rect;
	}
}