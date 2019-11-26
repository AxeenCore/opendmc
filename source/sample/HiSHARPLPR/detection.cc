/**************************************************************************//**
 * @file	detection.cc
 * @brief	車牌偵測函數定義
 * @date	2019-11-20
 * @date	2019-11-20
 * @author	Swang
 *****************************************************************************/
#include "detection.hh"

/**
 *	@brief	使用 OpenCV VideoCapture 與目標 IPCam or Server 建立 RTSP 連線
 *	@param[in] url	RTSP 位址或路徑
 *	@return	<b>型別: cv::VideoCapture 類別物件指標</b>
 *		\n 若函數操作成功，返回值為非零值，值為 cv::VideoCapture 類別物件指標
 *		\n 若函數操作失敗，返回職為零。
 */
cv::VideoCapture* OpenVideoCapture(const cv::String& url)
{
	cv::VideoCapture* stream = new (std::nothrow) cv::VideoCapture(url);
	if (stream != nullptr) {
		if (!stream->isOpened()) {
			stream->release();
			SAFE_DELETE(stream);
		}
	}
	return stream;
}

/**
 *	@brief	輸出影像至指定視窗
 *	@param[in] hWnd		Handle of window
 *	@param[in] wd		width
 *	@param[in] ht		height
 *	@param[in] bmiPtr	Point of structure BITMAPINFO
 *	@param[in] dataPtr	Point of image data buffer
 *	@return	此函數沒有返回值
 */
void TransferToWindow(HWND hWnd, int wd, int ht, BITMAPINFO* bmiPtr, void* dataPtr, bool haff)
{
	int x = haff ? wd : 0;
	HDC hDC = nullptr;

	for (;;) {
		if (hWnd == nullptr || dataPtr == nullptr || bmiPtr == nullptr) break;
		
		/* Get target window DC (device context) */
		if ((hDC = ::GetDC(hWnd)) == nullptr) break;

		/* draw image to target device contex */
		::SetDIBitsToDevice(
			hDC,				// handle of device context
			x,					// destination start x-coordinate
			0,					// destination start y-coordinate
			wd,					// width
			ht,					// height
			0,					// source strat x-coordinate
			0,					// source start y-coordinate
			0,					// start scan-line
			static_cast<UINT>(ht),	// lines
			dataPtr,			// bit data
			bmiPtr,				// BITMAPINFO structure
			DIB_RGB_COLORS);	// Color use 
		break;
	};

	if (hDC) ::ReleaseDC(hWnd, hDC);
}

/**
 *	@brief	增強對比度
 *	@param[in] frame	來源 cv::Mat 物件參考
 *	@return	此函數沒有返回值
 */
void FrameToContrast(cv::Mat& frame)
{
	cv::Mat tophat, blackhat;
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));
	cv::morphologyEx(frame, tophat, cv::MORPH_TOPHAT, element, cv::Point(-1, -1));
	cv::morphologyEx(frame, blackhat, cv::MORPH_BLACKHAT, element, cv::Point(-1, -1));
	cv::add(frame, tophat, frame);
	cv::subtract(frame, blackhat, frame);
}

/**
 *	@brief	Sobel 邊緣強化
 *	@param[in]	frame	來源 cv::Mat 物件參考
 *	@return	此函數沒有返回值
 */
void FrameToSobel(cv::Mat& frame)
{
	// 高斯平滑
	cv::GaussianBlur(frame, frame, cv::Size(9, 3), 0, 0);

	// 利用 Sobel 運算求取垂直邊緣
	// cv::Sobel(frame, frame, CV_8U, 1, 0);
	cv::Sobel(frame, frame, CV_8U, 2, 0, 3);

	// 二值化
	cv::threshold(frame, frame, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
}

void FrameToCanny(cv::Mat& frame)
{
	// 高斯平滑
	cv::GaussianBlur(frame, frame, cv::Size(9, 3), 0, 0);
	cv::Canny(frame, frame, 500, 200);

	// 二值化
	cv::threshold(frame, frame, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
}


void FrameErosionDilation(cv::Mat frame)
{
	cv::Mat kernel1 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 1), cv::Point(-1, -1));
	cv::Mat kernel2 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 3), cv::Point(-1, -1));
	cv::Mat k1, k2;

	cv::morphologyEx(frame, k1, cv::MORPH_CLOSE, kernel1);
	cv::morphologyEx(k1, k1, cv::MORPH_CLOSE, kernel2);
	frame = k1.clone();
}



void VideoCaptureProcess(cv::Mat& buffer, std::mutex& mutex, void* keepPtr, void* restPtr)
{
	const	int errLimit = 5;
	const	cv::String url = "rtsp://admin:hisharp123456@210.61.217.151:554/unicast/c5/s1/live";
	
	cv::VideoCapture* vstream = nullptr;
	cv::Mat tmp;
	auto arKeep = reinterpret_cast<bool*>(keepPtr);
	auto arRest = reinterpret_cast<bool*>(restPtr);
	auto errCount = static_cast<int>(0);

	while (*arKeep) {
		/* 影像串流不存在 */
		if (vstream == nullptr) {
			/* 開啟(建立)影像串流 */
			vstream = ::OpenVideoCapture(url);
			if (vstream == nullptr) {
				if (errCount > errLimit) {
					break;
				}
				errCount++;
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				continue;
			}
			vstream->set(cv::CAP_PROP_FORMAT, CV_8UC3);
			*arRest = true;
		}

		(*vstream) >> tmp;
		mutex.lock();
		buffer = tmp.clone();
		mutex.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

