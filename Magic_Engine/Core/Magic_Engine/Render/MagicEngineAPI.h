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

	DLL_MAGIC_ENGINE_OUTPUT_INPUT void AddEngineErrorMessage(const char* _text, bool _EndORStart = true);

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
	*	��
	*����ֵ��
	*	��
	*/
	DLL_MAGIC_ENGINE_OUTPUT_INPUT void ShutdownEngine();
	
	/*
	*���ܣ�
	*	����һ���µ��̲߳����һ������Ա����
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

	template <typename T>
	bool CreateScene(EntityCommon _ParentEntity, T** _SceneCommon)
	{
		bool _AutoRelase = false;
		if (!*_SceneCommon)
		{
			*_SceneCommon = new T;
			_AutoRelase = true;
		}

		bool _result = (*_SceneCommon)->SceneCommon::Initialize(_ParentEntity, *_SceneCommon, _AutoRelase);
		if (!_result)
			return false;

		return true;
	}
}

#endif