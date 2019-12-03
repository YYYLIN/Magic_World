#pragma once
#include <windows.h>

struct SBmpInfo
{
	BITMAPINFO	m_BitmapInfo;
	RGBQUAD		m_bmiColors[2];	// 为BITMAPINFO的m_bmiColors补充两个元素空间
};

struct SGDI
{
	HWND		m_hWnd;
	HDC			m_hMainDC;
	HDC			m_hMemoryDC;
	HBITMAP		m_hMainSurface;
	HBITMAP		m_hOldBitmap;
	int			m_Width;
	int			m_Height;
	UINT*		m_pBackBuffer;
	SBmpInfo	m_BmpInfo;
};


bool EnableAlphaWindow(HWND m_hWnd, int WinWidth, int WinHeight, SGDI* g_GDI);

bool UpdataAlphaWindow(SGDI* g_GDI);

void DisableAlphaWindow(SGDI* g_GDI);