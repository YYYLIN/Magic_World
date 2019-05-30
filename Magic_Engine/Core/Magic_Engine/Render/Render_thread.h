#ifndef _RENDER_THREAD_H_
#define _RENDER_THREAD_H_

#include "Magic_Thread.h"
#include "windows.h"
namespace Magic
{
	enum RENDER_THREAD_EVENT
	{
		RENDER_START = 1,
		RENDER_END = 2
	};

	class Render_thread :public Magic::Management::UpdataCommon
	{
	public:
		static Render_thread* Instance() { return pRender_thread; }

		Render_thread();
		~Render_thread();

		bool Initialize();

		void BindRC(HDC _HDC, HGLRC _hRC);

		Magic::Management::THREAD_OBJECT GetTHREAD_OBJECT() { return m_TO_Render_thread; }
	private:
		virtual bool Updata();

	private:
		Magic::Management::THREAD_OBJECT m_TO_Render_thread;

		HGLRC m_hRC;
		HDC m_HDC;

		static Render_thread* pRender_thread;
	};
}

#endif