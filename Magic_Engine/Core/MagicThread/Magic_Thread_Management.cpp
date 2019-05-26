#include "Magic_Thread_Management.h"

namespace Magic
{
	namespace Management
	{
		Message::Message() :m_MessageType(0), m_Message(0)
		{

		}

		Message::Message(const unsigned int& _MessageType, const long long& _Message, const Callback_Message& _CallBack)
			: m_MessageType(_MessageType), m_Message(_Message), m_CallBack(_CallBack)
		{

		}

		ThreadObject::ThreadObject()
		{
			pUpdataCommon = 0;
			m_ThreadTypeMode = THREAD_RUN_ONCE;
			m_ThreadRunState = THREAD_STATE_TIMEOUT;
			m_ThreadMessageMode = THREAD_MESSAGE_NO_WAIT;
		}

		ThreadObject::ThreadObject(UpdataCommon* _pUpdataCommon, ThreadTypeMode _ThreadTypeMode, ThreadRunState _ThreadRunState, const std::string& _name, ThreadMessageMode _ThreadMessageMode)
		{
			pUpdataCommon = _pUpdataCommon;
			m_ThreadTypeMode = _ThreadTypeMode;
			m_ThreadRunState = _ThreadRunState;
			m_ThreadMessageMode = _ThreadMessageMode;
			m_Name = _name;
		}

		bool ThreadPoolObject::Updata()
		{
			Message _Message;
			Magic_Thread_SEM_Wait(m_WWT_SEM);
			Magic_Thread_Mutex_Lock(&m_MessageMutex);
			size_t _Number = m_queue_Message.size();
			if (_Number)
			{
				_Message = m_queue_Message.front();
				m_queue_Message.pop();
			}
			Magic_Thread_Mutex_unLock(&m_MessageMutex);

			if (_Number)
				SystemThread::Instance()->SendMessageTo(_Message.m_MessageType, _Message.m_Message, _Message.m_CallBack);

			return true;
		}

		ThreadObject* SystemThread::m_S_T_pThreadObject = 0;
		ThreadPoolObject* SystemThread::m_S_T_pThreadPoolObject = 0;
		SystemThread* SystemThread::m_S_pSystemThread = 0;

		SystemThread::SystemThread()
		{
			m_S_pSystemThread = this;
		}

		SystemThread::~SystemThread()
		{
			Magic_Thread_Mutex_Lock(&m_Mutex);
			for (auto _auto = m_map_ThreadObject.begin(); _auto != m_map_ThreadObject.end(); _auto++)
			{
				if (m_S_T_pThreadObject != &_auto->second)
					Shutdown(&_auto->second);
			}

			for (auto _auto = m_map_ThreadObject.begin(); _auto != m_map_ThreadObject.end();)
				UpdataStop(_auto);
			Magic_Thread_Mutex_unLock(&m_Mutex);

			m_S_pSystemThread = 0;
			Magic_Thread_Mutex_Destroy(&m_MutexPoolObject);
			Magic_Thread_Mutex_Destroy(&m_Mutex);
		}

		SystemThread* SystemThread::Instance()
		{
			return m_S_pSystemThread;
		}

		bool SystemThread::Initialize(UpdataCommon* _pUpdataCommon)
		{
			Magic_Thread_Mutex_Init(&m_Mutex);
			Magic_Thread_Mutex_Init(&m_MutexPoolObject);

			m_map_ThreadObject.insert(std::make_pair("MAIN_THREAD", ThreadObject(_pUpdataCommon, THREAD_LOOP_RUN, THREAD_STATE_RUN, "MAIN_THREAD", THREAD_MESSAGE_NO_WAIT)));
			auto _findTO = m_map_ThreadObject.find("MAIN_THREAD");
			if (_findTO != m_map_ThreadObject.end())
			{
				m_S_T_pThreadObject = &_findTO->second;
				m_set_ThreadObject.insert(&_findTO->second);
				Magic_Thread_Mutex_Init(&_findTO->second.m_MessageMutex);
			}

			return true;
		}

		THREAD_OBJECT SystemThread::Create(const char* _name, UpdataCommon* _pUpdataCommon, ThreadTypeMode _ThreadTypeMode, ThreadMessageMode _ThreadMessageMode)
		{
			THREAD_OBJECT _THREAD_OBJECT = 0;
			Magic_Thread_Mutex_Lock(&m_Mutex);
			m_map_ThreadObject.insert(std::make_pair(_name, ThreadObject(_pUpdataCommon, _ThreadTypeMode, THREAD_STATE_RUN, _name, _ThreadMessageMode)));

			auto _findTO = m_map_ThreadObject.find(_name);
			if (_findTO != m_map_ThreadObject.end())
			{
				_THREAD_OBJECT = &_findTO->second;
				m_set_ThreadObject.insert(&_findTO->second);
				Magic_Thread_Mutex_Init(&_findTO->second.m_MessageMutex);
				Magic_Thread_SEM_init(_findTO->second.m_WWT_SEM, NULL, 0, LONG_MAX, NULL, NULL, 0);
				if (Magic_Thread_Create(_findTO->second.m_Thread, NULL, ThreadFunction, (void*)(&_findTO->second)))
					Magic_ResumeThread(_findTO->second.m_Thread);
			}
			Magic_Thread_Mutex_unLock(&m_Mutex);
			return _THREAD_OBJECT;
		}

		THREAD_POOL_OBJECT SystemThread::CreatePool(const char* _name, unsigned int _ThreadNumber)
		{
			THREAD_POOL_OBJECT  _THREAD_POOL_OBJECT = 0;

			Magic_Thread_Mutex_Lock(&m_MutexPoolObject);
			m_map_Srting_ThreadPoolObject.insert(std::make_pair(_name, ThreadPoolObject()));
			auto _findTO = m_map_Srting_ThreadPoolObject.find(_name);
			if (_findTO != m_map_Srting_ThreadPoolObject.end())
			{
				_THREAD_POOL_OBJECT = &_findTO->second;
				_findTO->second.m_Name = _name;
				Magic_Thread_Mutex_Init(&_findTO->second.m_MessageMutex);
				Magic_Thread_SEM_init(_findTO->second.m_WWT_SEM, NULL, 0, LONG_MAX, NULL, NULL, 0);

				for (unsigned int a = 0; a < _ThreadNumber; a++)
				{
					char _text[256];
					Magic_Sprintf_s(_text, 256, "%s_%d", _name, a);
					THREAD_OBJECT _THREAD_OBJECT = Create(_text, &_findTO->second, THREAD_LOOP_RUN, THREAD_MESSAGE_WAIT);
					SendMessageTo(_THREAD_OBJECT, 0, 0,
						[_THREAD_POOL_OBJECT](MESSAGE_TYPE, MESSAGE)
					{
						m_S_T_pThreadPoolObject = (ThreadPoolObject*)_THREAD_POOL_OBJECT;
						m_S_T_pThreadObject->m_ThreadMessageMode = THREAD_MESSAGE_NO_WAIT;
					});
					if (_THREAD_OBJECT)
						_findTO->second.m_vec_ThreadObject.push_back((ThreadObject*)_THREAD_OBJECT);
				}
			}
			Magic_Thread_Mutex_unLock(&m_MutexPoolObject);

			return _THREAD_POOL_OBJECT;
		}

		void SystemThread::ShutdownPool(THREAD_POOL_OBJECT _THREAD_POOL_OBJECT)
		{
			ThreadPoolObject* _pThreadPoolObject = (ThreadPoolObject*)_THREAD_POOL_OBJECT;
			if (!_pThreadPoolObject)
				return;

			Magic_Thread_Mutex_Lock(&m_MutexPoolObject);

			Magic_Thread_Mutex_Lock(&_pThreadPoolObject->m_MessageMutex);
			for (auto _auto : _pThreadPoolObject->m_vec_ThreadObject)
			{
				Shutdown(_auto);
				Magic_Thread_SEM_Post(_pThreadPoolObject->m_WWT_SEM);
			}
			Magic_Thread_Mutex_unLock(&_pThreadPoolObject->m_MessageMutex);

			for (auto _auto : _pThreadPoolObject->m_vec_ThreadObject)
			{
				bool _IsHave;
				Magic_Thread_Mutex_Lock(&m_Mutex);
				_IsHave = m_set_ThreadObject.find(_auto) != m_set_ThreadObject.end();
				Magic_Thread_Mutex_unLock(&m_Mutex);
				if (_IsHave)
				{
					Magic_Thread_Wait(_auto->m_Thread);
				}
			}

			Magic_Thread_SEM_destroy(_pThreadPoolObject->m_WWT_SEM);
			Magic_Thread_Mutex_Destroy(&_pThreadPoolObject->m_MessageMutex);

			std::string _Name = _pThreadPoolObject->m_Name;
			m_map_Srting_ThreadPoolObject.erase(_Name);

			Magic_Thread_Mutex_unLock(&m_MutexPoolObject);
		}

		void SystemThread::MonitorThreadMessage(MESSAGE_TYPE _MessageType, Callback_Message _CallBack)
		{

		}

		void SystemThread::MonitorThreadPoolMessage(MESSAGE_TYPE _MessageType, Callback_Message _CallBack)
		{

		}

		bool SystemThread::SendMessageTo(THREAD_OBJECT _THREAD_OBJECT, MESSAGE_TYPE _MessageType, MESSAGE _Message, Callback_Message _CallBack)
		{
			ThreadObject* _pThreadObject = (ThreadObject*)_THREAD_OBJECT;
			if (!_pThreadObject)
				return false;
			bool _IsHave;
			Magic_Thread_Mutex_Lock(&m_Mutex);
			_IsHave = m_set_ThreadObject.find(_pThreadObject) != m_set_ThreadObject.end();
			Magic_Thread_Mutex_unLock(&m_Mutex);
			if (!_IsHave)
				return false;

			Magic_Thread_Mutex_Lock(&_pThreadObject->m_MessageMutex);
			_pThreadObject->m_queue_Message.push(Message(_MessageType, _Message, _CallBack));
			if (_pThreadObject->m_ThreadMessageMode == THREAD_MESSAGE_WAIT)
				Magic_Thread_SEM_Post(_pThreadObject->m_WWT_SEM);
			Magic_Thread_Mutex_unLock(&_pThreadObject->m_MessageMutex);

			return true;
		}

		bool SystemThread::SendMessageTo(MESSAGE_TYPE _MessageType, MESSAGE _Message, Callback_Message _CallBack)
		{
			return SendMessageTo(m_S_T_pThreadObject, _MessageType, _Message, _CallBack);
		}

		bool SystemThread::SendMessageToPool(THREAD_POOL_OBJECT _THREAD_POOL_OBJECT, MESSAGE_TYPE _MessageType, MESSAGE _Message, Callback_Message _CallBack)
		{
			ThreadPoolObject* _pThreadPoolObject = (ThreadPoolObject*)_THREAD_POOL_OBJECT;
			if (!_pThreadPoolObject)
				return false;

			Magic_Thread_Mutex_Lock(&_pThreadPoolObject->m_MessageMutex);
			_pThreadPoolObject->m_queue_Message.push(Message(_MessageType, _Message, _CallBack));
			Magic_Thread_Mutex_unLock(&_pThreadPoolObject->m_MessageMutex);

			Magic_Thread_SEM_Post(_pThreadPoolObject->m_WWT_SEM);

			return true;
		}

		THREAD_OBJECT SystemThread::GetTHREAD_OBJECT(const char* _name)
		{
			THREAD_OBJECT _THREAD_OBJECT = 0;
			Magic_Thread_Mutex_Lock(&m_Mutex);
			auto _findTO = m_map_ThreadObject.find(_name);
			if (_findTO != m_map_ThreadObject.end())
				_THREAD_OBJECT = &_findTO->second;
			Magic_Thread_Mutex_unLock(&m_Mutex);

			return _THREAD_OBJECT;
		}

		THREAD_OBJECT SystemThread::GetTHREAD_POOL_OBJECT(const char* _name)
		{
			THREAD_POOL_OBJECT _THREAD_POOL_OBJECT = 0;
			Magic_Thread_Mutex_Lock(&m_MutexPoolObject);
			auto _findTO = m_map_Srting_ThreadPoolObject.find(_name);
			if (_findTO != m_map_Srting_ThreadPoolObject.end())
				_THREAD_POOL_OBJECT = &_findTO->second;
			Magic_Thread_Mutex_unLock(&m_MutexPoolObject);

			return _THREAD_POOL_OBJECT;
		}

		void SystemThread::GetTHREAD_OBJECT_Name(THREAD_OBJECT _THREAD_OBJECT, char* _name, int _size)
		{
			ThreadObject* _pThreadObject = (ThreadObject*)_THREAD_OBJECT;

			if (!_pThreadObject)
				return;
			bool _IsHave;
			Magic_Thread_Mutex_Lock(&m_Mutex);
			_IsHave = m_set_ThreadObject.find(_pThreadObject) != m_set_ThreadObject.end();
			Magic_Thread_Mutex_unLock(&m_Mutex);
			if (!_IsHave)
				return;

			Magic_Thread_Mutex_Lock(&_pThreadObject->m_MessageMutex);
			strcpy_s(_name, _size, _pThreadObject->m_Name.c_str());
			Magic_Thread_Mutex_unLock(&_pThreadObject->m_MessageMutex);
		}

		void SystemThread::GetTHREAD_POOL_OBJECT_Name(THREAD_POOL_OBJECT _THREAD_POOL_OBJECT, char* _name, int _size)
		{
			ThreadPoolObject* _pThreadPoolObject = (ThreadPoolObject*)_THREAD_POOL_OBJECT;

			if (!_pThreadPoolObject)
				return;

			Magic_Thread_Mutex_Lock(&_pThreadPoolObject->m_MessageMutex);
			strcpy_s(_name, _size, _pThreadPoolObject->m_Name.c_str());
			Magic_Thread_Mutex_unLock(&_pThreadPoolObject->m_MessageMutex);
		}

		void SystemThread::Shutdown(const char* _name)
		{
			Shutdown(GetTHREAD_OBJECT(_name));
		}

		void SystemThread::Shutdown(THREAD_OBJECT _THREAD_OBJECT)
		{
			ThreadObject* _pThreadObject = (ThreadObject*)_THREAD_OBJECT;

			if (!_pThreadObject)
				return;
			bool _IsHave;
			Magic_Thread_Mutex_Lock(&m_Mutex);
			_IsHave = m_set_ThreadObject.find(_pThreadObject) != m_set_ThreadObject.end();
			Magic_Thread_Mutex_unLock(&m_Mutex);
			if (!_IsHave)
				return;

			Magic_Thread_Mutex_Lock(&_pThreadObject->m_MessageMutex);
			_pThreadObject->m_ThreadRunState = THREAD_STOP;
			if (_pThreadObject->m_ThreadMessageMode == THREAD_MESSAGE_WAIT)
				Magic_Thread_SEM_Post(_pThreadObject->m_WWT_SEM);
			Magic_Thread_Mutex_unLock(&_pThreadObject->m_MessageMutex);


		}

		void SystemThread::UpdataStop(MAP_SRTING_THREADOBJECT::iterator& _auto)
		{
			if (m_S_T_pThreadObject != &_auto->second && _auto->second.m_ThreadRunState == THREAD_STOP)
			{
				Magic_Thread_Wait(_auto->second.m_Thread);
				Magic_CloseHandle(_auto->second.m_Thread);
				Magic_MUTEX _Mutex = _auto->second.m_MessageMutex;
				Magic_SEM _SEM = _auto->second.m_WWT_SEM;

				m_set_ThreadObject.erase(&_auto->second);

				Magic_Thread_Mutex_Lock(&_Mutex);
				_auto = m_map_ThreadObject.erase(_auto);
				Magic_Thread_Mutex_unLock(&_Mutex);

				Magic_Thread_SEM_destroy(_SEM);
				Magic_Thread_Mutex_Destroy(&_Mutex);
			}
			else
				_auto++;
		}

		bool SystemThread::Updata()
		{
			Magic_Thread_Mutex_Lock(&m_Mutex);
			for (auto _auto = m_map_ThreadObject.begin(); _auto != m_map_ThreadObject.end();)
				UpdataStop(_auto);
			Magic_Thread_Mutex_unLock(&m_Mutex);

			ThreadMessageHandle(m_S_T_pThreadObject);

			return true;
		}

		arcoss SystemThread::ThreadFunction(void* _data)
		{
			ThreadObject* _pThreadObject = (ThreadObject*)_data;
			m_S_T_pThreadObject = _pThreadObject;

			do
			{
				ThreadMessageHandle(_pThreadObject);

				if (_pThreadObject->pUpdataCommon)
					_pThreadObject->pUpdataCommon->Updata();
			} while (_pThreadObject->m_ThreadTypeMode == THREAD_LOOP_RUN && _pThreadObject->m_ThreadRunState != THREAD_STOP);

			_pThreadObject->m_ThreadRunState = THREAD_STOP;
			return arcoss_return(0);
		}

		int ThreadMessageHandle(ThreadObject* _pThreadObject)
		{
			Message _Message;
			size_t _Number, _HandleMaxNumber = 0;

			do
			{
				if (_pThreadObject->m_ThreadMessageMode == THREAD_MESSAGE_WAIT)
					Magic_Thread_SEM_Wait(_pThreadObject->m_WWT_SEM);
				Magic_Thread_Mutex_Lock(&_pThreadObject->m_MessageMutex);
				_Number = _pThreadObject->m_queue_Message.size();
				if (_Number)
				{
					_Message = _pThreadObject->m_queue_Message.front();
					_pThreadObject->m_queue_Message.pop();
				}
				Magic_Thread_Mutex_unLock(&_pThreadObject->m_MessageMutex);

				if (_Number)
				{
					if (_Message.m_CallBack)
						_Message.m_CallBack(_Message.m_MessageType, _Message.m_Message);
					if (_pThreadObject->pUpdataCommon)
					{
						ThreadMessage* _pThreadMessage = dynamic_cast<ThreadMessage*>(_pThreadObject->pUpdataCommon);
						if (_pThreadMessage && _Message.m_MessageType)
						{
							_pThreadMessage->MessageHandle(Magic_Message_Struct(_Message.m_MessageType, _Message.m_Message));
						}
					}
				}
				//当消息处理完时退出，或者当处理数量大于100时退出
			} while (_Number && _HandleMaxNumber++ < 100);

			return 0;
		}
	}
}
