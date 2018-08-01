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

	DLL_MAGIC_ENGINE_OUTPUT_INPUT bool CreateEngine();

	DLL_MAGIC_ENGINE_OUTPUT_INPUT void RunEngine();

	DLL_MAGIC_ENGINE_OUTPUT_INPUT void ShutdownEngine();

	DLL_MAGIC_ENGINE_OUTPUT_INPUT bool CreateEntityThreads(const char* _name);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT void ShutdownThreads();

	DLL_MAGIC_ENGINE_OUTPUT_INPUT bool CreateOpenglRender(HWND _hwnd, EntityCommon _EntityCommon);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT void AddDrawMessageNumber(unsigned int _number);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT Magic::Pen_Normal* GetPen_Normal();

	DLL_MAGIC_ENGINE_OUTPUT_INPUT EntityCommon GetEntityThreads(const char* _name);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT EntityCommon GetEntityThreads();

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