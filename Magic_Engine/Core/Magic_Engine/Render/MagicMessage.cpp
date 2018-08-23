#include "MagicMessage.h"
#include "MagicEngineContext.h"

namespace Magic
{
	CALLBACK_COMMON::CALLBACK_COMMON() :pCallBack_Common(NULL), m_Data(0)
	{}

	CALLBACK_COMMON::CALLBACK_COMMON(CallBack_Common _pCallBack, void* _data) : pCallBack_Common(_pCallBack), m_Data(_data)
	{}

	CALLBACK_COMMON::CALLBACK_COMMON(CallBack_Common _pCallBack) : pCallBack_Common(_pCallBack), m_Data(0)
	{}

	void CALLBACK_COMMON::operator()()
	{
		pCallBack_Common(m_Data);
	}

	void MessageCommon::Send_Message(uint32_t _MessageType, uint64_t _Message)
	{
		this->MessageHandle(_MessageType, _Message);
	}
}