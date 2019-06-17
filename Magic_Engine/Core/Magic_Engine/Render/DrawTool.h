#ifndef _DRAW_TOOL_H_
#define _DRAW_TOOL_H_

#include "TemplateEffects.h"

namespace Magic {
	
#define RT_ENVIRONMENT_START	struct RT_Environment_Object {



#define RT_ENVIRONMENT_END		};

#define ClassDrawTool(a)		DrawTool<a::RT_Environment_Object>

	template<class T>
	class DrawTool {
	public:
		DrawTool();
		~DrawTool();

		virtual void MonitorTEffects(const PTemplate_Effects& _pTemplate_Effects);

		inline T* operator->() {
			return m_this;
		}
	protected:
		T* m_this;
	private:
		std::map<Template_Effects*, T> m_vec_DrawContent;
	};

	template<class T>
	DrawTool<T>::DrawTool() {
		m_this = 0;
		MonitorTemplateEffects(BindClassFunction_1(MonitorTEffects)):
	}

	template<class T>
	DrawTool<T>::~DrawTool() {
		RemoveMonitorTemplateEffects(BindClassFunction_1(MonitorTEffects)):
	}

	template<class T>
	void DrawTool<T>::MonitorTEffects(const PTemplate_Effects& _pTemplate_Effects) {
		auto _auto = m_vec_DrawContent.find(_pTemplate_Effects);
		if (_auto != m_vec_DrawContent.end()) {
			m_this = &_auto->second;
		}
		else {
			auto _pair = std::make_pair(_pTemplate_Effects, T());
			m_this = &_pair->second;
			m_vec_DrawContent.insert(_pair);
		}
	}
}

#endif