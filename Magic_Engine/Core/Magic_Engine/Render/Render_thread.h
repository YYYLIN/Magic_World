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

	HGLRC CreateRCContxt(HDC _hdc);

	class Render_thread :public Magic::Management::UpdataCommon
	{
	public:
		Render_thread();
		~Render_thread();

		bool Initialize(HWND _hwnd);
	private:
		virtual bool Updata();

	private:
		Magic::Management::THREAD_OBJECT m_TO_Render_thread;

		HGLRC m_hRC;
		HDC m_HDC;
		HWND m_hWnd;
	};
}

#endif