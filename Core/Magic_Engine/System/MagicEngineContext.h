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

	/*
	*返回空指针创建失败，已经存在相同名字的字体
	*/
	//Render线程
	Magic::LRU_Font_Texture* CreateFont_Texture(const char* _Name);
	Magic::LRU_Font_Texture* GetFont_Texture(const char* _Name);
	void DeleteFont_Texture(const char* _Name);

	Magic::FT_Font* CreateFT_Font(const char* _Name);
	Magic::FT_Font* GetFT_Font(const char* _Name);
	void DeleteFT_Font(const char* _Name);

private:
	bool m_EngineRunState;
	unsigned int m_Error;
	//临时内存管理，以后移植
	std::map<std::string, MagicTexture*> Map_Texture;
	std::map<std::string, Magic::LRU_Font_Texture> m_map_Font_Textrue;
	std::map<std::string, Magic::FT_Font> m_map_FT_Font;

	Magic::Render_thread m_Render_thread;
	Magic::Management::THREAD_OBJECT m_Load_Thread;
	DrawSimpleGraphics m_DrawSimpleGraphics;

	static MagicEngineContext* pMagicEngineContext;
};

#pragma warning(pop)