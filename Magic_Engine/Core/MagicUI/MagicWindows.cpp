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

	Shutdown();
	//Magic::ShutdownEngine();
	pMagicWindows = 0;
}

bool MagicWindows::Initialize(const wchar_t* _name, int _x, int _y, int _w, int _h)
{
	bool result;
	result = CreateWindows(_name, _x, _y, _w, _h);
	if (!result)
		return false;

	result = Magic::CreateEngine();
	if (!result)
		return false;

	result = Magic::CreateOpenglRender(m_hwnd, Magic::GetThreadsResourceManager());
	if (!result)
		return false;

	/*
		result = EnableAlphaWindow(m_hwnd, 1024, 768, &m_SGDI);
		if (!result)
		return false;*/

	Magic::UI::UserScene* _pUserScene = &m_UserScene;

	result = Magic::CreateScene(Magic::GetThreadsResourceManager(), &_pUserScene);
	if (!result)
		return false;
	Magic::SetSceneSize(_pUserScene->GetEntity(), glm::vec2(_w, _h));
	Magic::SetSceneCallUpdata(_pUserScene->GetEntity(), Updata);


	Magic::UI::MenuScene* _pMenuScene = &m_MenuScene;
	result = Magic::CreateScene(Magic::GetThreadsResourceManager(), &_pMenuScene);
	if (!result)
		return false;
	Magic::SetSceneSize(_pMenuScene->GetEntity(), glm::vec2(_w, _h));

	Magic::UI::DebugScene* _pDebugScene = &m_DebugScene;
	result = Magic::CreateScene(Magic::GetThreadsResourceManager(), &_pDebugScene);
	if (!result)
		return false;
	Magic::SetSceneSize(_pDebugScene->GetEntity(), glm::vec2(_w, _h));

	Magic::UI::SystemScene* _pSystemScene = &m_SystemScene;
	result = Magic::CreateScene(Magic::GetThreadsResourceManager(), &_pSystemScene);
	if (!result)
		return false;
	Magic::SetSceneSize(_pSystemScene->GetEntity(), glm::vec2(_w, _h));

	/*
		result = m_MagicWorld.Initialize();
		if (!result)
		return false;
		m_MagicEngineContext.AddCommon(&m_MagicWorld);*/

		/*
			result = m_MagicScene.Initialize(&m_MagicEngineContext, glm::vec4(0, 0, 1024, 768));
			if (!result)
			return false;*/


			//--------------------------------------------
			/*
				result = CreateMagicGratingContext(&m_MGContext);
				if (!result)
				return false;
				result = InitMagicGratingContext(m_MGContext);
				if (!result)
				return false;
				BindMagicGratingContext(m_MGContext);
				BindMGDisplayContext(m_MGContext, m_SGDI.m_pBackBuffer, 1024, 768, MG_BGRA, MG_UCHAR);
				//--------------------------------------------
				MG_CreateFrameBufferObject(&m_MGFrameBuffer);
				MG_BindFrameBuffer(m_MGFrameBuffer);
				result = MG_InitFrameBuffer(1024, 768);
				if (!result)
				return false;
				MG_ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				//-------------Shader-------------------------
				CreateMagicShaderObject(&m_Shader);
				BindMagicShaderObject(m_Shader);
				m_ShaderCamera = MGAddShaderVariate(sizeof(Magic::Matrix4));
				m_Shaderprojection = MGAddShaderVariate(sizeof(Magic::Matrix4));
				m_Shaderworld = MGAddShaderVariate(sizeof(Magic::Matrix4));
				InitMagicShaderObject(Shader_VertexShader, Shader_PixelShader);

				MG_SetShaderVariate(m_ShaderCamera, &Magic::Matrix4());
				MG_SetShaderVariate(m_Shaderprojection, &Magic::Matrix4());
				MG_SetShaderVariate(m_Shaderworld, &Magic::Matrix4());
				//--------------------------------------------
				MG_CreateVertexArrays(&m_VertexArrays);
				MG_BindVertexArrays(m_VertexArrays);
				MG_EnableVertexAttribArray(1);

				float pVertex[] =
				{
				0.5f,0.0f,0.0f,
				0.0f,0.5f,0.0f,
				-0.5f,0.0f,0.0f
				};

				MG_CreateBuffer(&m_VertexBuffer);
				MG_BindBuffer(MG_ARRAY_BUFFER, m_VertexBuffer);
				MG_BufferData(MG_ARRAY_BUFFER, sizeof(float) * 3 * 3, pVertex);
				MG_VertexAttribPointer(0, 3, MG_FLOAT);*/

	return true;
}

LRESULT CALLBACK MagicWindows::MessageHandler(HWND _hwnd, UINT _umsg, WPARAM _wparam, LPARAM _lparam)    //��Ϣ������
{
	if (m_CallbackMessage_WIN32)
	{
		int _result = m_CallbackMessage_WIN32(_hwnd, _umsg, _wparam, _lparam);
		if (!_result)
			return 0;
	}

	switch (_umsg)
	{
		// ����Ƿ��м������¼����ϵġ�
	case WM_KEYDOWN:
		if (_wparam == VK_ESCAPE)    // ��������µļ���ESC
			DestroyWindow(_hwnd);		// ���ٴ���, ������һ��WM_DESTROY��Ϣ
		// m_MagicWorld.MessageHandle(wparam, lparam);
		return 0;
		// ����Ƿ����ڼ������ͷŵĹؼ���
	case WM_KEYUP:
		return 0;
		// �κ���������Ϣ���͵�Ĭ�ϵ���Ϣ�������Ϊ���ǵ�Ӧ�ó���ʹ�����ǡ�
	case WM_MOUSEMOVE:
	{
		RECT _rect;
		GetClientRect(_hwnd, &_rect);
		Magic::SendMessageToScene(0, { MAGIC_UI_MESSAGE_MOUSE_MOVE,(_lparam & 0xFFFF) | ((LPARAM)(_rect.bottom - GET_Y_LPARAM(_lparam))) << 16 });
		return 0;
	}
	default:
		return DefWindowProc(_hwnd, _umsg, _wparam, _lparam);
	}
}

void MagicWindows::Updata(EntityCommon _entity)     //���̴�����
{
	MSG msg;

	// ����Windows��Ϣ��
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	/*
				glReadPixels(0, 0, m_SGDI.m_Width, m_SGDI.m_Height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, m_SGDI.m_pBackBuffer);
				glBindTexture(GL_TEXTURE_2D, m_MagicSceneCircle.GetTextrue());
				glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, m_SGDI.m_pBackBuffer);*/
				/*
							MG_Clear(MG_COLOR_BUFFER);

							MGDrawArrays(MG_DrawPoints, 0, 3);

							MG_WaitSwapFrame();

							UpdataAlphaWindow(&m_SGDI);*/
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

	// �õ����Ӧ�ó����ʵ����
	m_hinstance = GetModuleHandle(NULL);

	// ��װWindows���Ĭ�����á�
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	//wc.hIcon = (HICON)::LoadImage(NULL, L"ħ������.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);  //���ؼ����Զ���icoͼ��
	wc.hIcon = NULL;
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = m_applicationName;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// ע�ᴰ���ࡣ
	RegisterClassEx(&wc);

	/*
	posX = (GetSystemMetrics(SM_CXSCREEN) - _width) / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - _height) / 2;*/

	// ����Ļ�����ô������ڲ���������
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_POPUP | WS_MINIMIZEBOX | WS_SYSMENU,
		_x, _y, _width, _height, NULL, NULL, m_hinstance, NULL);

	// �Ѵ�������Ļ�ϲ���������Ϊ���㡣
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	return true;
}

void MagicWindows::Shutdown()
{
	/*
		MG_DeleteBuffer(&m_VertexBuffer);
		MG_DeleteVertexArrays(&m_VertexArrays);
		DeleteMagicShaderObject(&m_Shader);
		MG_DeleteFrameBufferObject(&m_MGFrameBuffer);
		DeleteMagicGratingContext(&m_MGContext);*/

		/*	DisableAlphaWindow(&m_SGDI);*/
		// ɾ�����ڡ�
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// ɾ��Ӧ��ʵ����
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;
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
		return MagicWindows::Instance()->MessageHandler(hwnd, umessage, wparam, lparam);
	}
}