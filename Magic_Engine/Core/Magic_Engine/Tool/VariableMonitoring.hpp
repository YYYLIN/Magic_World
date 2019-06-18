#ifndef _VARIABLE_MONITORING_H_
#define _VARIABLE_MONITORING_H_

#include <vector>
#include <functional>

template<class T>
class VariableMonitoring {
public:
	typedef std::function<void(const T&)> Callback;

	void Monitor(const Callback& _Callback);
	void RemoveMonitor(const Callback& _Callback);

	inline const T& Value();
	const T& operator=(const T& _t);
protected:
	T m_Variable;
	std::vector<Callback> m_vec_Callback;
};

template<class T>
void VariableMonitoring<T>::Monitor(const Callback& _Callback) {
	m_vec_Callback.push_back(_Callback);
}

template<class T>
void VariableMonitoring<T>::RemoveMonitor(const Callback& _Callback) {
	for (std::vector<Callback>::iterator _i = m_vec_Callback.begin();_i != m_vec_Callback.end();) {
		if (*_i == _Callback) {
			_i = m_vec_Callback.erase(_i);
		}
		else
			++_i;
	}
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
			_auto(m_Variable);
		}
	}
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
		VariableMonitoring<T>::operator =(_t);
	}
}

template<class T>
void VariableRecord<T>::back() {
	if (m_vec_Record.size()) {
		VariableMonitoring<T>::operator=(m_vec_Record.back());
		m_vec_Record.erase(m_vec_Record.end() - 1);
	}
}

#endif