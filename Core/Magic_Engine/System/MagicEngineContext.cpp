#include "MagicEngineContext.h"
#include "MagicEngineAPI.h"
#include <GL/glew.h>  
#include "Tool/InjectFunction.h"

const glm::mat4 CONST_CAMERA = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

namespace Magic
{
	const unsigned char* GetRenderer()
	{
		return glGetString(GL_RENDERER);
	}

	const unsigned char* GetVendor()
	{
		return glGetString(GL_VENDOR);
	}

	const unsigned char* GetVersion()
	{
		return glGetString(GL_VERSION);
	}

	const unsigned char* GetGlSLVersion()
	{
		return glGetString(GL_SHADING_LANGUAGE_VERSION);
	}

	int Getmajor()
	{
		GLint major;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		return major;
	}

	int Getminor()
	{
		GLint minor;
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		return minor;
	}
}

MagicEngineContext* MagicEngineContext::pMagicEngineContext = 0;

MagicEngineContext::MagicEngineContext()
{
	pMagicEngineContext = this;
	m_EngineRunState = false;
	m_Error = 0;
}


MagicEngineContext::~MagicEngineContext()
{
	Shutdown();
	pMagicEngineContext = 0;
}

bool MagicEngineContext::Initialize(RenderContext _pRenderContext)
{
	bool _result = Magic::Management::CreateThreadManagement();
	if (!_result)
		return false;

	m_Load_Thread = Magic::Management::CreateThreadObject("Load_Thread", Magic::Management::THREAD_LOOP_RUN, Magic::Management::THREAD_MESSAGE_WAIT);

	_result = m_Render_thread.Initialize((Magic::Render_Context*)_pRenderContext);
	if (!_result)
		return false;

	_result = m_DrawSimpleGraphics.Initialize();
	if (!_result)
		return false;

	Inject_function("LoadEffects", NULL, true);

	return true;
}

void MagicEngineContext::ShutdownMessage(unsigned int _Message, const char* _Text) {
	std::string _str_text = _Text;

	Magic::Management::SendMessageTo(MAGIC_MAIN_THREAD_NAME, Magic::SHUTOWN_ENGINE, _Message,
		[this, _str_text](Magic::Management::MESSAGE_TYPE, Magic::Management::MESSAGE _Message) {
		m_Error = (unsigned int)_Message;
		Magic::SetEngineErrorMessage(_str_text.c_str());
		m_EngineRunState = false;
	});
}

void MagicEngineContext::Shutdown()
{
	for (std::map<std::string, MagicTexture*>::iterator i = Map_Texture.begin(); i != Map_Texture.end(); i++)
		delete i->second;

	Magic::Management::ShutdownThreadManagement();

	DeleteGSAll();
}

unsigned int MagicEngineContext::Run(void)
{
	Magic::RenderThread([](Magic::Management::MESSAGE_TYPE, Magic::Management::MESSAGE) {
		Inject_function("EngineRunStart_RT", NULL, true);
	});

	m_EngineRunState = true;

	while (m_EngineRunState)
	{
		Magic::Management::UpdataThreadManagement();
	}

	unsigned int _Error = m_Error;

	delete pMagicEngineContext;

	return _Error;
}

void MagicEngineContext::LoadThread(const Magic::Management::Callback_Message& _Callback_Message)
{
	Magic::Management::SendMessageTo(m_Load_Thread, 0, 0, _Callback_Message);
}


MagicTexture* MagicEngineContext::LoadTextrue(const char* file_name, const char* _name, char format)
{
	bool result;
	MagicTexture* pTextrue;
	pTextrue = new MagicTexture;
	if (!pTextrue)
		return 0;
	result = pTextrue->Initialize(file_name, format);
	if (!result)
		return 0;
	Map_Texture.insert(std::map<std::string, MagicTexture*>::value_type(_name, pTextrue));

	return pTextrue;
}

MagicTexture* MagicEngineContext::LoadTextrue(const unsigned char* Data, int _width, int _height, const char* _name)
{
	bool result;
	MagicTexture* pTextrue;
	pTextrue = new MagicTexture;
	if (!pTextrue)
		return 0;
	result = pTextrue->Initialize(Data, _width, _height, MagicTexture::RGBA, MagicTexture::UNSIGNED_BYTE);
	if (!result)
		return 0;
	Map_Texture.insert(std::map<std::string, MagicTexture*>::value_type(_name, pTextrue));

	return pTextrue;
}

void MagicEngineContext::DeleteTextrue(const char* _name)
{
	auto _auto = Map_Texture.find(_name);
	if (_auto != Map_Texture.end())
	{
		delete _auto->second;
		Map_Texture.erase(_auto);
	}

	return;
}

MagicTexture* MagicEngineContext::GetTextrue(const char* _name)
{
	auto _auto = Map_Texture.find(_name);
	if (_auto != Map_Texture.end())
		return _auto->second;
	else
		return 0;
}