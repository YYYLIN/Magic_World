#include "Magic_Thread.h"
#include "Magic_Thread_Management.h"

WWT_Message_Struct::WWT_Message_Struct()
{
	MessageType = 0;
	Message = 0;
}

WWT_Message_Struct::WWT_Message_Struct(const unsigned int& _MessageType, const long long& _message) :MessageType(_MessageType), Message(_message)
{
}

namespace WWT
{
	namespace Management
	{
		bool CreateThreadManagement(UpdataCommon* _pUpdataCommon)
		{
			if (!SystemThread::Instance())
			{
				//创建线程管理系统
				WWT::Management::SystemThread* _pSystemThread = 0;
				_pSystemThread = new WWT::Management::SystemThread;
				bool result = _pSystemThread->Initialize(_pUpdataCommon);
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

		THREAD_OBJECT CreateThreadObject(const char* _name, UpdataCommon* _pUpdataCommon, ThreadTypeMode _ThreadTypeMode, ThreadMessageMode _ThreadMessageMode)
		{
			return SystemThread::Instance()->Create(_name, _pUpdataCommon, _ThreadTypeMode, _ThreadMessageMode);
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

		bool SendMessageTo(THREAD_OBJECT _THREAD_OBJECT, unsigned int _MessageType, long long _Message, Callback_Message _CallBack)
		{
			return SystemThread::Instance()->SendMessageTo(_THREAD_OBJECT, _MessageType, _Message, _CallBack);
		}

		bool SendMessageTo(const char* _name, unsigned int _MessageType, long long _Message, Callback_Message _CallBack)
		{
			return SystemThread::Instance()->SendMessageTo(SystemThread::Instance()->GetTHREAD_OBJECT(_name), _MessageType, _Message, _CallBack);
		}

		bool SendMessageTo(unsigned int _MessageType, long long _Message, Callback_Message _CallBack)
		{
			return SystemThread::Instance()->SendMessageTo(_MessageType, _Message, _CallBack);
		}

		bool SendMessageToPool(const char* _name, unsigned int _MessageType, long long _Message, Callback_Message _CallBack)
		{
			return SystemThread::Instance()->SendMessageToPool(SystemThread::Instance()->GetTHREAD_POOL_OBJECT(_name), _MessageType, _Message, _CallBack);
		}

		bool SendMessageToPool(THREAD_POOL_OBJECT _THREAD_POOL_OBJECT, unsigned int _MessageType, long long _Message, Callback_Message _CallBack)
		{
			return SystemThread::Instance()->SendMessageToPool(_THREAD_POOL_OBJECT, _MessageType, _Message, _CallBack);
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