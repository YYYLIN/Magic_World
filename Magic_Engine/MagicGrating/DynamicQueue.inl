

template<class TYPE>
DynamicQueue<TYPE>::DynamicQueue()
{
	pData = 0;
	m_Size = 0;
	m_EndPos = 0;
	m_MaxSize = 0;
}

template<class TYPE>
DynamicQueue<TYPE>::~DynamicQueue()
{
	Delete();
}

template<class TYPE>
void DynamicQueue<TYPE>::operator<<(const TYPE& _value)
{
	Input(_value);
}

template<class TYPE>
void DynamicQueue<TYPE>::operator>>(TYPE* _value)
{
	Output(_value);
}

template<class TYPE>
void DynamicQueue<TYPE>::Input(const TYPE& _value)
{
	if (m_MaxSize <= m_Size)
	{
		unsigned int _MaxSize = (m_MaxSize == 0 ? 16 : m_MaxSize << 2);
		TYPE* pDataNew = (TYPE*)realloc(pData, _MaxSize * sizeof(TYPE));
		pData = pDataNew;
		m_MaxSize = _MaxSize;
	}

	if (m_EndPos >= m_MaxSize)
		m_EndPos = 0;

	pData[m_EndPos] = _value;
	++m_EndPos;
	++m_Size;
}

template<class TYPE>
void DynamicQueue<TYPE>::Output(TYPE* _value)
{
	if (m_Size)
	{
		*_value = pData[m_EndPos - m_Size >= 0 ? m_EndPos - m_Size : m_EndPos - m_Size + m_MaxSize];
		--m_Size;
	}
}


template<class TYPE>
void DynamicQueue<TYPE>::Delete()
{
	free(pData);
	pData = 0;
	m_Size = 0;
	m_EndPos = 0;
	m_MaxSize = 0;
}