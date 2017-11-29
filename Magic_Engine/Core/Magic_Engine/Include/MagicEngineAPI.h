#ifndef _MAGICENGINEAPI_H_
#define _MAGICENGINEAPI_H_

#include "Magic_Macro.h"

namespace Magic
{
	DLL_MAGIC_ENGINE_OUTPUT_INPUT const char* GetEngineErrorMessage();

	DLL_MAGIC_ENGINE_OUTPUT_INPUT void SetEngineErrorMessage(const char* _text);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT void AddEngineErrorMessage(const char* _text, bool _EndORStart = true);
}

#endif