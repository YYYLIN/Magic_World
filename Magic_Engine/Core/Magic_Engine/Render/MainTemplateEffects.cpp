#include "MainTemplateEffects.h"
#include "Tool/InjectFunction.h"
#include "Render_thread.h"
#include "MagicFBO.h"
#include "System/MagicEngineAPI.h"

namespace Magic {
	LOAD_TEMPLATE_EFFECTS(Main_Template_Effects);

	Main_Template_Effects::Main_Template_Effects() :Template_Effects("Main") {
	}

	Main_Template_Effects::~Main_Template_Effects() {
	}

	bool Main_Template_Effects::Initialize() {

		Magic::MonitorRenderThread(Magic::RENDER_SET_RECT, BindClassFunctionToMessage(&Main_Template_Effects::Event_Rect));

		return true;
	}

	void Main_Template_Effects::Event_Rect(Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message) {
		Rect(MESSAGE_TO_SCREEN_RECT(_Message));
	}

	void Main_Template_Effects::RenderStart() {
		MagicFBO::UnUse();
		MagicFBO::Clear(MagicFBO::BUFFER_MODE::B_COLOR | MagicFBO::BUFFER_MODE::B_DEPTH);
	}

	void Main_Template_Effects::RenderEnd() {
		Render_thread::Instance()->SwapBuffers();
	}

	void Main_Template_Effects::RenderToTarget(Template_Effects* _pTemplate_Effects) {
	}
}