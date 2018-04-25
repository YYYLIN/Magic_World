#ifndef _DEBUGSCENE_H_
#define _DEBUGSCENE_H_


#include "MagicScene.h"

namespace Magic
{
	namespace UI
	{

		class DebugScene :
			public SceneCommon
		{
		public:
			DebugScene();
			~DebugScene();

			virtual bool OnInitialize();
		};
	}
}

#endif