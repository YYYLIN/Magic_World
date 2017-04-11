//头文件声明:
#include "MagicWindows.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	MagicWindows* pMagicWindows = 0;

	pMagicWindows = new MagicWindows;

	glm::mat4 a(1.0f);

	a = a * a;

	if (!pMagicWindows->Initialize())
		return false;

	pMagicWindows->Run();

	if (pMagicWindows)
	{
		delete pMagicWindows;
		pMagicWindows = 0;
	}

	return 0;
}