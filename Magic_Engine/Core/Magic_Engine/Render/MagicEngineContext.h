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
/*

class DLL_MAGIC_ENGINE_OUTPUT_INPUT MagicScenes :public MagicUICommon
{
	friend class Magic::MessageScenes;
public:
	MagicScenes();
	virtual ~MagicScenes();
	bool Initialize(MagicScenes* _scene, glm::vec4 _PosSize);

	virtual void SetDisplayState(bool _state);

	virtual glm::vec2 GetFrameBufferSize();

	inline bool GetDisplayState() { return DisplayState; }

	inline glm::vec2 GetDrawPos() { return m_DrawPos; }

	virtual MagicFBOTexture* GetFBOTexture() { return 0; }

	void AddCommon(MagicCommon*);
	void RemoveCommon(MagicCommon*);

	virtual void ResetDrawRECT(float _x, float _y, float _w, float _h);

protected:
	virtual bool OnInitialize() { return true; }
	virtual void Updata();
	virtual	void Render(glm::vec2 _DrawPos);
	virtual void RenderStart();
	virtual void RenderEnd();

protected:
	bool DisplayState;

	glm::vec4 m_PosSize;
	glm::vec2 m_DrawPos;
	MagicScenes* pParentScene;

	std::vector<MagicCommon*> v_Common;
};*/

/*
class DLL_MAGIC_ENGINE_OUTPUT_INPUT MagicScenesEx :public MagicScenes
{
public:
	MagicScenesEx();

	virtual void DrawSpirit();
	virtual void SetDisplayState(bool);

	virtual glm::vec2 GetFrameBufferSize();

	inline virtual MagicFBOTexture* GetFBOTexture() { return &m_FBOBuffer; }

	virtual bool Initialize(MagicScenes* _scene, glm::vec4 _PosSize);

	virtual void CopyFBOBuffer();
protected:
	virtual void Render(glm::vec2 _DrawPos);
	virtual void RenderBuffer();

	virtual void RenderStart();
	virtual void RenderEnd();

protected:
	bool m_DrawMessage;

	MagicFBOTexture m_FBOBuffer;
};*/
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
	struct SceneCommonBox
	{
		SceneCommonBox(Magic::SceneCommon* _pSceneCommon, bool _AutoRelease)
			:pSceneCommon(_pSceneCommon), AutoRelease(_AutoRelease) {}

		Magic::SceneCommon* pSceneCommon;
		bool AutoRelease;
	};
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

	bool AddSceneCommon(Magic::SceneCommon* _pSceneCommon, bool _AutoRelease);
	bool DeleteSceneCommon(const char* _name);

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

	Magic::SceneCommon* GetSceneCommon(const char* _name);

	MagicTexture* GetTextrue(const char* _name);

private:
	static DWORD WINAPI UpdataThread(LPVOID lpParameter);
	void UpdataThread();

private:
	std::map<std::string, MagicTexture*> Map_Texture;
	std::map<std::string, SceneCommonBox> M_SceneCommonBox;
	std::map<std::string, EntityCommon> M_EntityThreads;

	EntityX::EntityX m_Supervisor;

	CRITICAL_SECTION m_MutexThreadsResourceManager;

	//�ֲ߳̾���̬����
	static __declspec(thread) EntityCommon* S_T_pEntityCommon;

	static MagicEngineContext* pMagicEngineContext;
};

#pragma warning(pop)