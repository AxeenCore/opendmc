/**************************************************************************//**
 * @file	lpr_frame.cc
 * @brief	CxLprFrame 類別成員函數定義
 * @date	2019-11-20
 * @date	2019-11-20
 * @author	Swang
 *****************************************************************************/
#include "lpr_frame.hh"

/**
 *	@brief	開啟影像串流
 *	@param[in] url 來源影像位置
 *	@return	<b>型別: cv::VideoCaption</b>
 *		\n 若開啟成功返回值為非零值，值為 cv::VideoCaption 類別物件指標
 *		\n 若開啟失敗返回值為零 (nullptr)
 */
cv::VideoCapture* OpenCapture(const cv::String& url)
{
	cv::VideoCapture* stream = new (std::nothrow) cv::VideoCapture(url);

	for (;;) {
		if (stream != NULL) {
			if (!stream->isOpened()) {
				stream->release();
				SAFE_DELETE(stream);
				break;
			}
		}
		break;
	}
	return stream;
}

void TransferToWindow(HWND hWnd, int wd, int ht, BITMAPINFO* bmiPtr, void* dataPtr)
{
	HDC hDC = nullptr;

	for (;;) {
		if (hWnd == nullptr || dataPtr == nullptr || bmiPtr == nullptr) break;

		/* Get target window DC (device context) */
		if ((hDC = ::GetDC(hWnd)) == nullptr) break;

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
	cv::threshold(frame, frame, 0, 255, CV_THRESH_BINARY + CV_THRESH_OTSU);
}

void FrameToCanny(cv::Mat& frame)
{
	// 高斯平滑
	cv::GaussianBlur(frame, frame, cv::Size(3, 3), 0, 0);
	cv::Canny(frame, frame, 150, 100);
	// 二值化
	cv::threshold(frame, frame, 0, 255, CV_THRESH_BINARY + CV_THRESH_OTSU);
}

void FrameErosionDilation(cv::Mat frame)
{
	//cv::Mat kernel1 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 1), cv::Point(-1, -1));
	//cv::Mat kernel2 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 3), cv::Point(-1, -1));
	cv::Mat kernel3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(21, 5), cv::Point(-1, -1));
	cv::Mat k1, k2, k3;

	//cv::morphologyEx(frame, k1, CV_MOP_CLOSE, kernel1);
	//cv::morphologyEx(k1, k2, CV_MOP_CLOSE, kernel2);
	cv::morphologyEx(frame, k3, CV_MOP_CLOSE, kernel3);
	frame = k3.clone();
}

/**
 *	@brief	CxLprFrame 建構式
 */
CxLprFrame::CxLprFrame()
	: DmWnds()
	, m_keepRename(FALSE)
	, m_keepDetection(FALSE)
	, m_threadRename(nullptr)
	, m_threadDetection(nullptr) {
}

/**
 *	@brief	CxLprFrame 解構式
 */
CxLprFrame::~CxLprFrame() { this->SafeWndsDestroy(); }

/**
 *	@brief	建立視窗
 *	@return	<b>型別: BOOL</b>
 *		\n 若視窗建立成功，返回值為非零值。
 *		\n 若視窗建立失敗，返回值為零。
 */
BOOL CxLprFrame::CreateSafe()
{
	const TCHAR* szClassPtr = this->GetControlsClassName();

	if (szClassPtr == NULL) {
		// 填寫 WINSFRAME 結構內容
		WNDSFRAME ws;
		::memset((void*)&ws, 0, sizeof(WNDSFRAME));
		ws.hInstance	= ::GetWinapp().GetInstanceHandle();
		ws.hWndParent	= nullptr;
		ws.pszClass		= TEXT(HSLPR_CLASSNAME);
		ws.pszCaption	= TEXT(HSLPR_TITLENAME);
		ws.nPosx		= 0;
		ws.nPosy		= 0;
		ws.nWidth		= WNDS_DEFAULT_WIDTH;
		ws.nHeight		= WNDS_DEFAULT_HEIGHT;
		ws.uClassStyle	= 0;
		ws.hBackground	= (HBRUSH)(COLOR_BTNFACE + 1);
		ws.hIcon		= nullptr;
		ws.hIconSm		= nullptr;
		ws.hCursor		= nullptr;
		ws.dwStyle		= WNDS_DEFAULT_STYLE;
		ws.dwExStyle	= WNDS_DEFAULT_EXSTYLE;
		ws.hMenuOrItemID= 0;
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
void CxLprFrame::SafeUserRelease()
{
	if (m_threadRename) {
		m_keepRename = FALSE;
		m_threadRename->join();
	}
	SAFE_DELETE(m_threadRename);
	m_cvMat.release();
}

/**
 *	@brief	視窗訊息處理 WM_USERCREATE (視窗建立訊息)
 *	@param[in] wParam	參數 1
 *	@param[in] lParam	參數 2
 *	@return	此函數沒有返回值
 */
void CxLprFrame::WmUserCreate(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	SAFE_DELETE(m_threadDetection);
	SAFE_DELETE(m_threadRename);
	//m_keepRename = TRUE;
	//m_threadRename = new (std::nothrow) std::thread(&CxLprFrame::Rename, this);

	m_keepDetection = TRUE;
	m_threadDetection = new (std::nothrow) std::thread(&CxLprFrame::Detection, this);
	if (m_threadDetection == nullptr) {
		::MessageBox(*this, TEXT("Create detection thread failed"), TEXT("HSLPR Initialize faile"), MB_OK | MB_ICONWARNING);
		this->PostMessage(WM_CLOSE, 0, 0);
	}
}

/**
 *	@brief	[重載] 視窗訊息處理 Callback 函數
 *	@param[in] uMessage	視窗訊息
 *	@param[in] wParam	參數 1, 依據訊息而定
 *	@param[in] lParam	參數 2, 依據訊息而定
 *	@return	<b>型別: LRESULT</b> \n 返回值為訊息處理後狀況，依據處理的訊息而定。
 */
LRESULT CxLprFrame::WndProc(UINT uMessage, WPARAM wParam, LPARAM lParam)
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
		m_keepRename = FALSE;
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
 *	@brief	改變標題名稱
 */
void CxLprFrame::Rename(CxLprFrame* thisPtr)
{
	int value = 0;
	TCHAR sz[BUFF_SIZE_SMALL];

	while (thisPtr->m_keepRename) {
		_itot(value++, sz, 10);
		thisPtr->SetWindowText(sz);
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}

/**
 *	@brief	車牌偵測 - 影像播放與擷取
 *	@param[in] thisPtr CxLprFrame 類別物件指標
 *	@return	此函數沒有返回值
 */
void CxLprFrame::Detection(CxLprFrame* thisPtr)
{
	const cv::String url = "rtsp://admin:hisharp123456@210.61.217.151:554/unicast/c5/s1/live";
	const cv::String wname = "原始圖";

	cv::VideoCapture* stream = nullptr;
	cv::Mat grayFrame;

	// cv::Mat frame;
	int			wd = 0;
	int			ht = 0;
	int			err = 0;
	BOOL		bReCamlink = FALSE;
	BITMAPINFO	bmi;
	TCHAR		sztext[BUFF_SIZE_SMALL];

	DWORD		lastTime;
	DWORD		thisTime;
	int			frameCount;

	// 驗證來源物件正確性
	if (thisPtr == nullptr || !thisPtr->IsWindow()) {
		SAFE_DELETE(stream);
		return;
	}

	cv::namedWindow(wname, cv::WINDOW_AUTOSIZE /* cv::WINDOW_NORMAL */);

	// 線程 (執行序) 迴圈
	while (thisPtr->m_keepDetection) {
		// 連接攝影機，進行串流 (RTSP)
		if (stream == nullptr) {
			stream = ::OpenCapture(url);

			if (stream == nullptr) {
				// 失敗次數限制
				if (err > 5) {
					thisPtr->SetWindowText(TEXT("開啟串流失敗"));
					break;
				}

				// 失敗!!!
				err++;
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
				continue;
			}

			// 指定影像色彩格式為 PixelFormat24bppRGB
			stream->set(CV_CAP_PROP_FORMAT, CV_8UC3);
			wd = static_cast<int>(stream->get(CV_CAP_PROP_FRAME_WIDTH));
			ht = static_cast<int>(stream->get(CV_CAP_PROP_FRAME_HEIGHT));
			bReCamlink = TRUE;
		}
		err = 0;

		for (;;) {
			if (err > 5) {
				thisPtr->SetWindowText(TEXT("讀取影像失敗"));
				stream->release();
				SAFE_DELETE(stream);
				break;
			}

			//thisPtr->SetWindowText(TEXT("等待攝影機 或 RTSP 影像"));
			if (!stream->read(thisPtr->m_cvMat)) {
				err++;
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
				continue;
			}
			err = 0;
			break;
		}

		// thisPtr->SetWindowText(TEXT("圖形演算..."));
		if (stream) {
			if (bReCamlink) {
				thisPtr->SetClientSize(wd, ht);
				thisPtr->CenterWindow();

				::memset(&bmi, 0, sizeof(BITMAPINFO));
				bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
				bmi.bmiHeader.biWidth = wd;
				bmi.bmiHeader.biHeight = -ht;		// 標準 BMP 起始座標為左下，若正像圖於 Windows DIB 顯示時將會被反向, 所以要正向顯示必須為高必須為負值。
				bmi.bmiHeader.biPlanes = 1;		// must be 1
				bmi.bmiHeader.biBitCount = 24;
				bmi.bmiHeader.biXPelsPerMeter = 0;
				bmi.bmiHeader.biYPelsPerMeter = 0;

				bmi.bmiHeader.biCompression = BI_RGB;
				bmi.bmiHeader.biClrUsed = 0;
				bmi.bmiHeader.biClrImportant = 0;
				bmi.bmiHeader.biSizeImage = 0;

				lastTime = ::timeGetTime();
				frameCount = 0;
				bReCamlink = FALSE;
			}

			thisTime = ::timeGetTime();
			if ((thisTime - lastTime) > 1000) {
				auto div = thisTime - lastTime;
				auto fps = 0.0f;



				lastTime = thisTime;
				::wsprintf(sztext, TEXT("輸出幀率 : %0.2f"), frameCount);
				//thisPtr->SetWindowText(sztext);
				frameCount = 0;
			}
			else frameCount++;

			cv::cvtColor(thisPtr->m_cvMat, grayFrame, CV_BGR2GRAY);
			//::FrameToContrast(grayFrame);
			::FrameToSobel(grayFrame);
			::FrameErosionDilation(grayFrame);
			cv::cvtColor(grayFrame, grayFrame, CV_GRAY2RGB);

			// 輸出至 Window
			cv::imshow(wname, thisPtr->m_cvMat);
			cv::waitKey(1);
			//::TransferToWindow(thisPtr->GetSafeHwnd(), wd, ht, &bmi, thisPtr->m_cvMat.data);
			::TransferToWindow(thisPtr->GetSafeHwnd(), wd, ht, &bmi, grayFrame.data);
		}

		//cv::imshow(caption, frame);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	cv::destroyAllWindows();
	thisPtr->m_keepDetection = FALSE;
	if (stream != nullptr) {
		stream->release();
		SAFE_DELETE(stream);
	}
}
