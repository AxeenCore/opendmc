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
	: m_bitPtr(NULL)
	, m_nWidth(0)
	, m_nHeight(0)
	, m_nBitCount(0)
	, m_nScanline(0)
	, m_uSize(0) {
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
	SAFE_DELETEARRAY(m_bitPtr);
	m_nWidth = 0;
	m_nHeight = 0;
	m_nBitCount = 0;
	m_nScanline = 0;
	m_uSize = 0;
	::memset(reinterpret_cast<void*>(&m_bmFile), 0, sizeof(m_bmFile));
	::memset(reinterpret_cast<void*>(&m_bmInfo), 0, sizeof(m_bmInfo));
}

/**
 *	@brief 建立 Surface
 *	@param[in]	width		圖形寬度
 *	@param[in]	height		圖形高度
 *	@param[in]	bitCount	色採深度 (單位 Bits)
 *	@return	<b>型別: int</b>
 *		\n 若成功返回值為非零值
 *		\n 若失敗返回值為零
 */
BOOL DmSurface::CreateSurface(int wd, int ht, int bitCount)
{
	UINT8*	bitPtr = nullptr;
	UINT32	cbSize;
	int		scanline;

	for (;;) {
		this->Release();
		scanline = this->ScanlineLength(wd, ht, bitCount);
		if (!scanline) break;

		/* 計算圖形所需記憶體容量，單位 byte */
		cbSize = scanline * ht;
		if (!cbSize) break;

		/* 配置圖形存放空間 */
		bitPtr = new (std::nothrow) UINT8[cbSize];
		if (bitPtr == nullptr) break;

		/* 回存相關資料 */
		m_bitPtr = bitPtr;
		m_nBitCount = bitCount;
		m_nWidth = wd;
		m_nHeight = ht;
		m_nScanline = scanline;
		m_uSize = cbSize;

		/* 設定 BMP 圖形資訊 */
		if (!this->SetBmpInfoHeader()) break;

		return TRUE;
	}

	/* 建立 Surface 失敗 */
	this->Release();
	return FALSE;
}

#if defined(ODMC_WINDOWS)
void DmSurface::TransferToWindow(HWND hWnd)
{
	const void* bitPtr = reinterpret_cast<const void*>(m_bitPtr);
	BITMAPINFO* infoPtr = reinterpret_cast<BITMAPINFO*>(&m_bmInfo);

	HDC hDC = nullptr;
	auto wd = static_cast<DWORD>(m_nWidth);
	auto ht = static_cast<DWORD>(m_nHeight);

	for (;;) {
		if (hWnd == nullptr || bitPtr == nullptr || infoPtr == nullptr) break;

		/* Get target window DC (device context) */
		if ((hDC = ::GetDC(hWnd)) == nullptr) break;

		wd = static_cast<DWORD>(m_nWidth);
		ht = static_cast<DWORD>(m_nHeight);

		/* draw image to target device contex */
		::SetDIBitsToDevice(
			hDC,				// handle of device context
			0,					// destination start x-coordinate
			0,					// destination start y-coordinate
			wd,					// width
			ht,					// height
			0,					// source strat x-coordinate
			0,					// source start y-coordinate
			0,					// start scan-line
			static_cast<UINT>(ht),	// lines
			bitPtr,				// bit data
			infoPtr,			// BITMAPINFO structure
			DIB_RGB_COLORS);	// Color use 
		break;
	};

	if (hDC) ::ReleaseDC(hWnd, hDC);
}
#endif

/**
 *	@brief	計算 scan-line 長度
 *	@param[in]	width		圖形寬度
 *	@param[in]	height		圖形高度
 *	@param[in]	bitCount	色採深度 (單位 Bits)
 *	@return	<b>型別: int</b>
 *		\n 若成功返回值為圖像 scan-line 長度，單位 pixel
 *		\n 若失敗返回值為零
 */
int DmSurface::ScanlineLength(int wd, int ht, int bitCount)
{
	auto bpp = static_cast<PixelFormat>(bitCount);
	auto scanline = static_cast<int>(0);

	for (;;) {
		// is size's parameter supported?
		if (wd < DMIMG_MINSIZE || wd > DMIMG_MAXSIZE)
			break;
		if (ht < DMIMG_MINSIZE || ht > DMIMG_MAXSIZE)
			break;

		switch (bpp)
		{
		case PixelFormat::RGB_1bpp:
			// 1 byte = 8 pixel, ==>> (width + 7) / 8;
			scanline = (wd + 7) >> 3;
			break;

		case PixelFormat::RGB_4bpp:
			// 1 byte = 2 pixel, ==>> (width + 1) / 2;
			scanline = (wd + 1) >> 1;
			break;

		case PixelFormat::RGB_15bpp:
		case PixelFormat::RGB_16bpp:
			// There are 2 16-bit color mode, (2bytes = 1 pixel)
			//  (1) 555, RGB R5, G5, B5 --> 0RRRRRGGGGGBBBBB (the highest bit not uses)
			//  (2) 565, RGB R5, G6, B5 --> RRRRRGGGGGGBBBBB (the green close uses 6-bits)
			// // ==>> width * (width / 8)
			if (bpp == PixelFormat::RGB_15bpp) {
				bitCount = static_cast<int>(PixelFormat::RGB_16bpp);
			}
			scanline = wd * (bitCount >> 3);
			break;

		case PixelFormat::RGB_8bpp:
		case PixelFormat::RGB_24bpp:
		case PixelFormat::RGB_32bpp:
			// 8-bits, 24-bits, 32-bits, uses same solution to get scan-line
			// ==>> width * (width / 8)
			scanline = wd * (bitCount >> 3);
			break;

		default:
			// not supported formats
			scanline = 0;
		}
	}

	// Base on Bitmap image format, the scan-line must be a multiple of 4bytes
	// scanline = ((scanline + 3) >> 2) << 2;
	// return scanline;
	return (((scanline + 3) >> 2) << 2);
}

/**
 *	@brief	設定 Bitmap 圖形檔案形容資訊
 *	@return	<b>型別: BOOL</b>
 *		\n 若設定成功返回值為非零值。
 *		\n 若設定失敗返回值為零。
 */
BOOL DmSurface::SetBmpFileHeader()
{
	return 0;
}

/**
 *	@brief	設定 Bitmap 圖形資訊
 *		\n 若設定成功返回值為非零值。
 *		\n 若設定失敗返回值為零。
 */
BOOL DmSurface::SetBmpInfoHeader()
{
	int res = FALSE;

	for (;;) {
		/* 圖像保存區尚未配置 */
		if (m_bitPtr == NULL) break;

		/* 定義 BMPINFOHEADER 內容 */
		::memset(&m_bmInfo, 0, sizeof(m_bmInfo));
		m_bmInfo.bmiHeader.biSize = sizeof(BMPINFOHEADER);
		m_bmInfo.bmiHeader.biWidth = m_nWidth;
		m_bmInfo.bmiHeader.biHeight = -(m_nHeight);	// 標準 BMP 起始座標為左下，若正像圖於 Windows DIB 顯示時將會被反向, 所以要正向顯示必須為高必須為負值。
		m_bmInfo.bmiHeader.biPlanes = 1;			// must be 1
		m_bmInfo.bmiHeader.biBitCount = m_nBitCount == static_cast<int>(PixelFormat::RGB_15bpp)
			? static_cast<WORD>(PixelFormat::RGB_16bpp)
			: static_cast<WORD>(m_nBitCount);
		m_bmInfo.bmiHeader.biCompression = BI_RGB;
		m_bmInfo.bmiHeader.biSizeImage = 0;
		m_bmInfo.bmiHeader.biXPelsPerMeter = 0;
		m_bmInfo.bmiHeader.biYPelsPerMeter = 0;
		m_bmInfo.bmiHeader.biClrUsed = 0;
		m_bmInfo.bmiHeader.biClrImportant = 0;

		/* 調色盤定義 */
		auto Bmpp = static_cast<PixelFormat>(m_nBitCount);
		UINT32* pQuad = reinterpret_cast<UINT32*>(&m_bmInfo.bmiColors[0]);

		res = TRUE;
		switch (Bmpp)
		{
		case PixelFormat::RGB_1bpp:
			// TBD
			break;

		case PixelFormat::RGB_4bpp:
			// TBD
			break;

		case PixelFormat::RGB_8bpp:
			// TBD
			break;

		case PixelFormat::RGB_16bpp:
			m_bmInfo.bmiHeader.biCompression = BI_BITFIELDS;
			m_bmInfo.bmiHeader.biClrUsed = 3;
			m_bmInfo.bmiHeader.biClrImportant = 0;

			*(pQuad + 0) = RGB_565_MASK_RED;
			*(pQuad + 1) = RGB_565_MASK_GREEN;
			*(pQuad + 2) = RGB_565_MASK_BLUE;
			break;

		case PixelFormat::RGB_15bpp:
		case PixelFormat::RGB_24bpp:
			m_bmInfo.bmiHeader.biCompression = BI_RGB;
			m_bmInfo.bmiHeader.biClrUsed = 0;
			m_bmInfo.bmiHeader.biClrImportant = 0;
			m_bmInfo.bmiHeader.biSizeImage = 0;
			break;

		case PixelFormat::RGB_32bpp:
			m_bmInfo.bmiHeader.biCompression = BI_BITFIELDS;
			m_bmInfo.bmiHeader.biClrUsed = 3;
			m_bmInfo.bmiHeader.biClrImportant = 0;

			*(pQuad + 0) = RGB_888_MASK_RED;
			*(pQuad + 1) = RGB_888_MASK_GREEN;
			*(pQuad + 2) = RGB_888_MASK_BLUE;
			break;

		default:
			::memset(&m_bmInfo.bmiHeader, 0, sizeof(BMPINFOHEADER));
			res = false;
		}
		break;
	}
	return res;
}
