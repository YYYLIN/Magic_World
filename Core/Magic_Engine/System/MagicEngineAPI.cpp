
#include "MagicEngineAPI.h"
#include "string"
#include "MagicEngineContext.h"
#include "Cross_Platform_Port.h"

namespace Magic
{
#define  ENGINE_ERROR_MESSAGE_BUFFER_SIZE 256
	static S_THREAD char Overall_EngineErrorMessage[ENGINE_ERROR_MESSAGE_BUFFER_SIZE] = { 0 };

	const char* GetEngineErrorMessage()
	{
		return Overall_EngineErrorMessage;
	}

	void SetEngineErrorMessage(const char* _text)
	{
		size_t _len = strlen(_text);
		memcpy(Overall_EngineErrorMessage, _text, _len > ENGINE_ERROR_MESSAGE_BUFFER_SIZE ? ENGINE_ERROR_MESSAGE_BUFFER_SIZE : _len);
	}

	bool CreateEngine(RenderContext _pRenderContext)
	{
		if (MagicEngineContext::Instance())
		{
			SetEngineErrorMessage("1.Already created\n");
			return false;
		}

		new MagicEngineContext;

		bool _result = MagicEngineContext::Instance()->Initialize(_pRenderContext);
		if (!_result)
			return false;

		return true;
	}

	unsigned int RunEngine()
	{
		return MagicEngineContext::Instance()->Run();
	}

	void ShutdownEngine(unsigned int _Message, const char* _Text)
	{
		if (MagicEngineContext::Instance())
		{
			MagicEngineContext::Instance()->ShutdownMessage(_Message, _Text);
		}
	}

	void ShutdownEngine(unsigned int _Message) {
		ShutdownEngine(_Message, GetEngineErrorMessage());
	}

	void Load(const Magic::Management::Callback_Message& _Callback_Message) {
		MagicEngineContext::Instance()->LoadThread(_Callback_Message);
	}

	void Engine(const Magic::Management::Callback_Void& _Callback) {
		Magic::Management::MonitorThread(MAGIC_MAIN_THREAD_NAME, _Callback);
	}

	void ShutdownMessage(const Magic::Management::Callback_Message& _Callback_Message) {
		Magic::Management::MonitorThreadMessage(MAGIC_MAIN_THREAD_NAME, Magic::SHUTOWN_ENGINE, _Callback_Message);
	}

	RenderContext CreateRenderContext(HWND _hwnd) {
		Render_Context_Opengl* _pRender_Context = new Render_Context_Opengl;

		bool _result = _pRender_Context->CreateRenderContext(_hwnd);
		if (!_result) {
			delete _pRender_Context;
			return NULL;
		}

		return (Render_Context*)_pRender_Context;
	}

	void ShutdownRenderContext(RenderContext _RenderContext) {
		Render_thread::Instance()->ShutdownRC((Render_Context*)_RenderContext);
	}

	void BindRenderContext(RenderContext _RenderContext) {
		Render_thread::Instance()->BindRC((Render_Context*)_RenderContext);
	}

	void SetRenderContextRect(const Screen_Rect& _Screen_Rect) {
		Render_thread::Instance()->SetRect(_Screen_Rect);
	}

	const FONT CreateFONT(const char* _Name, const char* _Path, unsigned int _w, unsigned int _h) {
		FONT _FONT = 0;
		Magic::RenderThread([&_FONT, _Name, _Path, _w, _h](MM_MESS) {
			Magic::LRU_Font_Texture* _Font = MagicEngineContext::Instance()->CreateFont_Texture(_Name);
			if (!_Font) {
				_FONT = 0;
				return;
			}

			Magic::FT_Font* _pFT_Font = MagicEngineContext::Instance()->CreateFT_Font(_Name);;
			int _Error = _pFT_Font->Initialize(_Path, _w, _h);
			if (_Error) {
				_FONT = 0;
				return;
			}

			bool _result = _Font->Initialize(_pFT_Font);
			if (!_result) {
				MagicEngineContext::Instance()->DeleteFont_Texture(_Name);
				MagicEngineContext::Instance()->DeleteFT_Font(_Name);
				_FONT = 0;
				return;
			}

			_FONT = (FONT)_Font;
		}, true);

		return _FONT;
	}

	const FONT GetFONT(const char* _Name) {
		FONT _FONT = 0;
		Magic::RenderThread([&_FONT, _Name](MM_MESS) {
			_FONT = (FONT)MagicEngineContext::Instance()->GetFont_Texture(_Name);
		}, true);
		return (FONT)_FONT;
	}

	void DeleteFONT(const char* _Name) {
		Magic::RenderThread([_Name](MM_MESS) {
			MagicEngineContext::Instance()->DeleteFont_Texture(_Name);
		}, true);

	}
}