#ifndef _DRAW_TOOL_H_
#define _DRAW_TOOL_H_

#include "TemplateEffects.h"
#include <map>
#include "Render_thread.h"

namespace Magic {

#define RT_ENVIRONMENT_START	struct RT_Environment_Object { 	::Magic::PTemplate_Effects Now_PTE;
#define RT_ENVIRONMENT_END		}; ::Magic::DrawTool<RT_Environment_Object> m_this

	template<class T>
	//RenderThread ¶ÔÏó---------->
	class DrawTool {
	public:
		struct PTEID
		{
			PTEID() :Layers(0), pTE(0) {}
			PTEID(const size_t& _Layers, const ::Magic::PTemplate_Effects& _pTE) :Layers(_Layers), pTE(_pTE) {}
			size_t Layers;
			::Magic::PTemplate_Effects pTE;
		};

		bool operator < (const PTEID& left, const PTEID& right) {
			if (left.Layers != right.Layers) {
				return left.Layers < right.Layers;
			}
			else {
				return left.pTE < right.pTE;
			}

			return false;
		}

		DrawTool(const ::Magic::Fun_Template_Effects& _Fun = NULL);
		virtual ~DrawTool();

		void MonitorTEffects(const ::Magic::PTemplate_Effects& _pTemplate_Effects);

		inline T* operator->() {
			return m_this.;
		}

	protected:
		T* m_this;
	private:
		std::map<PTEID, T> m_vec_DrawContent;
		::Magic::Fun_Template_Effects m_Fun, m_Monitor;
	};

	template<class T>
	DrawTool<T>::DrawTool(const ::Magic::Fun_Template_Effects& _Fun = NULL) {
		m_Monitor = BindClassFunction_F_1(&DrawTool<T>::MonitorTEffects);
		m_Fun = _Fun;
		m_this = 0;
		MonitorTemplateEffects(&m_Monitor);
	}

	template<class T>
	DrawTool<T>::~DrawTool() {
		RemoveMonitorTemplateEffects(&m_Monitor);
	}

	template<class T>
	void DrawTool<T>::MonitorTEffects(const ::Magic::PTemplate_Effects& _pTemplate_Effects) {
		if (!_pTemplate_Effects)
			return;
		PTEID _PTEID(_pTemplate_Effects, NumberOfLayersEffects());
		RenderThread([this, _PTEID](Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message) {
			auto _auto = m_vec_DrawContent.find(_PTEID);
			if (_auto != m_vec_DrawContent.end()) {
				m_this = &_auto->second;
			}
			else {
				m_this = &m_vec_DrawContent[_PTEID];
				m_this->Now_PTE = _pTemplate_Effects;
			}
			if (m_Fun)
				m_Fun(_pTemplate_Effects);
		});
	}
}

#endif