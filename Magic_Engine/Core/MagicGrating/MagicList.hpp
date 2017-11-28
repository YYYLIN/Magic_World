#ifndef _MAGICLIST_H_
#define _MAGICLIST_H_

template <class T>
struct OneWayNode
{
	T data;
	OneWayNode* pNext;
};

template <class T>
class MagicList
{
public:
	MagicList();
	~MagicList();
	void Add(const T& _data);
	void Delete();
private:
	OneWayNode* pFirst;
	OneWayNode* pEnd;
	unsigned int m_Number;
};

#include "MagicList.inl"

#endif
