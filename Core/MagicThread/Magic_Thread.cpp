#include "Magic_Thread.h"
#include "Magic_Thread_Management.h"

namespace Magic
{
	namespace Management
	{
		bool CreateThreadManagement()
		{
			if (!SystemThread::Instance())
			{
				//创建线程管理系统
				Magic::Management::SystemThread* _pSystemThread = 0;
				_pSystemThread = new Magic::Management::SystemThread;
				bool result = _pSystemThread->Initialize();
				if (!result)
					return 0;

				return true;
			}
			else
				return false;
		}

		void ShutdownThreadManagement()
		{
			if (!SystemThread::Instance())
			{
				delete SystemThread::Instance();
			}
		}

		void UpdataThreadManagement()
		{
			SystemThread::Instance()->Updata();
		}

		THREAD_OBJECT CreateThreadObject(const char* _name, ThreadTypeMode _ThreadTypeMode, ThreadMessageMode _ThreadMessageMode)
		{
			return SystemThread::Instance()->Create(_name, _ThreadTypeMode, _ThreadMessageMode, true);
		}

		THREAD_POOL_OBJECT CreateThreadPoolObject(const char* _name, unsigned int _ThreadNumber)
		{
			return SystemThread::Instance()->CreatePool(_name, _ThreadNumber);
		}

		void ShutdownThreadObject(const char* _name)
		{
			SystemThread::Instance()->Shutdown(_name);
		}

		void ShutdownThreadObject(THREAD_OBJECT _THREAD_OBJECT)
		{
			SystemThread::Instance()->Shutdown(_THREAD_OBJECT);
		}

		void ShutdownThreadPoolObject(const char* _name)
		{
			SystemThread::Instance()->ShutdownPool(SystemThread::Instance()->GetTHREAD_POOL_OBJECT(_name));
		}

		void ShutdownThreadPoolObject(THREAD_POOL_OBJECT _THREAD_POOL_OBJECT)
		{
			SystemThread::Instance()->ShutdownPool(_THREAD_POOL_OBJECT);
		}

		void ShutdownThreadObject()
		{
			SystemThread::Instance()->Shutdown(SystemThread::Instance()->GetNowTHREAD_OBJECT());
		}

		bool MonitorThread(THREAD_OBJECT _THREAD_OBJECT, const Callback_Void& _CallBack) {
			return SystemThread::Instance()->MonitorThread(_THREAD_OBJECT, _CallBack);
		}

		bool MonitorThread(const char* _name, const Callback_Void& _CallBack) {
			return SystemThread::Instance()->MonitorThread(SystemThread::Instance()->GetTHREAD_OBJECT(_name), _CallBack);
		}

		bool MonitorThreadPool(THREAD_POOL_OBJECT _THREAD_POOL_OBJECT, const Callback_Void& _CallBack) {
			return SystemThread::Instance()->MonitorThreadPool(_THREAD_POOL_OBJECT, _CallBack);
		}

		bool MonitorThreadPool(const char* _name, const Callback_Void& _CallBack) {
			return SystemThread::Instance()->MonitorThreadPool(SystemThread::Instance()->GetTHREAD_POOL_OBJECT(_name), _CallBack);
		}

		bool MonitorThreadMessage(THREAD_OBJECT _THREAD_OBJECT, MESSAGE_TYPE _MessageType, const Callback_Message& _CallBack)
		{
			return SystemThread::Instance()->MonitorThreadMessage(_THREAD_OBJECT, _MessageType, _CallBack);
		}

		bool MonitorThreadMessage(const char* _name, MESSAGE_TYPE _MessageType, const Callback_Message& _CallBack) {
			return SystemThread::Instance()->MonitorThreadMessage(SystemThread::Instance()->GetTHREAD_POOL_OBJECT(_name), _MessageType, _CallBack);
		}

		bool MonitorThreadPoolMessage(THREAD_POOL_OBJECT _THREAD_POOL_OBJECT, MESSAGE_TYPE _MessageType, const Callback_Message& _CallBack)
		{
			return SystemThread::Instance()->MonitorThreadPoolMessage(_THREAD_POOL_OBJECT, _MessageType, _CallBack);
		}

		bool MonitorThreadPoolMessage(const char* _name, MESSAGE_TYPE _MessageType, const Callback_Message& _CallBack) {
			return SystemThread::Instance()->MonitorThreadPoolMessage(SystemThread::Instance()->GetTHREAD_POOL_OBJECT(_name), _MessageType, _CallBack);
		}

		bool SendMessageTo(THREAD_OBJECT _THREAD_OBJECT, MESSAGE_TYPE _MessageType, MESSAGE _Message, const Callback_Message& _CallBack, bool _Synch)
		{
			return SystemThread::Instance()->SendMessageTo(_THREAD_OBJECT, _MessageType, _Message, _CallBack, _Synch);
		}

		bool SendMessageTo(const char* _name, MESSAGE_TYPE _MessageType, MESSAGE _Message, const Callback_Message& _CallBack, bool _Synch)
		{
			return SystemThread::Instance()->SendMessageTo(SystemThread::Instance()->GetTHREAD_OBJECT(_name), _MessageType, _Message, _CallBack, _Synch);
		}

		bool SendMessageTo(MESSAGE_TYPE _MessageType, MESSAGE _Message, const Callback_Message& _CallBack)
		{
			return SystemThread::Instance()->SendMessageTo(_MessageType, _Message, _CallBack);
		}

		bool SendMessageToPool(const char* _name, MESSAGE_TYPE _MessageType, MESSAGE _Message, const Callback_Message& _CallBack, bool _Synch)
		{
			return SystemThread::Instance()->SendMessageToPool(SystemThread::Instance()->GetTHREAD_POOL_OBJECT(_name), _MessageType, _Message, _CallBack, _Synch);
		}

		bool SendMessageToPool(THREAD_POOL_OBJECT _THREAD_POOL_OBJECT, MESSAGE_TYPE _MessageType, MESSAGE _Message, const Callback_Message& _CallBack, bool _Synch)
		{
			return SystemThread::Instance()->SendMessageToPool(_THREAD_POOL_OBJECT, _MessageType, _Message, _CallBack, _Synch);
		}

		THREAD_OBJECT GetNowTHREAD_OBJECT()
		{
			return SystemThread::Instance()->GetNowTHREAD_OBJECT();
		}

		THREAD_POOL_OBJECT GetNowTHREAD_POOL_OBJECT()
		{
			return SystemThread::Instance()->GetNowTHREAD_POOL_OBJECT();
		}

		THREAD_OBJECT GetTHREAD_OBJECT(const char* _name)
		{
			return SystemThread::Instance()->GetTHREAD_OBJECT(_name);
		}

		void GetTHREAD_OBJECT_Name(THREAD_OBJECT _THREAD_OBJECT, char* _name, int _size)
		{
			SystemThread::Instance()->GetTHREAD_OBJECT_Name(_THREAD_OBJECT, _name, _size);
		}

		void GetTHREAD_POOL_OBJECT_Name(THREAD_POOL_OBJECT _THREAD_POOL_OBJECT, char* _name, int _size)
		{
			SystemThread::Instance()->GetTHREAD_POOL_OBJECT_Name(_THREAD_POOL_OBJECT, _name, _size);
		}
	}
}