#ifndef _MAGICENGINEAPI_H_
#define _MAGICENGINEAPI_H_

#include "Define/Magic_Macro.h"
#include "MagicScene.h"
#include "MagicPen.h"
#include "windows.h"

namespace Magic
{
	DLL_MAGIC_ENGINE_OUTPUT_INPUT const char* GetEngineErrorMessage();

	DLL_MAGIC_ENGINE_OUTPUT_INPUT void SetEngineErrorMessage(const char* _text);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT void AddEngineErrorMessage(const char* _text, bool _EndORStart = true);

	/*
	功能：
		创建引擎完成初始操作
	参数：
		空
	返回值：
		bool = true 成功 | false 失败
		详细错误请调用GetEngineErrorMessage查看
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT bool CreateEngine();

	/*
	功能：
		运行引擎
	参数：
		空
	返回值：
		空
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT void RunEngine();

	/*
	功能：
		关闭引擎
	参数：
		空
	返回值：
		空
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT void ShutdownEngine();
	
	/*
	功能：
		创建一个新的线程并添加一个管理员对象
	参数：
		_name = 线程资源管理员名字
	返回值：
		bool = true 成功 | false 失败
		详细错误请调用GetEngineErrorMessage查看
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT bool CreateThreadsResourceManager(const char* _name);

	/*
	功能：
		关闭并释放当前线程资源管理员对象
	参数：
		空
	返回值：
		空
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT void ShutdownThreads();

	/*
	功能：
		在指定windows窗口句柄和指定线程资源管理员对象中创建一个Opengl渲染模块
	参数：
		_hwnd = windows窗口句柄
		_ThreadResourceManager = 线程资源管理员对象
	返回值:
		bool = true 成功 | false 失败
		详细错误请调用GetEngineErrorMessage查看
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT bool CreateOpenglRender(HWND _hwnd, EntityCommon _ThreadResourceManager);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT void AddDrawMessageNumber(unsigned int _number);

	/*
	功能：
		获取当前线程中的画笔对象
	警告：
		必须在当前线程创建渲染对象
	参数：
		空
	返回值：
		Magic::Pen_Normal* = 画笔的指针
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT Magic::Pen_Normal* GetPen_Normal();

	/*
	功能：
		获取指定线程中的线程资源管理员对象
	参数：
		_name = 线程名字
	返回值：
		EntityCommon = 线程资源对象
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT EntityCommon GetThreadsResourceManager(const char* _name);

	/*
	功能：
		获取当前线程中的线程资源管理员对象
	参数：
		空
	返回值：
		EntityCommon = 线程资源对象
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT EntityCommon GetThreadsResourceManager();

	/*
	功能：
		获取当前线程中的差值时间
	参数：
		空
	返回值：
		double = 以1毫秒为1.0单位的时间差值
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT double GetThreadsDiffTime();

#ifdef _DEBUG 
#define DEBUG_AddDrawMessageNumber(a)			AddDrawMessageNumber(a) 
#else
#define DEBUG_AddDrawMessageNumber(a)
#endif

	template <typename T>
	bool CreateScene(EntityCommon _ParentEntity, T** _SceneCommon)
	{
		bool _AutoRelase = false;
		if (!*_SceneCommon)
		{
			*_SceneCommon = new T;
			_AutoRelase = true;
		}

		bool _result = (*_SceneCommon)->SceneCommon::Initialize(_ParentEntity, *_SceneCommon, _AutoRelase);
		if (!_result)
			return false;

		return true;
	}
}

#endif