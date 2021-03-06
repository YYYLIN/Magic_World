#ifndef _MAGICMENU_H_
#define _MAGICMENU_H_

#include "Render/MagicScene.h"

namespace Magic
{
	namespace UI
	{
		class MenuScene :
			public SceneCommon
		{
		public:
			MenuScene();
			~MenuScene();

			virtual bool OnInitialize();
		};
	}
}

#endif