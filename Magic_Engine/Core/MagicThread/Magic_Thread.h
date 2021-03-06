#ifndef _MAGIC_THREAD_H_
#define _MAGIC_THREAD_H_

#include <functional>

#ifdef WIN32

#ifdef MAGIC_THREAD_EXPORTS

#define DLL_MAGIC_THREAD_OUTPUT         __declspec(dllexport)
#define DLL_MAGIC_THREAD_OUTPUT_INPUT   __declspec(dllexport)

#else

#define DLL_MAGIC_THREAD_OUTPUT        //__declspec(dllimport)
#define DLL_MAGIC_THREAD_OUTPUT_INPUT   __declspec(dllimport)

#endif

#elif LINUX

#define DLL_MAGIC_THREAD_OUTPUT  
#define DLL_MAGIC_THREAD_OUTPUT_INPUT

#endif

#define MAGIC_THREAD_MAIN_THREAD_NAME			"MAIN_THREAD"

struct DLL_MAGIC_THREAD_OUTPUT_INPUT Magic_Message_Struct
{
	unsigned int MessageType;
	long long Message;
	Magic_Message_Struct();
	Magic_Message_Struct(const unsigned int& _MessageType, const long long& _message);
};

namespace Magic
{
	namespace Management
	{
		class UpdataCommon
		{
		public:
			virtual bool Updata() = 0;
		};

		class ThreadMessage
		{
		public:
			virtual int MessageHandle(Magic_Message_Struct MessageStruct) = 0;
		};

		enum ThreadTypeMode
		{
			THREAD_RUN_ONCE,		//只运行一直
			THREAD_LOOP_RUN			//循环运行
		};

		enum ThreadMessageMode
		{
			THREAD_MESSAGE_NO_WAIT,		//如果没有消息，着继续运行
			THREAD_MESSAGE_WAIT			//如果没有消息，着等待消息接受后再运行
		};

		typedef void* THREAD_OBJECT;
		typedef void* THREAD_POOL_OBJECT;
		typedef std::function<void(unsigned int, long long)> Callback_Message;

		/*
		*功能：
		*	创建线程管理器
		*参数：
		*	空
		*返回值：
		*	bool = true 成功 | false 失败
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT bool CreateThreadManagement(UpdataCommon* _pUpdataCommon);

		/*
		*功能：
		*	关闭线程管理器
		*参数：
		*	空
		*返回值：
		*	空
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT void ShutdownThreadManagement();

		/*
		*功能：
		*	更新线程管理器
		*参数：
		*	空
		*返回值：
		*	空
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT void UpdataThreadManagement();

		/*
		*功能：
		*	创建线程对象
		*参数：
		*	[IN]_name = 线程对象名字
		*	[IN]_pWWTCommon = 执行对象
		*	[IN]_ThreadTypeMode = 线程类型模式 ThreadTypeMode
		*返回值：
		*	THREAD_OBJECT = 线程对象
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT THREAD_OBJECT CreateThreadObject(const char* _name, UpdataCommon* _pUpdataCommon, ThreadTypeMode _ThreadTypeMode, ThreadMessageMode _ThreadMessageMode = THREAD_MESSAGE_NO_WAIT);

		/*
		*功能：
		*	创建线程池对象
		*参数：
		*	[IN]_name = 线程对象名字
		*	[IN]_ThreadNumber = 需要创建的线程数量
		*返回值：
		*	THREAD_POOL_OBJECT = 线程池对象
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT THREAD_POOL_OBJECT CreateThreadPoolObject(const char* _name, unsigned int _ThreadNumber);

		/*
		*功能：
		*	关闭指定线程名的线程
		*参数：
		*	[IN]_name = 线程名
		*返回值：
		*	空
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT void ShutdownThreadObject(const char* _name);

		/*
		*功能：
		*	关闭指定线程对象的线程
		*参数：
		*	[IN]_THREAD_OBJECT = 线程对象
		*返回值：
		*	空
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT void ShutdownThreadObject(THREAD_OBJECT _THREAD_OBJECT);

		/*
		*功能：
		*	关闭指定线程池对象
		*参数：
		*	[IN]_THREAD_POOL_OBJECT = 线程池名
		*返回值：
		*	空
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT void ShutdownThreadPoolObject(const char* _name);

		/*
		*功能：
		*	关闭指定线程池对象
		*参数：
		*	[IN]_THREAD_POOL_OBJECT = 线程池对象
		*返回值：
		*	空
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT void ShutdownThreadPoolObject(THREAD_POOL_OBJECT _THREAD_POOL_OBJECT);

		/*
		*功能：
		*	关闭当前线程
		*参数：
		*	空
		*返回值：
		*	空
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT void ShutdownThreadObject();

		/*
		*功能：
		*	发送消息到指定线程
		*参数：
		*	[IN]_THREAD_OBJECT = 线程对象
		*	[IN]_MessageType = 消息类型
		*	[IN]_Message = 消息并且会作为函数参数传入
		*	[IN]_CallBack = 在指定线程延迟运行此函数
		*警告：
		*	 如果消息类型为0着不传递消息只执行函数
		*返回值：
		*	bool = true 发送成功 | false发送失败
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT bool SendMessageTo(THREAD_OBJECT _THREAD_OBJECT, unsigned int _MessageType, long long _Message, Callback_Message _CallBack = nullptr);

		/*
		*功能：
		*	发送消息到指定线程
		*参数：
		*	[IN]_name = 线程对象名
		*	[IN]_MessageType = 消息类型
		*	[IN]_Message = 消息并且会作为函数参数传入
		*	[IN]_CallBack = 在指定线程延迟运行此函数
		*警告：
		*	 如果消息类型为0着不传递消息只执行函数
		*返回值：
		*	bool = true 发送成功 | false发送失败
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT bool SendMessageTo(const char* _name, unsigned int _MessageType, long long _Message, Callback_Message _CallBack = nullptr);

		/*
		*功能：
		*	发送消息到当前线程
		*参数：
		*	[IN]_MessageType = 消息类型
		*	[IN]_Message = 消息并且会作为函数参数传入
		*	[IN]_CallBack = 在当前线程延迟运行此函数
		*警告：
		*	 如果消息类型为0着不传递消息只执行函数
		*返回值：
		*	bool = true 发送成功 | false发送失败
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT bool SendMessageTo(unsigned int _MessageType, long long _Message, Callback_Message _CallBack = nullptr);

		/*
		*功能：
		*	发送消息到指定线程池
		*参数：
		*	[IN]_name = 线程池对象名
		*	[IN]_MessageType = 消息类型
		*	[IN]_Message = 消息并且会作为函数参数传入
		*	[IN]_CallBack = 在指定线程延迟运行此函数
		*返回值：
		*	bool = true 发送成功 | false发送失败
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT bool SendMessageToPool(const char* _name, unsigned int _MessageType, long long _Message, Callback_Message _CallBack);

		/*
		*功能：
		*	发送消息到指定线程池
		*参数：
		*	[IN]_THREAD_POOL_OBJECT = 线程池对象
		*	[IN]_MessageType = 消息类型
		*	[IN]_Message = 消息并且会作为函数参数传入
		*	[IN]_CallBack = 在指定线程延迟运行此函数
		*返回值：
		*	bool = true 发送成功 | false发送失败
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT bool SendMessageToPool(THREAD_POOL_OBJECT _THREAD_POOL_OBJECT, unsigned int _MessageType, long long _Message, Callback_Message _CallBack);

		/*
		*功能：
		*	获取当前线程的线程对象
		*参数：
		*	空
		*返回值：
		*	THREAD_OBJECT = 线程对象
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT THREAD_OBJECT GetNowTHREAD_OBJECT();

		/*
		*功能：
		*	获取当前线程的线程池对象
		*参数：
		*	空
		*返回值：
		*	THREAD_POOL_OBJECT = 线程池对象
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT THREAD_POOL_OBJECT GetNowTHREAD_POOL_OBJECT();

		/*
		*功能：
		*	获取指定线程名的线程对象
		*参数：
		*	空
		*返回值：
		*	THREAD_OBJECT = 线程对象
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT THREAD_OBJECT GetTHREAD_OBJECT(const char* _name);

		/*
		*功能：
		*	获取指定线程的线程名
		*参数：
		*	[IN]_THREAD_OBJECT = 线程对象
		*	[OUT]_name = 线程名
		*	[IN]_size = _name的缓存区大小
		*返回值：
		*	空
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT void GetTHREAD_OBJECT_Name(THREAD_OBJECT _THREAD_OBJECT, char* _name, int _size);

		/*
		*功能：
		*	获取指定线程池的名
		*参数：
		*	[IN]_THREAD_POOL_OBJECT = 线程池对象
		*	[OUT]_name = 线程池名
		*	[IN]_size = _name的缓存区大小
		*返回值：
		*	空
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT void GetTHREAD_POOL_OBJECT_Name(THREAD_POOL_OBJECT _THREAD_POOL_OBJECT, char* _name, int _size);
	}
}

#endif