#ifndef _FUZZY_EFFECT_H_
#define _FUZZY_EFEECT_H_

#include "Render/TemplateEffects.h"
#include "Magic_Thread.h"

namespace Magic {
	class Fuzzy_Effect :public Template_Effects {
	public:
		Fuzzy_Effect();
		virtual ~Fuzzy_Effect();

		bool Initialize();
	protected:
		virtual MagicFBO* GetFBO() { return &m_FBOBuffer; }
		void Event_Rect(Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message);
		virtual void RenderStart();
		virtual void RenderEnd();
		virtual void RenderToTarget(Template_Effects* _pTemplate_Effects);
	private:
		MagicFBO m_FBOBuffer;
	};
}

#endif