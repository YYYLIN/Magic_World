#include "Render_thread.h"
#include "MagicEngineAPI.h"
#include "Cross_Platform_Port.h"
#include <GL/glew.h>  

namespace Magic
{
	bool RenderThread(Magic::Management::Callback_Message _Callback_Message)
	{
		return Magic::Management::SendMessageTo(Magic::Render_thread::Instance()->GetTHREAD_OBJECT(), 0, 0, _Callback_Message);
	}

	bool MonitorRenderThread(RENDER_THREAD_EVENT _event, Magic::Management::Callback_Message _Callback_Message)
	{
		return Magic::Management::MonitorThreadMessage(Magic::Render_thread::Instance()->GetTHREAD_OBJECT(), _event, _Callback_Message);
	}

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
		m_TO_Render_thread = Magic::Management::CreateThreadObject("Render_thread", this, Magic::Management::THREAD_LOOP_RUN, Magic::Management::THREAD_MESSAGE_WAIT);

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

			MonitorRenderThread(RENDER_START, BindClassFunctionToMessage(&Render_thread::RenderStart));
			MonitorRenderThread(RENDER_END, BindClassFunctionToMessage(&Render_thread::RenderEnd));
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

	void Render_thread::DrawFrame() {
		Magic::Management::SendMessageTo(RENDER_START, 0);
		Magic::Management::SendMessageTo(RENDER, 0);
		Magic::Management::SendMessageTo(RENDER_TRANSPARENT, 0);
		Magic::Management::SendMessageTo(RENDER_END, 0);
	}

	bool Render_thread::Updata() {
		return true;
	}

	void Render_thread::RenderStart(Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Render_thread::Render(Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message) {
		glDisable(GL_BLEND);
	}

	void Render_thread::RenderTransparent(Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message) {
		glEnable(GL_BLEND);
	}

	void Render_thread::RenderEnd(Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message) {
		SwapBuffers(m_HDC);
	}
}