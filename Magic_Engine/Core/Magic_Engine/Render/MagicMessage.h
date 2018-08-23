#ifndef _MAGICMESSAGE_H_
#define _MAGICMESSAGE_H_

#include "Define/Magic_Macro.h"
#include <stdint.h>
#include <vector>

#pragma warning(push)
#pragma warning(disable:4251)

namespace Magic
{
	typedef void(*CallBack_Common)(void* _data);

	struct MESSAGE
	{
		MESSAGE() :MessageType(0), Message(0) {}
		MESSAGE(uint32_t _MessageType, uint64_t _Message) :MessageType(_MessageType), Message(_Message) {}
		uint32_t MessageType;
		uint64_t Message;
	};

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
		virtual void Send_Message(uint32_t _MessageType, uint64_t _Message);
	protected:
		virtual int MessageHandle(uint32_t _MessageType, uint64_t _Message) = 0;
	};
}

#pragma warning(pop)

#endif