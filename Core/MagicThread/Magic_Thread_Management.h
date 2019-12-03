#ifndef _MAGIC_THREAD_MANAGEMENT_H_
#define _MAGIC_THREAD_MANAGEMENT_H_

#include "Cross_Platform_Port.h"
#include "Magic_Thread.h"
#include <queue>
#include <set>
#include <map>
#include <unordered_map>

namespace Magic
{
	namespace Management
	{
		struct ThreadObject;

		enum ThreadRunState
		{
			THREAD_STATE_RUN,
			THREAD_STATE_TIMEOUT,
			THREAD_STOP
		};

		struct Message
		{
			unsigned int m_MessageType;
			long long m_Message;
			Callback_Message m_CallBack;
			ThreadObject* m_pThreadObject;
			Message();
			Message(const unsigned int& _MessageType, const long long& _Message, const Callback_Message& _CallBack, ThreadObject* _pThreadObject);
		};

		struct ThreadObject
		{
			typedef std::unordered_map<MESSAGE_TYPE, std::vector<Callback_Message>> UMAP_VEC_CALLBACK;

			std::vector<Callback_Void> m_vec_Callback;
			Magic_THREAD m_Thread;
			std::string m_Name;
			ThreadTypeMode m_ThreadTypeMode;
			ThreadMessageMode m_ThreadMessageMode;
			ThreadRunState m_ThreadRunState;

			std::queue<Message> m_queue_Message;
			Magic_SEM m_Queue_SEM;
			Magic_SEM m_Synch_SEM;

			UMAP_VEC_CALLBACK m_umap_MonitorFunction;
			Magic_MUTEX m_MessageMutex;

			ThreadObject();
			ThreadObject(ThreadTypeMode _ThreadTypeMode, ThreadRunState _ThreadRunState, const std::string& _name, ThreadMessageMode _ThreadMessageMode);
		};

		class ThreadPoolObject 
		{
		public:
			void Updata();
		public:
			std::string m_Name;
			std::vector<ThreadObject*> m_vec_ThreadObject;

			std::queue<Message> m_queue_Message;

			Magic_MUTEX m_MessageMutex;
			Magic_SEM m_queue_SEM;
		};

		class SystemThread
		{
			typedef std::map<std::string, ThreadObject> MAP_SRTING_THREADOBJECT;
			typedef std::map<std::string, ThreadPoolObject> MAP_SRTING_THREADPOOLOBJECT;
		public:
			SystemThread();
			~SystemThread();

			static SystemThread* Instance();

			bool Initialize();

			THREAD_OBJECT Create(const char* _name, ThreadTypeMode _ThreadTypeMode, ThreadMessageMode _ThreadMessageMode, bool _IsNewThread);

			THREAD_POOL_OBJECT CreatePool(const char* _name, unsigned int _ThreadNumber);

			void ShutdownPool(THREAD_POOL_OBJECT _THREAD_POOL_OBJECT);

			bool MonitorThread(THREAD_OBJECT _THREAD_OBJECT, const Callback_Void& _CallBack);

			bool MonitorThreadPool(THREAD_POOL_OBJECT _THREAD_POOL_OBJECT, const Callback_Void& _CallBack);

			bool MonitorThreadMessage(THREAD_OBJECT _THREAD_OBJECT, MESSAGE_TYPE _MessageType, const Callback_Message& _CallBack);

			bool MonitorThreadPoolMessage(THREAD_POOL_OBJECT _THREAD_POOL_OBJECT, MESSAGE_TYPE _MessageType, const Callback_Message& _CallBack);

			bool SendMessageTo(THREAD_OBJECT _THREAD_OBJECT, MESSAGE_TYPE _MessageType, MESSAGE _Message, const Callback_Message& _CallBack = nullptr, bool _Synch = false);

			bool SendMessageTo(MESSAGE_TYPE _MessageType, MESSAGE _Message, const Callback_Message& _CallBack = nullptr, bool _Synch = false);

			bool SendMessageToPool(THREAD_POOL_OBJECT _THREAD_POOL_OBJECT, MESSAGE_TYPE _MessageType, MESSAGE _Message, const Callback_Message& _CallBack, bool _Synch = false);

			THREAD_OBJECT GetNowTHREAD_OBJECT() { return (void*)m_S_T_pThreadObject; }

			THREAD_POOL_OBJECT GetNowTHREAD_POOL_OBJECT() { return (void*)m_S_T_pThreadPoolObject; }

			THREAD_OBJECT GetTHREAD_OBJECT(const char* _name);

			THREAD_OBJECT GetTHREAD_POOL_OBJECT(const char* _name);

			void GetTHREAD_OBJECT_Name(THREAD_OBJECT _THREAD_OBJECT, char* _name, int _size);

			void GetTHREAD_POOL_OBJECT_Name(THREAD_POOL_OBJECT _THREAD_POOL_OBJECT, char* _name, int _size);

			void Shutdown(const char* _name);

			void Shutdown(THREAD_OBJECT _THREAD_OBJECT);

		protected:
			void UpdataStop(MAP_SRTING_THREADOBJECT::iterator& _auto);
		public:
			void Updata();
		private:
			static arcoss ThreadFunction(void* _data);
		private:
			MAP_SRTING_THREADOBJECT m_map_ThreadObject;
			std::set<ThreadObject*> m_set_ThreadObject;

			MAP_SRTING_THREADPOOLOBJECT m_map_Srting_ThreadPoolObject;

			Magic_MUTEX m_Mutex, m_MutexPoolObject;

			static S_THREAD ThreadObject* m_S_T_pThreadObject;
			static S_THREAD ThreadPoolObject* m_S_T_pThreadPoolObject;

			static SystemThread* m_S_pSystemThread;
		};

		int ThreadMessageHandle(ThreadObject* _pThreadObject);
	}
}



#endif