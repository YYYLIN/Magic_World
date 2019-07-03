#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform2.hpp>
#include <map>
#include <Vector>
#include <list>
#include "windows.h"
#include "Render/MagicTexture.h"
#include "Render/MagicPen.h"
#include "Define/MagicType.h"
#include "Magic_Thread.h"
#include "Render/Render_thread.h"
#include "Render/DrawSimpleGraphics.h"
#include "Render/MainTemplateEffects.h"

#include "Render/Effects/FuzzyEffect.h"

#pragma warning(push)
#pragma warning(disable:4251)

extern const glm::mat4 CONST_CAMERA;

class MagicEngineContext;

namespace Magic
{
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

	HGLRC CreateRCContxt(HDC _hdc);

	void ShutdownRC(HGLRC _hRC);

	bool CreateHD(HDC _hdc);
}

class MagicEngineContext
{
public:
	MagicEngineContext();
	~MagicEngineContext();

	static MagicEngineContext* Instance() { return pMagicEngineContext; }


	bool Initialize(RenderContext _pRenderContext);
	//_Message = 0 为正常退出
	void ShutdownMessage(unsigned int _Message, const char* _Text);
	void Shutdown();

	unsigned int Run(void);

	void LoadThread(const Magic::Management::Callback_Message& _Callback_Message);

	MagicTexture* LoadTextrue(const char* file_name, const char* _name, char format);
	MagicTexture* LoadTextrue(const unsigned char* Data, int _width, int _height, const char* _name);

	void DeleteTextrue(const char* _name);

	MagicTexture* GetTextrue(const char* _name);

private:
	bool m_EngineRunState;
	unsigned int m_Error;

	std::map<std::string, MagicTexture*> Map_Texture;

	Magic::Render_thread m_Render_thread;
	Magic::Management::THREAD_OBJECT m_Load_Thread;
	DrawSimpleGraphics m_DrawSimpleGraphics;

	static MagicEngineContext* pMagicEngineContext;
};

#pragma warning(pop)