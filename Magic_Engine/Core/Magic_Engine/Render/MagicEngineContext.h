#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform2.hpp>
#include <map>
#include <Vector>
#include <list>
#include "windows.h"
#include "MagicTexture.h"
#include "MagicPen.h"
#include "MagicMessage.h"
#include "System/Supervisor.h"
#include "Define/MagicType.h"
#include "MagicScene.h"
#include "Magic_Thread.h"

#pragma warning(push)
#pragma warning(disable:4251)

extern const glm::mat4 CONST_CAMERA;

class MagicScenes;
class MagicEngineContext;

class DLL_MAGIC_ENGINE_OUTPUT_INPUT MagicCommon
{
	friend MagicScenes;
	friend MagicEngineContext;
public:
	MagicCommon() = default;
	virtual ~MagicCommon()
	{}
	virtual void Updata() { this->OnUpdata(); }
	virtual void Render(glm::vec2 _DrawPos) { this->Draw(); }
protected:
	inline virtual void OnUpdata() {}
	virtual	void Draw() {}
};

class DLL_MAGIC_ENGINE_OUTPUT_INPUT MagicUICommon :public MagicCommon
{
public:
	MagicUICommon() = default;
	virtual ~MagicUICommon() {}

	virtual void DrawSpirit() {}
};

namespace Magic
{
	HGLRC CreateRCContxt(HDC _hdc);

	double EngineUpdataTime(EntityCommon _Entity);
	void EngineUpdataFPS(EntityCommon _Entity);

	void EngineRenderStart(EntityCommon _Entity);
	void EngineRenderEnd(EntityCommon _Entity);

	//�����Ⱦ��
	const unsigned char* GetRenderer();
	//���ͼ�ο���Ӧ��
	const unsigned char* GetVendor();
	//�汾��
	const unsigned char* GetVersion();
	//��Ⱦ���汾��
	const unsigned char* GetGlSLVersion();
	//��汾��
	int Getmajor();
	//С�汾��
	int Getminor();
}

class MagicEngineContext
{
public:
	MagicEngineContext();
	~MagicEngineContext();

	static MagicEngineContext* Instance() { return pMagicEngineContext; }


	bool Initialize();
	void Shutdown();

	void Render(void);

	MagicTexture* LoadTextrue(const char* file_name, const char* _name, char format);
	MagicTexture* LoadTextrue(const unsigned char* Data, int _width, int _height, const char* _name);

	/*
	*���ܣ�
	*	����һ���µ��̲߳����һ������Ա����
	*������
	*	_name = �߳���Դ����Ա����
	*����ֵ��
	*	bool = true �ɹ� | false ʧ��
	*��ϸ���������GetEngineErrorMessage�鿴
	*/
	bool CreateThreadsResourceManager(const char* _name);

	bool CreateOpenglRender(HWND _hwnd, EntityCommon _EntityCommon);

	void DeleteTextrue(const char* _name);

	EntityX::EntityX* GetSupervisor() { return &m_Supervisor; }
	EntityCommon GetThreadsResourceManager(const char* _name);

	/*
	���ܣ�
		��ȡ��ǰ�߳��е��߳���Դ����
	������
		��
	����ֵ��
		EntityCommon = �߳���Դ����
	*/
	EntityCommon GetThreadsResourceManager();

	MagicTexture* GetTextrue(const char* _name);

private:
	static DWORD WINAPI UpdataThread(LPVOID lpParameter);
	void UpdataThread();

private:
	std::map<std::string, MagicTexture*> Map_Texture;
	std::map<std::string, EntityCommon> M_EntityThreads;

	EntityX::EntityX m_Supervisor;

	CRITICAL_SECTION m_MutexThreadsResourceManager;

	//�ֲ߳̾���̬����
	static __declspec(thread) EntityCommon* S_T_pEntityCommon;

	static MagicEngineContext* pMagicEngineContext;
};

#pragma warning(pop)