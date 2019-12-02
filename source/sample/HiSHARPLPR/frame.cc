/**************************************************************************//**
 * @file	frame.cc
 * @brief	LPRFrame 類別成員函數定義
 * @date	2019-11-20
 * @date	2019-11-20
 * @author	Swang
 *****************************************************************************/
#include "frame.hh"

/**
 *	@brief	LPRFrame 建構式，初始化成員
 *	@return	此函數沒有返回值
 */
LPRFrame::LPRFrame()
	: DmWnds()
	, m_keeptoDetection(FALSE)
	, m_threadDetection(nullptr) {
}

/**
 *	@brief	LPRFrame 解構式，釋放所有成員
 *	@return	此函數沒有返回值
 */
LPRFrame::~LPRFrame() { this->SafeWndsDestroy(); }

/**
 *	@brief	建立視窗，HiSHARPLPR 主視窗框架
 *	@return	<b>型別: BOOL</b> \n 若視窗建立成功，返回值為非零值。\n 若視窗建立失敗，返回值為零。
 */
BOOL LPRFrame::CreateSafeWindow()
{
	const TCHAR* szClassPtr = this->GetControlsClassName();

	if (szClassPtr == NULL) {
		// 填寫 WINSFRAME 結構內容
		WNDSFRAME ws;
		::memset((void*)&ws, 0, sizeof(WNDSFRAME));
		ws.hInstance = ::GetWinapp().GetInstanceHandle();
		ws.hWndParent = nullptr;
		ws.pszClass = TEXT(HSLPR_CLASSNAME);
		ws.pszCaption = TEXT(HSLPR_TITLENAME);
		ws.nPosx = 0;
		ws.nPosy = 0;
		ws.nWidth = WNDS_DEFAULT_WIDTH;
		ws.nHeight = WNDS_DEFAULT_HEIGHT;
		ws.uClassStyle = 0;
		ws.hBackground = (HBRUSH)(COLOR_BTNFACE + 1);
		ws.hIcon = nullptr;
		ws.hIconSm = nullptr;
		ws.hCursor = nullptr;
		ws.dwStyle = WNDS_DEFAULT_STYLE;
		ws.dwExStyle = WNDS_DEFAULT_EXSTYLE;
		ws.hMenuOrItemID = 0;
		ws.vParam = nullptr;

		if (this->CreateFrame(&ws)) {
			this->SetClientSize(WNDS_DEFAULT_WIDTH, WNDS_DEFAULT_HEIGHT);
			this->CenterWindow();
			this->ShowWindow();
			this->UpdateWindow();
		}
		return this->IsWindow();
	}
	return FALSE;
}

/**
 *	@brief	[重載] 釋放使用者配置資源，此為虛擬函數比需重載。
 *	@return	此函數沒有返回值
 */
void LPRFrame::SafeUserRelease()
{
	this->EndDetection();
}

/**
 *	@brief	[重載] 視窗訊息 WM_CLOSE 通知 (視窗即將被關閉)
 *	@param[in] wParam	參數 1
 *	@param[in] lParam	參數 2
 *	@return	此函數沒有返回值
 */
void LPRFrame::WmClose(WPARAM wParam, LPARAM lParam)
{
	// 結束 車牌檢測 thread (等待 thread 結束)
	this->EndDetection();
	DmWnds::WmClose(wParam, lParam);
}

/**
 *	@brief	視窗訊息處理 WM_USERCREATE (視窗建立訊息)
 *	@param[in] wParam	參數 1
 *	@param[in] lParam	參數 2
 *	@return	此函數沒有返回值
 */
void LPRFrame::WmUserCreate(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	#if defined(USES_VIDEO_MODE)
	// 建立車牌偵測 thread
	SAFE_DELETE(m_threadDetection);
	m_threadDetection = new (std::nothrow) std::thread(&LPRFrame::DetectionProcess, this);
	if (m_threadDetection == nullptr) {
		::MessageBox(*this, TEXT("Create detection thread failed"), TEXT("HSLPR Initialize faile"), MB_OK | MB_ICONWARNING);
		this->PostMessage(WM_CLOSE, 0, 0);
	}
	#else
	cv::String photo = "D:\\Downloads\\14.png";
	(*this).DetectionFormPicture(photo);
	#endif // USES_VIDEO_MODE
}

/**
 *	@brief	[重載] 視窗訊息處理 Callback 函數
 *	@param[in] uMessage	視窗訊息
 *	@param[in] wParam	參數 1, 依據訊息而定
 *	@param[in] lParam	參數 2, 依據訊息而定
 *	@return	<b>型別: LRESULT</b> \n 返回值為訊息處理後狀況，依據處理的訊息而定。
 */
LRESULT LPRFrame::WndProc(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	auto lResult = static_cast<LRESULT>(0);

	switch (uMessage)
	{
	case WM_CREATE:
		lResult = this->WmCreate(wParam, lParam);
		break;
	case WM_DESTROY:
		this->WmDestroy(wParam, lParam);
		break;
	case WM_CLOSE:
		this->WmClose(wParam, lParam);
		break;
	case WM_USERCREATE:
		this->WmUserCreate(wParam, lParam);
		break;
	default:
		return this->DefaultWndProc(uMessage, wParam, lParam);
	}
	return lResult;
}

/**
 *	@brief	偵測車牌 (輸入圖片)
 *	@return	此函數沒有返回值
 */
void LPRFrame::DetectionFormPicture(cv::String& filename)
{
	cv::Mat imOrigin = cv::imread(filename);
	cv::Mat imGray;
	BITMAPINFO bmi;
	int wd, ht;

	if (!imOrigin.data) {
		return;
	}

	wd = imOrigin.cols;
	ht = imOrigin.rows;

	(*this).SetWindowSize(wd*2, ht);
	(*this).CenterWindow();

	::memset(&bmi, 0, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = wd;
	bmi.bmiHeader.biHeight = -ht;			// 標準 BMP 起始座標為左下，若正像圖於 Windows DIB 顯示時將會被反向, 所以要正向顯示必須為高必須為負值。
	bmi.bmiHeader.biPlanes = 1;				// must be 1
	bmi.bmiHeader.biBitCount = 24;
	bmi.bmiHeader.biXPelsPerMeter = 0;
	bmi.bmiHeader.biYPelsPerMeter = 0;

	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biClrUsed = 0;
	bmi.bmiHeader.biClrImportant = 0;
	bmi.bmiHeader.biSizeImage = 0;

	cv::cvtColor(imOrigin, imGray, cv::COLOR_BGR2GRAY);
	::FrameToContrast(imGray);
	::FrameToSobel(imGray);
	//::FrameToCanny(imGray);
	::FrameCloseOperation(imGray);

	cv::Mat a1, a2;
	cv::Mat b1, b2;

	cv::cvtColor(imOrigin, a1, CV_16S);
	cv::cvtColor(imGray, a2, CV_16S);

	cv::convertScaleAbs(a1, b1);
	cv::convertScaleAbs(a2, b2);
	cv::addWeighted(b1, 0.5, b2, 0.5, 0, imOrigin);

	//cv::cvtColor(imOrigin, imOrigin, cv::COLOR_GRAY2RGB);
	cv::cvtColor(imGray, imGray, cv::COLOR_GRAY2RGB);
	::ShowSurface((*this).GetSafeHwnd(), 0, 0, wd, ht, &bmi, reinterpret_cast<void*>(imOrigin.data));
	::ShowSurface((*this).GetSafeHwnd(), wd, 0, wd, ht, &bmi, reinterpret_cast<void*>(imGray.data));
}

/**
 *	@brief	結束車牌偵測 Thread
 *	@return	此函數沒有返回值
 */
void LPRFrame::EndDetection()
{
	// 結束 車牌檢測 thread (等待 thread 結束)
	m_keeptoDetection = FALSE;

	if (m_threadDetection != nullptr) {
		if (m_threadDetection->joinable()) {
			m_threadDetection->join();
		}
	}
	SAFE_DELETE(m_threadDetection);
}

/**
 *	@brief (Thread callback) 車牌偵測 - 影像播放與擷取
 *	@param[in] thisPtr LPRFrame 類別物件指標
 *	@return	此函數沒有返回值
 */
void LPRFrame::DetectionProcess(LPRFrame* framePtr)
{
	bool keepCapture = true;
	std::mutex videoMutex;
	cv::Mat imBuffer;
	cv::Mat imOrigin;
	cv::Mat imGrayScale;
	std::thread* threadCapture = nullptr;
	BITMAPINFO bmi;

	bool videoReset = true;
	auto videoWidth = static_cast<int>(0);
	auto videoHeight = static_cast<int>(0);

	auto lastTime = ::timeGetTime();
	auto thisTime = lastTime;
	auto frameCount = static_cast<int>(0);
	TCHAR text[BUFF_SIZE_SMALL];

	/* 無效來源物件指標 */
	if (framePtr == nullptr) {
		// todo : error handling 可從這裡拋出一個錯誤
		return;
	}

	/* 建立影像擷取 thread */
	threadCapture = new (std::nothrow) std::thread(VideoCaptureProcess, std::ref(imBuffer), std::ref(videoMutex), &keepCapture, &videoReset);
	if (threadCapture == nullptr) {
		// todo : error handling 可從這裡拋出一個錯誤
		return;
	}
	
	/* 車白偵測 thread 主要動作區塊 */
	(*framePtr).m_keeptoDetection = TRUE;
	while ((*framePtr).m_keeptoDetection) {
		videoMutex.lock();
		imOrigin = imBuffer.clone();
		videoMutex.unlock();

		if (imOrigin.data == nullptr) {
			std::this_thread::sleep_for(std::chrono::milliseconds(30));
			continue;
		}

		if (videoReset) {
			videoWidth = imOrigin.cols;
			videoHeight = imOrigin.rows;
			(*framePtr).SetClientSize(videoWidth * 2, videoHeight);
			(*framePtr).CenterWindow();

			::memset(&bmi, 0, sizeof(BITMAPINFO));
			bmi.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
			bmi.bmiHeader.biWidth			= videoWidth;
			bmi.bmiHeader.biHeight			= -videoHeight;		// 標準 BMP 起始座標為左下，若正像圖於 Windows DIB 顯示時將會被反向, 所以要正向顯示必須為高必須為負值。
			bmi.bmiHeader.biPlanes			= 1;				// must be 1
			bmi.bmiHeader.biBitCount		= 24;
			bmi.bmiHeader.biXPelsPerMeter	= 0;
			bmi.bmiHeader.biYPelsPerMeter	= 0;

			bmi.bmiHeader.biCompression		= BI_RGB;
			bmi.bmiHeader.biClrUsed			= 0;
			bmi.bmiHeader.biClrImportant	= 0;
			bmi.bmiHeader.biSizeImage		= 0;
			videoReset = false;
		}

		/* 建立灰階繪圖頁 */
		cv::cvtColor(imOrigin, imGrayScale, cv::COLOR_BGR2GRAY);
		//::FrameToContrast(imGrayScale);
		//::FrameToSobel(imGrayScale);
		//::FrameToCanny(imGrayScale);
		//::FrameCloseOperation(imGrayScale);

		::FrameToContrast(imGrayScale);
		::FrameToSobel(imGrayScale);
		//::FrameToCanny(imGrayScale);
		//::FrameCloseOperation(imGrayScale);

		cv::Mat a1, a2;
		cv::Mat b1, b2;

		cv::cvtColor(imOrigin, a1, CV_16S);
		cv::cvtColor(imGrayScale, a2, CV_16S);

		cv::convertScaleAbs(a1, b1);
		cv::convertScaleAbs(a2, b2);
		cv::addWeighted(b1, 1.0, b2, 0.5, 0, imOrigin);

		/* FPS (幀率) 計算與顯示 */
		thisTime = ::timeGetTime();
		auto div = thisTime - lastTime;

		if (div > 1000) {
			frameCount *= 1000;
			auto fps = static_cast<double>(frameCount / div);

			#if defined(ODMC_UNICODE)
			auto fpstext = std::to_wstring(fps);
			#else
			auto fpstext = std::to_string(fps);
			#endif

			::wsprintf(text, TEXT("Video (width = %4d, height = %4d), FPS = %s"), videoWidth, videoHeight, fpstext.c_str());
			framePtr->SetWindowText(text);
			lastTime = thisTime;
			frameCount = 0;
		}
		else frameCount++;

		/* 輸出圖像 */
		cv::cvtColor(imGrayScale, imGrayScale, cv::COLOR_GRAY2RGB);
		::ShowSurface((*framePtr).GetSafeHwnd(), 0, 0, videoWidth, videoHeight, &bmi, reinterpret_cast<void*>(imOrigin.data));
		::ShowSurface((*framePtr).GetSafeHwnd(), videoWidth, 0, videoWidth, videoHeight, &bmi, reinterpret_cast<void*>(imGrayScale.data));
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	// 清除 video capture thread
	if (threadCapture != nullptr) {
		if (threadCapture->joinable()) {
			keepCapture = false;
			threadCapture->join();
		}
	}
	SAFE_DELETE(threadCapture);
}
