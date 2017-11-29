#ifndef _MAGICENGINEAPI_H_
#define _MAGICENGINEAPI_H_

namespace Magic
{
	const char* GetEngineErrorMessage();

	void SetEngineErrorMessage(const char* _text);

	void AddEngineErrorMessage(const char* _text, bool _EndORStart = true);
}

#endif