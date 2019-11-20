/**************************************************************************//**
 * @file	imdefine.hh
 * @brief	opendmc_image Herader
 * @date	2010-01-25
 * @date	2019-01-25
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_IMAGE_IMDEFINE_HH
#define	ODMC_IMAGE_IMDEFINE_HH
#include "opendmc/conf/configure.hh"

/* Image surface support line of max pixel */
#define DMIMG_MINSIZE	1
#define DMIMG_MAXSIZE	10240

/* Color mask 555 */
#define RGB_555_MASK_RED	0x00007C00	//!< REB 555 R channel
#define	RGB_555_MASK_GREEN	0x000003E0	//!< RGB 555 G channel
#define	RGB_555_MASK_BLUE	0x0000001F	//!< RGB 555 B channel

/* Color mask 565 */
#define RGB_565_MASK_RED    0x0000F800  //!< RGB 565 R channel
#define RGB_565_MASK_GREEN  0x000007E0  //!< RGB 565 G channel
#define RGB_565_MASK_BLUE   0x0000001F  //!< RGB 565 B channel

/* Color mask 888 */
#define RGB_888_MASK_RED    0x00FF0000	//!< RGB 888 R channel
#define RGB_888_MASK_GREEN  0x0000FF00	//!< RGB 888 G channel
#define RGB_888_MASK_BLUE   0x000000FF	//!< RGB 888 B channel

// Convert 16-bits (565) color to 24-bits RGB color
#define RGB16TORGB24(x)     ( ((( x ) << 8) & 0x00F80000) | ((( x ) << 5) & 0x0000FC00) | ((( x ) << 3) & 0x000000F8) )

// Convert 24-bits RGB color to 16-bits (565) color
#define RGB24TORGB16(x)     ( ((( x ) & 0x00F80000) >> 8) + ((( x ) & 0x0000FC00) >> 5) + ((( x ) & 0x000000F8) >> 3) )

// Get RGB24 color
#define RGB24RED(x)         ( ((x) & 0x00FF0000) >> 16 )
#define RGB24GREEN(x)       ( ((x) & 0x0000FF00) >> 8 )
#define RGB24BLUE(x)        ( (x) & 0x000000FF )

/**
 *	@struct	RGB24
 *	@brief	RGB 888 - RGB 24bit 色彩深度結構
 */
struct RGB24 {
	UINT8	Red;		//!< R 紅色通道
	UINT8	Green;		//!< G 綠色通道
	UINT8	Blue;		//!< B 藍色通道
};
typedef	RGB24* LPRGB24;

/**
 *	@struct	RGB32
 *	@brief	RGB 888 - RGB 32bit 色彩深度結構
 */
struct RGB32 {
	UINT8	Red;		//!< R 紅色通道
	UINT8	Green;		//!< G 綠色通道
	UINT8	Blue;		//!< B 藍色通道
	UINT8	Result;		//!< 保留
};
typedef RGB32* LPREG32;

/**
 *	@enum	EmColorDeep
 *	@brief	列舉色彩深度, 單位 bit
 */
enum class EmColorDepth {
	BmppMin	= 1,
	Bmpp1	= 1,
	Bmpp2	= 2,
	Bmpp3	= 4,
	Bmpp8	= 8,
	Bmpp16	= 16,
	Bmpp24	= 24,
	Bmpp32	= 32,
	BmppMax	= 32
};

/* 強制編譯氣採用對其方式 Bitmap 結構若被自動使用預設對齊將造成資料位置不正確 如 Visual Studio 2010 資料預設對齊為 4-byte */
#if defined(ODMC_WINDOWS)
#	include <pshpack2.h>
#else
#	pragma pack(2)
#endif

/**
 *	@struct	BMPFILEHEADER (copy form Microsoft)
 *	@brief	Bitmap 檔頭格式
 *	@see	https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapfileheader
 */
struct BMPFILEHEADER {
	UINT16	bfType;			//!< Bitmap 檔案格式識別碼, 必為 "BM"
	UINT32	bfSize;			//!< Bitmap 圖形檔案大小(含各描述與資訊) 
	UINT16	bfReserved1;	//!< 保留
	UINT16	bfReserved2;	//!< 保留
	UINT32	bfOffBits;		//!< 影像數據開始位置
};
typedef BMPFILEHEADER* LPBMPFILEHEADER;

/**
 *	@struct	BMPINFOHEADER (copy form Microsoft)
 *	@brief	Bitmap 圖形資訊
 *	@remark	<b>biCompression 成員額外說明</b>
 *		- BI_RGB		未壓縮的格式。
 *		- BI_RLE8		具有8 bpp 的位圖的行程編碼（RLE）格式。壓縮格式是2字節格式，由計數字節和包含顏色索引的字節組成。有關更多信息，請參見位圖壓縮。
 *		- BI_RLE4		具有4 bpp 的位圖的RLE格式。壓縮格式是2字節格式，由計數字節和兩個字長的顏色索引組成。有關更多信息，請參見位圖壓縮。
 *		- BI_BITFIELDS	指定不壓縮位圖，並且顏色表由三個DWORD色罩組成，分別指定每個像素的紅色，綠色和藍色分量。與16和32 bpp位圖一起使用時有效。
 *		- BI_JPEG		表示該圖像是JPEG圖像。
 *		- BI_PNG		表示該圖像是PNG圖像。
 *	@see	https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapinfoheader
 */
struct BMPINFOHEADER {
	UINT32	biSize;				//!< BMPINFOHEADER 結構長度
	INT32	biWidth;			//!< 圖像寬度，單位 Pixel
	INT32	biHeight;			//!< 圖像高度，單位 Pixel, 若值為負，起始原點為左下角，若值為正起始原點為左上角 (則 則biCompression必須為BI_RGB或BI_BITFIELDS)
	UINT16	biPlanes;			//!< 目標設備的平面數。此值必須設置為 1。
	UINT16	biBitCount;			//!< 每個 pixel 的 Bit 數，BMPINFOHEADER 結構的 biBitCount 定義每個 pixel 色彩深度。
	UINT32	biCompression;		//!< 採用壓縮類型 (若 biHeight 成員為正數，則無法進行壓縮)
	UINT32	biSizeImage;		//!< 圖像大小
	INT32	biXPelsPerMeter;	//!< 水平解析度
	INT32	biYPelsPerMeter;	//!< 垂直解析度
	UINT32	biClrUsed;			//!< 圖像使用色彩索引
	UINT32	biClrImportant;		//!< 圖像使用色彩索引數量
};
typedef BMPINFOHEADER* LPBMPINFOHEADER;

/**
 *	@struct BMPRGBQUAD
 *	@brief	描述 Bitmap 一個像素的色彩內容
 */
struct BMPRGBQUAD {
	BYTE rgbBlue;				//!< 藍色通道
	BYTE rgbGreen;				//!< 綠色通道
	BYTE rgbRed;				//!< 紅色通道
	BYTE rgbReserved;			//!< 保留
};

/**
 *	@struct	BMPINFO
 *	@brief	Bitmap 圖形資訊
 */
struct BMPINFO {
	BITMAPINFOHEADER	bmiHeader;
	BMPRGBQUAD			bmiColors[1];
};
typedef BMPINFO* LPBMPINFO;

/* 恢復編譯器預設資料對齊方式 */
#if defined(ODMC_WINDOWS)
#include <poppack.h>
#else
#pragma pack()
#endif


/**
 *	@struct	IMGCLIPINFO
 *	@brief	圖像剪裁資訊, 單位 Pixel
 */
struct IMGCLIPINFO {
	int iDstPosx;		//!< 目標畫板起始點 x 座標
	int iDstPosy;		//!< 目標畫板起始點 y 座標
	int iDstWidth;		//!< 目標提供可繪製區域寬度
	int iDstHeight;		//!< 目標提供可繪製區域高度
	int iSrcPosx;		//!< 來源畫板起始點 x 座標
	int iSrcPosy;		//!< 來源畫板起始點 y 座標 
	int iSrcWidth;		//!< 來源畫板要剪裁的寬度
	int iSrcHeight;		//!< 來源畫板要剪裁的高度
};



#endif // !ODMC_IMAGE_IMDEFINE_HH
