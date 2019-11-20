/**************************************************************************//**
 * @file	lpr_frame.cc
 * @brief	CxLprFrame 類別成員函數定義
 * @date	2019-11-20
 * @date	2019-11-20
 * @author	Swang
 *****************************************************************************/
#include "lpr_frame.hh"

 /**
  *	@brief	CxLprFrame 建構式
  */
CxLprFrame::CxLprFrame() : DmWnds() { }

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
		ws.hInstance = ::GetWinapp().GetInstanceHandle();
		ws.hWndParent = NULL;
		ws.pszClass = TEXT(HSLPR_CLASSNAME);
		ws.pszCaption = TEXT(HSLPR_TITLENAME);
		ws.nPosx = 0;
		ws.nPosy = 0;
		ws.nWidth = WNDS_DEFAULT_WIDTH;
		ws.nHeight = WNDS_DEFAULT_HEIGHT;
		ws.uClassStyle = 0;
		ws.hBackground = (HBRUSH)(COLOR_BTNFACE + 1);
		ws.hIcon = NULL;
		ws.hIconSm = NULL;
		ws.hCursor = NULL;
		ws.dwStyle = WNDS_DEFAULT_STYLE;
		ws.dwExStyle = WNDS_DEFAULT_EXSTYLE;
		ws.hMenuOrItemID = 0;
		ws.vParam = NULL;

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
