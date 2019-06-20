#include "Render_thread.h"
#include "MagicEngineAPI.h"
#include "Cross_Platform_Port.h"
#include <GL/glew.h>  

namespace Magic
{
	HGLRC CreateRCContxt(HDC _hdc)
	{
		HGLRC m_hRC = wglCreateContext(_hdc);
		if (m_hRC == 0)
		{
			Magic::SetEngineErrorMessage("Error Creating RC");
			return false;
		}

		return m_hRC;
	}

	void ShutdownRC(HGLRC _hRC) {
		if (wglDeleteContext(_hRC) == FALSE) {
			return;
		}
	}

	bool CreateHD(HDC _hdc) {
		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR), // size of this pfd
			1,                                                     // version number
			PFD_DRAW_TO_WINDOW |           // support window
			PFD_SUPPORT_OPENGL |               // support OpenGL
			PFD_DOUBLEBUFFER,                     // double buffered
			PFD_TYPE_RGBA,                           // RGBA type
			24,                                                 // 24-bit color depth
			0, 0, 0, 0, 0, 0,                               // color bits ignored
			0,                                                   // no alpha buffer
			0,                                                   // shift bit ignored
			0,                                                   // no accumulation buffer
			0, 0, 0, 0,                                       // accum bits ignored
			32,                                                 // 32-bit z-buffer
			0,                                                   // no stencil buffer
			0,                                                   // no auxiliary buffer
			PFD_MAIN_PLANE,                         // main layer
			0,                                                   // reserved
			0, 0, 0                                            // layer masks ignored
		};

		int m_nPixelFormat = ChoosePixelFormat(_hdc, &pfd);
		if (m_nPixelFormat == 0)
		{
			Magic::SetEngineErrorMessage("ChoosePixelFormat failed.");
			return false;
		}
		if (SetPixelFormat(_hdc, m_nPixelFormat, &pfd) == FALSE)
		{
			Magic::SetEngineErrorMessage("SetPixelFormat failed.");
			return false;
		}

		return true;
	}

	Render_Context::Render_Context() {
	}

	Render_Context_Opengl::Render_Context_Opengl() {
		m_hwnd = 0;
		m_hRC = 0;
		m_HDC = 0;
	}

	Render_Context_Opengl::~Render_Context_Opengl() {
		m_hwnd = 0;
		m_hRC = 0;
		m_HDC = 0;
	}

	bool Render_Context_Opengl::CreateRenderContext(HWND _hwnd)
	{
		m_hwnd = _hwnd;
		m_HDC = GetDC(_hwnd);

		bool _result = CreateHD(m_HDC);
		if (!_result)
			return false;

		m_hRC = CreateRCContxt(m_HDC);
		if (!m_hRC)
			return false;

		return true;
	}

	void Render_Context_Opengl::ShutdownRenderContext() {
		ShutdownRC(m_hRC);
	}

	void Render_Context_Opengl::BindRenderContext() {
		if (wglMakeCurrent(m_HDC, m_hRC) == FALSE) {
			Magic::SetEngineErrorMessage("Error making RC Current");
		}
	}

	void Render_Context_Opengl::SwapBuffers() {
		::SwapBuffers(m_HDC);
	}

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
		m_pRender_Context = 0;
	}

	Render_thread::~Render_thread()
	{

	}

	bool Render_thread::Initialize(Render_Context* _pRender_Context) {

		m_TO_Render_thread = Magic::Management::CreateThreadObject("Render_thread", Magic::Management::THREAD_LOOP_RUN, Magic::Management::THREAD_MESSAGE_WAIT);

		char _text[256];
		char* _ptext = _text;
		bool _result = false;

		Magic::Management::SendMessageTo(m_TO_Render_thread, 0, 0,
			[this, _ptext, &_result, _pRender_Context](Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message) {
			m_pRender_Context = _pRender_Context;
			if (_pRender_Context)
				m_pRender_Context->BindRenderContext();

			GLenum err = glewInit();
			if (GLEW_OK != err)
			{
				Magic_Sprintf_s(_ptext, 256, "Error:'%s'\n", glewGetErrorString(err));
				_result = false;
				return;
			}

			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

			//启用背面剔除
			glEnable(GL_CULL_FACE);

			MonitorRenderThread(RENDER_START, BindClassFunctionToMessage(&Render_thread::RenderStart));
			MonitorRenderThread(RENDER, BindClassFunctionToMessage(&Render_thread::Render));
			MonitorRenderThread(RENDER_TRANSPARENT, BindClassFunctionToMessage(&Render_thread::RenderTransparent));
			MonitorRenderThread(RENDER_END, BindClassFunctionToMessage(&Render_thread::RenderEnd));

			MonitorRenderThread(RENDER_SET_RECT, BindClassFunctionToMessage(&Render_thread::SetScreenWidthHeight));

			_result = true;
		}, true);

		if (!_result) {
			Magic::SetEngineErrorMessage(_text);
			return false;
		}

		return true;
	}

	void Render_thread::BindRC(Render_Context* _pRender_Context) {
		Magic::Management::SendMessageTo(m_TO_Render_thread, 0, 0, [this, _pRender_Context](unsigned int, long long) {
			//Make the RC Current
			m_pRender_Context = _pRender_Context;
			if (_pRender_Context)
				m_pRender_Context->BindRenderContext();
			else {
				if (wglMakeCurrent(NULL, NULL) == FALSE) {
					Magic::SetEngineErrorMessage("Error making RC Current");
				}
			}
		});
	}

	void Render_thread::ShutdownRC(Render_Context* _pRender_Context) {
		Magic::Management::SendMessageTo(m_TO_Render_thread, 0, 0, [this, _pRender_Context](unsigned int, long long) {
			//Make the RC Current

			_pRender_Context->ShutdownRenderContext();
			delete _pRender_Context;

			m_pRender_Context = NULL;
			if (wglMakeCurrent(NULL, NULL) == FALSE) {
				Magic::SetEngineErrorMessage("Error making RC Current");
			}
		});
	}

	void Render_thread::DrawFrame() {
		Magic::Management::SendMessageTo(m_TO_Render_thread, RENDER_START, 0);
		Magic::Management::SendMessageTo(m_TO_Render_thread, RENDER, 0);
		Magic::Management::SendMessageTo(m_TO_Render_thread, RENDER_TRANSPARENT, 0);
		Magic::Management::SendMessageTo(m_TO_Render_thread, RENDER_END, 0, NULL, true);
	}

	void Render_thread::SetScreenWidthHeight(Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message) {
		m_Screen_Rect = MESSAGE_TO_SCREEN_RECT(_Message);
		glViewport(m_Screen_Rect.x, m_Screen_Rect.y, m_Screen_Rect.w, m_Screen_Rect.h); //设置视频口
	}

	void Render_thread::SetRect(const Screen_Rect& _Screen_Rect) {
		Magic::Management::SendMessageTo(m_TO_Render_thread, RENDER_SET_RECT, SCREEN_RECT_TO_MESSAGE(_Screen_Rect));
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
		m_pRender_Context->SwapBuffers();
	}
}