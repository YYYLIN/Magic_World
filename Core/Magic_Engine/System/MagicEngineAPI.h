#ifndef _MAGICENGINEAPI_H_
#define _MAGICENGINEAPI_H_

#include "Define/Magic_Macro.h"
#include "Define/MagicType.h"
#include "Render/MagicPen.h"
#include "windows.h"
#include "Magic_Thread.h"

namespace Magic
{
	DLL_MAGIC_ENGINE_OUTPUT_INPUT const char* GetEngineErrorMessage();

	DLL_MAGIC_ENGINE_OUTPUT_INPUT void SetEngineErrorMessage(const char* _text);

	//DLL_MAGIC_ENGINE_OUTPUT_INPUT void AddEngineErrorMessage(const char* _text, bool _EndORStart = true);

	/*
	*功能：
	*	创建引擎完成初始操作
	*参数：
	*	空
	*返回值：
	*	bool = true 成功 | false 失败
	*	详细错误请调用GetEngineErrorMessage查看
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT bool CreateEngine(RenderContext _pRenderContext);

	/*
	*功能：
	*	运行引擎
	*参数：
	*	空
	*返回值：
	*	空
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT unsigned int RunEngine();

	/*
	*功能：
	*	关闭引擎
	*参数：
	*	_Message = 0 为正常退出
	*返回值：
	*	空
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT void ShutdownEngine(unsigned int _Message, const char* _Text);
	DLL_MAGIC_ENGINE_OUTPUT_INPUT void ShutdownEngine(unsigned int _Message);

	/*
	*功能：
	*	加载功能线程
	*参数：
	*	_Callback_Message = 处理函数
	*返回值：
	*	空
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT void Load(const Magic::Management::Callback_Message& _Callback_Message);


	DLL_MAGIC_ENGINE_OUTPUT_INPUT void Engine(const Magic::Management::Callback_Void& _Callback);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT void ShutdownMessage(const Magic::Management::Callback_Message& _Callback_Message);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT RenderContext CreateRenderContext(HWND _hwnd);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT void ShutdownRenderContext(RenderContext _RenderContext);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT void BindRenderContext(RenderContext _RenderContext);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT void SetRenderContextRect(const Screen_Rect& _Screen_Rect);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT const FONT CreateFONT(const char* _Name, const char* _Path, unsigned int _w, unsigned int _h);
	DLL_MAGIC_ENGINE_OUTPUT_INPUT const FONT GetFONT(const char* _Name);
	DLL_MAGIC_ENGINE_OUTPUT_INPUT void DeleteFONT(const char* _Name);

#ifdef _DEBUG 
#define DEBUG_AddDrawMessageNumber(a)			AddDrawMessageNumber(a) 
#else
#define DEBUG_AddDrawMessageNumber(a)
#endif
}

#endif