/**************************************************************************//**
 * @file	surface.cc
 * @brief	DmSurface 類別成員函數定義
 * @date	2010-01-25
 * @date	2019-01-25
 * @author	Swang
 *****************************************************************************/
#include "opendmc/image/surface.hh"

/**
 *	@brief	DmSurface 建構式
 */
DmSurface::DmSurface()
	: m_BitPtr(NULL)
	, m_nWidth(0)
	, m_nHeight(0)
	, m_nBitCount(0)
	, m_nScanline(0)
	, m_uImageSize(0) {
	::memset(reinterpret_cast<void*>(&m_bmFile), 0, sizeof(m_bmFile));
	::memset(reinterpret_cast<void*>(&m_bmInfo), 0, sizeof(m_bmInfo));
}

/**
 *	@brief	DmSurface 解構式
 */
DmSurface::~DmSurface() { this->Release(); }

/**
 *	@brief	釋放所有配置資源
 *	@return	此函數沒有返回值
 */
void DmSurface::Release()
{
	SAFE_DELETEARRAY(m_BitPtr);
	m_nWidth = 0;
	m_nHeight = 0;
	m_nBitCount = 0;
	m_nScanline = 0;
	m_uImageSize = 0;
	::memset(reinterpret_cast<void*>(&m_bmFile), 0, sizeof(m_bmFile));
	::memset(reinterpret_cast<void*>(&m_bmInfo), 0, sizeof(m_bmInfo));
}

/**
 *	@brief	計算掃描線長度
 *	@param[in]	nWidth		圖形寬度
 *	@param[in]	nHeight		圖形高度
 *	@param[in]	nBitCount	色採深度 (單位 Bits)
 *	@return	<b>型別: int</b>
 *		\n 若成功返回值為圖像掃描線長度，單位 pixel
 *		\n 若失敗返回值為零
 */
int DmSurface::ScanlineMatch(int nWidth, int nHeight, int nBitCount)
{
	int sline = 0;

	for (;;) {
		/* 圖形大小是否支援 */
		if (nWidth < DMIMG_MINSIZE || nWidth > DMIMG_MAXSIZE)
			break;
		if (nHeight < DMIMG_MINSIZE || nHeight > DMIMG_MAXSIZE)
			break;

		/* 圖像色彩深度是否支援 */
		if (nBitCount < static_cast<int>(EmColorDepth::BmppMin) || nBitCount > static_cast<int>(EmColorDepth::BmppMax))
			break;
		
		// TODO
		break;
	}

	return sline;
}
