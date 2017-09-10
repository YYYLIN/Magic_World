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
	MagicCommon() = default;
	virtual ~MagicCommon()
	{}

protected:
	inline virtual void Updata() { return; }
	virtual	void Render(glm::mat4 CameraMatrix) = 0;
};

class MagicUICommon :public MagicCommon
{
public:
	MagicUICommon() = default;
	virtual ~MagicUICommon() {}

	virtual void DrawSpirit() {}
};

class MagicScene :public MagicUICommon
{
public:
	MagicScene();
	virtual ~MagicScene();
	bool Initialize(glm::vec4 _PosSize, MagicScene* _scene);
	bool Initialize(glm::vec4 _PosSize, MagicEngineContext* _engine);

	virtual void SetDisplayState(bool _state);

	inline bool GetDisplayState() { return DisplayState; }

	virtual GLuint GetFBOTextrue() { return 0; }

	void AddCommon(MagicCommon*);
	void RemoveCommon(MagicCommon*);

	virtual void ResetDrawRECT(float _x, float _y, float _w, float _h);

protected:
	virtual bool Initialize(glm::vec4 _PosSize);
	virtual void Updata();
	virtual	void Render(glm::mat4 CameraMatrix);
	virtual void RenderStart();
	virtual void RenderEnd();

protected:
	bool DisplayState;

	glm::vec4 m_PosSize;
	MagicScene* pUpperScene;

	std::vector<MagicCommon*> v_Common;
};

HGLRC CreateRCContxt(HDC _hdc);

class MagicEngineContext :public MagicScene
{
public:
	MagicEngineContext();
	~MagicEngineContext();

	bool Initialize(HWND _hwnd, float _x, float _y, float _w, float _h);
	void Shutdown();

	void Render(void);

	MagicTexture* LoadTextrue(const char* file_name, const char* _name, char format);
	MagicTexture* LoadTextrue(const unsigned char* Data, int _width, int _height, const char* _name);
	void DeleteTextrue(const char* _name);

	void SetBackColor(float, float, float, float);

	virtual void ResetDrawRECT(float _x, float _y, float _w, float _h);

	bool AddPen_Common(const char* _name, Magic::Pen_Common* _common);

	//获得渲染器
	const GLubyte* GetRenderer();
	//获得图形卡供应商
	const GLubyte* GetVendor();
	//版本号
	const GLubyte* GetVersion();
	//渲染器版本号
	const GLubyte* GetGlSLVersion();
	//大版本号
	GLint Getmajor();
	//小版本号
	GLint Getminor();
	MagicTexture* GetTextrue(const char* _name);

	Magic::Pen_Common* GetPen(const char* _name);

	float GetDiffTime() { return diffTime; }

protected:
	void Updata();

private:
	void SetFPS();
	virtual void RenderStart();
	virtual void RenderEnd();

private:
	Magic::Color4 m_BackColor;

	float FPS, FPSTime;
	double diffTime, lastTime;

	std::map<std::string, Magic::Pen_Common*> Map_Pen_Common;
	std::map<std::string, MagicTexture*> Map_Texture;

	Magic::Pen_Basis m_Pen_Basis;

	//Windows
	HGLRC m_hRC;
	HDC m_HDC;
	HWND m_hWnd;

public:
	static MagicEngineContext* pMagicEngineContext;
};

