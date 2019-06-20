#include "MainTemplateEffects.h"
#include "Tool/InjectFunction.h"
#include "Render_thread.h"

namespace Magic {

	Main_Template_Effects* Main_Template_Effects::m_S_pMain_Template_Effects = nullptr;

	Main_Template_Effects::Main_Template_Effects() :Template_Effects("Main") {
		m_S_pMain_Template_Effects = this;
	}

	Main_Template_Effects::~Main_Template_Effects() {
		m_S_pMain_Template_Effects = nullptr;
	}

	bool Main_Template_Effects::Initialize() {

		Magic::MonitorRenderThread(Magic::RENDER_SET_RECT, BindClassFunctionToMessage(&Main_Template_Effects::Event_Rect));

		return true;
	}

	void Main_Template_Effects::Event_Rect(Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message) {
		Rect(MESSAGE_TO_SCREEN_RECT(_Message));
	}

	void Main_Template_Effects::Render() {
		Render_thread::Instance()->DrawFrame();
	}
}