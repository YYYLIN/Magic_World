#ifndef _TEMPLATE_EFFECTS_H_
#define _TEMPLATE_EFFECTS_H_

#include <unordered_map>
#include <functional>
#include <string>
#include "Tool/VariableMonitoring.hpp"

#define BindClassFunction_1(F) std::bind(F, this, std::placeholders::_1)
#define BindClassFunction_1(F,O) std::bind(F, O, std::placeholders::_1)

namespace Magic {

	class Template_Effects;

	typedef Template_Effects* PTemplate_Effects;

	typedef	std::function<void(const PTemplate_Effects&)> Fun_Template_Effects;

	bool TemplateEffects(const char* _Name,const Fun_Template_Effects& _Fun);

	bool TemplateEffects(Template_Effects* _pTemplate_Effects, const Fun_Template_Effects& _Fun);

	bool EnableTemplateEffects(const char* _Name);

	bool EnableTemplateEffects(Template_Effects* _pTemplate_Effects);

	bool DisableTemplateEffects(const char* _Name);

	bool DisableTemplateEffects(Template_Effects* _pTemplate_Effects);

	void MonitorTemplateEffects(const Fun_Template_Effects& _Fun);

	void RemoveMonitorTemplateEffects(const Fun_Template_Effects& _Fun);

	class Template_Effects {
	public:
		Template_Effects(const char* _Name);
		~Template_Effects();

		virtual void Render() = 0;
	private:
		std::string m_Name;
	};
}


#endif