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
		DrawTool(const ::Magic::Fun_Template_Effects& _Fun = NULL);
		virtual ~DrawTool();

		void MonitorTEffects(const ::Magic::PTemplate_Effects& _pTemplate_Effects);

		inline T* operator->() {
			return m_this;
		}

	protected:
		T* m_this;
	private:
		std::map<::Magic::PTemplate_Effects, T> m_vec_DrawContent;
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
		RenderThread([this, _pTemplate_Effects](Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message) {
			auto _auto = m_vec_DrawContent.find(_pTemplate_Effects);
			if (_auto != m_vec_DrawContent.end()) {
				m_this = &_auto->second;
			}
			else {
				m_this = &m_vec_DrawContent[_pTemplate_Effects];
				m_this->Now_PTE = _pTemplate_Effects;
			}
			if (m_Fun)
				m_Fun(_pTemplate_Effects);
		});
	}
}

#endif