#include "FuzzyEffect.h"
#include "Render/Render_thread.h"
#include "System/MagicEngineAPI.h"

namespace Magic {
	LOAD_TEMPLATE_EFFECTS(Fuzzy_Effect);

	Fuzzy_Effect::Fuzzy_Effect() :
		Template_Effects("Fuzzy") {

	}

	Fuzzy_Effect::~Fuzzy_Effect() {

	}

	bool Fuzzy_Effect::Initialize() {

		bool _result = m_FBOBuffer.Initialize(1024, 768, MagicFBO::COLOR4_DEPTH32, 8);
		if (!_result)
			return false;

		Magic::MonitorRenderThread(Magic::RENDER_SET_RECT, BindClassFunctionToMessage(&Fuzzy_Effect::Event_Rect));

		return true;
	}

	void Fuzzy_Effect::Event_Rect(Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message) {
		Rect(MESSAGE_TO_SCREEN_RECT(_Message));
		m_FBOBuffer.ResetSize(Rect().w, Rect().h, 8);
	}

	void Fuzzy_Effect::RenderStart() {
		m_FBOBuffer.Use();
		m_FBOBuffer.Clear(MagicFBO::BUFFER_MODE::B_COLOR | MagicFBO::BUFFER_MODE::B_DEPTH);
	}

	void Fuzzy_Effect::RenderEnd() {
		m_FBOBuffer.UnUse();
	}

	void Fuzzy_Effect::RenderToTarget(Template_Effects* _pTemplate_Effects) {
		m_FBOBuffer.CopyFBOTO(_pTemplate_Effects->GetFBO(), 0, 0, _pTemplate_Effects->Rect().w, _pTemplate_Effects->Rect().h,
			&m_FBOBuffer, 0, 0, Rect().w, Rect().h);
	}
}