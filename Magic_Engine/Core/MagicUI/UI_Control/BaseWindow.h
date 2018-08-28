#pragma once

#include "MagicUIMacro.h"
#include "Render/MagicScene.h"


namespace Magic
{
	namespace UI
	{
		class DLL_MAGICUI_OUTPUT_INPUT BaseWindow :public SceneCommon
		{
		public:
			BaseWindow(const char* _name);

			virtual void SetDisplayState(bool _State);

			inline bool GetDisplayState() { return m_DisplayState; }

			virtual bool Initialize(const EntityCommon& _ParentEntity, bool _AutoRelease);
		protected:
			virtual int MessageHandle(const Magic::System::MessageStruct& _MessageStruct);
			virtual void Updata();
			virtual void Render();

			virtual int OnMessage(const Magic::System::MessageStruct& _MessageStruct);
			virtual void OnUpdata();
			virtual void Draw();

		private:
			bool m_DisplayState;
		};
	}
}