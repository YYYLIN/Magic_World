#include "MagicUIAPI.h"
#include "MagicWindows.h"


namespace Magic
{
	static MagicWindows* pMagicWindows = 0;

	bool CreateSystemUI(const char* _name, int _x, int _y, int _w, int _h)
	{
		if (pMagicWindows)
			return false;
		pMagicWindows = new MagicWindows;

		return pMagicWindows->Initialize(_name, _x, _y, _w, _h);
	}

	void ShutdownSystemUI()
	{
		if (pMagicWindows)
		{
			delete pMagicWindows;
			pMagicWindows = 0;
		}
	}

	void RunSystem()
	{
		pMagicWindows->Run();
	}
}