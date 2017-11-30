#ifndef _MAGICMESSAGE_H_
#define _MAGICMESSAGE_H_

class MagicMessageScenes;

class MagicMessageCommon
{
	friend class MagicMessageScenes;
public:
	void Send_Message(unsigned int _MessageType, unsigned int _Message);
protected:
	virtual int MessageHandle(unsigned int _MessageType, unsigned int _Message) = 0;
};

class MagicMessageScenes :public MagicMessageCommon
{
protected:
	virtual int MessageHandle(unsigned int _MessageType, unsigned int _Message);
};


#endif