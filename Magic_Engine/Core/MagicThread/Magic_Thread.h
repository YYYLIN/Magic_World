#ifndef _WWT_MANAGEMENT_H_
#define _WWT_MANAGEMENT_H_

#include <functional>

#ifdef WIN32

#ifdef WWTMANAGEMENT_EXPORTS

#define DLL_WWTMANAGEMENT_OUTPUT         __declspec(dllexport)
#define DLL_WWTMANAGEMENT_OUTPUT_INPUT   __declspec(dllexport)

#else

#define DLL_WWTMANAGEMENT_OUTPUT        //__declspec(dllimport)
#define DLL_WWTMANAGEMENT_OUTPUT_INPUT   __declspec(dllimport)

#endif

#elif LINUX

#define DLL_WWTMANAGEMENT_OUTPUT  
#define DLL_WWTMANAGEMENT_OUTPUT_INPUT

#endif

#define WWT_MANAGEMENT_CREATE_THREAD_NAME			"MAIN_THREAD"

struct DLL_WWTMANAGEMENT_OUTPUT_INPUT WWT_Message_Struct
{
	unsigned int MessageType;
	long long Message;
	WWT_Message_Struct();
	WWT_Message_Struct(const unsigned int& _MessageType, const long long& _message);
};

namespace WWT
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
			virtual int MessageHandle(WWT_Message_Struct MessageStruct) = 0;
		};

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
		typedef std::function<void(unsigned int, long long)> Callback_Message;

		/*
		*���ܣ�
		*	�����̹߳�����
		*���棺
		*	�������Engine������Ҫ����CreateThreadManagement|ShutdownThreadManagement|UpdataThreadManagement
		*������
		*	��
		*����ֵ��
		*	bool = true �ɹ� | false ʧ��
		*/
		DLL_WWTMANAGEMENT_OUTPUT_INPUT bool CreateThreadManagement(UpdataCommon* _pUpdataCommon);

		/*
		*���ܣ�
		*	�ر��̹߳�����
		*���棺
		*	�������Engine������Ҫ����CreateThreadManagement|ShutdownThreadManagement|UpdataThreadManagement
		*������
		*	��
		*����ֵ��
		*	��
		*/
		DLL_WWTMANAGEMENT_OUTPUT_INPUT void ShutdownThreadManagement();

		/*
		*���ܣ�
		*	�����̹߳�����
		*���棺
		*	�������Engine������Ҫ����CreateThreadManagement|ShutdownThreadManagement|UpdataThreadManagement
		*������
		*	��
		*����ֵ��
		*	��
		*/
		DLL_WWTMANAGEMENT_OUTPUT_INPUT void UpdataThreadManagement();

		/*
		*���ܣ�
		*	�����̶߳���
		*������
		*	[IN]_name = �̶߳�������
		*	[IN]_pWWTCommon = ִ�ж���
		*	[IN]_ThreadTypeMode = �߳�����ģʽ ThreadTypeMode
		*����ֵ��
		*	THREAD_OBJECT = �̶߳���
		*/
		DLL_WWTMANAGEMENT_OUTPUT_INPUT THREAD_OBJECT CreateThreadObject(const char* _name, UpdataCommon* _pUpdataCommon, ThreadTypeMode _ThreadTypeMode, ThreadMessageMode _ThreadMessageMode = THREAD_MESSAGE_NO_WAIT);

		/*
		*���ܣ�
		*	�����̳߳ض���
		*������
		*	[IN]_name = �̶߳�������
		*	[IN]_ThreadNumber = ��Ҫ�������߳�����
		*����ֵ��
		*	THREAD_POOL_OBJECT = �̳߳ض���
		*/
		DLL_WWTMANAGEMENT_OUTPUT_INPUT THREAD_POOL_OBJECT CreateThreadPoolObject(const char* _name, unsigned int _ThreadNumber);

		/*
		*���ܣ�
		*	�ر�ָ���߳������߳�
		*������
		*	[IN]_name = �߳���
		*����ֵ��
		*	��
		*/
		DLL_WWTMANAGEMENT_OUTPUT_INPUT void ShutdownThreadObject(const char* _name);

		/*
		*���ܣ�
		*	�ر�ָ���̶߳�����߳�
		*������
		*	[IN]_THREAD_OBJECT = �̶߳���
		*����ֵ��
		*	��
		*/
		DLL_WWTMANAGEMENT_OUTPUT_INPUT void ShutdownThreadObject(THREAD_OBJECT _THREAD_OBJECT);

		/*
		*���ܣ�
		*	�ر�ָ���̳߳ض���
		*������
		*	[IN]_THREAD_POOL_OBJECT = �̳߳���
		*����ֵ��
		*	��
		*/
		DLL_WWTMANAGEMENT_OUTPUT_INPUT void ShutdownThreadPoolObject(const char* _name);

		/*
		*���ܣ�
		*	�ر�ָ���̳߳ض���
		*������
		*	[IN]_THREAD_POOL_OBJECT = �̳߳ض���
		*����ֵ��
		*	��
		*/
		DLL_WWTMANAGEMENT_OUTPUT_INPUT void ShutdownThreadPoolObject(THREAD_POOL_OBJECT _THREAD_POOL_OBJECT);

		/*
		*���ܣ�
		*	�رյ�ǰ�߳�
		*������
		*	��
		*����ֵ��
		*	��
		*/
		DLL_WWTMANAGEMENT_OUTPUT_INPUT void ShutdownThreadObject();

		/*
		*���ܣ�
		*	������Ϣ��ָ���߳�
		*������
		*	[IN]_THREAD_OBJECT = �̶߳���
		*	[IN]_MessageType = ��Ϣ����
		*	[IN]_Message = ��Ϣ���һ���Ϊ������������
		*	[IN]_CallBack = ��ָ���߳��ӳ����д˺���
		*���棺
		*	 �����Ϣ����Ϊ0�Ų�������Ϣִֻ�к���
		*����ֵ��
		*	bool = true ���ͳɹ� | false����ʧ��
		*/
		DLL_WWTMANAGEMENT_OUTPUT_INPUT bool SendMessageTo(THREAD_OBJECT _THREAD_OBJECT, unsigned int _MessageType, long long _Message, Callback_Message _CallBack = nullptr);

		/*
		*���ܣ�
		*	������Ϣ��ָ���߳�
		*������
		*	[IN]_name = �̶߳�����
		*	[IN]_MessageType = ��Ϣ����
		*	[IN]_Message = ��Ϣ���һ���Ϊ������������
		*	[IN]_CallBack = ��ָ���߳��ӳ����д˺���
		*���棺
		*	 �����Ϣ����Ϊ0�Ų�������Ϣִֻ�к���
		*����ֵ��
		*	bool = true ���ͳɹ� | false����ʧ��
		*/
		DLL_WWTMANAGEMENT_OUTPUT_INPUT bool SendMessageTo(const char* _name, unsigned int _MessageType, long long _Message, Callback_Message _CallBack = nullptr);

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
		DLL_WWTMANAGEMENT_OUTPUT_INPUT bool SendMessageTo(unsigned int _MessageType, long long _Message, Callback_Message _CallBack = nullptr);

		/*
		*���ܣ�
		*	������Ϣ��ָ���̳߳�
		*������
		*	[IN]_name = �̳߳ض�����
		*	[IN]_MessageType = ��Ϣ����
		*	[IN]_Message = ��Ϣ���һ���Ϊ������������
		*	[IN]_CallBack = ��ָ���߳��ӳ����д˺���
		*����ֵ��
		*	bool = true ���ͳɹ� | false����ʧ��
		*/
		DLL_WWTMANAGEMENT_OUTPUT_INPUT bool SendMessageToPool(const char* _name, unsigned int _MessageType, long long _Message, Callback_Message _CallBack);

		/*
		*���ܣ�
		*	������Ϣ��ָ���̳߳�
		*������
		*	[IN]_THREAD_POOL_OBJECT = �̳߳ض���
		*	[IN]_MessageType = ��Ϣ����
		*	[IN]_Message = ��Ϣ���һ���Ϊ������������
		*	[IN]_CallBack = ��ָ���߳��ӳ����д˺���
		*����ֵ��
		*	bool = true ���ͳɹ� | false����ʧ��
		*/
		DLL_WWTMANAGEMENT_OUTPUT_INPUT bool SendMessageToPool(THREAD_POOL_OBJECT _THREAD_POOL_OBJECT, unsigned int _MessageType, long long _Message, Callback_Message _CallBack);

		/*
		*���ܣ�
		*	��ȡ��ǰ�̵߳��̶߳���
		*������
		*	��
		*����ֵ��
		*	THREAD_OBJECT = �̶߳���
		*/
		DLL_WWTMANAGEMENT_OUTPUT_INPUT THREAD_OBJECT GetNowTHREAD_OBJECT();

		/*
		*���ܣ�
		*	��ȡ��ǰ�̵߳��̳߳ض���
		*������
		*	��
		*����ֵ��
		*	THREAD_POOL_OBJECT = �̳߳ض���
		*/
		DLL_WWTMANAGEMENT_OUTPUT_INPUT THREAD_POOL_OBJECT GetNowTHREAD_POOL_OBJECT();

		/*
		*���ܣ�
		*	��ȡָ���߳������̶߳���
		*������
		*	��
		*����ֵ��
		*	THREAD_OBJECT = �̶߳���
		*/
		DLL_WWTMANAGEMENT_OUTPUT_INPUT THREAD_OBJECT GetTHREAD_OBJECT(const char* _name);

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
		DLL_WWTMANAGEMENT_OUTPUT_INPUT void GetTHREAD_OBJECT_Name(THREAD_OBJECT _THREAD_OBJECT, char* _name, int _size);

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
		DLL_WWTMANAGEMENT_OUTPUT_INPUT void GetTHREAD_POOL_OBJECT_Name(THREAD_POOL_OBJECT _THREAD_POOL_OBJECT, char* _name, int _size);
	}
}

#endif