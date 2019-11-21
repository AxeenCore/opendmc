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
	cv::VideoCapture* stream = nullptr;
	cv::Mat frame;
	BITMAPINFO bmi;
	BOOL reset = FALSE;
	int wd = 0, ht = 0, err = 0;

	while (thisPtr->m_keepDetection) {
		// 連接攝影機，進行串流
		if (stream == nullptr) {
			stream = ::OpenCapture(url);

			if (stream == nullptr) {
				if (err > 5) {
					thisPtr->SetWindowText(TEXT("開啟串流失敗"));
					break;
				}
				err++;
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
				continue;
			}

			// 指定影像色彩格式為 PixelFormat24bppRGB
			stream->set(CV_CAP_PROP_FORMAT, CV_8UC3);
			wd = static_cast<int>(stream->get(CV_CAP_PROP_FRAME_WIDTH));
			ht = static_cast<int>(stream->get(CV_CAP_PROP_FRAME_HEIGHT));
			reset = TRUE;
		}
		err = 0;

		for (;;) {
			if (err > 5) {
				thisPtr->SetWindowText(TEXT("讀取影像失敗"));
				stream->release();
				SAFE_DELETE(stream);
				break;
			}

			if (!stream->read(frame)) {
				err++;
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
				continue;
			}
			err = 0;
			break;
		}

		if (stream) {
			if (reset) {
				thisPtr->SetClientSize(wd, ht);
				thisPtr->CenterWindow();

				::memset(&bmi, 0, sizeof(BITMAPINFO));
				bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
				bmi.bmiHeader.biWidth = wd;
				bmi.bmiHeader.biHeight = -ht;	// 標準 BMP 起始座標為左下，若正像圖於 Windows DIB 顯示時將會被反向, 所以要正向顯示必須為高必須為負值。
				bmi.bmiHeader.biPlanes = 1;		// must be 1
				bmi.bmiHeader.biBitCount = 24;
				bmi.bmiHeader.biXPelsPerMeter = 0;
				bmi.bmiHeader.biYPelsPerMeter = 0;

				bmi.bmiHeader.biCompression = BI_RGB;
				bmi.bmiHeader.biClrUsed = 0;
				bmi.bmiHeader.biClrImportant = 0;
				bmi.bmiHeader.biSizeImage = 0;

				reset = FALSE;
			}

			// 輸出至 Window
			::TransferToWindow(thisPtr->GetSafeHwnd(), wd, ht, &bmi, frame.data);
		}

		//cv::imshow(caption, frame);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	thisPtr->m_keepDetection = FALSE;
	if (stream != nullptr) {
		stream->release();
		SAFE_DELETE(stream);
	}
}
