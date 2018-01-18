#ifndef _MAGICMESSAGE_H_
#define _MAGICMESSAGE_H_

#include "Magic_Macro.h"

namespace Magic
{
	typedef void(*CallBack_Common)(void* _data);

	typedef struct DLL_MAGIC_ENGINE_OUTPUT_INPUT CALLBACK_COMMON
	{
		CallBack_Common pCallBack_Common;
		void* m_Data;
		CALLBACK_COMMON();
		CALLBACK_COMMON(CallBack_Common _pCallBack);
		CALLBACK_COMMON(CallBack_Common _pCallBack, void* _data);
		void operator()();
	}*PCALLBACK_COMMON;

	class MessageScenes;

	class DLL_MAGIC_ENGINE_OUTPUT_INPUT MessageCommon
	{
		friend class MessageScenes;
	public:
		void Send_Message(unsigned int _MessageType, unsigned int _Message);
	protected:
		virtual int MessageHandle(unsigned int _MessageType, unsigned int _Message) = 0;
	};

	class DLL_MAGIC_ENGINE_OUTPUT_INPUT MessageScenes :public MessageCommon
	{
	protected:
		virtual int MessageHandle(unsigned int _MessageType, unsigned int _Message);
	};
}

#endif