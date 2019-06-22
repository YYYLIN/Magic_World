#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform2.hpp>
#include <map>
#include <Vector>
#include <list>
#include "windows.h"
#include "Render/MagicTexture.h"
#include "Render/MagicPen.h"
#include "Define/MagicType.h"
#include "Magic_Thread.h"
#include "Render/Render_thread.h"
#include "Render/DrawSimpleGraphics.h"
#include "Render/MainTemplateEffects.h"

#pragma warning(push)
#pragma warning(disable:4251)

extern const glm::mat4 CONST_CAMERA;

class MagicEngineContext;

namespace Magic
{
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

	HGLRC CreateRCContxt(HDC _hdc);

	void ShutdownRC(HGLRC _hRC);

	bool CreateHD(HDC _hdc);
}

class MagicEngineContext
{
public:
	MagicEngineContext();
	~MagicEngineContext();

	static MagicEngineContext* Instance() { return pMagicEngineContext; }


	bool Initialize(RenderContext _pRenderContext);
	//_Message = 0 Ϊ�����˳�
	void ShutdownMessage(Magic::Management::MESSAGE _Message, const char* _Text);
	void Shutdown();

	void Run(void);

	void LoadThread(const Magic::Management::Callback_Message& _Callback_Message);

	MagicTexture* LoadTextrue(const char* file_name, const char* _name, char format);
	MagicTexture* LoadTextrue(const unsigned char* Data, int _width, int _height, const char* _name);

	void DeleteTextrue(const char* _name);

	MagicTexture* GetTextrue(const char* _name);

private:
	bool m_EngineRunState;

	std::map<std::string, MagicTexture*> Map_Texture;

	Magic::Render_thread m_Render_thread;
	Magic::Management::THREAD_OBJECT m_Load_Thread;
	DrawSimpleGraphics m_DrawSimpleGraphics;
	Magic::Main_Template_Effects m_Main_Template_Effects;

	static MagicEngineContext* pMagicEngineContext;
};

#pragma warning(pop)