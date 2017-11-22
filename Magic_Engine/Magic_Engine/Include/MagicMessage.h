#ifndef _MAGICMESSAGE_H_
#define _MAGICMESSAGE_H_



class MagicMessage
{
public:
/*
	MagicMessage();
	~MagicMessage();*/

	virtual void MessageHandle(unsigned int _MessageType, unsigned int _Message) = 0;
};


#endif