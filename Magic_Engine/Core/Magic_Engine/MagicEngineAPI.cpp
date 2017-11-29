
#include "MagicEngineAPI.h"
#include "string"

namespace Magic
{
	static std::string Overall_EngineErrorMessage;

	const char* GetEngineErrorMessage()
	{
		return Overall_EngineErrorMessage.c_str();
	}

	void SetEngineErrorMessage(const char* _text)
	{
		Overall_EngineErrorMessage = _text;
	}

	void AddEngineErrorMessage(const char* _text, bool _EndORStart)
	{
		if (_EndORStart)
			Overall_EngineErrorMessage += _text;
		else
			Overall_EngineErrorMessage.insert(0, _text);
	}

}