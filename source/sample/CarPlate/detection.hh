/**************************************************************************//**
 * @file	detection.hh
 * @brief	CxPlateDetection 類別宣告 Header
 * @date	2019-12-09
 * @date	2019-12-09
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_CARPLATE_DETECTION_HH
#define	ODMC_CARPLATE_DETECTION_HH
#include "carplate.hh"

/**
 *	@enum	EdgeType
 *	@brief	邊緣強化方式列表
 */
enum class EdgeType {
	Threshold = 1,		//!< 自適應閥值
	Sobel,				//!< Sobel
	Canny,				//!< Canny
};

/**
 *	@class	CarPlateDetection
 *	@brief	車牌偵測類別
 */
class CarPlateDetection
{
public:
	CarPlateDetection();
	virtual ~CarPlateDetection();

public:
	bool DetectionRun(HWND hWnd);
	void DetectionEnd();

protected:
	// 影像處理
	bool ImagePreprocess(cv::Mat& imgFrame, cv::Mat& imgGrayscale, cv::Mat& imgThresh, EdgeType edgeType = EdgeType::Threshold);
	void ImageGrayscale(cv::Mat& imgFrame, cv::Mat& imgGrayScale, int scaleType = 0);
	void ImageContrast(cv::Mat& imgGrayscale, cv::Mat& imgContrast);
	void ImageDrawContours(cv::Mat& imgThresh);
	void ImageMatch(cv::Mat& imgFrame, cv::Mat& imgAddone, int alpha = 50);

	// 視窗處理函數
	bool DisplayBitmap(int x, int y, int wd, int ht, BITMAPINFO* bmiPtr, void* dataPtr) const;
	bool SetCenterPosition() const;
	bool SetWindowPosition(int x, int y) const;
	bool SetWindowSize(int wd, int ht) const;
	void Resize(BITMAPINFO* bmi, int wd, int ht) const;
	DWORD ShowFPS(DWORD lastTime, int frameCount, int bufSize);

private:
	CarPlateDetection(const CarPlateDetection&) = delete;				//!< Disable copy construction
	CarPlateDetection& operator=(const CarPlateDetection&) = delete;	//!< Disable assignment operator
	static void DetectionProcess(CarPlateDetection* cThisPtr);
	static void CaptureProcess(std::atomic<bool>& camKeep, std::queue<cv::Mat>& camQueue, std::mutex& camMutex, void* rstPtr);

	bool m_isDetecting;							//!< 車牌偵測是否正常運行
	HWND m_hWndTarget;							//!< 影像輸出視窗 handle
	std::atomic<bool>*	m_cKeepDetection;		//!< 持續車牌偵測識別 (keep running thread process)
	std::thread*		m_cProcDetection;		//!< 車牌偵測 process
};

#endif // !ODMC_CARPLATE_DETECTION_HH
