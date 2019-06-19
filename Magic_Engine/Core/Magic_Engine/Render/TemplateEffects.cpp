#include "TemplateEffects.h"
#include "MagicEngineAPI.h"
#include "Cross_Platform_Port.h"
#include "Render_thread.h"
#include "Tool/InjectFunction.h"

namespace Magic {

	static VariableRecord<Template_Effects*> m_S_Now_Template_Effects;
	static std::unordered_map<std::string, Template_Effects*> m_vec_Template_Effects;

	bool TemplateEffects(const char* _Name, const Fun_Template_Effects& _Fun) {
		auto _TE = m_vec_Template_Effects.find(_Name);
		if (_TE != m_vec_Template_Effects.end()) {
			m_S_Now_Template_Effects = _TE->second;

			_Fun(_TE->second);

			m_S_Now_Template_Effects.back();

			Template_Effects* _pTemplate_Effects = _TE->second;

			Magic::RenderThread([_pTemplate_Effects](Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message) {
				_pTemplate_Effects->Render();
			});

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

			m_S_Now_Template_Effects.back();

			Magic::RenderThread([_pTemplate_Effects](Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message) {
				_pTemplate_Effects->Render();
			});

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

			m_S_Now_Template_Effects.back();

			Template_Effects* _pTemplate_Effects = _TE->second;

			Magic::RenderThread([_pTemplate_Effects](Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message) {
				_pTemplate_Effects->Render();
			});

			return true;
		}
		else {
			return false;
		}
	}

	bool DisableTemplateEffects(Template_Effects* _pTemplate_Effects) {
		if (_pTemplate_Effects) {
			m_S_Now_Template_Effects.back();

			Magic::RenderThread([_pTemplate_Effects](Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message) {
				_pTemplate_Effects->Render();
			});

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

	/*INJECT_FUNCTION("EngineRunStart", []() {
		m_S_Now_Template_Effects = 0;
	});*/

	Template_Effects::~Template_Effects() {
		m_vec_Template_Effects.erase(m_Name);
	}

	static int S_INJECT_FUNCTION_222 = []() {
		S_map_inject_function["EngineRunStart"].push_back([]() {
			m_S_Now_Template_Effects = 0;
		});
		return 0;
	}();
}