#include "MagicMessage.h"
#include "MagicEngineContext.h"



void MagicMessageCommon::Send_Message(unsigned int _MessageType, unsigned int _Message)
{
	this->MessageHandle(_MessageType, _Message);
}

int MagicMessageScenes::MessageHandle(unsigned int _MessageType, unsigned int _Message)
{
	MagicScene* _pMagicScene = dynamic_cast<MagicScene*>(this);
	if (_pMagicScene)
	{
		bool _RETURN_FINISH = false;
		for (std::vector<MagicCommon*>::reverse_iterator _iterator = _pMagicScene->v_Common.rbegin();
			_iterator != _pMagicScene->v_Common.rend();)
		{
			MagicMessageCommon* _pMagicMessageCommon = dynamic_cast<MagicMessageCommon*>(*_iterator);
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
	else
		return MAGIC_MESSAGE_RETURN_OK;
}