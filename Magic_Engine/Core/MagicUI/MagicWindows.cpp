#include "MagicWindows.h"
#include "Render/MagicEngineAPI.h"

#include "Windowsx.h"
/*
struct VertexShaderType
{
Magic::vector3 m_pos;
};

Magic::vector4 Shader_VertexShader(void* const pVertex, void* const pVariate)
{
VertexShaderType* pVertexShaderType = (VertexShaderType*)pVertex;
return Magic::vector4(pVertexShaderType->m_pos.x, pVertexShaderType->m_pos.y, pVertexShaderType->m_pos.z, 1.0f);
}

Magic::vector4 Shader_PixelShader(void* const pIn, void* const pVariate)
{
return Magic::vector4(1.0f, 0, 0, 1);
}*/

MagicWindows* MagicWindows::pMagicWindows = 0;

MagicWindows::MagicWindows()
{
	pMagicWindows = this;
	m_CallbackMessage_WIN32 = 0;
}


MagicWindows::~MagicWindows()
{
	pMagicWindows = 0;
}

bool MagicWindows::Initialize(const wchar_t* _name, int _x, int _y, int _w, int _h)
{
	bool result;
	result = CreateWindows(_name, _x, _y, _w, _h);
	if (!result)
		return false;

	m_RenderContext = Magic::CreateRenderContext(m_hwnd);
	if (m_RenderContext == NULL)
		return false;

	result = Magic::CreateEngine(m_RenderContext);
	if (!result)
		return false;

	Magic::Engine(BindClassFunction(&MagicWindows::Updata));

	Magic::ShutdownMessage(BindClassFunctionToMessage(&MagicWindows::Shutdown));

	Magic::Screen_Rect _Screen_Rect;
	_Screen_Rect.x = 0;
	_Screen_Rect.y = 0;
	_Screen_Rect.w = _w;
	_Screen_Rect.h = _h;
	Magic::SetRenderContextRect(_Screen_Rect);

	return true;
}

LRESULT CALLBACK MagicWindows::MessageHandler(HWND _hwnd, UINT _umsg, WPARAM _wparam, LPARAM _lparam)    //消息处理函数
{
	if (m_CallbackMessage_WIN32)
	{
		int _result = m_CallbackMessage_WIN32(_hwnd, _umsg, _wparam, _lparam);
		if (!_result)
			return 0;
	}

	switch (_umsg)
	{
		// 检查是否有键被按下键盘上的。
	case WM_KEYDOWN:
		if (_wparam == VK_ESCAPE)    // 如果被按下的键是ESC
			DestroyWindow(_hwnd);		// 销毁窗口, 并发送一条WM_DESTROY消息
		// m_MagicWorld.MessageHandle(wparam, lparam);
		return 0;
		// 检查是否已在键盘上释放的关键。
	case WM_KEYUP:
		return 0;
		// 任何其他的消息发送到默认的消息处理程序为我们的应用程序不使用它们。
	case WM_MOUSEMOVE:
	{
		RECT _rect;
		GetClientRect(_hwnd, &_rect);
		//Magic::SendMessageToScene(0, { MAGIC_UI_MESSAGE_MOUSE_MOVE,(_lparam & 0xFFFF) | ((LPARAM)(_rect.bottom - GET_Y_LPARAM(_lparam))) << 16 });
		return 0;
	}
	default:
		return DefWindowProc(_hwnd, _umsg, _wparam, _lparam);
	}
}

void MagicWindows::Updata()     //过程处理函数
{
	MSG msg;

	// 处理Windows消息。
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

}

void MagicWindows::SetCallbackMessage_WIN32(Magic::CallbackMessage_WIN32 _CallbackMessage_WIN32)
{
	m_CallbackMessage_WIN32 = _CallbackMessage_WIN32;
}

void MagicWindows::RequestOuitSystem()
{
	PostQuitMessage(0);
}

bool MagicWindows::CreateWindows(const wchar_t* _Name, int _x, int _y, int _width, int _height)
{
	WNDCLASSEX wc;
	wcscpy_s(m_applicationName, 256, _Name);

	// 得到这个应用程序的实例。
	m_hinstance = GetModuleHandle(NULL);

	// 安装Windows类的默认设置。
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	//wc.hIcon = (HICON)::LoadImage(NULL, L"魔法阵：鑫.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);  //本地加载自定义ico图标
	wc.hIcon = NULL;
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = m_applicationName;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// 注册窗口类。
	RegisterClassEx(&wc);

	/*
	posX = (GetSystemMetrics(SM_CXSCREEN) - _width) / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - _height) / 2;*/

	// 在屏幕上设置创建窗口并掌握它。
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_POPUP | WS_MINIMIZEBOX | WS_SYSMENU,
		_x, _y, _width, _height, NULL, NULL, m_hinstance, NULL);

	// 把窗口在屏幕上并将它设置为焦点。
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	return true;
}

void MagicWindows::Shutdown(Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message)
{
	Magic::ShutdownRenderContext(m_RenderContext);
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// 删除应用实例。
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	if (MagicWindows::Instance())
		delete MagicWindows::Instance();
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)    //消息处理函数
{
	switch (umessage)
	{
		// 如果窗口被销毁。
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		// 如果窗口被关闭。
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

		// 所有其它的消息传递系统中的类的消息处理程序。
	default:
		return MagicWindows::Instance()->MessageHandler(hwnd, umessage, wparam, lparam);
	}
}