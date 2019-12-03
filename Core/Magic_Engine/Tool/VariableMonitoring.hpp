#ifndef _VARIABLE_MONITORING_H_
#define _VARIABLE_MONITORING_H_

#include <set>
#include <functional>

template<class T>
class VariableMonitoring {
public:
	typedef std::function<void(const T&)> Callback;
	VariableMonitoring() :m_Variable(0) {}

	void Monitor(Callback* _Callback);
	void RemoveMonitor(Callback* _Callback);

	void Value(const T& _t);
	inline const T& Value();
	const T& operator=(const T& _t);
	void operator()(const T&);
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
void VariableMonitoring<T>::Value(const T& _t) {
	m_Variable = _t;
}

template<class T>
const T&  VariableMonitoring<T>::Value() {
	return m_Variable;
}

template<class T>
const T& VariableMonitoring<T>::operator=(const T& _t) {
	if (m_Variable != _t) {
		m_Variable = _t;

		for (auto& _auto : m_vec_Callback) {
			(*_auto)(m_Variable);
		}
	}

	return m_Variable;
}

template<class T>
void VariableMonitoring<T>::operator()(const T& _t) {
	for (auto& _auto : m_vec_Callback) {
		(*_auto)(_t);
	}
}

template<class T>
class VariableRecord :public VariableMonitoring<T> {
public:
	const T& operator=(const T& _t);

	const T& back();

	inline size_t LogNumber() {
		return m_vec_Record.size();
	}
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
const T& VariableRecord<T>::back() {
	static T result(0);

	if (m_vec_Record.size()) {
		T _t = m_vec_Record.back();
		m_vec_Record.pop_back();
		return VariableMonitoring<T>::operator=(_t);
	}
	else {
		return result;
	}
}

#endif