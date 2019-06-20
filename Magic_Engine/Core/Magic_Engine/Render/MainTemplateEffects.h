#ifndef _MAIN_TEMPLAT_EFFECTS_H_
#define _MAIN_TEPLATE_EFFECTS_H_

#include "TemplateEffects.h"
#include "Magic_Thread.h"

namespace Magic {
	class Main_Template_Effects :public Template_Effects {
	public:
		Main_Template_Effects();
		virtual ~Main_Template_Effects();

		static Main_Template_Effects* Instance() { return m_S_pMain_Template_Effects; }

		bool Initialize();
	protected:
		void Event_Rect(Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message);
		virtual void Render();
	private:
		static Main_Template_Effects* m_S_pMain_Template_Effects;
	};
}

#endif