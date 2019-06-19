#ifndef _VARIABLE_MONITORING_H_
#define _VARIABLE_MONITORING_H_

#include <set>
#include <functional>

template<class T>
class VariableMonitoring {
public:
	typedef std::function<void(const T&)> Callback;

	void Monitor(Callback* _Callback);
	void RemoveMonitor(Callback* _Callback);

	inline const T& Value();
	const T& operator=(const T& _t);
protected:
	T m_Variable;
	std::set<Callback*> m_vec_Callback;
};

template<class T>
void VariableMonitoring<T>::Monitor(Callback* _Callback) {
	m_vec_Callback.insert(_Callback);
}

template<class T>
void VariableMonitoring<T>::RemoveMonitor(Callback* _Callback) {
	m_vec_Callback.erase(_Callback);
}

template<class T>
const T&  VariableMonitoring<T>::Value() {
	return m_Variable;
}

template<class T>
const T&  VariableMonitoring<T>::operator=(const T& _t) {
	if (m_Variable != _t) {
		m_Variable = _t;

		for (auto& _auto : m_vec_Callback) {
			(*_auto)(m_Variable);
		}
	}

	return m_Variable;
}

template<class T>
class VariableRecord :public VariableMonitoring<T> {
public:
	const T& operator=(const T& _t);

	void back();
protected:
	std::vector<T> m_vec_Record;
};

template<class T>
const T& VariableRecord<T>::operator=(const T& _t) {
	if (m_Variable != _t) {
		m_vec_Record.push_back(m_Variable);
	}
	return VariableMonitoring<T>::operator =(_t);
}

template<class T>
void VariableRecord<T>::back() {
	if (m_vec_Record.size()) {
		VariableMonitoring<T>::operator=(m_vec_Record.back());
		m_vec_Record.erase(m_vec_Record.end() - 1);
	}
}

#endif