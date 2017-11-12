#include "MagicEngineContext.h"


const glm::mat4 CONST_CAMERA = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));


MagicScene::MagicScene()
{
	DisplayState = 0;
}

MagicScene::~MagicScene()
{
	v_Common.clear();
}

bool MagicScene::Initialize(MagicScene* _scene, glm::vec4 _PosSize)
{
	pParentScene = _scene;
	pParentScene->AddCommon(this);
	return this->Initialize(_PosSize);
}

void MagicScene::SetDisplayState(bool _state)
{
	DisplayState = _state;
}

glm::vec2 MagicScene::GetFrameBufferSize()
{
	return pParentScene->GetFrameBufferSize();
}

void MagicScene::AddCommon(MagicCommon* _common)
{
	v_Common.push_back(_common);
}

void MagicScene::RemoveCommon(MagicCommon* _common)
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

void MagicScene::ResetDrawRECT(float _x, float _y, float _w, float _h)
{
	m_PosSize.x = _x;
	m_PosSize.y = _y;
	m_PosSize.z = _w;
	m_PosSize.w = _h;
}

bool MagicScene::Initialize(glm::vec4 _PosSize)
{
	this->ResetDrawRECT(_PosSize.x, _PosSize.y, _PosSize.z, _PosSize.w);
	DisplayState = true;

	return true;
}

void MagicScene::OnUpdata()
{
	if (DisplayState)
	{
		this->Updata();
		for (unsigned int a = 0; a < v_Common.size(); a++)
			v_Common[a]->OnUpdata();
	}
}

void MagicScene::Render(glm::vec2 _DrawPos)
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

void MagicScene::RenderStart()
{
	glm::mat4 _Camera = CONST_CAMERA;
	_Camera[3].x = m_DrawPos.x;
	_Camera[3].y = m_DrawPos.y;
	MagicEngineContext::pMagicEngineContext->GetPen_Normal()->SetCameraMatrix(_Camera);
}

void MagicScene::RenderEnd()
{
	glm::mat4 _Camera = CONST_CAMERA;
	_Camera[3].x = pParentScene->GetDrawPos().x;
	_Camera[3].y = pParentScene->GetDrawPos().y;
	MagicEngineContext::pMagicEngineContext->GetPen_Normal()->SetCameraMatrix(_Camera);
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
	Map_Texture.clear();
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

	result = MagicScene::Initialize(glm::vec4(_x, _y, _w, _h));
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

	//���ñ����޳�
	glEnable(GL_CULL_FACE);

	result = m_Pen_Normal.Initialize();
	if (!result)
		return false;

	return true;
}
void MagicEngineContext::Shutdown()
{
	for (std::map<std::string, MagicTexture*>::iterator i = Map_Texture.begin(); i != Map_Texture.end(); i++)
		delete i->second;
}

void MagicEngineContext::Render(void)
{
	this->OnUpdata();
	MagicScene::Render(glm::vec2());
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
	result = pTextrue->Initialize(Data, _width, _height);
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
	MagicScene::ResetDrawRECT(_x, _y, _w, _h);
	glViewport(_x, _y, _w, _h); //������Ƶ��
}

bool MagicEngineContext::AddPen_Common(const char* _name, Magic::Pen_Common* _common)
{
	auto _auto = Map_Pen_Common.insert(std::map<std::string, Magic::Pen_Common*>::value_type(_name, _common));
	if (_auto.second)
		return true;
	else
		return false;
}


const GLubyte* MagicEngineContext::GetRenderer()
{
	return glGetString(GL_RENDERER);
}

const GLubyte* MagicEngineContext::GetVendor()
{
	return glGetString(GL_VENDOR);
}

const GLubyte* MagicEngineContext::GetVersion()
{
	return glGetString(GL_VERSION);
}

const GLubyte* MagicEngineContext::GetGlSLVersion()
{
	return glGetString(GL_SHADING_LANGUAGE_VERSION);
}

GLint MagicEngineContext::Getmajor()
{
	GLint major;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	return major;
}

GLint MagicEngineContext::Getminor()
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

void MagicEngineContext::Updata()
{
	double time = clock();
	diffTime = time - lastTime;
	lastTime = clock();

	SetFPS();
}

void MagicEngineContext::SetFPS()
{
	static int    frameCount = 0;//֡��
	static float  currentTime = 0.0f;//��ǰʱ��
	static float  lastTime = 0.0f;//����ʱ��

	frameCount++;//ÿ����һ��Get_FPS()������֡������1
	currentTime = clock()*0.001f;//��ȡϵͳʱ�䣬����timeGetTime�������ص����Ժ���Ϊ��λ��ϵͳʱ�䣬������Ҫ����0.001���õ���λΪ���ʱ��

								 //�����ǰʱ���ȥ����ʱ�������1���ӣ��ͽ���һ��FPS�ļ���ͳ���ʱ��ĸ��£�����֡��ֵ����
	if (currentTime - lastTime > FPSTime) //��ʱ�������1����
	{
		FPS = (float)frameCount / (currentTime - lastTime);//������1���ӵ�FPSֵ
		lastTime = currentTime; //����ǰʱ��currentTime��������ʱ��lastTime����Ϊ��һ��Ļ�׼ʱ��
		frameCount = 0;//������֡��frameCountֵ����
	}
}

void MagicEngineContext::RenderStart()
{
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