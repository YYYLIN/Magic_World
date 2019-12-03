#ifndef _TEMPLATE_EFFECTS_H_
#define _TEMPLATE_EFFECTS_H_

#include <unordered_map>
#include <functional>
#include <string>
#include "Tool/VariableMonitoring.hpp"
#include "Define/MagicType.h"
#include "Define/Magic_Macro.h"
#include "MagicFBO.h"
#include "Tool/InjectFunction.h"
#include "Tool/GlobalStaticMemory.h"

#define BindClassFunction_F_1(F) std::bind(F, this, std::placeholders::_1)
#define BindClassFunction_FO_1(F,O) std::bind(F, O, std::placeholders::_1)

#define LOAD_TEMPLATE_EFFECTS(C) INJECT_FUNCTION("LoadEffects",		\
									[](){							\
										C* _pC = NewGS<C>();				\
										if(!_pC->Initialize()){		\
											::Magic::ShutdownEngine(1,STRING_AND_STRING(C,_Initialize_Fail!));\
										}							\
									})

namespace Magic {

	class Template_Effects;

	typedef Template_Effects* PTemplate_Effects;

	typedef	std::function<void(const ::Magic::PTemplate_Effects&)> Fun_Template_Effects;

	bool DLL_MAGIC_ENGINE_OUTPUT_INPUT TemplateEffects(const char* _Name, const Fun_Template_Effects& _Fun);

	bool DLL_MAGIC_ENGINE_OUTPUT_INPUT TemplateEffects(Template_Effects* _pTemplate_Effects, const Fun_Template_Effects& _Fun);

	bool DLL_MAGIC_ENGINE_OUTPUT_INPUT EnableTemplateEffects(const char* _Name);

	bool DLL_MAGIC_ENGINE_OUTPUT_INPUT EnableTemplateEffects(Template_Effects* _pTemplate_Effects);

	bool DLL_MAGIC_ENGINE_OUTPUT_INPUT DisableTemplateEffects(const char* _Name);

	bool DLL_MAGIC_ENGINE_OUTPUT_INPUT DisableTemplateEffects(Template_Effects* _pTemplate_Effects);

	void DLL_MAGIC_ENGINE_OUTPUT_INPUT MonitorTemplateEffects(Fun_Template_Effects* _Fun);

	void DLL_MAGIC_ENGINE_OUTPUT_INPUT RemoveMonitorTemplateEffects(Fun_Template_Effects* _Fun);

	size_t DLL_MAGIC_ENGINE_OUTPUT_INPUT NumberOfLayersEffects();

	class Template_Effects {
	public:
		Template_Effects(const char* _Name);
		virtual ~Template_Effects();

		void Rect(const Magic::Screen_Rect& _Rect);

		inline const Magic::Screen_Rect& Rect() { return m_Rect; }

		void RenderRequest(Template_Effects* _pTemplate_Effects);

		virtual MagicFBO* GetFBO() = 0;
	protected:
		//RT线程函数
		virtual void RenderStart() = 0;
		//RT线程函数
		virtual void RenderEnd() = 0;
		//RT线程函数
		virtual void RenderToTarget(Template_Effects* _pTemplate_Effects) = 0;

		virtual bool SynchRender() { return false; }
	private:
		void Target(Template_Effects* _pTemplate_Effects);

		void RenderTarget(Template_Effects* _pTemplate_Effects);

	private:
		std::string m_Name;
		Magic::Screen_Rect m_Rect;
		std::set<Template_Effects*> m_vec_Template_Effects;
	};
}


#endif