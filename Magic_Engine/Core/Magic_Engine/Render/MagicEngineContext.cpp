#include "MagicEngineContext.h"
#include "MagicEngineAPI.h"
#include <GL/glew.h>  
#include <time.h>



const glm::mat4 CONST_CAMERA = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));


/*
MagicScenes::MagicScenes()
{
	DisplayState = 0;
}

MagicScenes::~MagicScenes()
{
	v_Common.clear();
}

bool MagicScenes::Initialize(MagicScenes* _scene, glm::vec4 _PosSize)
{
	pParentScene = _scene;
	if (pParentScene)
		pParentScene->AddCommon(this);
	this->ResetDrawRECT(_PosSize.x, _PosSize.y, _PosSize.z, _PosSize.w);
	DisplayState = true;
	return this->OnInitialize();
}

void MagicScenes::SetDisplayState(bool _state)
{
	DisplayState = _state;
}

glm::vec2 MagicScenes::GetFrameBufferSize()
{
	return pParentScene->GetFrameBufferSize();
}

void MagicScenes::AddCommon(MagicCommon* _common)
{
	v_Common.push_back(_common);
}

void MagicScenes::RemoveCommon(MagicCommon* _common)
{
	if (v_Common.size())
	{
		for (auto _auto = v_Common.begin(); _auto != v_Common.end(); _auto++)
		{
			if (_common == *_auto)
			{
				v_Common.erase(_auto);
				return;
			}
		}
	}
}

void MagicScenes::ResetDrawRECT(float _x, float _y, float _w, float _h)
{
	m_PosSize.x = _x;
	m_PosSize.y = _y;
	m_PosSize.z = _w;
	m_PosSize.w = _h;
}

void MagicScenes::Updata()
{
	if (DisplayState)
	{
		Magic::MessageScenes* _pMessageScenes = dynamic_cast<Magic::MessageScenes*>(this);
		if (_pMessageScenes)
			_pMessageScenes->ProcessMessage();

		this->OnUpdata();
		for (unsigned int a = 0; a < v_Common.size(); a++)
			v_Common[a]->Updata();
	}
}

void MagicScenes::Render(glm::vec2 _DrawPos)
{
	if (DisplayState)
	{
		_DrawPos += glm::vec2(m_PosSize);
		m_DrawPos = _DrawPos;
		this->RenderStart();
		for (unsigned int a = 0; a < v_Common.size(); a++)
			v_Common[a]->Render(_DrawPos);
		this->Draw();
		this->RenderEnd();
	}
}

void MagicScenes::RenderStart()
{
	glm::mat4 _Camera = CONST_CAMERA;
	_Camera[3].x = m_DrawPos.x;
	_Camera[3].y = m_DrawPos.y;
	MagicEngineContext::pMagicEngineContext->GetPen_Normal()->SetCameraMatrix(_Camera);
}

void MagicScenes::RenderEnd()
{
	glm::mat4 _Camera = CONST_CAMERA;
	_Camera[3].x = pParentScene->GetDrawPos().x;
	_Camera[3].y = pParentScene->GetDrawPos().y;
	MagicEngineContext::pMagicEngineContext->GetPen_Normal()->SetCameraMatrix(_Camera);
}*/

/*
//拷贝缓冲区消息回掉
void ScenesExCopyFBOBufferMessage(void *_data)
{
	((MagicScenesEx*)_data)->CopyFBOBuffer();
}

MagicScenesEx::MagicScenesEx()
{
	m_DrawMessage = false;
}

void MagicScenesEx::DrawSpirit()
{
	if (pParentScene)
		pParentScene->DrawSpirit();
	m_DrawMessage = true;
}

void MagicScenesEx::SetDisplayState(bool _state)
{
	if (DisplayState != _state)
	{
		DisplayState = _state;
		if (pParentScene)
			pParentScene->DrawSpirit();
	}
}

glm::vec2 MagicScenesEx::GetFrameBufferSize()
{
	return glm::vec2(m_FBOBuffer.GetWidth(), m_FBOBuffer.GetHeight());
}

bool MagicScenesEx::Initialize(MagicScenes* _scene, glm::vec4 _PosSize)
{
	bool result;

	int w = (int)_PosSize.z, h = (int)_PosSize.w;
	result = m_FBOBuffer.Initialize(w, h, MagicFBOTexture::COLOR4);
	if (!result)
		return false;

	result = MagicScenes::Initialize(_scene, _PosSize);
	if (!result)
		return false;

	this->DrawSpirit();

	return true;
}

void MagicScenesEx::CopyFBOBuffer()
{
	MagicFBOTexture::CopyFBOTO(pParentScene->GetFBOTexture(), 0, 0,
		(int)m_PosSize.z, (int)m_PosSize.w, &m_FBOBuffer, 0, 0,
		(int)m_PosSize.z, (int)m_PosSize.w);
}

void MagicScenesEx::Render(glm::vec2 _DrawPos)
{
	if (DisplayState)
	{
		if (m_DrawMessage)
		{
			m_FBOBuffer.Use();
			m_FBOBuffer.Clear(MagicFBOTexture::B_COLOR);
			MagicScenes::Render(glm::vec2());
			if (pParentScene->GetFBOTexture())
				pParentScene->GetFBOTexture()->Use();
			else
				m_FBOBuffer.UnUse();
			m_DrawMessage = false;
		}
		this->RenderBuffer();
	}
}

void MagicScenesEx::RenderBuffer()
{
	MagicEngineContext::pMagicEngineContext->GetPen_Normal()->AddCallBackMessage({ ScenesExCopyFBOBufferMessage ,this });
}

void MagicScenesEx::RenderStart()
{
	Magic::Pen_Normal* pPen_Normal = MagicEngineContext::pMagicEngineContext->GetPen_Normal();
	pPen_Normal->RenderStart();
	pPen_Normal->SetCameraMatrix(CONST_CAMERA);

}

void MagicScenesEx::RenderEnd()
{
	Magic::Pen_Normal* pPen_Normal = MagicEngineContext::pMagicEngineContext->GetPen_Normal();
	glm::vec2 _WH = this->GetFrameBufferSize();
	pPen_Normal->SetDrawWH(_WH.x, _WH.y);
	pPen_Normal->RenderEnd();
}*/

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

	double EngineUpdataTime(EntityCommon _Entity)
	{
		Magic::System::ThreadsComponent* _pRenderThreadsComponent = _Entity.GetComponent<Magic::System::ThreadsComponent>().operator->();

		double _time = clock();
		_pRenderThreadsComponent->m_DiffTime = _time - _pRenderThreadsComponent->m_LastTime;
		_pRenderThreadsComponent->m_LastTime = _time;

		return _time;
	}

	void EngineUpdataFPS(EntityCommon _Entity)
	{
		if (_Entity.has_component<Magic::System::RenderThreadsComponent>())
		{
			Magic::System::RenderThreadsComponent* _pRenderThreadsComponent = _Entity.GetComponent<Magic::System::RenderThreadsComponent>().operator->();

			double time = clock();


			_pRenderThreadsComponent->frameCount++;//每调用一次Get_FPS()函数，帧数自增1
			_pRenderThreadsComponent->currentTime = time*0.001f;//获取系统时间，其中timeGetTime函数返回的是以毫秒为单位的系统时间，所以需要乘以0.001，得到单位为秒的时间

																//如果当前时间减去持续时间大于了1秒钟，就进行一次FPS的计算和持续时间的更新，并将帧数值清零
			if (_pRenderThreadsComponent->currentTime - _pRenderThreadsComponent->ContinuedTime > _pRenderThreadsComponent->FPSTime) //将时间控制在1秒钟
			{
				_pRenderThreadsComponent->FPS = _pRenderThreadsComponent->frameCount / (_pRenderThreadsComponent->currentTime - _pRenderThreadsComponent->ContinuedTime);//计算这1秒钟的FPS值
				_pRenderThreadsComponent->ContinuedTime = _pRenderThreadsComponent->currentTime; //将当前时间currentTime赋给持续时间lastTime，作为下一秒的基准时间
				_pRenderThreadsComponent->frameCount = 0;//将本次帧数frameCount值清零
			}
		}
	}

	void EngineRenderStart(EntityCommon _Entity)
	{
		EntityX::ComponentHandle<Magic::System::RenderThreadsComponent> _RenderThreadsComponent = _Entity.GetComponent<Magic::System::RenderThreadsComponent>();
		_RenderThreadsComponent->m_DrawMessageNumber = 0;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		_RenderThreadsComponent->m_Pen_Normal.RenderStart();
		_RenderThreadsComponent->m_Pen_Normal.SetCameraMatrix(glm::mat4());
	}

	void EngineRenderEnd(EntityCommon _Entity)
	{
		EntityX::ComponentHandle<Magic::System::PosSizeComponent> _PosSizeComponent = _Entity.GetComponent<Magic::System::PosSizeComponent>();
		EntityX::ComponentHandle<Magic::System::RenderThreadsComponent> _RenderThreadsComponent = _Entity.GetComponent<Magic::System::RenderThreadsComponent>();

		glm::vec2 _WH = _PosSizeComponent->m_Size;
		_RenderThreadsComponent->m_Pen_Normal.SetDrawWH(_WH.x, _WH.y);
		_RenderThreadsComponent->m_Pen_Normal.RenderEnd();

		SwapBuffers(_RenderThreadsComponent->m_HDC);
	}


	const unsigned char* GetRenderer()
	{
		return glGetString(GL_RENDERER);
	}

	const unsigned char* GetVendor()
	{
		return glGetString(GL_VENDOR);
	}

	const unsigned char* GetVersion()
	{
		return glGetString(GL_VERSION);
	}

	const unsigned char* GetGlSLVersion()
	{
		return glGetString(GL_SHADING_LANGUAGE_VERSION);
	}

	int Getmajor()
	{
		GLint major;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		return major;
	}

	int Getminor()
	{
		GLint minor;
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		return minor;
	}
}

EntityCommon* MagicEngineContext::S_T_pEntityCommon = 0;
MagicEngineContext* MagicEngineContext::pMagicEngineContext = 0;

MagicEngineContext::MagicEngineContext()
{
	pMagicEngineContext = this;
}


MagicEngineContext::~MagicEngineContext()
{
	Shutdown();
	pMagicEngineContext = 0;
}

bool MagicEngineContext::Initialize()
{
	EntityCommon _MainEntity = m_Supervisor.m_entities.create();

	M_EntityThreads.insert(std::make_pair("MainThreads", _MainEntity));
	S_T_pEntityCommon = &(M_EntityThreads["MainThreads"]);

	EntityX::EntityX* _Supervisor = &_MainEntity.assign<Magic::System::ObjectSupervisor>()->m_Supervisor;
	EntityX::ComponentHandle<Magic::System::ThreadsComponent> _ThreadsComponent = _MainEntity.assign<Magic::System::ThreadsComponent>();
	_ThreadsComponent->m_RunState = true;

	_Supervisor->m_systems.add<Magic::System::ThreadsMessageForwardSystem>();
	_Supervisor->m_systems.add<Magic::System::ObjectUpdataSystem>();
	_Supervisor->m_systems.add<Magic::System::ThreadsRenderSystem>();
	_Supervisor->m_systems.configure();

	_MainEntity.assign<Magic::System::UpdataComponent>((Magic::System::Call_Entity)0);

	InitializeCriticalSection(&m_MutexThreadsResourceManager);

	return true;
}
void MagicEngineContext::Shutdown()
{
	DeleteCriticalSection(&m_MutexThreadsResourceManager);

	for (std::map<std::string, MagicTexture*>::iterator i = Map_Texture.begin(); i != Map_Texture.end(); i++)
		delete i->second;
}

void MagicEngineContext::Render(void)
{
	UpdataThread();
}


MagicTexture* MagicEngineContext::LoadTextrue(const char* file_name, const char* _name, char format)
{
	bool result;
	MagicTexture* pTextrue;
	pTextrue = new MagicTexture;
	if (!pTextrue)
		return 0;
	result = pTextrue->Initialize(file_name, format);
	if (!result)
		return 0;
	Map_Texture.insert(std::map<std::string, MagicTexture*>::value_type(_name, pTextrue));

	return pTextrue;
}

MagicTexture* MagicEngineContext::LoadTextrue(const unsigned char* Data, int _width, int _height, const char* _name)
{
	bool result;
	MagicTexture* pTextrue;
	pTextrue = new MagicTexture;
	if (!pTextrue)
		return 0;
	result = pTextrue->Initialize(Data, _width, _height, MagicTexture::RGBA, MagicTexture::UNSIGNED_BYTE);
	if (!result)
		return 0;
	Map_Texture.insert(std::map<std::string, MagicTexture*>::value_type(_name, pTextrue));

	return pTextrue;
}

bool MagicEngineContext::CreateThreadsResourceManager(const char* _name)
{
	EnterCriticalSection(&m_MutexThreadsResourceManager);

	EntityCommon _EntityCommon = m_Supervisor.m_entities.create();

	M_EntityThreads.insert(std::make_pair(_name, _EntityCommon));

	EntityX::EntityX* _Supervisor = &_EntityCommon.assign<Magic::System::ObjectSupervisor>()->m_Supervisor;

	_Supervisor->m_systems.add<Magic::System::ThreadsMessageForwardSystem>();
	_Supervisor->m_systems.add<Magic::System::ObjectUpdataSystem>();
	_Supervisor->m_systems.add<Magic::System::ThreadsRenderSystem>();
	_Supervisor->m_systems.configure();

	EntityX::ComponentHandle<Magic::System::ThreadsComponent> _ThreadsComponent = _EntityCommon.assign<Magic::System::ThreadsComponent>();
	_EntityCommon.assign<Magic::System::UpdataComponent>((Magic::System::Call_Entity)0);


	HANDLE _Threads = (HANDLE)CreateThread(NULL, 0, UpdataThread, (LPVOID)(&M_EntityThreads[_name]), 0, NULL);
	_ThreadsComponent->m_Threads = _Threads;
	_ThreadsComponent->m_RunState = true;
	ResumeThread(_Threads);

	LeaveCriticalSection(&m_MutexThreadsResourceManager);

	return true;
}

bool MagicEngineContext::CreateOpenglRender(HWND _hwnd, EntityCommon _ThreadResourceManager)
{
	if (!_ThreadResourceManager.valid())
	{
		Magic::SetEngineErrorMessage("1.Invalid module\n");
		return false;
	}
	else if (_ThreadResourceManager.has_component<Magic::System::RenderThreadsComponent>())
	{
		Magic::SetEngineErrorMessage("1.RenderThreadsComponent already exists\n");
		return false;
	}
	else
	{
		Magic::System::RenderThreadsComponent* _pRenderThreadsComponent = _ThreadResourceManager.assign<Magic::System::RenderThreadsComponent>().operator->();

		_pRenderThreadsComponent->m_hWnd = _hwnd;
		_pRenderThreadsComponent->m_HDC = GetDC(_pRenderThreadsComponent->m_hWnd);
		_pRenderThreadsComponent->m_hRC = Magic::CreateRCContxt(_pRenderThreadsComponent->m_HDC);
		if (!_pRenderThreadsComponent->m_hRC)
			return false;

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			fprintf(stderr, "Error:'%s'\n", glewGetErrorString(err));
			return false;
		}

		//启用背面剔除
		glEnable(GL_CULL_FACE);

		bool result = _pRenderThreadsComponent->m_Pen_Normal.Initialize();
		if (!result)
			return false;

		RECT _Rect;
		GetClientRect(_hwnd, &_Rect);

		::EntityX::ComponentHandle<Magic::System::PosSizeComponent> _PosSizeComponent =
			_ThreadResourceManager.assign<Magic::System::PosSizeComponent>(glm::vec2(0.0f, 0.0f), glm::vec2(_Rect.right, _Rect.bottom));
		_ThreadResourceManager.assign<Magic::System::RenderComponent>(EntityCommon(), Magic::EngineRenderStart,
			(Magic::System::Call_Entity)0, Magic::EngineRenderEnd);

		return true;
	}
}

void MagicEngineContext::DeleteTextrue(const char* _name)
{
	auto _auto = Map_Texture.find(_name);
	if (_auto != Map_Texture.end())
	{
		delete _auto->second;
		Map_Texture.erase(_auto);
	}

	return;
}

EntityCommon MagicEngineContext::GetThreadsResourceManager(const char* _name)
{
	EntityCommon _EntityCommon;
	EnterCriticalSection(&m_MutexThreadsResourceManager);
	auto _auto = M_EntityThreads.find(_name);
	if (_auto != M_EntityThreads.end())
		_EntityCommon = _auto->second;
	else
		_EntityCommon = EntityCommon();
	LeaveCriticalSection(&m_MutexThreadsResourceManager);

	return _EntityCommon;
}

EntityCommon MagicEngineContext::GetThreadsResourceManager()
{
	if (S_T_pEntityCommon)
		return *S_T_pEntityCommon;
	else
		return EntityCommon();
}

MagicTexture* MagicEngineContext::GetTextrue(const char* _name)
{
	auto _auto = Map_Texture.find(_name);
	if (_auto != Map_Texture.end())
		return _auto->second;
	else
		return 0;
}

DWORD WINAPI MagicEngineContext::UpdataThread(LPVOID lpParameter)
{
	MagicEngineContext::S_T_pEntityCommon = (EntityCommon*)lpParameter;

	pMagicEngineContext->UpdataThread();

	return 0;
}

void MagicEngineContext::UpdataThread()
{
	EntityCommon _EntityCommon = pMagicEngineContext->GetThreadsResourceManager();
	bool _RunState = false;

	do
	{
		EntityX::ComponentHandle<Magic::System::ObjectSupervisor> _ObjectSupervisor = _EntityCommon.GetComponent<Magic::System::ObjectSupervisor>();
		EntityX::ComponentHandle<Magic::System::ThreadsComponent> _ThreadsComponent = _EntityCommon.GetComponent<Magic::System::ThreadsComponent>();

		double _time = Magic::EngineUpdataTime(_EntityCommon);
		Magic::EngineUpdataFPS(_EntityCommon);

		_ObjectSupervisor->m_Supervisor.m_systems.update_all(_EntityCommon, _time);

		_RunState = _ThreadsComponent->m_RunState;
	} while (_RunState);
}