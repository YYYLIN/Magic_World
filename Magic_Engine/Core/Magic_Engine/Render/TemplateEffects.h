#ifndef _TEMPLATE_EFFECTS_H_
#define _TEMPLATE_EFFECTS_H_

#include <unordered_map>
#include <string>
#include "Tool/VariableMonitoring.hpp"

namespace Magic {

	void TemplateEffects(const char* _Name);

	class Template_Effects {
	public:
		Template_Effects(const char* _Name);
		~Template_Effects();

	protected:
		virtual void Render() = 0;
	private:
		std::string m_Name;
	};
}


#endif