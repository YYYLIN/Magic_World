#include "Include/MagicAlphaWindow.h"


bool EnableAlphaWindow(HWND m_hWnd, int WinWidth, int WinHeight, SGDI* g_GDI)
{
	// ��ʼ��g_GDI

	memset(g_GDI, 0, sizeof(SGDI));
	g_GDI->m_hWnd = m_hWnd;
	g_GDI->m_Width = WinWidth;
	g_GDI->m_Height = WinHeight;
	g_GDI->m_hMainDC = GetDC(m_hWnd);
	g_GDI->m_hMemoryDC = CreateCompatibleDC(NULL);
	g_GDI->m_hMainSurface = CreateCompatibleBitmap(g_GDI->m_hMainDC, g_GDI->m_Width, g_GDI->m_Height);
	g_GDI->m_pBackBuffer = (UINT*)malloc(g_GDI->m_Width*g_GDI->m_Height * 4);

	//���ð����й�DIB����Ϣ
	BITMAPINFO& bmpInfo = g_GDI->m_BmpInfo.m_BitmapInfo;
	bmpInfo.bmiHeader.biSize = sizeof(bmpInfo.bmiHeader);
	bmpInfo.bmiHeader.biWidth = g_GDI->m_Width;
	bmpInfo.bmiHeader.biHeight = -g_GDI->m_Height;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 32;
	bmpInfo.bmiHeader.biCompression = BI_BITFIELDS;

	*(UINT*)(bmpInfo.bmiColors + 0) = 0xFF0000;	// red����
	*(UINT*)(bmpInfo.bmiColors + 1) = 0x00FF00;	// green����
	*(UINT*)(bmpInfo.bmiColors + 2) = 0x0000FF;	// blue����

												// ��������ѡ���ڴ�DC
	g_GDI->m_hOldBitmap = (HBITMAP)::SelectObject(g_GDI->m_hMemoryDC, g_GDI->m_hMainSurface);


	//�����µķ��WS_EX_LAYERED
	//�������WS_EX_TRANSPARENTģʽ���ᴩ͸
	DWORD dwExStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
	if (!(dwExStyle & WS_EX_LAYERED))
		SetWindowLong(m_hWnd, GWL_EXSTYLE, dwExStyle | WS_EX_LAYERED);
	else
		return false;

	return true;
}


bool UpdataAlphaWindow(SGDI* g_GDI)
{
	//��������Ӧ�ڴ�DC��
	::SetDIBitsToDevice(g_GDI->m_hMemoryDC,
		0, 0, g_GDI->m_Width, g_GDI->m_Height,
		0, 0, 0, g_GDI->m_Height,
		g_GDI->m_pBackBuffer, &g_GDI->m_BmpInfo.m_BitmapInfo, DIB_RGB_COLORS);

	//��ʾ����
	BLENDFUNCTION pBlendFunCtion;
	pBlendFunCtion.BlendOp = AC_SRC_OVER;
	pBlendFunCtion.BlendFlags = 0;
	pBlendFunCtion.SourceConstantAlpha = 255;
	pBlendFunCtion.AlphaFormat = AC_SRC_ALPHA;
	RECT rcWnd;
	::GetWindowRect(g_GDI->m_hWnd, &rcWnd);
	POINT dstPos = { rcWnd.left, rcWnd.top };
	POINT srcPos = { 0, 0 };
	SIZE dstSize = { rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top };
	bool result = UpdateLayeredWindow(g_GDI->m_hWnd, g_GDI->m_hMainDC, &dstPos, &dstSize, g_GDI->m_hMemoryDC, &srcPos, RGB(255, 255, 255), &pBlendFunCtion, ULW_ALPHA) != 0;
	if (!result)
		return false;

	return true;
}

void DisableAlphaWindow(SGDI* g_GDI)
{
	if (g_GDI->m_pBackBuffer)
	{
		free(g_GDI->m_pBackBuffer);
		g_GDI->m_pBackBuffer = 0;
	}
}