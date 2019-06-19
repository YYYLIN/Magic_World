#ifndef _DRAW_TOOL_H_
#define _DRAW_TOOL_H_

#include "TemplateEffects.h"
#include <map>

namespace Magic {

#define RT_ENVIRONMENT_START	struct RT_Environment_Object {
#define RT_ENVIRONMENT_END		}; ::Magic::DrawTool<RT_Environment_Object> m_this

	template<class T>
	class DrawTool {
	public:
		DrawTool(const ::Magic::Fun_Template_Effects& _Fun = NULL);
		~DrawTool();

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
		auto _auto = m_vec_DrawContent.find(_pTemplate_Effects);
		if (_auto != m_vec_DrawContent.end()) {
			m_this = &_auto->second;
		}
		else {
			auto _pair = std::make_pair(_pTemplate_Effects, T());
			m_this = &_pair.second;
			m_vec_DrawContent.insert(_pair);
		}

		m_Fun(_pTemplate_Effects);
	}
}

#endif