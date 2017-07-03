#ifndef _MAGICBERTEXBUFFEROBJECT_H_
#define _MAGICBERTEXBUFFEROBJECT_H_

class MagicVertexBufferObject;

typedef MagicVertexBufferObject* PMagicVertexBufferObject;

class MagicVertexBuffer
{

public:
	MagicVertexBuffer();
	~MagicVertexBuffer();

	inline PMagicVertexBufferObject operator[](unsigned char _number) { return pMagicVertexBufferObject[_number]; }

	void Shutdown();
	void BindBuffer(unsigned char _pos, MagicVertexBufferObject* _MagicVertexBufferObject);
	void EnableVertexAttribArray(unsigned char _number);

	inline unsigned char GetNumber() { return m_number; }
private:
	PMagicVertexBufferObject* pMagicVertexBufferObject;
	unsigned char m_number;
};

class MagicVertexBufferObject
{
public:
	MagicVertexBufferObject();
	~MagicVertexBufferObject();

	void Shutdown();

	inline unsigned int GetTypeSize() { return m_TypeSize; }
	inline unsigned short GetType() { return m_Type; }
	inline unsigned char GetSize() { return m_size; }
	void GetVertex(const unsigned int& _pos, void* _pdata);

	void SetBufferData(const void* _pdata, const unsigned int& _size);
	void SetVertexAttribPointer(unsigned char _Size, const unsigned short& _type);
private:
	void* pBuffer;
	//每个顶点大小等于sizef(m_Type) * m_size
	unsigned char m_size;
	unsigned short m_Type;
	unsigned char m_TypeSize;
};

#endif