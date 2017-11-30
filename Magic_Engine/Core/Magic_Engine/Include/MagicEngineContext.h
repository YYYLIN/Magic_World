#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform2.hpp>
#include <map>
#include <Vector>
#include <list>
#include "windows.h"
#include "MagicTexture.h"
#include "MagicPen.h"

#pragma warning(push)
#pragma warning(disable:4251)

extern const glm::mat4 CONST_CAMERA;

class MagicScene;
class MagicEngineContext;

class DLL_MAGIC_ENGINE_OUTPUT_INPUT MagicCommon
{
	friend MagicScene;
	friend MagicEngineContext;
public:
	MagicCommon() = default;
	virtual ~MagicCommon()
	{}
	virtual void Updata() { this->OnUpdata(); }
	virtual void Render(glm::vec2 _DrawPos) { this->Draw(); }
protected:
	inline virtual void OnUpdata() {}
	virtual	void Draw() {};
};

class DLL_MAGIC_ENGINE_OUTPUT_INPUT MagicUICommon :public MagicCommon
{
public:
	MagicUICommon() = default;
	virtual ~MagicUICommon() {}

	virtual void DrawSpirit() {}
};

class DLL_MAGIC_ENGINE_OUTPUT_INPUT MagicScene :public MagicUICommon
{
public:
	MagicScene();
	virtual ~MagicScene();
	bool Initialize(MagicScene* _scene, glm::vec4 _PosSize);

	virtual void SetDisplayState(bool _state);

	virtual glm::vec2 GetFrameBufferSize();

	inline bool GetDisplayState() { return DisplayState; }

	inline glm::vec2 GetDrawPos() { return m_DrawPos; }

	virtual unsigned int GetFBOTextrue() { return 0; }

	void AddCommon(MagicCommon*);
	void RemoveCommon(MagicCommon*);

	virtual void ResetDrawRECT(float _x, float _y, float _w, float _h);

protected:
	virtual bool OnInitialize() { return true; }
	virtual void Updata();
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


class DLL_MAGIC_ENGINE_OUTPUT_INPUT MagicSceneEx :public MagicScene
{
public:
	MagicSceneEx();

	virtual void DrawSpirit();
	virtual void SetDisplayState(bool);

	virtual glm::vec2 GetFrameBufferSize();

	inline virtual unsigned int GetFBOTextrue() { return m_FBOBuffer.GetFBOTextrue(); }
	inline virtual unsigned int GetTextrue() { return m_FBOBuffer.GetTextrue(); }
protected:
	virtual bool Initialize(MagicScene* _scene, glm::vec4 _PosSize);
	virtual void Render(glm::vec2 _DrawPos);
	virtual void RenderBuffer();

	virtual void RenderStart();
	virtual void RenderEnd();

protected:
	bool m_DrawMessage;

	MagicFBOTextrue m_FBOBuffer;
};

HGLRC CreateRCContxt(HDC _hdc);

class DLL_MAGIC_ENGINE_OUTPUT_INPUT MagicEngineContext :public MagicScene
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
	const unsigned char* GetRenderer();
	//获得图形卡供应商
	const unsigned char* GetVendor();
	//版本号
	const unsigned char* GetVersion();
	//渲染器版本号
	const unsigned char* GetGlSLVersion();
	//大版本号
	int Getmajor();
	//小版本号
	int Getminor();
	MagicTexture* GetTextrue(const char* _name);

	Magic::Pen_Common* GetPen(const char* _name);

	virtual glm::vec2 GetFrameBufferSize();

	inline Magic::Pen_Normal* GetPen_Normal() { return &m_Pen_Normal; }

	double GetDiffTime() { return diffTime; }

protected:
	void OnUpdata();

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

#pragma warning(pop)