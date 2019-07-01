#ifndef _RENDER_THREAD_H_
#define _RENDER_THREAD_H_

#include "Magic_Thread.h"
#include "windows.h"
#include "Define/Magic_Macro.h"
#include "Define/MagicType.h"


namespace Magic
{
	class Render_Context
	{
	public:
		Render_Context();
		virtual ~Render_Context() = 0;
	public:
		virtual bool CreateRenderContext(HWND _hwnd) = 0;
		virtual void ShutdownRenderContext() = 0;
		virtual void BindRenderContext() = 0;
		virtual void SwapBuffers() = 0;
	};

	class Render_Context_Opengl
	{
	public:
		Render_Context_Opengl();
		virtual ~Render_Context_Opengl();
	public:
		virtual bool CreateRenderContext(HWND _hwnd);
		virtual void ShutdownRenderContext();
		virtual void BindRenderContext();
		virtual void SwapBuffers();

	private:
		HWND m_hwnd;
		HGLRC m_hRC;
		HDC m_HDC;
	};

	enum RENDER_THREAD_EVENT
	{
		//RENDER_START = 1,
		RENDER = 1,
		RENDER_TRANSPARENT,
		//RENDER_END,

		RENDER_SET_RECT
	};

	bool SendRenderThread(Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message, bool _Synch = false);

	bool RenderThread(Magic::Management::Callback_Message _Callback_Message, bool _Synch = false);

	bool MonitorRenderThread(RENDER_THREAD_EVENT _event, Magic::Management::Callback_Message _Callback_Message);

	class Render_thread
	{
	public:
		static Render_thread* Instance() { return pRender_thread; }

		Render_thread();
		~Render_thread();

		bool Initialize(Render_Context* _pRender_Context);

		void BindRC(Render_Context* _pRender_Context);

		void ShutdownRC(Render_Context* _pRender_Context);

		Magic::Management::THREAD_OBJECT GetTHREAD_OBJECT() { return m_TO_Render_thread; }

		//void DrawFrame();

		void SetScreenWidthHeight(Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message);

		void SetRect(const Screen_Rect& _Screen_Rect);

		inline const Screen_Rect& GetScreenWidthHeight() { return m_Screen_Rect; }
	private:
		//void RenderStart(Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message);
		void Render(Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message);
		void RenderTransparent(Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message);
		//void RenderEnd(Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message);

	private:
		//render_thread线程对象
		Magic::Management::THREAD_OBJECT m_TO_Render_thread;

		Render_Context* m_pRender_Context;

		Screen_Rect m_Screen_Rect;

		static Render_thread* pRender_thread;
	};
}

#endif
