#ifndef _RENDER_THREAD_H_
#define _RENDER_THREAD_H_

#include "Magic_Thread.h"
#include "windows.h"


namespace Magic
{
	enum RENDER_THREAD_EVENT
	{
		RENDER_START = 1,
		RENDER,
		RENDER_TRANSPARENT,
		RENDER_END
	};

	bool RenderThread(Magic::Management::Callback_Message _Callback_Message);

	bool MonitorRenderThread(RENDER_THREAD_EVENT _event, Magic::Management::Callback_Message _Callback_Message);

	class Render_thread :public Magic::Management::UpdataCommon
	{
	public:
		static Render_thread* Instance() { return pRender_thread; }

		Render_thread();
		~Render_thread();

		bool Initialize();

		void BindRC(HDC _HDC, HGLRC _hRC);

		Magic::Management::THREAD_OBJECT GetTHREAD_OBJECT() { return m_TO_Render_thread; }

		void DrawFrame();
	private:
		virtual bool Updata();

		void RenderStart(Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message);
		void Render(Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message);
		void RenderTransparent(Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message);
		void RenderEnd(Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message);

	private:
		Magic::Management::THREAD_OBJECT m_TO_Render_thread;

		HGLRC m_hRC;
		HDC m_HDC;

		static Render_thread* pRender_thread;
	};
}

#endif
