#ifndef _MAGIC_MACRO_H_
#define _MAGIC_MACRO_H_

#ifdef MAGIC_ENGINE_EXPORTS

#define DLL_MAGIC_ENGINE_OUTPUT					__declspec(dllexport)
#define DLL_MAGIC_ENGINE_OUTPUT_INPUT			__declspec(dllexport)

#else

#define DLL_MAGIC_ENGINE_OUTPUT
#define DLL_MAGIC_ENGINE_OUTPUT_INPUT			__declspec(dllimport)

#endif

#define CHARATER_LEFT_ARRAY						0x01
#define CHARATER_RIGHT_ARRAY					0x02
#define CHARATER_MIDDLE_ARRAY					0x03

#define MAGIC_RETURN_OK					0x00000000
#define MAGIC_RETURN_RESET				0x00000001
#define MAGIC_RETURN_EXIT				0x00000002
#define MAGIC_RETURN_FINISH				0x00000003

#define MAGIC_MESSAGE_RETURN_FINISH			0x00000000
#define MAGIC_MESSAGE_SET_DISPLAY			0x00000001
#define MAGIC_UI_MESSAGE_MOUSE_MOVE			0x00000002

#define LONG_TO_MOUSE_X(a)			(short)(a)
#define LONG_TO_MOUSE_Y(a)			(short)(a >> 16)

#define MOUSE_X_TO_LONG(a)			(long)(a)
#define MOUSE_Y_TO_LONG(a)			((long)a << 16)

#define SCREEN_RECT_TO_MESSAGE(a)		(*((Magic::Management::MESSAGE*)(&a)))
#define MESSAGE_TO_SCREEN_RECT(a)		(*((Magic::Screen_Rect*)(&a)))

namespace Magic {
	enum ENGINE_MESSAGE {
		SHUTOWN_ENGINE = 1
	};

	typedef void* FONT;
}

typedef void* RenderContext;

#endif