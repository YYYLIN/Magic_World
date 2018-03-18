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
#include "MagicMessage.h"

#pragma warning(push)
#pragma warning(disable:4251)

extern const glm::mat4 CONST_CAMERA;

class MagicScenes;
class MagicEngineContext;

class DLL_MAGIC_ENGINE_OUTPUT_INPUT MagicCommon
{
	friend MagicScenes;
	friend MagicEngineContext;
public:
	MagicCommon() = default;
	virtual ~MagicCommon()
	{}
	virtual void Updata() { this->OnUpdata(); }
	virtual void Render(glm::vec2 _DrawPos) { this->Draw(); }
protected:
	inline virtual void OnUpdata() {}
	virtual	void Draw() {}
};

class DLL_MAGIC_ENGINE_OUTPUT_INPUT MagicUICommon :public MagicCommon
{
public:
	MagicUICommon() = default;
	virtual ~MagicUICommon() {}

	virtual void DrawSpirit() {}
};

class DLL_MAGIC_ENGINE_OUTPUT_INPUT MagicScenes :public MagicUICommon
{
	friend class Magic::MessageScenes;
public:
	MagicScenes();
	virtual ~MagicScenes();
	bool Initialize(MagicScenes* _scene, glm::vec4 _PosSize);

	virtual void SetDisplayState(bool _state);

	virtual glm::vec2 GetFrameBufferSize();

	inline bool GetDisplayState() { return DisplayState; }

	inline glm::vec2 GetDrawPos() { return m_DrawPos; }

	virtual MagicFBOTexture* GetFBOTexture() { return 0; }

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
	MagicScenes* pParentScene;

	std::vector<MagicCommon*> v_Common;
};


class DLL_MAGIC_ENGINE_OUTPUT_INPUT MagicScenesEx :public MagicScenes
{
public:
	MagicScenesEx();

	virtual void DrawSpirit();
	virtual void SetDisplayState(bool);

	virtual glm::vec2 GetFrameBufferSize();

	inline virtual MagicFBOTexture* GetFBOTexture() { return &m_FBOBuffer; }

	virtual bool Initialize(MagicScenes* _scene, glm::vec4 _PosSize);

	virtual void CopyFBOBuffer();
protected:
	virtual void Render(glm::vec2 _DrawPos);
	virtual void RenderBuffer();

	virtual void RenderStart();
	virtual void RenderEnd();

protected:
	bool m_DrawMessage;

	MagicFBOTexture m_FBOBuffer;
};

HGLRC CreateRCContxt(HDC _hdc);

class DLL_MAGIC_ENGINE_OUTPUT_INPUT MagicEngineContext :public MagicScenes
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

	void AddDrawMessageNumber(unsigned int _number);

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

	unsigned int m_DrawMessageNumber;

	//Windows
	HGLRC m_hRC;
	HDC m_HDC;
	HWND m_hWnd;

public:
	static MagicEngineContext* pMagicEngineContext;
};

#ifdef _DEBUG 
#define DEBUG_AddDrawMessageNumber(a)			MagicEngineContext::pMagicEngineContext->AddDrawMessageNumber(a) 
#else
#define DEBUG_AddDrawMessageNumber(a)
#endif

#pragma warning(pop)