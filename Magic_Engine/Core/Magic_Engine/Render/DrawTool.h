#ifndef _DRAW_TOOL_H_
#define _DRAW_TOOL_H_

#include "TemplateEffects.h"

namespace Magic {

	template<class T>
	class DrawTool {
	public:
		DrawTool();
		~DrawTool();

		virtual void MonitorTemplateEffects(Template_Effects* _pTemplate_Effects);

		inline T* operator this() {
			return m_this;
		}
	protected:
		T* m_this;
	private:
		std::map<Template_Effects*, T> m_vec_DrawContent;
	};

	template<class T>
	DrawTool<T>::DrawTool() {

	}

	template<class T>
	DrawTool<T>::~DrawTool() {

	}

	template<class T>
	void DrawTool<T>::MonitorTemplateEffects(Template_Effects* _pTemplate_Effects) {

	}
}

#endif