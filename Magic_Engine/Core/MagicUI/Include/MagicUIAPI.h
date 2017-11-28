#ifndef _MAGICENGINEAPI_H_
#define _MAGICENGINEAPI_H_


#include "MagicEngineContext.h"


namespace Magic
{
	bool CreateSystemUI(const char* _name, int _x, int _y, int _w, int _h);

	void ShutdownSystemUI();

	void RunSystem();

	MagicScene* GetSystemScenes();
}


#endif