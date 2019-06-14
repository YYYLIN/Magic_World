#ifndef _VARIABLE_MONITORING_H_
#define _VARIABLE_MONITORING_H_

#include <vector>
#include <functional>

template<class T>
class VariableMonitoring {
public:
	typedef std::function<void(const T&)> Callback;

	void Monitor(const Callback& _Callback);

	const T& operator=();
	const T& operator=(const T& _t);
private:
	T m_Variable;
	std::vector<Callback> m_vec_Callback;
};

template<class T>
void VariableMonitoring<T>::Monitor(const Callback& _Callback) {
	m_vec_Callback.push_back(_Callback);
}

template<class T>
const T&  VariableMonitoring<T>::operator=() {
	return m_Variable;
}

template<class T>
const T&  VariableMonitoring<T>::operator=(const T& _t) {
	if (m_Variable != _t) {
		m_Variable = _t;

		for (auto& _auto : m_vec_Callback) {
			_auto(m_Variable);
		}
	}
}

#endif