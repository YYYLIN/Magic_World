#ifndef _TOOLSCENE_H_
#define _TOOLSCENE_H_

#include "Render/MagicScene.h"

namespace Magic
{
	namespace UI
	{
		class SystemScene :
			public SceneCommon
		{
		public:
			SystemScene();
			~SystemScene();

			virtual bool OnInitialize();
		};

	}
}

#endif

