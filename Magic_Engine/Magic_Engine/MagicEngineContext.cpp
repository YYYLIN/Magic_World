#include "MagicEngineContext.h"


const glm::mat4 CONST_CAMERA = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));


MagicSpirit::MagicSpirit()
{

}
MagicSpirit::~MagicSpirit()
{

}

bool MagicSpirit::Initialize(MagicScene* _scene)
{
	pUpperScene = _scene;
	pUpperScene->SpiritAdd(this);
	return this->Initialize();
}

bool MagicSpirit::Initialize(MagicEngineContext* _engine)
{
	pUpperScene = 0;
	_engine->SpiritAdd(this);
	return this->Initialize();
}

void MagicSpirit::SetDisplayState(bool _state)
{
	if (DisplayState != _state)
	{
		DisplayState = _state;
		DrawSpirit();
	}
}

void MagicSpirit::DrawSpirit()
{
	if (pUpperScene)
		pUpperScene->DrawSpirit();
}

bool MagicSpirit::Initialize()
{
	DisplayState = true;
	return true;
}

void MagicSpirit::Render(glm::mat4 CameraMatrix)
{
}

MagicScene::MagicScene()
{

}

MagicScene::~MagicScene()
{
	v_Common.clear();
}

bool MagicScene::Initialize(glm::vec4 _PosSize, MagicScene* _scene)
{
	pUpperScene = _scene;
	pUpperScene->SpiritAdd(this);
	return this->Initialize(_PosSize);
}

bool MagicScene::Initialize(glm::vec4 _PosSize, MagicEngineContext* _engine)
{
	pUpperScene = 0;
	_engine->SpiritAdd(this);
	return this->Initialize(_PosSize);
}

void MagicScene::SetDisplayState(bool _state)
{
	DisplayState = _state;
}

void MagicScene::SpiritAdd(MagicUICommon* _common)
{
	v_Common.push_back(_common);
}

void MagicScene::DeleteSpirit(MagicUICommon* _common)
{
	if (v_Common.size())
	{
		for (unsigned int a = 0; a < v_Common.size(); a++)
		{
			if (_common == v_Common[a])
				v_Common.erase(v_Common.begin() + a);
		}
	}
}

bool MagicScene::Initialize(glm::vec4 _PosSize)
{
	m_PosSize = _PosSize;
	DisplayState = true;

	return true;
}

void MagicScene::Updata()
{
	if (DisplayState)
	{
		for (unsigned int a = 0; a < v_Common.size(); a++)
			v_Common[a]->Updata();
	}
}

void MagicScene::Render(glm::mat4 CameraMatrix)
{
	if (DisplayState)
	{
		CameraMatrix[3].x += m_PosSize.x;
		CameraMatrix[3].y += m_PosSize.y;
		for (unsigned int a = 0; a < v_Common.size(); a++)
			v_Common[a]->Render(CameraMatrix);
	}
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


bool MagicEngineContext::Initialize(HWND _hwnd)
{
	bool result;

	pMagicEngineContext = this;
	m_hWnd = _hwnd;
	m_HDC = GetDC(m_hWnd);
	m_hRC = CreateRCContxt(m_HDC);
	if (!m_hRC)
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

	result = m_MagicPen.Initialize();
	if (!result)
		return false;

	return true;
}
void MagicEngineContext::Shutdown()
{
	v_Common.clear();


	for (map<string, MagicTexture*>::iterator i = Map_Texture.begin(); i != Map_Texture.end(); /*i++*/)
	{
		delete Map_Texture[i->first];
		Map_Texture.erase(i);
		i++;
	}

}

void MagicEngineContext::Render(void)
{
	Updata();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (unsigned int a = 0; a < v_Common.size(); a++)
		v_Common[a]->Render(CONST_CAMERA);

	SwapBuffers(m_HDC);
}


MagicTexture* MagicEngineContext::LoadTextrue(const char* file_name, string name, char format)
{
	bool result;
	MagicTexture* pTextrue;
	pTextrue = new MagicTexture;
	if (!pTextrue)
		return 0;
	result = pTextrue->Initialize(file_name, format);
	if (!result)
		return 0;
	Map_Texture[name] = pTextrue;
	return pTextrue;
}

MagicTexture* MagicEngineContext::LoadTextrue(const unsigned char* Data, int _width, int _height, string name)
{
	bool result;
	MagicTexture* pTextrue;
	pTextrue = new MagicTexture;
	if (!pTextrue)
		return 0;
	result = pTextrue->Initialize(Data, _width, _height);
	if (!result)
		return 0;
	Map_Texture[name] = pTextrue;
	return pTextrue;
}

void MagicEngineContext::DeleteTextrue(string name)
{
	MagicTexture* pTextrue = Map_Texture[name];
	delete pTextrue;
	Map_Texture.erase(name);

	return;
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

MagicTexture* MagicEngineContext::GetTextrue(string _name)
{
	return Map_Texture[_name];
}

void MagicEngineContext::SetBackColor(float r, float g, float b, float a)
{
	m_BackColor.R = r;
	m_BackColor.G = g;
	m_BackColor.B = b;
	m_BackColor.A = a;
	glClearColor(r, g, b, a);
}

void MagicEngineContext::SpiritAdd(MagicCommon* _common)
{
	v_Common.push_back(_common);
}

void MagicEngineContext::DeleteSpirit(MagicCommon* _common)
{
	if (v_Common.size())
	{
		for (unsigned int a = 0; a < v_Common.size(); a++)
		{
			if (_common == v_Common[a])
				v_Common.erase(v_Common.begin() + a);
		}
	}
}

void MagicEngineContext::Updata()
{
	double time = clock();
	diffTime = time - lastTime;
	lastTime = clock();

	SetFPS();

	for (unsigned int a = 0; a < v_Common.size(); a++)
		v_Common[a]->Updata();
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