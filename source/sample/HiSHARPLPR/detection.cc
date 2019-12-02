/**************************************************************************//**
 * @file	detection.cc
 * @brief	車牌偵測函數定義
 * @date	2019-11-20
 * @date	2019-11-20
 * @author	Swang
 *****************************************************************************/
#include "detection.hh"

/**
 *	@brief 建立 Bitmap 結構資訊
 *	@param[out]	biPtr	指向結構 BITMAPINFO 緩衝區位址，用來保存設定內容 
 *	@param[in]	wd		圖像寬度
 *	@param[in]	ht		圖像高度
 *	@return	此函數沒有返回值
 */
void SetBMPInfo(BITMAPINFO* biPtr, int wd, int ht)
{
	if (biPtr != nullptr) {
		::memset(biPtr, 0, sizeof(BITMAPINFO));
		(*biPtr).bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		(*biPtr).bmiHeader.biWidth = wd;
		(*biPtr).bmiHeader.biHeight = -ht;			// 標準 BMP 起始座標為左下，若正像圖於 Windows DIB 顯示時將會被反向, 所以要正向顯示必須為高必須為負值。
		(*biPtr).bmiHeader.biPlanes = 1;			// must be 1
		(*biPtr).bmiHeader.biBitCount = 24;			// 色彩深度
		(*biPtr).bmiHeader.biXPelsPerMeter = 0;
		(*biPtr).bmiHeader.biYPelsPerMeter = 0;
		(*biPtr).bmiHeader.biCompression = BI_RGB;
		(*biPtr).bmiHeader.biClrUsed = 0;
		(*biPtr).bmiHeader.biClrImportant = 0;
		(*biPtr).bmiHeader.biSizeImage = 0;
	}
}

/**
 *	@brief 輸出繪圖頁內容至視窗畫面
 *	@param[in]	hWnd	Handle of window
 *	@param[in]	x		start x-coordinate
 *	@param[in]	y		start y-coordinate
 *	@param[in]	wd		width
 *	@param[in]	ht		height
 *	@param[in]	bmiPtr	Pointer of BITMAPINFO structure
 *	@param[in]	dataPtr	Pointer of Image data
 *	@return	此函數沒有返回值
 */
void ShowSurface(HWND hWnd, int x, int y, int wd, int ht, BITMAPINFO* bmiPtr, void* dataPtr)
{
	HDC hDC = nullptr;

	for (;;) {
		if (hWnd == nullptr || dataPtr == nullptr || bmiPtr == nullptr) break;

		/* Get target window DC (device context) */
		if ((hDC = ::GetDC(hWnd)) == nullptr) break;

		/* draw image to target device contex */
		::SetDIBitsToDevice(
			hDC,				// handle of device context
			x,					// destination start x-coordinate
			y,					// destination start y-coordinate
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
 *	@brief 使用 OpenCV VideoCapture 與目標 IPCam or Server 建立 RTSP 連線
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
 *	@brief 增強對比度
 *	@param[in] frame 繪圖頁物件參考位址 (cv::Mat)
 *	@return	此函數沒有返回值
 */
void FrameToContrast(cv::Mat& frame)
{
	cv::Mat tophat, blackhat;
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(17, 17));
	cv::morphologyEx(frame, tophat, cv::MORPH_TOPHAT, element, cv::Point(-1, -1));
	cv::morphologyEx(frame, blackhat, cv::MORPH_BLACKHAT, element, cv::Point(-1, -1));
	cv::add(frame, tophat, frame);
	cv::subtract(frame, blackhat, frame);
}

/**
 *	@brief 將來源繪圖頁內容進行 Sobel 計算
 *	@param[in,out] frame 繪圖頁物件參考位址 (cv::Mat)
 *	@return	此函數沒有返回值
 */
void FrameToSobel(cv::Mat& frame)
{
	cv::Mat tmp;
	int ksize = 3;	// default value is -1;
	int scale = 1;
	int delta = 1;
	int ddepth = CV_16S;

	// 高斯平滑
	cv::GaussianBlur(frame, tmp, cv::Size(5, 5), cv::BORDER_DEFAULT);

	cv::Mat grad_x, grad_y;
	cv::Mat abs_grad_x, abs_grad_y;
	cv::Sobel(tmp, grad_x, ddepth, 1, 0, ksize, scale, delta, cv::BORDER_DEFAULT);
	cv::Sobel(tmp, grad_y, ddepth, 0, 1, ksize, scale, delta, cv::BORDER_DEFAULT);

	/* converting back to CV_8U */
	cv::convertScaleAbs(grad_x, abs_grad_x);
	cv::convertScaleAbs(grad_y, abs_grad_y);
	cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, frame);

	// 二值化
	//cv::threshold(frame, frame, 180, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
	cv::threshold(frame, frame, 180, 255, cv::THRESH_BINARY);
}

/**
 *	@brief 將來源繪圖頁內容進行 Canny 計算
 *	@param[in,out] frame 繪圖頁物件參考位址 (cv::Mat)
 *	@return	此函數沒有返回值
 */
void FrameToCanny(cv::Mat& frame)
{
	// 高斯平滑
	cv::GaussianBlur(frame, frame, cv::Size(5, 5), 0, 0);
	cv::Canny(frame, frame, 170, 200, 3);

	// 二值化
	//cv::threshold(frame, frame, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
}

void FrameCloseOperation(cv::Mat& frame)
{
	// 48:22 => 24:11 => 12 : 5.5 => 13:7
	auto e1 = cv::Size(17, 5);
	auto e2 = cv::Size(7, 7);
	auto e3 = cv::Size(17, 5);

	// 閉鎖運算
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, e1 /*cv::Size(17, 5)*/, cv::Point(-1, -1));
	cv::Mat closeImage;
	cv::morphologyEx(frame, closeImage, cv::MORPH_CLOSE, element);

	// 再次腐蝕 (去除小區塊)
	cv::Mat ele = cv::getStructuringElement(cv::MORPH_RECT, e2/*cv::Size(5, 5)*/);
	cv::erode(closeImage, closeImage, ele);

	// 再次膨脹 (區域再膨脹)
	ele = cv::getStructuringElement(cv::MORPH_RECT, e3/*cv::Size(17, 5)*/);
	cv::dilate(closeImage, closeImage, ele);
	cv::dilate(closeImage, closeImage, ele);

	// 返回運算後結果
	frame = closeImage.clone();
}

void getMaxArea(cv::Mat& frame, cv::Mat& closeImage)
{
	cv::Mat markedImage;
	std::vector<cv::Point> maxArea;
	cv::RotatedRect minRect;	// 最小外接矩形
	cv::Point2f P[4];			// 矩形的頂點

	cv::cvtColor(closeImage, markedImage, CV_16S);

	// 取輪廓
	std::vector< std::vector<cv::Point> > contours;
	findContours(closeImage,
		contours,				// a vector of contours
		cv::RETR_EXTERNAL,		// 提取外部輪廓
		cv::CHAIN_APPROX_NONE);	// all pixels of each contours

	int max = 0;
	for (size_t i = 0; i < contours.size(); i++) {
		std::vector<cv::Point> p;
		p = contours[i];
		if (p.size() > max) {
			max = static_cast<int>(p.size());
			maxArea = p;
		}
	}
	// 繪製最大區域
	for (size_t i = 0; i < maxArea.size(); i++) {
		cv::circle(markedImage, maxArea[i], 1, cv::Scalar(240, 255, 25));
	}

	// 繪製最小區域
	minRect = minAreaRect(maxArea);
	minRect.points(P);
	for (int j = 0; j <= 3; j++) {
		line(markedImage, P[j], P[(j + 1) % 4], cv::Scalar(255), 2);
	}

	cv::cvtColor(markedImage, frame, CV_8UC3);
}

/**
 *	@brief	影像攫取 Thread-callback
 *	@param[out]	buffer	要輸出影像的繪圖頁的 Ref (cv::Mat)
 *	@param[in]	mutex	用來阻塞用，非同步化石使用同步限制。
 *	@param[in]	keepPtr	用來識別是否結束此一 thread
 *	@param[in]	restPtr	用來識別是否有重新連接影像來源裝置。
 *	@return	此函數沒有返回值
 */
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
