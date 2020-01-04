/**************************************************************************//**
 * @file	mainform.cc
 * @brief	CarMainform 類別成員函數定義
 * @date	2019-12-05
 * @date	2019-12-05
 * @author	Swang
 *****************************************************************************/
#include "mainform.hh"

/**
 *	@brief	CarMainform 建構式
 *	@return	此函數沒有返回值
 */
CarMainform::CarMainform()
	: DmWnds()
	, m_cDetection(nullptr) {
}

/**
 *	@brief	CarMainform 解構式
 *	@return	此函數沒有返回值
 */
CarMainform::~CarMainform() { this->DestroyMine(); }

/**
 *	@brief	建立視窗 (框架)
 *	@return	<b>型別: BOOL</b> \n 若視窗建立成功，返回值為非零值。 \n 若視窗建立失敗，返回值為零。
 *	@note	依據使用者自訂義內容，建立一個視窗。
 */
BOOL CarMainform::CreateMine()
{
	// 取得控制項 class 名稱
	const TCHAR* szClassPtr = this->GetControlsClassName();

	if (szClassPtr == NULL) {
		// 填寫 WNDSFRAME 結構內容
		WNDSFRAME ws;
		::memset((void*)&ws, 0, sizeof(WNDSFRAME));
		ws.hInstance	= ::GetWinapp().GetInstanceHandle();
		ws.hWndParent	= nullptr;
		ws.pszClass		= TEXT(CAR_PLATE_CLASSEX);
		ws.pszCaption	= TEXT(CAR_PLATE_CAPTION);
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
		ws.hMenuOrItemID = 0;
		ws.vParam		= nullptr;

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
 *	@brief	[虛擬重載] 釋放使用者定義資源
 *	@return	此函數沒有返回值
 */
void CarMainform::SafeUserRelease()
{

}

/**
 *	@brief	[重載] WM_CLOSE 訊息處理
 *	@param[in] wParam	參數 1
 *	@param[in] lParam	參數 2
 *	@return	此函數沒有返回值
 */
void CarMainform::WmClose(WPARAM wParam, LPARAM lParam)
{
#if 0
	/* 銷毀車牌偵測 thread */
	this->DestroyPlateDetection();
#endif

	if (m_cDetection != nullptr) {
		m_cDetection->DetectionEnd();
	}

	/* 拋給父類別處理 */
	DmWnds::WmClose(wParam, lParam);
}

/**
 *	@brief	[重載] WM_USERCREATE 訊息處理，類別建立並完成建立視窗時的訊息。
 *	@param[in] wParam	視窗建立者等入資訊
 *	@param[in] lParam	視窗建立者導入資訊
 *	@return	此函數沒有返回值
 */
void CarMainform::WmUserCreate(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	m_cDetection = new (std::nothrow) CarPlateDetection();
	if (m_cDetection == nullptr) {
		this->MessageBox(TEXT("error: Create Detection object failed."), TEXT(CAR_PLATE_CAPTION), MB_OK | MB_ICONWARNING);															// show error message
		this->PostMessage(WM_CLOSE, 0, 0);
		return;
	}

	if (!m_cDetection->DetectionRun(*this)) {
		this->MessageBox(TEXT("error: Start Detection failed."), TEXT(CAR_PLATE_CAPTION), MB_OK | MB_ICONWARNING);															// show error message
		this->PostMessage(WM_CLOSE, 0, 0);
		return;
	}
}

/**
 *	@brief	[重載] 視窗訊息處理 Callback
 *	@param[in] uMessage	訊息代碼
 *	@param[in] wParam	參數 1
 *	@param[in] lParam	參數 2
 *	@return	<b>型別: LRESULT</b> \n 返回值為，視窗訊息處理結果。
 */
LRESULT CarMainform::WndProc(UINT uMessage, WPARAM wParam, LPARAM lParam)
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
