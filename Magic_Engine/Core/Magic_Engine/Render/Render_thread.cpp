#include "Render_thread.h"
#include "MagicEngineAPI.h"
#include <GL/glew.h>  

namespace Magic
{

	HGLRC CreateRCContxt(HDC _hdc)
	{
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
			MessageBoxA(0, "ChoosePixelFormat failed.", "errer", MB_OK);
			return false;
		}
		if (SetPixelFormat(_hdc, m_nPixelFormat, &pfd) == FALSE)
		{
			MessageBoxA(0, "SetPixelFormat failed.", "errer", MB_OK);
			return false;
		}

		HGLRC m_hRC = wglCreateContext(_hdc);
		if (m_hRC == 0)
		{
			MessageBoxA(0, "Error Creating RC", "errer", MB_OK);
			return false;
		}

		//Make the RC Current
		if (wglMakeCurrent(_hdc, m_hRC) == FALSE)
		{
			MessageBoxA(0, "Error making RC Current", "errer", MB_OK);
			return false;
		}

		return m_hRC;
	}

	Render_thread::Render_thread()
	{
		m_TO_Render_thread = 0;
	}

	Render_thread::~Render_thread()
	{

	}

	bool Render_thread::Initialize(HWND _hwnd)
	{
		m_TO_Render_thread = Magic::Management::CreateThreadObject("Render_thread", this, Magic::Management::THREAD_LOOP_RUN);

		Magic::Management::SendMessageTo(m_TO_Render_thread, 0, 0, [this, _hwnd](unsigned int, long long) {
			m_hWnd = _hwnd;
			m_HDC = GetDC(m_hWnd);
			m_hRC = Magic::CreateRCContxt(m_HDC);
			if (!m_hRC)
			{
				ShutdownEngine();
				return false;
			}
		

			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

			GLenum err = glewInit();
			if (GLEW_OK != err)
			{
				fprintf(stderr, "Error:'%s'\n", glewGetErrorString(err));
				ShutdownEngine();
				return false;
			}

			//ÆôÓÃ±³ÃæÌÞ³ý
			glEnable(GL_CULL_FACE);
		});

		return true;
	}

	bool Render_thread::Updata()
	{

	}

}