
template <class T>
MagicList<T>::MagicList()
{
	pFirst = 0;
	pEnd = 0;
	m_Number = 0;
}

template <class T>
MagicList<T>::~MagicList()
{
	Delete();
}

template <class T>
void MagicList<T>::Add(const T& _data)
{
	if (!m_Number)
	{
		pEnd = new OneWayNode;
		pEnd->data = _data;
		pEnd->pNext = NULL;
		pFirst = pEnd;
		m_Number++;
	}
	else
	{
		pEnd->pNext = new OneWayNode;
		pEnd->pNext->data = _data;
		pEnd->pNext->pNext = NULL;
		pEnd = pEnd->pNext;
		m_Number++;
	}
}

template <class T>
void MagicList<T>::Delete()
{
	for (OneWayNode* _pOneWayNode = pFirst;_pOneWayNode != NULL;_pOneWayNode = pFirst)
	{
		pFirst = _pOneWayNode->pNext;
		delete _pOneWayNode;
	}
	pFirst = 0;
	pEnd = 0;
	m_Number = 0;
}