
#include "Include/MagicEngineAPI.h"
#include "string"
#include "System/Supervisor.h"
#include "Include/MagicEngineContext.h"

namespace Magic
{
	static std::string Overall_EngineErrorMessage;

	const char* GetEngineErrorMessage()
	{
		return Overall_EngineErrorMessage.c_str();
	}

	void SetEngineErrorMessage(const char* _text)
	{
		Overall_EngineErrorMessage = _text;
	}

	void AddEngineErrorMessage(const char* _text, bool _EndORStart)
	{
		if (_EndORStart)
			Overall_EngineErrorMessage += _text;
		else
			Overall_EngineErrorMessage.insert(0, _text);
	}

	bool CreateEngine()
	{
		if (MagicEngineContext::Instance())
		{
			SetEngineErrorMessage("1.Already created\n");
			return false;
		}

		new MagicEngineContext;

		bool _result = MagicEngineContext::Instance()->Initialize();
		if (!_result)
			return false;

		return true;
	}

	void RunEngine()
	{
		MagicEngineContext::Instance()->Render();
	}

	void ShutdownEngine()
	{
		if (MagicEngineContext::Instance())
		{
			delete MagicEngineContext::Instance();
		}
	}

	bool CreateEntityThreads(const char* _name)
	{
		return MagicEngineContext::Instance()->CreateEntityThreads(_name);
	}

	void ShutdownThreads()
	{
		MagicEngineContext::Instance()->GetEntityThreads().GetComponent<Magic::System::ThreadsComponent>()->m_RunState = false;
	}

	bool CreateOpenglRender(HWND _hwnd, EntityCommon _EntityCommon)
	{
		return MagicEngineContext::Instance()->CreateOpenglRender(_hwnd, _EntityCommon);
	}

	void AddDrawMessageNumber(unsigned int _number)
	{
		MagicEngineContext::Instance()->GetEntityThreads().GetComponent<Magic::System::RenderThreadsComponent>()->m_DrawMessageNumber += _number;
	}

	Magic::Pen_Normal* GetPen_Normal()
	{
		return &(MagicEngineContext::Instance()->GetEntityThreads().GetComponent<Magic::System::RenderThreadsComponent>()->m_Pen_Normal);
	}

	EntityCommon GetEntityThreads(const char* _name)
	{
		return MagicEngineContext::Instance()->GetEntityThreads(_name);
	}

	EntityCommon GetEntityThreads()
	{
		return MagicEngineContext::Instance()->GetEntityThreads();
	}

	double GetThreadsDiffTime()
	{
		return MagicEngineContext::Instance()->GetEntityThreads().GetComponent<Magic::System::ThreadsComponent>()->m_DiffTime;
	}
}