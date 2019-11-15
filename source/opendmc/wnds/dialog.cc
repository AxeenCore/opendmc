/**************************************************************************//**
 * @file	dialog.cc
 * @brief	DmDialog 對話框類別成員函數定義
 * @date	2000-10-10
 * @date	2018-08-10
 * @author	Swang
 *****************************************************************************/
#include "opendmc/wnds/dialog.hh"

/**
 *	@brief DmDialog 建構式
 *	@return 此函數沒有返回值
 */
DmDialog::DmDialog() : DmWnds(EmCtrls::Dialog) { }

/**
 *	@brief DmDialog 解構式
 *	@return 此函數沒有返回值
 *	@remark 解構時，進行釋放(銷毀)控制項
 */
DmDialog::~DmDialog()
{
	if (this->IsWindow()) {
		this->RequestToDestroy(0);
	}
}

/**
 *	@brief 建立 Modal 對話框
 *	@param[in] szTemplatePtr	指向對話框模板名稱緩衝區位址
 *	@param[in] hWndParent		父視窗操作代碼 (擁有者視窗操作代碼)
 *	@return <b>型別: INT_PTR</b>
 *		\n 如果運作成功，則返回值為結束對話框 EndDialog() 函數的參數值 nResult。
 *		\n 如果因 hWndParent 參數無效而失敗，則返回值為零。(在這種情況下，該函數返回零，以便與以前版本的Windows兼容。)
 *		\n 如果因任何其他原因而失敗，則返回值為 (-1)。呼叫 GetLastError 取得錯誤訊息。
 */
INT_PTR DmDialog::DoModal(const TCHAR* szTemplatePtr, HWND hWndParent)
{
	INT_PTR nResult = -1;	// 預設返回錯誤
	auto hInst = ::GetWinapp().GetInstanceHandle();
	auto fnProc = reinterpret_cast<DLGPROC>(this->GetSafeCallback());
	WNDSPARAM smParam;

	for (;;) {
		if (this->IsWindow()) {
			::SetLastError(ERROR_ALREADY_EXISTS);
			break;
		}

		if (hInst == NULL) {
			::SetLastError(ERROR_INVALID_HANDLE);
			break;
		}

		if (szTemplatePtr == NULL) {
			::SetLastError(ERROR_INVALID_DATA);
			break;
		}

		::memset(&smParam, 0, sizeof(smParam));
		smParam.lParam = reinterpret_cast<LPARAM>(this);

		if (fnProc != NULL) {
			// DialogBoxParam 若運作失敗將傳回 0 or -1
			m_bModal = TRUE;
			nResult = ::DialogBoxParam(hInst, szTemplatePtr, hWndParent, fnProc, reinterpret_cast<LPARAM>(&smParam));
		}
		break;
	}
	return nResult;
}

/**
 *	@brief 建立 Modal 對話框
 *	@param[in] nIDDItem		資源檔對話框識別碼
 *	@param[in] hWndParent	父視窗操作代碼
 *	@return <b>型別: INT_PTR</b>
 *		\n 如果運作成功，則返回值為結束對話框 EndDialog() 函數的參數值 nResult。
 *		\n 如果因 hWndParent 參數無效而失敗，則返回值為零。(在這種情況下，該函數返回零，以便與以前版本的Windows兼容。)
 *		\n 如果因任何其他原因而失敗，則返回值為 (-1)。呼叫 GetLastError 取得錯誤訊息。
 *	@remark <b>對話框識別碼額外說明</b> \n 如果參數指定資源識別碼，則其高字組 (HIWORD) 必須為零，並且其低字組 (LOWORD) 必須為識別碼。
 */
INT_PTR DmDialog::DoModal(int nIDDItem, HWND hWndParent)
{
	INT_PTR nResult = (-1);

	for (;;) {
		// 控制項識別碼是否在合法範圍
		if (nIDDItem < MIN_WINC_IDITEM || nIDDItem > MAX_WINC_IDITEM) {
			::SetLastError(ERROR_INVALID_ACCESS);
			break;
		}

		// 使用 MAKEINTRESOURCE 巨集轉換控制項資源識別碼，建立 DialogBoxParam、CreateDialogParam 函數所需資源識別碼模板字串
		auto szPtr = reinterpret_cast<TCHAR*>(MAKEINTRESOURCE(nIDDItem));
		nResult = this->DoModal(szPtr, hWndParent);
		break;
	}
	return nResult;
}

/**
 *	@brief 建立 Modeless 對話框
 *	@param[in] szTemplatePtr	指向對話框模板名稱緩衝區位址
 *	@param[in] hWndParent		父視窗操作代碼 (擁有者視窗操作代碼)
 *	@return <b>型別: BOOL</b> \n 若建立對話框成功則返回值為非零值。 \n 若建立對話框失敗則返回值為零。
 */
BOOL DmDialog::DoModeless(const TCHAR* szTemplatePtr, HWND hWndParent)
{
	BOOL bResult = FALSE;	// 預設返回錯誤
	auto hInst = ::GetWinapp().GetInstanceHandle();
	auto fnProc = reinterpret_cast<DLGPROC>(this->GetSafeCallback());
	WNDSPARAM smParam;

	for (;;) {
		if (this->IsWindow()) {
			::SetLastError(ERROR_ALREADY_EXISTS);
			break;
		}

		if (hInst == NULL) {
			::SetLastError(ERROR_INVALID_HANDLE);
			break;
		}

		if (szTemplatePtr == NULL) {
			::SetLastError(ERROR_INVALID_DATA);
			break;
		}

		// set user parameter
		::memset(&smParam, 0, sizeof(smParam));
		smParam.lParam = reinterpret_cast<LPARAM>(this);

		if (fnProc != NULL) {
			m_bModal = FALSE;
			bResult = ::CreateDialogParam(hInst, szTemplatePtr, hWndParent, fnProc, reinterpret_cast<LPARAM>(&smParam)) != NULL;
		}
		break;
	}
	return bResult;
}

/**
 *	@brief 建立 Modeless 對話框
 *	@param[in] nIDDItem		對話框識別碼 (identifier)
 *	@param[in] hWndParent	父視窗操作代碼(Handle)
 *	@return <b>型別: BOOL</b> \n 若建立對話框成功則返回值為非零值。 \n 若建立對話框失敗則返回值為零。
 */
BOOL DmDialog::DoModeless(int nIDDItem, HWND hWndParent)
{
	BOOL bResult = FALSE;

	for (;;) {
		// 控制項識別碼是否在合法範圍
		if (nIDDItem < MIN_WINC_IDITEM || nIDDItem > MAX_WINC_IDITEM) {
			::SetLastError(ERROR_INVALID_ACCESS);
			break;
		}

		// 使用 MAKEINTRESOURCE 巨集轉換控制項資源識別碼，建立 DialogBoxParam、CreateDialogParam 函數所需資源識別碼模板字串
		auto szPtr = reinterpret_cast<TCHAR*>(MAKEINTRESOURCE(nIDDItem));
		bResult = this->DoModeless(szPtr, hWndParent);
		break;
	}
	return bResult;
}

/**
 *	@brief 取得自定義對話框訊息處理 Callback 函數位址
 *	@return <b>型別: LONG_PTR</b> \n 對話框訊息處理 Callback 函數位址
 */
LONG_PTR DmDialog::GetSafeCallback() const { return reinterpret_cast<LONG_PTR>(DmDialog::SafeDlgProc); }





///////////////////////////////////////////////////////////////////////////////
//
// Message dispose
// 
///////////////////////////////////////////////////////////////////////////////

/**
 *	@brief 預設對話框訊息處理函數
 *	@param[in] uMessage	對話框訊息
 *	@param[in] wParam	對話框訊息參數
 *	@param[in] lParam	對話框訊息參數
 *	@return <b>型別: INT_PTR</b>
 *		\n 通常對話框在處理消息時應返回 TRUE，如果不處理則返回 FALSE。
 *		\n 如果對話框返回 FALSE, 則對話框管理器將執行默認對話框操作以響應該消息。
 */
INT_PTR DmDialog::DefaultDlgProc(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	INT_PTR lResult = TRUE;

	switch (uMessage)
	{
	case WM_DESTROY:
		this->WmDestroy(wParam, lParam);
		break;
	case WM_CLOSE:
		this->WmClose(wParam, lParam);
		break;
	case WM_NCDESTROY:
		this->WmNcDestroy(wParam, lParam);
		break;
	case WM_INITDIALOG:
		lResult = static_cast<INT_PTR>(this->WmInitDialog(wParam, lParam));
		break;
	case WM_COMMAND:
		this->WmCommand(wParam, lParam);
		break;
	case UWM_WINSCREATE:
		this->WmWinsCreate(wParam, lParam);
		break;
	default:
		lResult = 0;
	}

	return static_cast<INT_PTR>(lResult);
}

/**
 *	@brief <b>[虛擬函數]</b> WM_INTIDIALOG 訊息處理，初始化對話框 (對話框建立後第一個訊息)
 *	@param[in] wParam 對話框的操作代碼，用於接收鍵盤輸入的焦點。僅當對話框過程返回 TRUE 時，系統才會分配鍵盤輸入的焦點。
 *	@param[in] lParam 附加的初始化數據
 *	@return <b>型別: BOOL</b> \n 訊息處理狀態, 訊息處理後必定返回 TRUE
 */
BOOL DmDialog::WmInitDialog(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	this->PostWinsCreateMessage(wParam, lParam);
	return TRUE;
}

/**
 *	@brief <b>[虛擬函數]</b> WM_COMMAND 訊息處理，當用戶點選 Menu 或其他控制項時會向父視窗發出訊息。
 *	@param[in] wParam see remark
 *	@param[in] lParam see remark
 *	@return 此函數沒有返回值
 *	@remark <b>參數說明</b>
 *		<pre>
 *		Message Source	|	wParam (high word)	|	wParam (low word)				|	lParam
 *						|						|									|
 *		Menu			|	0					|	Menu identifier (IDM_*)			|	0
 *						|						|									|
 *		Accelerator		|	1					|	Accelerator identifier (IDM_*)	|	0
 *						|						|									|
 *		Control			|	Control-defined		|	Control identifier				|	Handle to the control window
 *						|	notification code	|									|
 *		</pre>
 */
void DmDialog::WmCommand(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	auto uIDCItem = static_cast<UINT>(LOWORD(wParam));

	switch (uIDCItem)
	{
	case IDOK:
		this->RequestToDestroy(0);
		break;
	case IDCANCEL:
		this->RequestToDestroy(0);
		break;
	}
}

/**
 *	@brief <b>[重載]</b> 使用者訊息 UWM_WINCCREATE，建立視窗內容
 *	@param[in] wParam 未使用
 *	@param[in] lParam 未使用
 *	@return 此函數沒有返回值
 */
void DmDialog::WmWinsCreate(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
}

/**
 *	@brief <b>[虛擬函數]</b> 對話框訊息處理函數
 *	@param[in] uMessage	對話框訊息
 *	@param[in] wParam	對話框訊息參數
 *	@param[in] lParam	對話框訊息參數
 *	@return <b>型別: INT_PTR</b>
 *		\n 通常對話框在處理消息時應返回 TRUE，如果不處理則返回 FALSE。
 *		\n 如果對話框返回 FALSE, 則對話框管理器將執行默認對話框操作以響應該消息。
 */
INT_PTR DmDialog::DlgProc(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	return this->DefaultDlgProc(uMessage, wParam, lParam);
}

/**
 *	@brief <b>[重載]</b> 綁定對話框至物件
 *	@param[in] hWnd 對話框操作代碼
 *	@return	<b>型別: BOOL</b> \n 若綁定成功返回值為非零值，若綁定失敗返回值為零。
 */
BOOL DmDialog::BindWindow(HWND hWnd)
{
	for (;;) {
		// 此物件已存在視窗握控制項?
		if (this->IsWindow()) {
			// Error handling
			break;
		}
		// 來源操作碼為視窗或控制項?
		if (!::IsWindow(hWnd)) {
			// Error handling
			break;
		}

		this->SetSafeHwnd(hWnd);
		auto dmPrev = ::GetWindowLongPtr(hWnd, DWLP_DLGPROC);
		auto dmProc = this->GetSafeCallback();

		::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
		if (dmPrev != dmProc) {
			// 指向新的訊息處理 Callback 位址
			dmPrev = ::SetWindowLongPtr(hWnd, DWLP_DLGPROC, dmProc);
			this->SetSafePrevCallback(dmPrev);
		}
		break;
	}
	return this->GetSafeHwnd() != NULL;
}

/**
 *	@brief <b>[重載]</b> 解除綁定
 *	@return 此函數沒有返回值
 */
void DmDialog::LooseWindow()
{
	// 恢復原來的的訊息處理 Callback 函數呼叫位址
	auto fnCallback = this->GetSafePrevCallback();
	if (fnCallback != 0) {
		::SetWindowLongPtr(*this, GWLP_USERDATA, 0);
		::SetWindowLongPtr(*this, DWLP_DLGPROC, fnCallback);
		this->SetSafePrevCallback(0);
	}
}

/**
 *	@brief 對話框訊息處理函數
 *	@param[in] hWndDlg	對話框操作代碼
 *	@param[in] uMessage	對話框訊息
 *	@param[in] wParam	訊息參數, 依據訊息不同而有所不同
 *	@param[in] lParam	訊息參數, 依據訊息不同而有所不同
 *	@return <b>型別: INT_PTR</b>
 *		\n 通常對話框在處理消息時應返回 TRUE，如果不處理則返回 FALSE。
 *		\n 如果對話框返回 FALSE, 則對話框管理器將執行默認對話框操作以響應該消息。
 *	@remark <b>返回值說明</b>
 *		\n 如果對話框處理過程，需要返回特定值，則對話框處理過程應在返回 TRUE 之前立即呼叫 SetWindowLong（hwndDlg，DWL_MSGRESULT，lResult）來設置所需的返回值。
 */
INT_PTR DmDialog::SafeDlgProc(HWND hWndDlg, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	auto dmCtrl = reinterpret_cast<DmDialog*>(::GetWindowLongPtr(hWndDlg, GWLP_USERDATA));

	if (dmCtrl == NULL) {
		// WM_INITDIALOG 訊息觸發
		if (uMessage == WM_INITDIALOG) {
			WNDSPARAM* smPtr = reinterpret_cast<WNDSPARAM*>(lParam);
			if (smPtr != NULL) {
				dmCtrl = reinterpret_cast<DmDialog*>(smPtr->lParam);
				if (dmCtrl != NULL) {
					// 綁定此對話框
					if (!dmCtrl->BindWindow(hWndDlg)) {
						return 0L;
					}
					return dmCtrl->DlgProc(uMessage, wParam, lParam);
				}
			}
		}
		return 0L;
	}
	return dmCtrl->DlgProc(uMessage, wParam, lParam);
}
