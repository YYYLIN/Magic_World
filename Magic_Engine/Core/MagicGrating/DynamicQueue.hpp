#ifndef _DYNAMICQUEUE_HPP
#define _DYNAMICQUEUE_HPP

template<class TYPE>
class DynamicQueue
{
public:
	DynamicQueue();
	~DynamicQueue();

	void operator<<(const TYPE& _value);
	void operator>>(TYPE* _value);

	void Input(const TYPE& _value);
	void Output(TYPE* _value);

	void Delete();

	inline unsigned int GetSize() { return m_Size; }
private:
	TYPE* pData;
	unsigned int m_EndPos;
	unsigned int m_Size;
	unsigned int m_MaxSize;
};

#include "DynamicQueue.inl"

#endif