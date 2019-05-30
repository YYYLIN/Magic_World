#ifndef _MAGICENGINEAPI_H_
#define _MAGICENGINEAPI_H_

#include "Define/Magic_Macro.h"
#include "MagicScene.h"
#include "MagicPen.h"
#include "windows.h"

namespace Magic
{
	DLL_MAGIC_ENGINE_OUTPUT_INPUT const char* GetEngineErrorMessage();

	DLL_MAGIC_ENGINE_OUTPUT_INPUT void SetEngineErrorMessage(const char* _text);

	//DLL_MAGIC_ENGINE_OUTPUT_INPUT void AddEngineErrorMessage(const char* _text, bool _EndORStart = true);

	/*
	*���ܣ�
	*	����������ɳ�ʼ����
	*������
	*	��
	*����ֵ��
	*	bool = true �ɹ� | false ʧ��
	*	��ϸ���������GetEngineErrorMessage�鿴
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT bool CreateEngine();

	/*
	*���ܣ�
	*	��������
	*������
	*	��
	*����ֵ��
	*	��
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT void RunEngine();

	/*
	*���ܣ�
	*	�ر�����
	*������
	*	_Message = 0 Ϊ�����˳�
	*����ֵ��
	*	��
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT void ShutdownEngine(Magic::Management::MESSAGE _Message, const char* _Text);

	/*
	*���ܣ�
	*	���ع����߳�
	*������
	*	_Callback_Message = ������
	*����ֵ��
	*	��
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT void Load(Magic::Management::Callback_Message _Callback_Message);

	/*
	*���ܣ�
	*	����һ���µ��̲߳����һ������Ա����
	*�̱߳�����
	*	��
	*������
	*	_name = �߳���Դ����Ա����
	*����ֵ��
	*	bool = true �ɹ� | false ʧ��
	*	��ϸ���������GetEngineErrorMessage�鿴
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT bool CreateThreadsResourceManager(const char* _name);

	/*
	*���ܣ�
	*	�رղ��ͷŵ�ǰ�߳���Դ����Ա����
	*������
	*	��
	*����ֵ��
	*	��
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT void ShutdownThreads();

	/*
	*���ܣ�
	*	��ָ��windows���ھ����ָ���߳���Դ����Ա�����д���һ��Opengl��Ⱦģ��
	*������
	*	_hwnd = windows���ھ��
	*	_ThreadResourceManager = �߳���Դ����Ա����
	*����ֵ:
	*	bool = true �ɹ� | false ʧ��
	*	��ϸ���������GetEngineErrorMessage�鿴
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT bool CreateOpenglRender(HWND _hwnd, EntityCommon _ThreadResourceManager);

	DLL_MAGIC_ENGINE_OUTPUT_INPUT void AddDrawMessageNumber(unsigned int _number);

	/*
	*���ܣ�
	*	��ȡ��ǰ�߳��еĻ��ʶ���
	*���棺
	*	�����ڵ�ǰ�̴߳�����Ⱦ����
	*������
	*	��
	*����ֵ��
	*	Magic::Pen_Normal* = ���ʵ�ָ��
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT Magic::Pen_Normal* GetPen_Normal();

	/*
	*���ܣ�
	*	��ȡָ���߳��е��߳���Դ����Ա����
	*�̱߳�����
	*	��
	*������
	*	_name = �߳�����
	*����ֵ��
	*	EntityCommon = �߳���Դ����
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT EntityCommon GetThreadsResourceManager(const char* _name);

	/*
	*���ܣ�
	*	��ȡ��ǰ�߳��е��߳���Դ����Ա����
	*������
	*	��
	*����ֵ��
	*	EntityCommon = �߳���Դ����
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT EntityCommon GetThreadsResourceManager();

	/*
	*���ܣ�
	*	������Ϣ����ǰ�߳�ָ������
	*	���_SceneNameΪ�շ��͵���ǰ�߳�
	*�̱߳�����
	*	��
	*������
	*	_SceneName = �������֣�����ΪNULL��
	*	_MessageStruct = ��Ϣ�ṹ��
	*����ֵ��
	*	bool = true �ɹ� | ʧ��
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT bool SendMessageToScene(const char* _SceneName, Magic::System::MessageStruct _MessageStruct);

	/*
	*���ܣ�
	*	������Ϣ����ǰ�߳�ָ������
	*�̱߳�����
	*	��
	*���棺
	*	���ڶ������Ϊ��ǰ�̵߳Ĵ��ڶ�����������̰߳�ȫ����
	*������
	*	_SceneEntity = ���ڶ���
	*	_MessageStruct = ��Ϣ�ṹ��
	*����ֵ��
	*	bool = true �ɹ� | ʧ��
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT bool SendMessageToScene(const EntityCommon& _SceneEntity, Magic::System::MessageStruct _MessageStruct);

	/*
	*���ܣ�
	*	������Ϣ��ָ���߳�
	*�̱߳�����
	*	��
	*������
	*	_ThreadsName = �߳�����
	*	_MessageStruct = ��Ϣ�ṹ��
	*����ֵ��
	*	bool = true �ɹ� | ʧ��
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT bool SendMessageToThreads(const char* _ThreadsName, Magic::System::MessageStruct _MessageStruct);

	/*
	*���ܣ�
	*	������Ϣ��ָ���߳�
	*�̱߳�����
	*	��
	*������
	*	_ThreadsEntity = �̶߳���
	*	_MessageStruct = ��Ϣ�ṹ��
	*����ֵ��
	*	bool = true �ɹ� | ʧ��
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT bool SendMessageToThreads( EntityCommon& _ThreadsEntity, Magic::System::MessageStruct _MessageStruct);

	/*
	*���ܣ�
	*	������Ϣ��ָ���߳�ָ������
	*	���_SceneNameΪ�շ��͵���ǰ�߳�
	*�̱߳�����
	*	��
	*������
	*	_ThreadsName = �߳�����
	*	_SceneName = �������֣�����ΪNULL��
	*	_MessageStruct = ��Ϣ�ṹ��
	*����ֵ��
	*	bool = true �ɹ� | ʧ��
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT bool SendMessageToThreadsScene(const char* _ThreadsName, const char* _SceneName, Magic::System::MessageStruct _MessageStruct);

	/*
	*���ܣ�
	*	������Ϣ��ָ���߳�ָ������
	*�̱߳�����
	*	��
	*���棺
	*	���ڶ������Ϊָ���̵߳Ĵ��ڶ�����������̰߳�ȫ����
	*������
	*	_ThreadsEntity = �̶߳���
	*	_SceneEntity = ���ڶ���
	*	_MessageStruct = ��Ϣ�ṹ��
	*����ֵ��
	*	bool = true �ɹ� | ʧ��
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT bool SendMessageToThreadsScene( EntityCommon& _ThreadsEntity, const EntityCommon& _SceneEntity, Magic::System::MessageStruct _MessageStruct);

	/*
	*���ܣ�
	*	��ȡ��ǰ�߳��еĲ�ֵʱ��
	*������
	*	��
	*����ֵ��
	*	double = ��1����Ϊ1.0��λ��ʱ���ֵ
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT double GetThreadsDiffTime();

#ifdef _DEBUG 
#define DEBUG_AddDrawMessageNumber(a)			AddDrawMessageNumber(a) 
#else
#define DEBUG_AddDrawMessageNumber(a)
#endif
}

#endif