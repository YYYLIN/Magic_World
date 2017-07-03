#ifndef _MAGICGRATINGAPI_H_
#define _MAGICGRATINGAPI_H_

#include "MagicMacro.h"
#include "MagicStruct.h"

bool MG_DLL CreateMagicGratingContext(MGContext* _MGContext);
bool MG_DLL InitMagicGratingContext(const MGContext& _MGContext);
void MG_DLL DeleteMagicGratingContext(MGContext* _MGContext);
void MG_DLL BindMagicGratingContext(const MGContext& _MGContext);
bool MG_DLL BindMGDisplayContext(const MGContext& _MGContext, void* _data,
	const unsigned int& _width, const unsigned int& _Height, const short& _mode, const unsigned short& _type);
void MG_DLL MG_WaitSwapFrame();

void MG_DLL MGDrawArrays(const unsigned int& _DrawMode, const unsigned int& _VertexStart, const unsigned int&  _VertexNumber);

bool MG_DLL CreateMagicShaderObject(MGShader* _MGShader);
bool MG_DLL InitMagicShaderObject(const MG_VertexShader& _pMG_VertexShader, const MG_PixelShader& _pMG_PixelShader);
void MG_DLL DeleteMagicShaderObject(MGShader* _MGShader);
void MG_DLL BindMagicShaderObject(const MGShader& _MGShader);
unsigned int MG_DLL MGAddShaderVariate(const unsigned int& _size);
void MG_DLL MG_SetShaderVariate(const unsigned int& _Pos, const void* _pValue);

void MG_DLL MG_Enable(const unsigned int& _Type);
void MG_DLL MG_Disable(const unsigned int& _Type);

bool MG_DLL MG_CreateBuffer(MGBuffer* _MGBuffer);
void MG_DLL MG_DeleteBuffer(MGBuffer* _MGBuffer);
void MG_DLL MG_BindBuffer(const unsigned int& _mode, const MGBuffer& _MGBuffer);
void MG_DLL MG_BufferData(const unsigned int& _mode, const unsigned int& _size, const void* _pdata);
/*
功能:设置顶点缓存区数据格式
参数:
_pos = 顶点位置
_Size = 顶点数量
_type = 数据类型(MG_FLOAT)
返回值:空
*/
void MG_DLL MG_VertexAttribPointer(unsigned char _pos, unsigned char _Size, const unsigned short& _type);

bool MG_DLL MG_CreateVertexArrays(MGVertexArrays* _MGVertexArrays);
void MG_DLL MG_DeleteVertexArrays(MGVertexArrays* _MGVertexArrays);
void MG_DLL MG_BindVertexArrays(const MGVertexArrays& _MGVertexArrays);
void MG_DLL MG_EnableVertexAttribArray(unsigned char _number);

bool MG_DLL MG_CreateFrameBufferObject(MGFrameBuffer* _MGFrameBuffer);
void MG_DLL MG_DeleteFrameBufferObject(MGFrameBuffer* _MGFrameBuffer);
void MG_DLL MG_BindFrameBuffer(const MGFrameBuffer& _MGFrameBuffer);
bool MG_DLL MG_InitFrameBuffer(const unsigned short& _width, const unsigned short& _height);
void MG_DLL MG_ClearColor(const float& _r, const float& _g, const float& _b, const float& _a);
void MG_DLL MG_Clear(const unsigned int& _mode);

#endif