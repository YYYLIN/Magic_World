#include "MagicEngineContext.h"
#include "System/Supervisor.h"

#include <GL/glew.h>  

#include <time.h>

const glm::mat4 CONST_CAMERA = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));


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
}

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
}


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

MagicEngineContext* MagicEngineContext::pMagicEngineContext = 0;

MagicEngineContext::MagicEngineContext()
{
	m_DrawMessageNumber = 0;
}


MagicEngineContext::~MagicEngineContext()
{
}


bool MagicEngineContext::Initialize(HWND _hwnd, float _x, float _y, float _w, float _h)
{
	bool result;

	pMagicEngineContext = this;
	m_hWnd = _hwnd;
	m_HDC = GetDC(m_hWnd);
	m_hRC = CreateRCContxt(m_HDC);
	if (!m_hRC)
		return false;

	result = MagicScenes::Initialize(0, glm::vec4(_x, _y, _w, _h));
	if (!result)
		return false;

	m_BackColor.R = 0.0f; m_BackColor.G = 0.0f; m_BackColor.B = 0.0f;
	glClearColor(m_BackColor.R, m_BackColor.B, m_BackColor.G, m_BackColor.A);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error:'%s'\n", glewGetErrorString(err));
		return false;
	}

	//启用背面剔除
	glEnable(GL_CULL_FACE);

	result = m_Pen_Normal.Initialize();
	if (!result)
		return false;

	Magic::System::Supervisor _Supervisor;

	_Supervisor.Initialize();

	return true;
}
void MagicEngineContext::Shutdown()
{
	for (std::map<std::string, MagicTexture*>::iterator i = Map_Texture.begin(); i != Map_Texture.end(); i++)
		delete i->second;
}

void MagicEngineContext::Render(void)
{
	this->Updata();
	MagicScenes::Render(glm::vec2());
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

void MagicEngineContext::SetBackColor(float r, float g, float b, float a)
{
	m_BackColor.R = r;
	m_BackColor.G = g;
	m_BackColor.B = b;
	m_BackColor.A = a;
	glClearColor(r, g, b, a);
}

void MagicEngineContext::ResetDrawRECT(float _x, float _y, float _w, float _h)
{
	MagicScenes::ResetDrawRECT(_x, _y, _w, _h);
	glViewport((int)_x, (int)_y, (int)_w, (int)_h); //设置视频口
}

bool MagicEngineContext::AddPen_Common(const char* _name, Magic::Pen_Common* _common)
{
	auto _auto = Map_Pen_Common.insert(std::map<std::string, Magic::Pen_Common*>::value_type(_name, _common));
	if (_auto.second)
		return true;
	else
		return false;
}

void MagicEngineContext::AddDrawMessageNumber(unsigned int _number)
{
	m_DrawMessageNumber += _number;
}

const unsigned char* MagicEngineContext::GetRenderer()
{
	return glGetString(GL_RENDERER);
}

const unsigned char* MagicEngineContext::GetVendor()
{
	return glGetString(GL_VENDOR);
}

const unsigned char* MagicEngineContext::GetVersion()
{
	return glGetString(GL_VERSION);
}

const unsigned char* MagicEngineContext::GetGlSLVersion()
{
	return glGetString(GL_SHADING_LANGUAGE_VERSION);
}

int MagicEngineContext::Getmajor()
{
	GLint major;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	return major;
}

int MagicEngineContext::Getminor()
{
	GLint minor;
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	return minor;
}

MagicTexture* MagicEngineContext::GetTextrue(const char* _name)
{
	auto _auto = Map_Texture.find(_name);
	if (_auto != Map_Texture.end())
		return _auto->second;
	else
		return 0;
}

Magic::Pen_Common* MagicEngineContext::GetPen(const char* _name)
{
	auto _auto = Map_Pen_Common.find(_name);
	if (_auto != Map_Pen_Common.end())
		return _auto->second;
	else
		return 0;
}

glm::vec2 MagicEngineContext::GetFrameBufferSize()
{
	return glm::vec2(m_PosSize.z, m_PosSize.w);
}

void MagicEngineContext::OnUpdata()
{
	double time = clock();
	diffTime = time - lastTime;
	lastTime = clock();

	SetFPS();
}

void MagicEngineContext::SetFPS()
{
	static int    frameCount = 0;//帧数
	static float  currentTime = 0.0f;//当前时间
	static float  lastTime = 0.0f;//持续时间

	frameCount++;//每调用一次Get_FPS()函数，帧数自增1
	currentTime = clock()*0.001f;//获取系统时间，其中timeGetTime函数返回的是以毫秒为单位的系统时间，所以需要乘以0.001，得到单位为秒的时间

	//如果当前时间减去持续时间大于了1秒钟，就进行一次FPS的计算和持续时间的更新，并将帧数值清零
	if (currentTime - lastTime > FPSTime) //将时间控制在1秒钟
	{
		FPS = (float)frameCount / (currentTime - lastTime);//计算这1秒钟的FPS值
		lastTime = currentTime; //将当前时间currentTime赋给持续时间lastTime，作为下一秒的基准时间
		frameCount = 0;//将本次帧数frameCount值清零
	}
}

void MagicEngineContext::RenderStart()
{
	m_DrawMessageNumber = 0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_Pen_Normal.RenderStart();
	m_Pen_Normal.SetCameraMatrix(CONST_CAMERA);
}

void MagicEngineContext::RenderEnd()
{
	glm::vec2 _WH = this->GetFrameBufferSize();
	m_Pen_Normal.SetDrawWH(_WH.x, _WH.y);
	m_Pen_Normal.RenderEnd();

	SwapBuffers(m_HDC);
}