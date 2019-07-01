#include "TemplateEffects.h"
#include "System/MagicEngineAPI.h"
#include "Cross_Platform_Port.h"
#include "Render_thread.h"
#include "MainTemplateEffects.h"

namespace Magic {

	static VariableRecord<Template_Effects*> m_S_Now_Template_Effects;
	static std::unordered_map<std::string, Template_Effects*> m_umap_Template_Effects;

	bool TemplateEffects(const char* _Name, const Fun_Template_Effects& _Fun) {
		auto _TE = m_umap_Template_Effects.find(_Name);
		if (_TE != m_umap_Template_Effects.end()) {
			m_S_Now_Template_Effects = _TE->second;

			_Fun(_TE->second);

			_TE->second->RenderRequest(m_S_Now_Template_Effects.back());

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

			_pTemplate_Effects->RenderRequest(m_S_Now_Template_Effects.back());

			return true;
		}
		else {
			return false;
		}
	}

	bool EnableTemplateEffects(const char* _Name) {
		auto _TE = m_umap_Template_Effects.find(_Name);
		if (_TE != m_umap_Template_Effects.end()) {
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
		auto _TE = m_umap_Template_Effects.find(_Name);
		if (_TE != m_umap_Template_Effects.end()) {

			_TE->second->RenderRequest(m_S_Now_Template_Effects.back());

			return true;
		}
		else {
			return false;
		}
	}

	bool DisableTemplateEffects(Template_Effects* _pTemplate_Effects) {
		if (_pTemplate_Effects) {

			_pTemplate_Effects->RenderRequest(m_S_Now_Template_Effects.back());

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

	size_t NumberOfLayersEffects() {
		return m_S_Now_Template_Effects.LogNumber();
	}

	Template_Effects::Template_Effects(const char* _Name) {
		auto _TE = m_umap_Template_Effects.find(_Name);
		if (_TE != m_umap_Template_Effects.end()) {
			char _text[256];
			Magic_Sprintf_s(_text, 256, "m_vec_Template_Effects:Name conflict£¡%s", _Name);
			ShutdownEngine(1, _text);
			return;
		}
		else {
			m_Name = _Name;
			m_umap_Template_Effects.insert(std::make_pair(m_Name, this));
		}
	}

	Template_Effects::~Template_Effects() {
		m_umap_Template_Effects.erase(m_Name);
	}

	void Template_Effects::Rect(const Magic::Screen_Rect& _Rect) {
		m_Rect = _Rect;
	}

	void Template_Effects::RenderRequest(Template_Effects* _pTemplate_Effects) {
		if (_pTemplate_Effects) {
			_pTemplate_Effects->Target(this);
		}
		else {
			this->RenderTarget(0);
		}
	}

	void Template_Effects::Target(Template_Effects* _pTemplate_Effects) {
		m_vec_Template_Effects.insert(_pTemplate_Effects);
	}

	void Template_Effects::RenderTarget(Template_Effects* _pTemplate_Effects) {
		m_S_Now_Template_Effects = _pTemplate_Effects;

		for (auto& _p : m_vec_Template_Effects) {
			_p->RenderTarget(this);
		}
		m_vec_Template_Effects.clear();

		RenderThread([this](MM_MESS) {
			this->RenderStart();
		});

		SendRenderThread(Magic::RENDER, 0);
		SendRenderThread(Magic::RENDER_TRANSPARENT, 0);

		RenderThread([this, _pTemplate_Effects](MM_MESS) {
			this->RenderEnd(_pTemplate_Effects);
		}, this->SynchRender());

		m_S_Now_Template_Effects.back();
	}
}