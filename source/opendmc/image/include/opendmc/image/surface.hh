﻿/**************************************************************************//**
 * @file	surface.hh
 * @brief	DmSurface 類別宣告 Herader
 * @date	2010-01-25
 * @date	2019-01-25
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_IMAGE_SURFACE_HH
#define	ODMC_IMAGE_SURFACE_HH
#include "opendmc/image/imdefine.hh"

/**
 *	@class DmSurface
 *	@brief DmSurface 繪圖頁類別
 */
class DmSurface
{
public:
	DmSurface();
	virtual ~DmSurface();
	virtual void Release();

	BOOL CreateSurface(int wd, int ht, int bitCount);
	BOOL Blt(DmSurface& surDest, int px, int py, int x, int y, int wd, int ht);

	#if defined(ODMC_WINDOWS)
	void TransferToWindow(HWND hWnd);
	#endif

	int GetWidth()		{ return m_nWidth; }
	int GetHeight()		{ return m_nHeight; }
	int GetScanline()	{ return m_nScanline; }
	int GetBitCount()	{ return m_nBitCount; }
	UINT8* GetImageData() { return m_bitPtr; }
	UINT32 GetImageSize() { return m_uImageSize; }

protected:
	BOOL ConvertDrawArea(DmSurface& surDest, IMGDRAWAREA* drawPtr, int px, int py, int x, int y, int wd, int ht);

	int	 ScanlineLength(int wd, int ht, int bitCount);
	BOOL SetBmpFileHeader();
	BOOL SetBmpInfoHeader();

protected:
	UINT8*	m_bitPtr;			//!< 圖像資料存放位置
	int		m_nWidth;			//!< Surface 寬度
	int		m_nHeight;			//!< Surface 高度
	int		m_nBitCount;		//!< Surface 色彩深度
	int		m_nScanline;		//!< 一掃描線長度, 單位 pixel
	UINT32	m_uImageSize;		//!< Surface 大小

	BMPFILEHEADER	m_bmFile;	//!< Bitmap file header 結構
	BMPINFO			m_bmInfo;	//!< Bitmap information
};

#endif // !ODMC_IMAGE_SURFACE_HH