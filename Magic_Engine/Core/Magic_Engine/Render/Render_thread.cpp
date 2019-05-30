#include "Render_thread.h"
#include "MagicEngineAPI.h"
#include "Cross_Platform_Port.h"
#include <GL/glew.h>  

namespace Magic
{
	Render_thread* Render_thread::pRender_thread = 0;

	Render_thread::Render_thread()
	{
		pRender_thread = this;
		m_TO_Render_thread = 0;
	}

	Render_thread::~Render_thread()
	{

	}

	bool Render_thread::Initialize() {
		m_TO_Render_thread = Magic::Management::CreateThreadObject("Render_thread", this, Magic::Management::THREAD_LOOP_RUN);

		Magic::Management::SendMessageTo(m_TO_Render_thread, 0, 0, 
			[this](Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message) {
			GLenum err = glewInit();
			if (GLEW_OK != err)
			{
				char _text[256];
				Magic_Sprintf_s(_text, 256, "Error:'%s'\n", glewGetErrorString(err));

				ShutdownEngine(err, _text);
				return;
			}

			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

			//ÆôÓÃ±³ÃæÌÞ³ý
			glEnable(GL_CULL_FACE);
		});

		return true;
	}

	void Render_thread::BindRC(HDC _HDC, HGLRC _hRC) {
		Magic::Management::SendMessageTo(m_TO_Render_thread, 0, 0, [this, _HDC, _hRC](unsigned int, long long) {
			//Make the RC Current
			if (wglMakeCurrent(_HDC, _hRC) == FALSE) {
				Magic::SetEngineErrorMessage("Error making RC Current");
			}
			else {
				m_HDC = _HDC;
				m_hRC = _hRC;
			}
		});
	}

	bool Render_thread::Updata() {
		Magic::Management::SendMessageTo(RENDER_START, 0);
		Magic::Management::SendMessageTo(RENDER_END, 0);
	}

}