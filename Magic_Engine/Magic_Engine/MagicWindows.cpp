#include "MagicWindows.h"



MagicWindows::MagicWindows()
{

}


MagicWindows::~MagicWindows()
{
	Shutdown();
}

bool MagicWindows::Initialize()
{
	bool result;
	result = CreateWindows(L"MagicWindow", 1024, 768);
	if (!result)
		return false;

	result = m_MagicEngineContext.Initialize(m_hwnd);
	if (!result)
		return false;
	m_MagicEngineContext.GetPen()->ReshapeWinSize(1024, 768);

//	result = EnableAlphaWindow(m_hwnd, 1024, 768, &m_SGDI);
	if (!result)
		return false;

	m_MagicEngineContext.SetBackColor(1.0f, 0.0f, 0.0f, 1.0f);

	result = m_MagicSceneCircle.MagicScene::Initialize(glm::vec4(0, 0, 1024, 768), &m_MagicEngineContext);
	if (!result)
		return false;

	result = m_SpecialEffectsPanel.Initialize(1024, 768);
	if (!result)
		return false;

	return true;
}

bool MagicWindows::CreateWindows(wchar_t* _Name, int _width, int _height)
{
	WNDCLASSEX wc;
	int posX, posY;
	wcscpy(m_applicationName, _Name);
	// �õ�һ���ⲿָ��ָ���������
	ApplicationHandle = this;

	// �õ����Ӧ�ó����ʵ����
	m_hinstance = GetModuleHandle(NULL);

	// ��װWindows���Ĭ�����á�
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = (HICON)::LoadImage(NULL, L"ħ������.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);  //���ؼ����Զ���icoͼ��
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// ע�ᴰ���ࡣ
	RegisterClassEx(&wc);

	posX = (GetSystemMetrics(SM_CXSCREEN) - _width) / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - _height) / 2;

	// ����Ļ�����ô������ڲ���������
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_POPUP | WS_MINIMIZEBOX | WS_SYSMENU,
		posX, posY, _width, _height, NULL, NULL, m_hinstance, NULL);

	// �Ѵ�������Ļ�ϲ���������Ϊ���㡣
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	return true;
}

LRESULT CALLBACK MagicWindows::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)    //��Ϣ������
{
	switch (umsg)
	{
		// ����Ƿ��м������¼����ϵġ�
	case WM_KEYDOWN:
	{
		if (wparam == VK_ESCAPE)    // ��������µļ���ESC
			DestroyWindow(hwnd);		// ���ٴ���, ������һ��WM_DESTROY��Ϣ
		return 0;
	}

	// ����Ƿ����ڼ������ͷŵĹؼ���
	case WM_KEYUP:
	{
		return 0;
	}

	// �κ���������Ϣ���͵�Ĭ�ϵ���Ϣ�������Ϊ���ǵ�Ӧ�ó���ʹ�����ǡ�
	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
}

void MagicWindows::Run()     //���̴�����
{
	MSG msg;
	bool done;


	// ��ʼ����Ϣ�ṹ��
	ZeroMemory(&msg, sizeof(MSG));

	// ѭ��ֱ���дӴ��ڻ��û��˳���Ϣ��
	done = false;
	while (!done)
	{
		// ����Windows��Ϣ��
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// ���Windows�źŶ˵�Ӧ��Ȼ���˳���
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			m_MagicEngineContext.Render();

			//glReadPixels(0, 0, m_SGDI.m_Width, m_SGDI.m_Height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, m_SGDI.m_pBackBuffer);
//			glBindTexture(GL_TEXTURE_2D, m_MagicSceneCircle.GetTextrue());
//			glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, m_SGDI.m_pBackBuffer);

//			UpdataAlphaWindow(&m_SGDI);
		}
	}

	return;

}

void MagicWindows::Shutdown()
{
	DisableAlphaWindow(&m_SGDI);
	// ɾ�����ڡ�
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// ɾ��Ӧ��ʵ����
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// �ͷ�ָ��ָ������ࡣ
	ApplicationHandle = NULL;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)    //��Ϣ������
{
	switch (umessage)
	{
		// ������ڱ����١�
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		// ������ڱ��رա�
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

		// ������������Ϣ����ϵͳ�е������Ϣ�������
	default:
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
}