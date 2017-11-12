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
	virtual void OnUpdata() { this->Updata(); }
	virtual void Render(glm::vec2 _DrawPos) { this->Draw(); }
protected:
	inline virtual void Updata() {}
	virtual	void Draw() {};
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
	bool Initialize(MagicScene* _scene, glm::vec4 _PosSize);

	virtual void SetDisplayState(bool _state);

	virtual glm::vec2 GetFrameBufferSize();

	inline bool GetDisplayState() { return DisplayState; }

	inline glm::vec2 GetDrawPos() { return m_DrawPos; }

	virtual GLuint GetFBOTextrue() { return 0; }

	void AddCommon(MagicCommon*);
	void RemoveCommon(MagicCommon*);

	virtual void ResetDrawRECT(float _x, float _y, float _w, float _h);

protected:
	virtual bool Initialize(glm::vec4 _PosSize);
	virtual void OnUpdata();
	virtual	void Render(glm::vec2 _DrawPos);
	virtual void RenderStart();
	virtual void RenderEnd();

protected:
	bool DisplayState;

	glm::vec4 m_PosSize;
	glm::vec2 m_DrawPos;
	MagicScene* pParentScene;

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
	MagicTexture* GetTextrue(const char* _name);

	Magic::Pen_Common* GetPen(const char* _name);

	virtual glm::vec2 GetFrameBufferSize();

	inline Magic::Pen_Normal* GetPen_Normal() { return &m_Pen_Normal; }

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

	Magic::Pen_Normal m_Pen_Normal;

	//Windows
	HGLRC m_hRC;
	HDC m_HDC;
	HWND m_hWnd;

public:
	static MagicEngineContext* pMagicEngineContext;
};

