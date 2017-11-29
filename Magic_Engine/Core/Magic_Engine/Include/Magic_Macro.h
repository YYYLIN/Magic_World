#ifndef _MAGIC_MACRO_H_
#define _MAGIC_MACRO_H_

#ifdef MAGIC_ENGINE_EXPORTS

#define DLL_MAGIC_ENGINE_OUTPUT					__declspec(dllexport)
#define DLL_MAGIC_ENGINE_OUTPUT_INPUT			__declspec(dllexport)

#else

#define DLL_MAGIC_ENGINE_OUTPUT
#define DLL_MAGIC_ENGINE_OUTPUT_INPUT			__declspec(dllimport)

#endif

#define CHARATER_LEFT_ARRAY				0x01
#define CHARATER_RIGHT_ARRAY			0x02
#define CHARATER_MIDDLE_ARRAY			0x03

#endif