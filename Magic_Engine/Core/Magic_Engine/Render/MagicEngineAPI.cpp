
#include "MagicEngineAPI.h"
#include "string"
#include "System/Supervisor.h"
#include "MagicEngineContext.h"
#include "MagicScene.h"

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