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

#define MAGIC_MAIN_THREAD_NAME			"MAIN_THREAD"

#define BindClassFunctionToMessage(F) std::bind(F, this, std::placeholders::_1, std::placeholders::_2)
#define BindClassFunctionToMessageObject(F,O) std::bind(F, O, std::placeholders::_1, std::placeholders::_2)
#define BindClassFunction(F) std::bind(F, this)
#define BindClassFunctionObject(F,O) std::bind(F, O)

namespace Magic
{
	namespace Management
	{
		typedef unsigned int MESSAGE_TYPE;
		typedef long long MESSAGE;

		enum ThreadTypeMode
		{
			THREAD_RUN_ONCE,		//ֻ����һֱ
			THREAD_LOOP_RUN			//ѭ������
		};

		enum ThreadMessageMode
		{
			THREAD_MESSAGE_NO_WAIT,		//���û����Ϣ���ż�������
			THREAD_MESSAGE_WAIT			//���û����Ϣ���ŵȴ���Ϣ���ܺ�������
		};

		typedef void* THREAD_OBJECT;
		typedef void* THREAD_POOL_OBJECT;
		typedef std::function<void(MESSAGE_TYPE, MESSAGE)> Callback_Message;
		typedef std::function<void(void)> Callback_Void;

		/*
		*���ܣ�
		*	�����̹߳�����
		*������
		*	��
		*����ֵ��
		*	bool = true �ɹ� | false ʧ��
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT bool CreateThreadManagement();

		/*
		*���ܣ�
		*	�ر��̹߳�����
		*������
		*	��
		*����ֵ��
		*	��
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT void ShutdownThreadManagement();

		/*
		*���ܣ�
		*	�����̹߳�����
		*������
		*	��
		*����ֵ��
		*	��
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT void UpdataThreadManagement();

		/*
		*���ܣ�
		*	�����̶߳���
		*������
		*	[IN]_name = �̶߳�������
		*	[IN]_ThreadTypeMode = �߳�����ģʽ ThreadTypeMode
		*����ֵ��
		*	THREAD_OBJECT = �̶߳���
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT THREAD_OBJECT CreateThreadObject(
			const char* _name,
			ThreadTypeMode _ThreadTypeMode,
			ThreadMessageMode _ThreadMessageMode = THREAD_MESSAGE_NO_WAIT);

		/*
		*���ܣ�
		*	�����̳߳ض���
		*������
		*	[IN]_name = �̶߳�������
		*	[IN]_ThreadNumber = ��Ҫ�������߳�����
		*����ֵ��
		*	THREAD_POOL_OBJECT = �̳߳ض���
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT THREAD_POOL_OBJECT CreateThreadPoolObject(const char* _name, unsigned int _ThreadNumber);

		/*
		*���ܣ�
		*	�ر�ָ���߳������߳�
		*������
		*	[IN]_name = �߳���
		*����ֵ��
		*	��
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT void ShutdownThreadObject(const char* _name);

		/*
		*���ܣ�
		*	�ر�ָ���̶߳�����߳�
		*������
		*	[IN]_THREAD_OBJECT = �̶߳���
		*����ֵ��
		*	��
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT void ShutdownThreadObject(THREAD_OBJECT _THREAD_OBJECT);

		/*
		*���ܣ�
		*	�ر�ָ���̳߳ض���
		*������
		*	[IN]_THREAD_POOL_OBJECT = �̳߳���
		*����ֵ��
		*	��
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT void ShutdownThreadPoolObject(const char* _name);

		/*
		*���ܣ�
		*	�ر�ָ���̳߳ض���
		*������
		*	[IN]_THREAD_POOL_OBJECT = �̳߳ض���
		*����ֵ��
		*	��
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT void ShutdownThreadPoolObject(THREAD_POOL_OBJECT _THREAD_POOL_OBJECT);

		/*
		*���ܣ�
		*	�رյ�ǰ�߳�
		*������
		*	��
		*����ֵ��
		*	��
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT void ShutdownThreadObject();

		/*
		*���ܣ�
		*	�����߳�
		*������
		*	_THREAD_OBJECT = �̶߳���
		*	_CallBack = ������
		*����ֵ��
		*	��
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT bool MonitorThread(THREAD_OBJECT _THREAD_OBJECT, const Callback_Void& _CallBack);
		DLL_MAGIC_THREAD_OUTPUT_INPUT bool MonitorThread(const char* _name, const Callback_Void& _CallBack);

		/*
		*���ܣ�
		*	�����̳߳�
		*������
		*	_THREAD_POOL_OBJECT = �̳߳ض���
		*	_CallBack = ������
		*����ֵ��
		*	��
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT bool MonitorThreadPool(THREAD_POOL_OBJECT _THREAD_POOL_OBJECT, const Callback_Void& _CallBack);
		DLL_MAGIC_THREAD_OUTPUT_INPUT bool MonitorThreadPool(const char* _name, const Callback_Void& _CallBack);

		/*
		*���ܣ�
		*	�����߳���Ϣ
		*������
		*	_THREAD_OBJECT = �̶߳���
		*	_MessageType = ������Ϣ����
		*	_CallBack = ������
		*����ֵ��
		*	��
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT bool MonitorThreadMessage(THREAD_OBJECT _THREAD_OBJECT, MESSAGE_TYPE _MessageType, const Callback_Message& _CallBack);
		DLL_MAGIC_THREAD_OUTPUT_INPUT bool MonitorThreadMessage(const char* _name, MESSAGE_TYPE _MessageType, const Callback_Message& _CallBack);

		/*
		*���ܣ�
		*	�����̳߳���Ϣ
		*������
		*	_THREAD_POOL_OBJECT =  �̳߳ض���
		*	_MessageType = ������Ϣ����
		*	_CallBack = ������
		*����ֵ��
		*	��
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT bool MonitorThreadPoolMessage(THREAD_POOL_OBJECT _THREAD_POOL_OBJECT, MESSAGE_TYPE _MessageType, const Callback_Message& _CallBack);
		DLL_MAGIC_THREAD_OUTPUT_INPUT bool MonitorThreadPoolMessage(const char* _name, MESSAGE_TYPE _MessageType, const Callback_Message& _CallBack);

		/*
		*���ܣ�
		*	������Ϣ��ָ���߳�
		*������
		*	[IN]_THREAD_OBJECT = �̶߳���
		*	[IN]_MessageType = ��Ϣ����
		*	[IN]_Message = ��Ϣ���һ���Ϊ������������
		*	[IN]_CallBack = ��ָ���߳��ӳ����д˺���
		*	[IN]_Synch = ͬ��ģʽ��_CallBack������nullʱ,
		*				 ����ȴ�_CallBack����������˳���
		*���棺
		*	 �����Ϣ����Ϊ0�Ų�������Ϣִֻ�к���
		*����ֵ��
		*	bool = true ���ͳɹ� | false����ʧ��
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT bool SendMessageTo(THREAD_OBJECT _THREAD_OBJECT, MESSAGE_TYPE _MessageType, MESSAGE _Message,const Callback_Message& _CallBack = nullptr, bool _Synch = false);

		/*
		*���ܣ�
		*	������Ϣ��ָ���߳�
		*������
		*	[IN]_name = �̶߳�����
		*	[IN]_MessageType = ��Ϣ����
		*	[IN]_Message = ��Ϣ���һ���Ϊ������������
		*	[IN]_CallBack = ��ָ���߳��ӳ����д˺���
		*	[IN]_Synch = ͬ��ģʽ��_CallBack������nullʱ,
		*				 ����ȴ�_CallBack����������˳���
		*���棺
		*	 �����Ϣ����Ϊ0�Ų�������Ϣִֻ�к���
		*����ֵ��
		*	bool = true ���ͳɹ� | false����ʧ��
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT bool SendMessageTo(const char* _name, MESSAGE_TYPE _MessageType, MESSAGE _Message, const Callback_Message& _CallBack = nullptr, bool _Synch = false);

		/*
		*���ܣ�
		*	������Ϣ����ǰ�߳�
		*������
		*	[IN]_MessageType = ��Ϣ����
		*	[IN]_Message = ��Ϣ���һ���Ϊ������������
		*	[IN]_CallBack = �ڵ�ǰ�߳��ӳ����д˺���
		*���棺
		*	 �����Ϣ����Ϊ0�Ų�������Ϣִֻ�к���
		*����ֵ��
		*	bool = true ���ͳɹ� | false����ʧ��
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT bool SendMessageTo(MESSAGE_TYPE _MessageType, MESSAGE _Message, const Callback_Message& _CallBack = nullptr);

		/*
		*���ܣ�
		*	������Ϣ��ָ���̳߳�
		*������
		*	[IN]_name = �̳߳ض�����
		*	[IN]_MessageType = ��Ϣ����
		*	[IN]_Message = ��Ϣ���һ���Ϊ������������
		*	[IN]_CallBack = ��ָ���߳��ӳ����д˺���
		*	[IN]_Synch = ͬ��ģʽ��_CallBack������nullʱ,
		*				 ����ȴ�_CallBack����������˳���
		*����ֵ��
		*	bool = true ���ͳɹ� | false����ʧ��
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT bool SendMessageToPool(const char* _name, MESSAGE_TYPE _MessageType, MESSAGE _Message, const Callback_Message& _CallBack, bool _Synch = false);

		/*
		*���ܣ�
		*	������Ϣ��ָ���̳߳�
		*������
		*	[IN]_THREAD_POOL_OBJECT = �̳߳ض���
		*	[IN]_MessageType = ��Ϣ����
		*	[IN]_Message = ��Ϣ���һ���Ϊ������������
		*	[IN]_CallBack = ��ָ���߳��ӳ����д˺���
		*	[IN]_Synch = ͬ��ģʽ��_CallBack������nullʱ,
		*				 ����ȴ�_CallBack����������˳���
		*����ֵ��
		*	bool = true ���ͳɹ� | false����ʧ��
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT bool SendMessageToPool(THREAD_POOL_OBJECT _THREAD_POOL_OBJECT, MESSAGE_TYPE _MessageType, MESSAGE _Message, const Callback_Message& _CallBack, bool _Synch = false);

		/*
		*���ܣ�
		*	��ȡ��ǰ�̵߳��̶߳���
		*������
		*	��
		*����ֵ��
		*	THREAD_OBJECT = �̶߳���
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT THREAD_OBJECT GetNowTHREAD_OBJECT();

		/*
		*���ܣ�
		*	��ȡ��ǰ�̵߳��̳߳ض���
		*������
		*	��
		*����ֵ��
		*	THREAD_POOL_OBJECT = �̳߳ض���
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT THREAD_POOL_OBJECT GetNowTHREAD_POOL_OBJECT();

		/*
		*���ܣ�
		*	��ȡָ���߳������̶߳���
		*������
		*	��
		*����ֵ��
		*	THREAD_OBJECT = �̶߳���
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT THREAD_OBJECT GetTHREAD_OBJECT(const char* _name);

		/*
		*���ܣ�
		*	��ȡָ���̵߳��߳���
		*������
		*	[IN]_THREAD_OBJECT = �̶߳���
		*	[OUT]_name = �߳���
		*	[IN]_size = _name�Ļ�������С
		*����ֵ��
		*	��
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT void GetTHREAD_OBJECT_Name(THREAD_OBJECT _THREAD_OBJECT, char* _name, int _size);

		/*
		*���ܣ�
		*	��ȡָ���̳߳ص���
		*������
		*	[IN]_THREAD_POOL_OBJECT = �̳߳ض���
		*	[OUT]_name = �̳߳���
		*	[IN]_size = _name�Ļ�������С
		*����ֵ��
		*	��
		*/
		DLL_MAGIC_THREAD_OUTPUT_INPUT void GetTHREAD_POOL_OBJECT_Name(THREAD_POOL_OBJECT _THREAD_POOL_OBJECT, char* _name, int _size);
	}
}

#endif