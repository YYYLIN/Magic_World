#pragma once

#pragma comment(lib,"glew32.lib")
#include <GL/glew.h>  
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform2.hpp>
#include <map>
#include <Vector>
#include <list>
#include "windows.h"
#include "MagicTexture.h"
#include "MagicPen.h"

extern const glm::mat4 CONST_CAMERA;


class MagicScene;
class MagicEngineContext;

class MagicCommon
{
	friend MagicScene;
	friend MagicEngineContext;
public:
	MagicCommon::MagicCommon() {}
	virtual MagicCommon::~MagicCommon()
	{}

	virtual void DrawSpirit() {}

protected:
	inline virtual void Updata() { return; }
	virtual	void Render(glm::mat4 CameraMatrix) = 0;
};

class MagicSpirit :public MagicCommon
{
public:
	MagicSpirit();
	virtual ~MagicSpirit();

	bool Initialize(MagicScene* _scene);
	bool Initialize(MagicEngineContext* _engine);

	void SetDisplayState(bool _state);

	void DrawSpirit();
protected:
	virtual bool Initialize();
	virtual void Render(glm::mat4 CameraMatrix);
protected:
	bool DisplayState;
	MagicScene* pUpperScene;
};


class MagicScene :public MagicCommon
{
public:
	MagicScene();
	virtual ~MagicScene();
	bool Initialize(glm::vec4 _PosSize, MagicScene* _scene);
	bool Initialize(glm::vec4 _PosSize, MagicEngineContext* _engine);

	virtual void SetDisplayState(bool _state);

	inline bool GetDisplayState() { return DisplayState; }

	void SpiritAdd(MagicCommon*);
	void DeleteSpirit(MagicCommon*);

	virtual GLuint GetFBOTextrue() { return 0; }

protected:
	virtual bool Initialize(glm::vec4 _PosSize);
	virtual void Updata();
	virtual	void Render(glm::mat4 CameraMatrix);

protected:
	bool DisplayState;

	glm::vec4 m_PosSize;
	MagicScene* pUpperScene;

	vector<MagicCommon*> v_Common;
};

HGLRC CreateRCContxt(HDC _hdc);

class MagicEngineContext
{
public:
	MagicEngineContext();
	~MagicEngineContext();

	bool Initialize(HWND _hwnd);
	void Shutdown();

	void Render(void);

	MagicTexture* LoadTextrue(const char* file_name, string name, char format);
	MagicTexture* LoadTextrue(const unsigned char* Data, int _width, int _height, string name);
	void DeleteTextrue(string);

	//�����Ⱦ��
	const GLubyte* GetRenderer();
	//���ͼ�ο���Ӧ��
	const GLubyte* GetVendor();
	//�汾��
	const GLubyte* GetVersion();
	//��Ⱦ���汾��
	const GLubyte* GetGlSLVersion();
	//��汾��
	GLint Getmajor();
	//С�汾��
	GLint Getminor();
	MagicTexture* GetTextrue(string _name);

	MagicPen* GetPen() { return &m_MagicPen; }

	void SetBackColor(float, float, float, float);

	void SpiritAdd(MagicCommon*);
	void DeleteSpirit(MagicCommon*);

	float GetDiffTime() { return diffTime; }

protected:
	void Updata();

private:
	void SetFPS();

private:
	Magic::Color4 m_BackColor;

	float FPS, FPSTime;
	double diffTime, lastTime;

	vector<MagicCommon*> v_Common;
	map<string, MagicTexture*> Map_Texture;

	MagicPen m_MagicPen;

	//Windows
	HGLRC m_hRC;
	HDC m_HDC;
	HWND m_hWnd;

public:
	static MagicEngineContext* pMagicEngineContext;
};

