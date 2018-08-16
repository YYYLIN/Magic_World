
#include "MagicEngineAPI.h"
#include "string"
#include "System/Supervisor.h"
#include "MagicEngineContext.h"

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

	bool CreateEngine()
	{
		if (MagicEngineContext::Instance())
		{
			SetEngineErrorMessage("1.Already created\n");
			return false;
		}

		new MagicEngineContext;

		bool _result = MagicEngineContext::Instance()->Initialize();
		if (!_result)
			return false;

		return true;
	}

	void RunEngine()
	{
		MagicEngineContext::Instance()->Render();
	}

	void ShutdownEngine()
	{
		if (MagicEngineContext::Instance())
		{
			delete MagicEngineContext::Instance();
		}
	}

	bool CreateThreadsResourceManager(const char* _name)
	{
		return MagicEngineContext::Instance()->CreateThreadsResourceManager(_name);
	}

	void ShutdownThreads()
	{
		MagicEngineContext::Instance()->GetThreadsResourceManager().GetComponent<Magic::System::ThreadsComponent>()->m_RunState = false;
	}

	bool CreateOpenglRender(HWND _hwnd, EntityCommon _ThreadResourceManager)
	{
		return MagicEngineContext::Instance()->CreateOpenglRender(_hwnd, _ThreadResourceManager);
	}

	void AddDrawMessageNumber(unsigned int _number)
	{
		MagicEngineContext::Instance()->GetThreadsResourceManager().GetComponent<Magic::System::RenderThreadsComponent>()->m_DrawMessageNumber += _number;
	}

	Magic::Pen_Normal* GetPen_Normal()
	{
		return &(MagicEngineContext::Instance()->GetThreadsResourceManager().GetComponent<Magic::System::RenderThreadsComponent>()->m_Pen_Normal);
	}

	EntityCommon GetThreadsResourceManager(const char* _name)
	{
		return MagicEngineContext::Instance()->GetThreadsResourceManager(_name);
	}

	EntityCommon GetThreadsResourceManager()
	{
		return MagicEngineContext::Instance()->GetThreadsResourceManager();
	}

	void SendMessageToScene(const char* _SceneName, Magic::System::MessageStruct _MessageStruct)
	{

	}

	void SendMessageToScene(const EntityCommon& _SceneEntity, Magic::System::MessageStruct _MessageStruct)
	{

	}

	void SendMessageToThreads(const char* _ThreadsName, Magic::System::MessageStruct _MessageStruct)
	{

	}

	void SendMessageToThreads(const EntityCommon& _ThreadsEntity, Magic::System::MessageStruct _MessageStruct)
	{

	}

	void SendMessageToThreadsScene(const char* _SceneName, const char* _ThreadsName, Magic::System::MessageStruct _MessageStruct)
	{

	}

	void SendMessageToThreadsScene(const EntityCommon& _ThreadsEntity, const EntityCommon& _SceneEntity, Magic::System::MessageStruct _MessageStruct)
	{

	}

	double GetThreadsDiffTime()
	{
		return MagicEngineContext::Instance()->GetThreadsResourceManager().GetComponent<Magic::System::ThreadsComponent>()->m_DiffTime;
	}
}