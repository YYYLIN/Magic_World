
#include "MagicEngineAPI.h"
#include "string"
#include "System/Supervisor.h"
#include "MagicEngineContext.h"
#include "MagicScene.h"
#include "Cross_Platform_Port.h"

namespace Magic
{
#define  ENGINE_ERROR_MESSAGE_BUFFER_SIZE 256
	static S_THREAD char Overall_EngineErrorMessage[ENGINE_ERROR_MESSAGE_BUFFER_SIZE] = { 0 };

	const char* GetEngineErrorMessage()
	{
		return Overall_EngineErrorMessage;
	}

	void SetEngineErrorMessage(const char* _text)
	{
		size_t _len = strlen(_text);
		memcpy(Overall_EngineErrorMessage, _text, _len > ENGINE_ERROR_MESSAGE_BUFFER_SIZE ? ENGINE_ERROR_MESSAGE_BUFFER_SIZE : _len);
	}

	bool CreateEngine(RenderContext _pRenderContext)
	{
		if (MagicEngineContext::Instance())
		{
			SetEngineErrorMessage("1.Already created\n");
			return false;
		}

		new MagicEngineContext;

		bool _result = MagicEngineContext::Instance()->Initialize(_pRenderContext);
		if (!_result)
			return false;

		return true;
	}

	void RunEngine()
	{
		MagicEngineContext::Instance()->Run();
	}

	void ShutdownEngine(Magic::Management::MESSAGE _Message, const char* _Text)
	{
		if (MagicEngineContext::Instance())
		{
			MagicEngineContext::Instance()->ShutdownMessage(_Message, _Text);
		}
	}

	void ShutdownEngine(Magic::Management::MESSAGE _Message) {
		ShutdownEngine(_Message, GetEngineErrorMessage());
	}

	void Load(const Magic::Management::Callback_Message& _Callback_Message) {
		MagicEngineContext::Instance()->LoadThread(_Callback_Message);
	}

	void Engine(const Magic::Management::Callback_Void& _Callback) {
		Magic::Management::MonitorThread(MAGIC_MAIN_THREAD_NAME, _Callback);
	}

	void ShutdownMessage(const Magic::Management::Callback_Message& _Callback_Message) {
		Magic::Management::MonitorThreadMessage(MAGIC_MAIN_THREAD_NAME, Magic::SHUTOWN_ENGINE, _Callback_Message);
	}

	RenderContext CreateRenderContext(HWND _hwnd) {
		Render_Context_Opengl* _pRender_Context = new Render_Context_Opengl;

		bool _result = _pRender_Context->CreateRenderContext(_hwnd);
		if (!_result) {
			delete _pRender_Context;
			return NULL;
		}

		return (Render_Context*)_pRender_Context;
	}

	void ShutdownRenderContext(RenderContext _RenderContext) {
		Render_thread::Instance()->ShutdownRC((Render_Context*)_RenderContext);
	}

	void BindRenderContext(RenderContext _RenderContext) {
		Render_thread::Instance()->BindRC((Render_Context*)_RenderContext);
	}

	void SetRenderContextRect(const Screen_Rect& _Screen_Rect) {
		Render_thread::Instance()->SetRect(_Screen_Rect);
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

	bool SendMessageToScene(const char* _SceneName, Magic::System::MessageStruct _MessageStruct)
	{
		EntityCommon _EntityCommon = MagicEngineContext::Instance()->GetThreadsResourceManager();
		if (_EntityCommon.has_component<Magic::System::ThreadsComponent>())
		{
			if (_SceneName)
			{
				Magic::SceneCommon* _pSceneCommon = GetSceneCommon(_SceneName);
				if (_pSceneCommon)
				{
					//线程安全--------Start
					Magic::System::ThreadsComponent* _pThreadsComponent = _EntityCommon.GetComponent<Magic::System::ThreadsComponent>().operator->();
					EnterCriticalSection(&_pThreadsComponent->m_MutexObjectMessage);
					_pThreadsComponent->m_vec_ObjectMessageStruct.push_back({ _pSceneCommon->GetEntity(), _MessageStruct });
					LeaveCriticalSection(&_pThreadsComponent->m_MutexObjectMessage);
					//线程安全--------End

					return true;
				}
				else
				{
					SetEngineErrorMessage("没有此场景\n");
					return false;
				}
			}
			else
			{
				Magic::System::ThreadsComponent* _pThreadsComponent = _EntityCommon.GetComponent<Magic::System::ThreadsComponent>().operator->();
				EnterCriticalSection(&_pThreadsComponent->m_MutexObjectMessage);
				_pThreadsComponent->m_vec_ObjectMessageStruct.push_back({ EntityCommon(), _MessageStruct });
				LeaveCriticalSection(&_pThreadsComponent->m_MutexObjectMessage);

				return true;
			}
		}
		else
		{
			SetEngineErrorMessage("错误线程对象\n");
			return false;
		}
	}

	bool SendMessageToScene(const EntityCommon& _SceneEntity, Magic::System::MessageStruct _MessageStruct)
	{
		EntityCommon _EntityCommon = MagicEngineContext::Instance()->GetThreadsResourceManager();
		if (_EntityCommon.has_component<Magic::System::ThreadsComponent>())
		{
			//线程安全--------Start
			Magic::System::ThreadsComponent* _pThreadsComponent = _EntityCommon.GetComponent<Magic::System::ThreadsComponent>().operator->();
			EnterCriticalSection(&_pThreadsComponent->m_MutexObjectMessage);
			_pThreadsComponent->m_vec_ObjectMessageStruct.push_back({ _SceneEntity, _MessageStruct });
			LeaveCriticalSection(&_pThreadsComponent->m_MutexObjectMessage);
			//线程安全--------End

			return true;
		}
		else
		{
			SetEngineErrorMessage("错误线程对象\n");
			return false;
		}
	}

	bool SendMessageToThreads(const char* _ThreadsName, Magic::System::MessageStruct _MessageStruct)
	{
		EntityCommon _EntityCommon = MagicEngineContext::Instance()->GetThreadsResourceManager(_ThreadsName);
		if (_EntityCommon.has_component<Magic::System::ThreadsComponent>())
		{
			//线程安全--------Start
			Magic::System::ThreadsComponent* _pThreadsComponent = _EntityCommon.GetComponent<Magic::System::ThreadsComponent>().operator->();
			EnterCriticalSection(&_pThreadsComponent->m_MutexObjectMessage);
			_pThreadsComponent->m_vec_ObjectMessageStruct.push_back({ EntityCommon(), _MessageStruct });
			LeaveCriticalSection(&_pThreadsComponent->m_MutexObjectMessage);
			//线程安全--------End

			return true;
		}
		else
		{
			SetEngineErrorMessage("错误线程对象\n");
			return false;
		}
	}

	bool SendMessageToThreads(EntityCommon& _ThreadsEntity, Magic::System::MessageStruct _MessageStruct)
	{
		if (_ThreadsEntity.has_component<Magic::System::ThreadsComponent>())
		{
			//线程安全--------Start
			Magic::System::ThreadsComponent* _pThreadsComponent = _ThreadsEntity.GetComponent<Magic::System::ThreadsComponent>().operator->();
			EnterCriticalSection(&_pThreadsComponent->m_MutexObjectMessage);
			_pThreadsComponent->m_vec_ObjectMessageStruct.push_back({ EntityCommon(), _MessageStruct });
			LeaveCriticalSection(&_pThreadsComponent->m_MutexObjectMessage);
			//线程安全--------End

			return true;
		}
		else
		{
			SetEngineErrorMessage("错误线程对象\n");
			return false;
		}
	}

	bool SendMessageToThreadsScene(const char* _ThreadsName, const char* _SceneName, Magic::System::MessageStruct _MessageStruct)
	{
		EntityCommon _EntityCommon = MagicEngineContext::Instance()->GetThreadsResourceManager(_ThreadsName);
		if (_EntityCommon.has_component<Magic::System::ThreadsComponent>())
		{
			if (_SceneName)
			{
				Magic::SceneCommon* _pSceneCommon = GetSceneCommon(_EntityCommon, _SceneName);
				if (_pSceneCommon)
				{
					//线程安全--------Start
					Magic::System::ThreadsComponent* _pThreadsComponent = _EntityCommon.GetComponent<Magic::System::ThreadsComponent>().operator->();
					EnterCriticalSection(&_pThreadsComponent->m_MutexObjectMessage);
					_pThreadsComponent->m_vec_ObjectMessageStruct.push_back({ _pSceneCommon->GetEntity(), _MessageStruct });
					LeaveCriticalSection(&_pThreadsComponent->m_MutexObjectMessage);
					//线程安全--------End

					return true;
				}
				else
				{
					SetEngineErrorMessage("没有此场景\n");
					return false;
				}
			}
			else
			{
				Magic::System::ThreadsComponent* _pThreadsComponent = _EntityCommon.GetComponent<Magic::System::ThreadsComponent>().operator->();
				EnterCriticalSection(&_pThreadsComponent->m_MutexObjectMessage);
				_pThreadsComponent->m_vec_ObjectMessageStruct.push_back({ EntityCommon(), _MessageStruct });
				LeaveCriticalSection(&_pThreadsComponent->m_MutexObjectMessage);

				return true;
			}
		}
		else
		{
			SetEngineErrorMessage("错误线程对象\n");
			return false;
		}
	}

	bool SendMessageToThreadsScene(EntityCommon& _ThreadsEntity, const EntityCommon& _SceneEntity, Magic::System::MessageStruct _MessageStruct)
	{
		if (_ThreadsEntity.has_component<Magic::System::ThreadsComponent>())
		{
			//线程安全--------Start
			Magic::System::ThreadsComponent* _pThreadsComponent = _ThreadsEntity.GetComponent<Magic::System::ThreadsComponent>().operator->();
			EnterCriticalSection(&_pThreadsComponent->m_MutexObjectMessage);
			_pThreadsComponent->m_vec_ObjectMessageStruct.push_back({ _SceneEntity, _MessageStruct });
			LeaveCriticalSection(&_pThreadsComponent->m_MutexObjectMessage);
			//线程安全--------End

			return true;
		}
		else
		{
			SetEngineErrorMessage("错误线程对象\n");
			return false;
		}

		return true;
	}

	double GetThreadsDiffTime()
	{
		return MagicEngineContext::Instance()->GetThreadsResourceManager().GetComponent<Magic::System::ThreadsComponent>()->m_DiffTime;
	}
}