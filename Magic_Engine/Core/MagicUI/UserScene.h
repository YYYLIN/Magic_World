#ifndef _USERSCENE_H_
#define _USERSCENE_H_

#include "Render/MagicScene.h"

namespace Magic
{
	namespace UI
	{
		class UserScene :
			public SceneCommon
		{
		public:
			UserScene();
			~UserScene();

			virtual bool OnInitialize();
		};
	}
}

#endif