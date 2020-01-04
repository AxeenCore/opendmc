/**************************************************************************//**
 * @file	detection.cc
 * @brief	CxPlateDetection 類別成員函數定義
 * @date	2019-12-09
 * @date	2019-12-09
 * @author	Swang
 *****************************************************************************/
#include "detection.hh"

/**
 *	@brief	CarPlateDetection 建構式
 *	@return	此函數沒有返回值
 */
CarPlateDetection::CarPlateDetection()
	: m_isDetecting(false)
	, m_hWndTarget(nullptr)
	, m_cKeepDetection(nullptr)
	, m_cProcDetection(nullptr) {

}

/**
 *	@brief	CarPlateDetection 解構式
 *	@return	此函數沒有返回值
 */
CarPlateDetection::~CarPlateDetection()
{
	this->DetectionEnd();
}

/**
 *	@brief	開始進行車牌偵測
 *	@param[in]	要輸出影像的視窗 handle
 *	@return	<b>型別: bool</b> \n 若開啟成功返回值為非零值 (true)。 \n 若開啟失敗返回值為零 (false)。
 */
bool CarPlateDetection::DetectionRun(HWND hWnd)
{
	for (;;) {
		// is exist thread running?
		this->DetectionEnd();

		// 目標不是視窗
		if (!::IsWindow(hWnd)) {
			break;
		}
		m_hWndTarget = hWnd;

		m_cKeepDetection = new (std::nothrow) std::atomic<bool>();
		if (m_cKeepDetection == nullptr) {
			break;
		}

		m_cProcDetection = new (std::nothrow) std::thread(&CarPlateDetection::DetectionProcess, this);
		if (m_cProcDetection == nullptr) {
			break;
		}
		return m_cProcDetection != nullptr;
	}

	this->DetectionEnd();
	return false;
}

/**
 *	@brief	結束進行車牌偵測，釋放所有資源。
 *	@return	此函數沒有返回值
 */
void CarPlateDetection::DetectionEnd()
{
	// 設定執行車牌偵測識別
	if (m_cKeepDetection != nullptr) {
		m_cKeepDetection->store(false);
	}

	// 等候車牌偵測 thread (process) 結束
	if (m_cProcDetection != nullptr) {
		if (m_cProcDetection->joinable()) {
			m_cProcDetection->join();
		}
	}

	// 釋放資源
	SAFE_DELETE(m_cProcDetection);
	SAFE_DELETE(m_cKeepDetection);
	m_hWndTarget = nullptr;
}

/**
 *	@brief	預處理車牌定位圖形
 *	@param[in]	imgFrame		cv::Mat 物件參考，要預處理的原始影像。
 *	@param[out]	imgGrayscale	cv::Mat 物件參考，用來保存灰階化影像。
 *	@return	<b>型別: bool</b> \n 若預處理過程成功，返回值為非零值 (true)。 \n 若預處理過程失敗，返回值為零 (false)。
 */
bool CarPlateDetection::ImagePreprocess(
	cv::Mat& imgFrame,
	cv::Mat& imgGrayscale,
	cv::Mat& imgThresh,
	EdgeType edgeType)
{
	cv::Mat imgContrast;
	cv::Mat imgBlurred;
	cv::Size sizeBlurred(CAR_PLATE_BLUE_WD, CAR_PLATE_BLUE_HT);

	for (;;) {
		// 灰階化
		// cv::cvtColor(imgFrame, imgGrayscale, cv::COLOR_BGR2GRAY);
		this->ImageGrayscale(imgFrame, imgGrayscale);
		if (imgGrayscale.data == nullptr) break;

		// 提高灰階影像對比
		this->ImageContrast(imgGrayscale, imgContrast);
		if (imgContrast.data == nullptr) break;

		// 進行平滑化
		cv::GaussianBlur(imgContrast, imgBlurred, sizeBlurred, cv::BORDER_CONSTANT);
		if (imgBlurred.data == nullptr) break;

		// 
		switch (edgeType) {
		case EdgeType::Sobel:
			break;
		case EdgeType::Canny:
			cv::Canny(imgBlurred, imgThresh, 70, 210);
			break;
		default:
			cv::adaptiveThreshold(imgBlurred, imgThresh, 255.0, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, CAR_PLATE_THRESH_BLOCK_SIZE, CAR_PLATE_THRESH_WEIGHT);
			break;
		}

		if (imgThresh.data == nullptr) break;
		return true;
	}
  	return false;
}

/**
 *	@brief	灰階轉換, RGB or BGR to grayscale
 *	@param[in]	imgFrame		cv::Mat 物件參考，要進行灰階轉換的影像。
 *	@param[out]	imgGrayscale	cv::Mat 物件參考，存放轉換後灰階化影像。
 *	@param[in]	scaleType		指定要使用的方式(預設為 0 = YUV 方式取 Y)
 *	@return	此函數沒有返回值
 */
void CarPlateDetection::ImageGrayscale(cv::Mat& imgFrame, cv::Mat& imgGrayscale, int scaleType)
{
	if (scaleType == 1) {
		cv::Mat imgHSV;
		cv::Mat imgValue;
		std::vector<cv::Mat> vectorOfHSVImages;

		// 轉換為 HSV 通道
		cv::cvtColor(imgFrame, imgHSV, cv::COLOR_BGR2HSV);

		cv::split(imgHSV, vectorOfHSVImages);
		imgGrayscale = vectorOfHSVImages[2].clone();

		// 釋放 vector 資源
		vectorOfHSVImages.clear();
		vectorOfHSVImages.shrink_to_fit();
	}
	else {
		cv::cvtColor(imgFrame, imgGrayscale, cv::COLOR_BGR2GRAY);
	}
}

/**
 *	@brief	提高影像對比
 *	@param[in]	imgGrayscale	cv::Mat 物件參考，要進行提高對比的灰階(單通道)影像
 *	@param[out]	imgContrast		cv::Mat 物件參考，用來保存提高對比後的影像
 *	@return	此函數沒有返回值
 */
void CarPlateDetection::ImageContrast(cv::Mat& imgGrayscale, cv::Mat& imgContrast)
{
	cv::Mat imgTophat;
	cv::Mat imgBlackhat;
	cv::Mat	imgTophatPlus;

	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
	cv::morphologyEx(imgGrayscale, imgTophat, cv::MORPH_TOPHAT, element);
	cv::morphologyEx(imgGrayscale, imgBlackhat, cv::MORPH_BLACKHAT, element);
	cv::add(imgGrayscale, imgTophat, imgTophatPlus);
	cv::subtract(imgTophatPlus, imgBlackhat, imgContrast);
}

/**
 *	@brief	繪製輪廓
 *	@param[in,out]	imgThresh	cv::Mat 物件參考，輸入二值化影像，導出輪廓影像。
 *	@return	此函數沒有返回值
 */
void CarPlateDetection::ImageDrawContours(cv::Mat& imgThresh)
{
	std::vector<std::vector<cv::Point> > vecContours;

	cv::findContours(imgThresh, vecContours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
	imgThresh = cv::Scalar::all(0);
	cv::drawContours(imgThresh, vecContours, -1, cv::Scalar::all(255));

	// 清理 contours for each contour
	for (auto& _itemContours : vecContours) {
		_itemContours.clear();
		_itemContours.shrink_to_fit();
	}
	vecContours.clear();
	vecContours.shrink_to_fit();
}

/**
 *	@brief	合併圖像
 *	@param[in,out]	imgFrame	cv::Mat 物件參考, 要疊合的基底影像	
 *	@param[in]		imgAddone	cv::Mat	物件參考, 要附加的影像	
 *	@param[in]		alpha		附加影像 alpha 深度 (預設 50%) (最大值 = 100, 最小值 = 0)
 *	@return	此函數蠻有返回職
 */
void CarPlateDetection::ImageMatch(cv::Mat& imgFrame, cv::Mat& imgAddone, int alpha)
{
	auto alphaPercen = static_cast<double>(alpha) / 100.0;
	cv::Mat tmp1, tmp2;
	cv::Mat3b::iterator it = imgAddone.begin<cv::Vec3b>();
	cv::Mat3b::iterator itend = imgAddone.end<cv::Vec3b>();

	for (; it != itend; ++it)
	{
		//(*it)[0] = (*it)[0] = 0;
		//(*it)[1] = (*it)[1] = 0;
		(*it)[2] = (*it)[2] = 0;
	}

	cv::cvtColor(imgFrame,  tmp1, CV_16S);
	cv::cvtColor(imgAddone, tmp2, CV_16S);

	cv::convertScaleAbs(tmp1, tmp1);
	cv::convertScaleAbs(tmp2, tmp2);
	cv::addWeighted(tmp1, 1.0, tmp2, alphaPercen, 0, imgFrame);
}

/**
 *	@brief	顯示 BMP 圖形
 *	@param[in] x		座標 X
 *	@param[in] y		座標 Y
 *	@param[in] wd		圖形寬度
 *	@param[in] ht		圖形高度
 *	@param[in] bmiPtr	(指標) 指向 BITMAPINFO 結構緩衝區，內容為 BMP 圖形資訊。
 *	@param[in] dataPtr	(指標) 指向圖形資料緩衝區
 *	@return	<b>型別: bool</b> \n 若函數操作成功返回值為非零值。 \n 若函數操作失敗則返回值為零。
 */
bool CarPlateDetection::DisplayBitmap(int x, int y, int wd, int ht, BITMAPINFO* bmiPtr, void* dataPtr) const
{
	assert(::IsWindow(m_hWndTarget));
	HDC hDC = nullptr;

	if (dataPtr == nullptr || bmiPtr == nullptr) {
		return false;
	}

	// Get target window DC (device context)
	if ((hDC = ::GetDC(m_hWndTarget)) == nullptr) {
		return false;
	}

	// draw image to target device contex
	auto okey = ::SetDIBitsToDevice(
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

	if (hDC) ::ReleaseDC(m_hWndTarget, hDC);
	return okey != 0;
}

/**
 *	@brief 將視窗顯示未置定於父視窗 Client 區的中央位置, 若沒有父視窗則以桌面上中央位置顯示
 *	@return <b>型別: BOOL</b>
 *		\n 如果運作成功，則返回值為非零值。
 *		\n 如果函數失敗，則返回值為零。要取得錯誤信息，調用 GetLastError()
 *	@remark 若有父視窗, 將以父視窗的 Client 區塊為基準. 若無父視窗將以桌面做為基準.
 */
bool CarPlateDetection::SetCenterPosition() const
{
	assert(::IsWindow(m_hWndTarget));

	BOOL bResult = FALSE;
	HWND hWnd = m_hWndTarget;
	HWND hParent;
	int  x, y, w, h;
	RECT rc;

	if ((hParent = ::GetParent(hWnd)) == NULL) {
		// 沒有父視窗，就取得桌面邊界尺寸
		x = ::GetSystemMetrics(SM_CXSCREEN);
		y = ::GetSystemMetrics(SM_CYSCREEN);
	}
	else {
		// 取得父視窗工作區邊界
		::GetClientRect(hParent, &rc);
		x = static_cast<int>(rc.right - rc.left);
		y = static_cast<int>(rc.bottom - rc.top);
	}

	if (::GetWindowRect(hWnd, &rc)) {
		// 計算居中值
		w = static_cast<int>(rc.right - rc.left);
		h = static_cast<int>(rc.bottom - rc.top);

		if (x > w) x = (x - w) >> 1;
		if (y > h) y = (y - h) >> 1;
		bResult = this->SetWindowPosition(x, y);
	}
	return bResult;
}

/**
 *	@brief 設定視窗顯示位置
 *	@param[in] x 於父視窗 client-area 座標 x (左邊界)
 *	@param[in] y 於父視窗 client-area 座標 y (上邊界)
 *	@return <b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero. To get extended error information, call GetLastError.
 */
bool CarPlateDetection::SetWindowPosition(int x, int y) const
{
	assert(::IsWindow(m_hWndTarget));
	const UINT flag = SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE;
	return ::SetWindowPos(m_hWndTarget, NULL, x, y, 0, 0, flag) == TRUE;
}

/**
 *	@brief 設定視窗大小
 *	@param[in] wd	寬度
 *	@param[in] ht	高度
 *	@return <b>型別: BOOL</b>
 *		\n 若運作成功返回值為非零值。
 *		\n 若運作失敗則返回值為零，失敗呼叫 GetLastError 取得錯誤訊息。
 */
bool CarPlateDetection::SetWindowSize(int wd, int ht) const
{
	assert(::IsWindow(m_hWndTarget));
	const UINT uFlag = SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE;
	return ::SetWindowPos(m_hWndTarget, NULL, 0, 0, wd, ht, uFlag) == TRUE;
}

/**
 *	@breif	設定 bitmap 資訊，並改變目標視窗大小。
 *	@param[out]	bmi	結構 BITMAPINFO 緩衝區位址，存放資料定義結果。
 *	@param[in]	wd	影像寬度
 *	@param[in]	ht	影像高度
 *	@return	此函數沒有返回值
 */
void CarPlateDetection::Resize(BITMAPINFO* bmi, int wd, int ht) const
{
	if (bmi != nullptr) {
		// 設定 BITMAPINFO 資訊
		::memset(bmi, 0, sizeof(BITMAPINFO));
		(*bmi).bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		(*bmi).bmiHeader.biWidth = wd;
		(*bmi).bmiHeader.biHeight = -ht;			// 標準 BMP 起始座標為左下，若正像圖於 Windows DIB 顯示時將會被反向, 所以要正向顯示必須為高必須為負值。
		(*bmi).bmiHeader.biPlanes = 1;				// must be 1
		(*bmi).bmiHeader.biBitCount = 24;			// 色彩深度
		(*bmi).bmiHeader.biXPelsPerMeter = 0;
		(*bmi).bmiHeader.biYPelsPerMeter = 0;
		(*bmi).bmiHeader.biCompression = BI_RGB;
		(*bmi).bmiHeader.biClrUsed = 0;
		(*bmi).bmiHeader.biClrImportant = 0;
		(*bmi).bmiHeader.biSizeImage = 0;

		// 改變視窗大小
		if (m_hWndTarget != nullptr) {
			this->SetWindowSize(wd, ht);
			this->SetCenterPosition();
		}
	}
}

DWORD CarPlateDetection::ShowFPS(DWORD lastTime, int frameCount, int bufSize)
{
	const int multi = 1000;

	auto timeThis = ::timeGetTime();
	TCHAR text[BUFF_SIZE_SMALL];

	if ((timeThis - lastTime) > multi) {
		frameCount *= multi;
		auto fps = static_cast<double>(frameCount / multi);

		#if defined(ODMC_UNICODE)
		auto fpstext = std::to_wstring(fps);
		#else
		auto fpstext = std::to_string(fps);
		#endif

		::wsprintf(text
			, TEXT("HS 車牌辨識 - FPS = %s, 待處理 = %lu 幀")
			, fpstext.c_str()
			, static_cast<int>(bufSize));

		::SetWindowText(m_hWndTarget, text);
		return timeThis;
	}
	return 0;
}

/**
 *	@brief	車牌偵測 thread process
 *	@param[in] hWnd		輸出畫面目標視窗的 handle
 *	@param[in] keepPtr	車牌偵測持續執行識別
 */
void CarPlateDetection::DetectionProcess(CarPlateDetection* cThisPtr)
{
	if (cThisPtr == nullptr) {
		// To do error handling
		return;
	}

	if (cThisPtr->m_cKeepDetection == nullptr) {
		// To do error handling
		return;
	}

	if (cThisPtr->m_hWndTarget == nullptr) {
		// To do error handling
		return;
	}

	bool isReset = false;
	std::atomic<bool>	camKeep;
	std::queue<cv::Mat>	camQueue;
	std::mutex			camMutex;
	std::thread			threadCapture(&CarPlateDetection::CaptureProcess, std::ref(camKeep), std::ref(camQueue), std::ref(camMutex), &isReset);

	// 影像資訊宣告與定義
	cv::Mat imgFrame;
	cv::Mat	imgGrayscale;
	cv::Mat	imgThresh;

	BITMAPINFO bmi;
	size_t	bufSize;
	int wd = 0;
	int ht = 0;

	DWORD thisTime;
	DWORD lastTime = ::timeGetTime();
	DWORD frameCount = 0;

	DWORD nextTime;
	DWORD prevTime = lastTime;

	// 設定 thread 運作識別，並開始運作
	cThisPtr->m_cKeepDetection->store(true);
	while (cThisPtr->m_cKeepDetection->load()) {
		camMutex.lock();
		bufSize = camQueue.size();
		if (bufSize > 0) {
			// get the oldest grabbed imgFrame (queue=FIFO)
			camQueue.front().copyTo(imgFrame);
			// release the queue item
			camQueue.pop();
		}
		camMutex.unlock();

		if (bufSize <= 0) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			continue;
		}

		// 設定圖像輸出資訊
		if (isReset) {
			wd = imgFrame.cols;
			ht = imgFrame.rows;			
			cThisPtr->Resize(&bmi, wd, ht);
			isReset = false;
		}

		// 幀處理累加 (for FPS)
		frameCount++;

		// 跳幀處理
		nextTime = ::timeGetTime();
		if ((nextTime - prevTime) < 250) {
			continue;
		}
		else prevTime = nextTime;

		// 預處理影像
		if (!cThisPtr->ImagePreprocess(imgFrame, imgGrayscale, imgThresh)) {
			continue;
		}

		// 繪製輪廓
		cThisPtr->ImageDrawContours(imgThresh);

		// 輸出影像至視窗
		cv::cvtColor(imgThresh, imgThresh, cv::COLOR_GRAY2RGB);
		cThisPtr->ImageMatch(imgFrame, imgThresh);

		// FPS (幀率) 計算與顯示
		thisTime = cThisPtr->ShowFPS(lastTime, static_cast<int>(frameCount), static_cast<int>(bufSize));
		if (thisTime != 0) {
			lastTime = thisTime;
			frameCount = 0;
		}
		else frameCount++;

		// 顯示影像
		cThisPtr->DisplayBitmap(0, 0, wd, ht, &bmi, imgFrame.data);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	// thread 結束運作
	cThisPtr->m_cKeepDetection->store(false);

	// 關閉並銷毀 threadCapture 物件
	camKeep.store(false);
	if (threadCapture.joinable()) {
		threadCapture.join();
	}

	// 清除 matQueue
	while (!camQueue.empty()) {
		imgFrame = camQueue.front();
		camQueue.pop();
	}
}

/**
 *	@brief	Video caputre thread process
 *	@param[in]		atmKeep		(參考) Thread 是否持續運作識別
 *	@param[in]		matQueue	(參考) 紀錄影像的 Queue (buffer 格式為 cv::Mat 格式)
 *	@param[in]		camMutex	(參考) 用來保護紀錄影像 Queue 的 mutex
 *	@param[in,out]	rstPtr		(指標) 用來標註是否重新連接攝影機影像串流
 *	@return	此函數沒有返回值
 */
void CarPlateDetection::CaptureProcess(std::atomic<bool>& camKeep, std::queue<cv::Mat>& camQueue, std::mutex& camMutex, void* rstPtr)
{
	const int errLimit = 90;
	#if defined(CAR_PLATE_USING_VIDEOFILE)
	const cv::String url = "d:\\Downloads\\0000\\d5_001.mp4";
	#else
	const cv::String url = "rtsp://admin:hisharp123456@210.61.217.151:554/unicast/c2/s1/live";
	int errCount = 0;
	#endif

	size_t	bufSize;
	cv::VideoCapture* cap = nullptr;
	cv::Mat tmp;

	// 無效的指標
	if (rstPtr == nullptr) {
		camKeep.store(false);
		return;
	}

	// 設定 thread 運作識別，並開始運作
	camKeep.store(true);
	while (camKeep.load() == true) {
		// 若 cap 為無效的 cv::VideoCapture 物件, 建立新的 VideoCapture 物件
		if (cap == nullptr) {
			cap = new (std::nothrow) cv::VideoCapture(url);

			// cap 物件建立失敗
			if (cap == nullptr) {
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				continue;
			}

			// cap 已正確開啟串流?
			if (!(*cap).isOpened()) {
				(*cap).release();
				SAFE_DELETE(cap);
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				continue;
			}

			// cap 物件建立成功
			(*cap).set(cv::CAP_PROP_FORMAT, CV_8UC3);
			auto resetPtr = reinterpret_cast<bool*>(rstPtr);
			*resetPtr = true;
		}

		// cap 進行影像保存作業
		*cap >> tmp;

		// 沒有擷取到影像
		if (tmp.empty()) {
			#if !defined(CAR_PLATE_USING_VIDEOFILE)
			if (errCount >= errLimit) {
				(*cap).release();
				SAFE_DELETE(cap);
			}
			else errCount++;
			#endif
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			continue;
		}

		#if defined(CAR_PLATE_USING_VIDEOFILE)
		cv::resize(tmp, tmp, cv::Size(1280, 720));
		#endif

		// 將影像保存至 Queue
		camMutex.lock();

		// 防止記憶體爆掉
		bufSize = camQueue.size();
		if (bufSize > 30) {
			camQueue.pop();
		}
		camQueue.push(cv::Mat(tmp.size(), tmp.type()));
		tmp.copyTo(camQueue.back());
		camMutex.unlock();

		#if !defined(CAR_PLATE_USING_VIDEOFILE)
		errCount = 0;
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		#else
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
		#endif
	}

	// thread 持續運作關閉, 銷毀 VideoCapture 物件
	if (cap != nullptr) {
		(*cap).release();
		SAFE_DELETE(cap);
	}
	camKeep.store(false);
}
