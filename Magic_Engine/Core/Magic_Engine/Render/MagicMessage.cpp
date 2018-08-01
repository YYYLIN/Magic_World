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

	void MessageScenes::Send_Message(uint32_t _MessageType, uint64_t _Message)
	{
		V_Message.push_back(MESSAGE(_MessageType, _Message));
	}

	void MessageScenes::ProcessMessage()
	{
		for (auto _message : V_Message)
		{
			this->MessageHandle(_message.MessageType, _message.Message);
		}
		V_Message.clear();
	}

	int MessageScenes::MessageHandle(uint32_t _MessageType, uint64_t _Message)
	{
/*
		MagicScenes* _pMagicScene = dynamic_cast<MagicScenes*>(this);
		if (_pMagicScene && _pMagicScene->GetDisplayState())
		{
			bool _RETURN_FINISH = false;
			for (std::vector<MagicCommon*>::reverse_iterator _iterator = _pMagicScene->v_Common.rbegin();
				_iterator != _pMagicScene->v_Common.rend();)
			{
				MessageCommon* _pMagicMessageCommon = dynamic_cast<MessageCommon*>(*_iterator);
				if (_pMagicMessageCommon)
				{
					switch (_pMagicMessageCommon->MessageHandle(_MessageType, _Message))
					{
					case MAGIC_MESSAGE_RETURN_OK:
						_iterator++;
						break;
					case MAGIC_MESSAGE_RETURN_RESET:
						break;
					case MAGIC_MESSAGE_RETURN_EXIT:
						return MAGIC_MESSAGE_RETURN_EXIT;
					case MAGIC_MESSAGE_RETURN_FINISH:
						_MessageType = MAGIC_UI_MESSAGE_RETURN_FINISH;
						_Message = 0;
						_RETURN_FINISH = true;
						_iterator++;
						break;
					default:
						_iterator++;
						break;
					}
				}
				else
					_iterator++;
			}
			if (_RETURN_FINISH)
				return MAGIC_MESSAGE_RETURN_FINISH;
			else
				return MAGIC_MESSAGE_RETURN_OK;
		}
		else*/
			return MAGIC_MESSAGE_RETURN_OK;
	}
}