/**************************************************************************//**
 * @file	button.cc
 * @brief	DmButton 按鈕類別成員函數定義
 * @date	2000-10-10
 * @date	2018-09-01
 * @author	Swang
 *****************************************************************************/
#include "opendmc/wnds/button.hh"

/**
 *	@brief DmButton 建構式
 */
DmButton::DmButton() : DmWnds(EmCtrls::Button) { }

/**
 *	@brief DmButton 解構式
 *	@remark 解構時，進行釋放(銷毀)控制項
 */
DmButton::~DmButton() { this->SafeWndsDestroy(); }

/**
 *	@brief [重載] 建立一個按鈕 (Push button)
 *	@param[in] szCaptionPtr	指向控制項名稱字串緩衝區位址，用來設定控制項的視窗名稱。
 *	@param[in] x			起始位置 (左上角) X
 *	@param[in] y			起始最至 (左上角) Y
 *	@param[in] wd			寬度
 *	@param[in] ht			高度
 *	@param[in] hWndParent	父視窗操作代碼
 *	@param[in] nIDCItem		項目識別碼
 *	@param[in] hInstance	視窗應用程式實例操作代碼
 *	@return <b>型別: BOOL</b> \n 若函數操作成功返回值為非零值，值為視窗或控制項操作代碼。 \n 若函數操作失敗返回值為零。
 */
BOOL DmButton::Create(const TCHAR* szCaptionPtr, int x, int y, int wd, int ht, HWND hWndParent, int nIDCItem)
{
	const DWORD dwStyle = BS_PUSHBUTTON;
	const DWORD dwExStyle = 0;
	WNDSCTRLS smCtrl;

	::memset(&smCtrl, 0, sizeof(WNDSCTRLS));
	smCtrl.hInstance	= ::GetWinapp().GetInstanceHandle();
	smCtrl.hParent		= hWndParent;
	smCtrl.pszClass		= nullptr;
	smCtrl.pszCaption	= szCaptionPtr;
	smCtrl.dwStyle		= dwStyle;
	smCtrl.dwExStyle	= dwExStyle;
	smCtrl.nPosx		= x;
	smCtrl.nPosy		= y;
	smCtrl.nWidth		= wd;
	smCtrl.nHeight		= ht;
	smCtrl.nIDCItem		= nIDCItem;
	return this->CreateControls(&smCtrl);
}

/**
 *	@brief 建立一個按鈕樣本 (由繼承者發展)
 *	@return	<b>型別: BOOL</b> 若視窗建立成功返回值為非零值，若視窗建立失敗責返回值為零。
 */
BOOL DmButton::CreateSample() { return FALSE; }

/**
 *	@brief 取得按鈕理想的大小，如果存在圖像列表, 則獲取最適合的文本和圖像按鈕的尺寸.
 *	@param[in,out] sizePtr SIZE 結構資料位址。
 *		- 如果 SIZE.cx 與 SIZE.cy 皆為零，自動取得理想值
 *		- 如果 SIZE.cx 不為零，則依據 cx 取得理想的 cy 值
 *	@return	<b>型別: BOOL</b> \n 若成功則返回值為非零值。 \n 若失敗返回值為零。
 */
BOOL DmButton::GetIdealSize(SIZE* sizePtr) const
{
	assert(this->IsWindow());
	// BCM_GETIDEALSIZE
	// wParam = 不使用，必須為零
	// lParam = 指向 SIZE 結構資料位址
	return ::SendMessage(*this, BCM_GETIDEALSIZE, 0, reinterpret_cast<LPARAM>(sizePtr)) != 0;
}

/**
 *	@brief 取得按鈕的圖像列表
 *	@param[in,out] imePtr BUTTON_IMAGELIST 結構資料位址
 *	@return <b>型別: BOOL</b> \n 若成功則返回非零值, 若失敗返回零.
 */
BOOL DmButton::GetImageList(BUTTON_IMAGELIST * imePtr) const
{
	assert(this->IsWindow());
	// BCM_GETIDEALSIZE
	// wParam = 不使用，必須為零
	// lParam = 指向 BUTTON_IMAGELIST 結構資料位址
	return ::SendMessage(*this, BCM_GETIMAGELIST, 0, reinterpret_cast<LPARAM>(imePtr)) != 0;
}

/**
 *	@brief 取得連接控件註解說明文本
 *	@param[out]		szNotePtr	指向註釋說明文字字串緩衝區位址
 *	@param[in,out]	ccNotePtr	指向描述文字字串緩衝區大小的變數位址
 *	@return <b>型別: BOOL</b>
 *		\n 若成功，返回值為非零值，szNotePtr 緩衝區的內容為當前命令與連接的控件說明文本。
 *		\n 若失敗，返回值為零，ccNotePtr 內容為實際需要緩衝區的大小。
 */
BOOL DmButton::GetNote(LPTSTR szNotePtr, DWORD* ccNotePtr) const
{
	assert(this->IsWindow());
	// BCM_GETNOTE
	// wParam = [in] 描述緩衝區大小 [out] 實際緩衝區大小。(指向描述文字字串緩衝區大小的變數位址)
	// lParam = 指向接收註釋的字串緩衝區位址，緩衝區必須足夠大且含可容納 NULL 結尾符號
	return ::SendMessage(*this, BCM_GETNOTE, reinterpret_cast<WPARAM>(ccNotePtr), reinterpret_cast<LPARAM>(szNotePtr)) != 0;
}

/**
 *	@brief 取得控件註解說明文本的大小
 *	@return <b>型別: DWORD</b> \n 文字字串的長度(in TCHAR)，不包含 NULL 結尾符號。
 */
DWORD DmButton::GetNoteLength() const
{
	assert(this->IsWindow());
	// BCM_GETNOTELENGTH
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<DWORD>(::SendMessage(*this, BCM_GETNOTELENGTH, 0, 0));
}

/**
 *	@brief 取得拆分按鈕的資料
 *	@param[in,out] splitPtr 指向 BUTTON_SPLITINFO 結構資料位址
 *	@return <b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 *	@remark 此訊息需與 BS_SPLITBUTTON 和 BS_DEFSPLITBUTTON 按鈕樣式一起使用。
 *	@see https://docs.microsoft.com/en-us/windows/desktop/api/Commctrl/ns-commctrl-tagbutton_splitinfo
 */
BOOL DmButton::GetSplitInfo(BUTTON_SPLITINFO* splitPtr) const
{
	assert(this->IsWindow());
	// BCM_GETSPLITINFO
	// wParam = 不使用，必須為零
	// lParam = 指向 BUTTON_SPLITINFO 結構資料位址
	return ::SendMessage(*this, BCM_GETSPLITINFO, 0, reinterpret_cast<LPARAM>(splitPtr)) != 0;
}

/**
 *	@brief 取得按鈕繪製文本的邊距
 *	@param[out] rcPtr 指向 RECT 結構資料位址
 *	@return <b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmButton::GetTextMargin(RECT* rcPtr) const
{
	assert(this->IsWindow());
	// BCM_GETTEXTMARGIN
	// wParam = 不使用，必須為零
	// lParam = 指向 RECT 結構資料位址
	return ::SendMessage(*this, BCM_GETTEXTMARGIN, 0, reinterpret_cast<LPARAM>(rcPtr)) != 0;
}

/**
 *	@brief 設定按鈕為 TBSTYLE_DROPDOWN 樣式按鈕的下拉狀態。
 *	@param[in] bState 設定狀態值，TRUE = 設定為 BST_DROPDOWNPUSHED 狀態 / FALSE = 取消狀態
 *	@return <b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmButton::SetDropDownState(BOOL bState) const
{
	assert(this->IsWindow());
	// BCM_SETDROPDOWNSTATE
	// wParam = 設定狀態值
	// lParam = 不使用，必須為零
	return ::SendMessage(*this, BCM_SETDROPDOWNSTATE, static_cast<WPARAM>(bState), 0) != 0;
}

/**
 *	@brief 將圖像列表分配給按鈕控件
 *	@param[in] btnImageListPtr	指向 PBUTTON_IMAGELIST 結構位址
 *	@return <b>型別: BOOL</b> \n 若成功返回值為非零值，若失敗則返回值為零。
 */
BOOL DmButton::SetImageList(const PBUTTON_IMAGELIST* btnImageListPtr) const
{
	assert(this->IsWindow());
	// BCM_SETIMAGELIST
	// wParam = 不使用，必須為零
	// lParam = 指向 PBUTTON_IMAGELIST 結構位址
	return static_cast<BOOL>(::SendMessage(*this, BCM_GETNOTELENGTH, 0, reinterpret_cast<LPARAM>(btnImageListPtr)));
}

/**
 *	@brief 設定按鈕註釋的文字內容
 *	@param[in]	szNotePtr	指向註釋文字字串緩衝區的位址 (以 NULL 作為結尾的字串)
 *	@return <b>型別: BOOL</b> \n 若成功返回值為非零值，若失敗則返回值為零。
 *	@remark BCM_SETNOTE 訊息只能發給具備有 BS_COMMANDLINK 或 BS_DEFCOMMANDLINK 風格的按鈕
 */
BOOL DmButton::SetNote(const TCHAR* szNotePtr) const
{
	assert(this->IsWindow());
	// BCM_SETNOTE
	// wParam = 不使用，必須為零
	// lParam = 指向註釋文字字串緩衝區位址
	return static_cast<BOOL>(::SendMessage(*this, BCM_SETNOTE, 0, reinterpret_cast<LPARAM>(szNotePtr)));
}

/**
 *	@brief 將按鈕控件提高安全等級的盾牌的 elevation required 樣式
 *	@param[in] bRequired 設定或取消 elevation required 樣式
 *	@return <b>型別: BOOL</b> \n 若成功返回值為非零值，若失敗則返回值為零。
 */
BOOL DmButton::SetElevationRequired(BOOL bRequired) const
{
	assert(this->IsWindow());
	// BCM_SETSHIELD
	// wParam = 不使用，必須為零
	// lParam = 設定或取消 elevation required 樣式
	return ::SendMessage(*this, BCM_SETSHIELD, 0, static_cast<LPARAM>(bRequired)) == 1;
}

/**
 *	@brief 設定拆分按鈕資料
 *	@param[in] splitPtr 指向 BUTTON_SPLITINFO 結構資料位址
 *	@return <b>型別: BOOL</b> \n 若成功返回值為非零值，若失敗則返回值為零。
 *	@remark 此訊息需與 BS_SPLITBUTTON 和 BS_DEFSPLITBUTTON 按鈕樣式一起使用。
 *	@see	https://docs.microsoft.com/en-us/windows/desktop/api/Commctrl/ns-commctrl-tagbutton_splitinfo
 */
BOOL DmButton::SetSplitInfo(BUTTON_SPLITINFO* splitPtr) const
{
	assert(this->IsWindow());
	// BCM_SETSPLITINFO
	// wParam = 不使用，必須為零
	// lParam = 指向 BUTTON_SPLITINFO 結構資料位址
	return ::SendMessage(*this, BCM_SETSPLITINFO, 0, reinterpret_cast<LPARAM>(splitPtr)) != 0;
}

/**
 *	@brief 設定按鈕控繪製文本的邊距
 *	@param[out] rcPtr 指向 RECT 結構資料位址
 *	@return <b>型別: BOOL</b> \n 若成功返回值為非零值，若失敗則返回值為零。
 */
BOOL DmButton::SetTextMargin(RECT * rcPtr) const
{
	assert(this->IsWindow());
	// BCM_SETTEXTMARGIN
	// wParam = 不使用，必須為零
	// lParam = 指向 RECT 結構資料位址
	return static_cast<BOOL>(::SendMessage(*this, BCM_SETTEXTMARGIN, 0, reinterpret_cast<LPARAM>(rcPtr)));
}

/**
 *	@brief 發送按鈕 Click 訊息
 *	@remark	額外說明
 *		- 模擬用戶單擊按鈕。
 *		- 此消息使按鈕接收 WM_LBUTTONDOWN 和 WM_LBUTTONUP 消息。
 *		- 按鈕的父窗口接收 BN_CLICKED 通知代碼。
 */
void DmButton::Click() const
{
	assert(this->IsWindow());
	// BM_CLICK
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	::SendMessage(*this, BM_CLICK, 0, 0);
}

/**
 *	@brief 獲取單選按鈕或複選框(CheckBox)的選取狀態
 *	@return <b>型別: LRESULT</b>
 *		- BST_CHECKED		\n 按鈕被選中
 *		- BST_INDETERMINATE	\n 按鈕呈灰色，表示不確定狀態（僅在按鈕具有BS_3STATE或BS_AUTO3STATE樣式時適用）
 *		- BST_UNCHECKED		\n 按鈕選中狀態被清除
 */
LRESULT DmButton::GetCheck() const
{
	assert(this->IsWindow());
	// BM_GETCHECK
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return ::SendMessage(*this, BM_GETCHECK, 0, 0);
}

/**
 *	@brief 取得按鈕的圖像 Handle
 *	@param[in] nType 指定要取得的圖像類型，如下:
 *		- IMAGE_BITMAP		\n 取得 Bitmap 操作代碼
 *		- IMAGE_ICON		\n 取得 Icon 操作代碼
 *		- IMAGE_CURSOR		\n 取得 Cursor 操作代碼
 *		- IMAGE_ENHMETAFILE	\n 取得 圖像元素？
 *	@return	<b>型別: LRESULT</b> \n 若成功返回值為非零值，值為圖像 Handle。若失敗則返回值為零。
 */
LRESULT DmButton::GetImage(int nType) const
{
	assert(this->IsWindow());
	// BM_GETIMAGE
	// wParam = 圖像種類
	// lParam = 不使用，必須為零
	return ::SendMessage(*this, BM_GETIMAGE, static_cast<WPARAM>(nType), 0);
}

/**
 *	@brief 取得按鈕的圖像操作代碼
 *	@return <b>型別: HBITMAP</b> \n 若成功返回值為非零值，值為圖像操作代碼(Handle)。若失敗則返回值零。
 *	@see DmButton::GetImage
 */
HBITMAP DmButton::GetButtonBitmap() const
{
	assert(this->IsWindow());
	// BM_GETIMAGE
	// wParam = IMAGE_BITMAP
	// lParam = 不使用，必須為零
	return reinterpret_cast<HBITMAP>(::SendMessage(*this, BM_GETIMAGE, static_cast<WPARAM>(IMAGE_BITMAP), 0));
}

/**
 *	@brief 取得按鈕光標操作代碼
 *	@return <b>型別: HBITMAP</b> \n 若成功返回值為非零值，值為圖像操作代碼。若失敗則返回值零。
 *	@see DmButton::GetImage
 */
HCURSOR DmButton::GetButtonCursor() const
{
	assert(this->IsWindow());
	// BM_GETIMAGE
	// wParam = IMAGE_BITMAP
	// lParam = 不使用，必須為零
	return reinterpret_cast<HCURSOR>(::SendMessage(*this, BM_GETIMAGE, static_cast<WPARAM>(IMAGE_CURSOR), 0));
}

/**
 *	@brief 取得按鈕的圖像操作代碼(Handle)
 *	@return <b>型別: HICON</b> \n 若成功返回值為非零值，值為圖示操作代碼。若失敗則返回值為零。
 */
HICON DmButton::GetButtonIcon() const
{
	assert(this->IsWindow());
	// BM_GETIMAGE
	// wParam = IMAGE_BITMAP
	// lParam = 不使用，必須為零
	return reinterpret_cast<HICON>(::SendMessage(*this, BM_GETIMAGE, static_cast<WPARAM>(IMAGE_ICON), 0));
}

/**
 *	@brief 取得單選按鈕或複選框狀態
 *	@return <b>型別L LRESULT</b>
 *		- BST_CHECKED			\n	該按鈕被選中
 *		- BST_DROPDOWNPUSHED	\n	Windows Vista。該按鈕處於下拉狀態。僅當按鈕具有 TBSTYLE_DROPDOWN 樣式時才適用
 *		- BST_FOCUS				\n	該按鈕具有鍵盤焦點
 *		- BST_HOT				\n	Hot 狀態，也就是說，滑鼠游標停在它上面
 *		- BST_INDETERMINATE		\n	按鈕的狀態是不確定的，僅當按鈕具有 BS_3STATE 或 BS_AUTO3STATE 樣式時才適用
 *		- BST_PUSHED			\n	按鈕顯示處於按下狀態
 *		- BST_UNCHECKED			\n	為被選中，沒有特殊的狀態，相當於零
 */
LRESULT DmButton::GetState() const
{
	assert(this->IsWindow());
	// BM_GETSTATE
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return ::SendMessage(*this, BM_GETSTATE, 0, 0);
}

/**
 *	@brief 取得按鈕樣式
 *	@return <b>型別: UINT</b> \n 返回值為按鈕的樣式，若有錯誤發生請呼叫 GetLastError 取得錯誤訊息。
 *	@remark <b>樣式為 BS_ 開頭的樣式，為按鈕專屬的樣式</b>
 *		\n BS_3STATE, BS_AUTO3STATE, BS_AUTOCHECKBOX, VS_AUTORADIOBUTTON, BS_CHECKBOX, BS_DEFPUSHBUTTON, BS_GROUPBOX,
 *		\n BS_LEFTEXT, BS_OWNERDRAW, BS_BITMAP, BS_BOTTOM, BS_CENTER, BS_ICON, BS_FLAT, BS_MULTILINE, BS_PUSHLIKE,
 *		\n BS_RIGHT, BS_RIGHTBUTTON, BS_TEXT, BS_TOP, BS_TYHPEMASK, BS_VCENTER.
 */
UINT DmButton::GetButtonStyle() const
{
	// 使用取得視窗樣式取得視窗樣式
	// 過濾出按鈕樣式
	return static_cast<UINT>(this->GetStyle() & 0xFFFF);
}

/**
 *	@brief 設置單選按鈕或複選框的檢查狀態
 *	@param[in] nCheck 設定按鈕 Cechk 狀態值
 *		- BST_CHECKED		\n	將按鈕狀態設置為選中(選取中)
 *		- BST_INDETERMINATE	\n	將按鈕狀態設置為灰色，表示不確定狀態。僅當按鈕具有BS_3STATE或BS_AUTO3STATE樣式時才使用此值。
 *		- BST_UNCHECKED		\n	將按鈕狀態設置為清除(未選取)
 *	@return <b>型別: LRESULT</b> \n 傳回值始終為零
 */
LRESULT DmButton::SetCheck(int nCheck) const
{
	assert(this->IsWindow());
	// BM_SETCHECK
	// wParam = 設定按鈕 Check 狀態值
	// lParam = 不使用，必須為零
	return ::SendMessage(*this, BM_SETCHECK, static_cast<WPARAM>(nCheck), 0);
}

/**
 *	@brief 單選按鈕上設置狀態。
 *	@param[in] bState 設定按鈕 State 狀態值
 *		- TRUE	\n 設定按鈕為觸發狀態(按下、被選中、反白)
 *		- FALSE	\n 清除按鈕所有被設定狀態
 *	@return	此函數沒有返回值
 */
void DmButton::SetRadioClick(BOOL bState) const
{
	assert(this->IsWindow());
	// BM_SETDONTCLICK 傳回值始終為 0
	// wParam = 設定按鈕 Check 狀態值
	// lParam = 不使用，必須為零
	::SendMessage(*this, BM_SETDONTCLICK, static_cast<WPARAM>(bState), 0);
}

/**
 *	@brief 設定按鈕的圖像
 *	@param[in] nType 圖像類型
 *		- IMAGE_BITMAP	取得 Bitmap 操作代碼
 *		- IMAGE_ICON	取得 Icon 操作代碼
 *	@param[in] vPtr 圖像操作碼(handle)
 *	@return <b>型別: LRESULT</b> \n 若先前已有圖像返回值為非零值，值為圖像 HANDLE。若先前無圖像則返回值為零。
 */
LRESULT DmButton::SetImage(int nType, void* vPtr) const
{
	assert(this->IsWindow());
	if (nType != IMAGE_BITMAP && nType != IMAGE_ICON) {
		return 0L;
	}

	// BM_SETIMAGE
	// wParam = 圖像的種類
	// lParam = 圖像的操作代碼 (handle)
	return ::SendMessage(*this, BM_SETIMAGE, static_cast<WPARAM>(nType), reinterpret_cast<LPARAM>(vPtr));
}

/**
 *	@brief 設定按鈕的圖像
 *	@param[in] hBitmap 圖像操作代碼
 *	@return <b>型別: HBITMAP</b> \n 若先前已有圖像返回非零值，值為圖像操作代碼(Handle)。若先前無圖像則返回值為零。
 */
HBITMAP DmButton::SetButtonBitmap(HBITMAP hBitmap) const
{
	assert(this->IsWindow());
	// BM_SETIMAGE
	// wParam = 圖像的種類
	// lParam = 圖像的操作代碼 (handle)
	return reinterpret_cast<HBITMAP>(::SendMessage(*this, BM_SETIMAGE, static_cast<WPARAM>(IMAGE_BITMAP), reinterpret_cast<LPARAM>(hBitmap)));
}

/**
 *	@brief 設定光標
 *	@param[in] hCursor 圖像做代碼
 *	@return <b>型別: HCURSOR</b> \n 若先前已有圖像返回非零值，值為光標操作代碼(Handle)。若先前無光標設定則返回值為零。
 */
HCURSOR DmButton::SetButtonCursor(HCURSOR hCursor) const
{
	assert(this->IsWindow());
	// BM_SETIMAGE
	// wParam = 圖像的種類
	// lParam = 圖像的操作代碼 (handle)
	return reinterpret_cast<HCURSOR>(::SendMessage(*this, BM_SETIMAGE, static_cast<WPARAM>(IMAGE_BITMAP), reinterpret_cast<LPARAM>(hCursor)));
}

/**
 *	@brief 設定按鈕的圖像
 *	@param[in] hIcon 圖示的操作代碼
 *	@return <b>型別: HICON</b> \n 若先前已有圖示返回非零值，值為圖示操作代碼(Handle)。若先前無圖示則返回值為零。
 */
HICON DmButton::SetButtonIcon(HICON hIcon) const
{
	assert(this->IsWindow());
	// BM_SETIMAGE
	// wParam = 圖像的種類
	// lParam = 圖像的操作代碼 (handle)
	return reinterpret_cast<HICON>(::SendMessage(*this, BM_SETIMAGE, static_cast<WPARAM>(IMAGE_ICON), reinterpret_cast<LPARAM>(hIcon)));
}
/**
 *	@brief 設定單選按鈕或複選框狀態
 *	@param[in] bState 設定按鈕 State 狀態值
 *		- TRUE	\n 設定按鈕為觸發狀態(按下、被選中、反白)
 *		- FALSE	\n 清除按鈕所有被設定狀態
 *	@return <b>LRESULT</b> \n 返回值始終為零。
 */
LRESULT DmButton::SetState(BOOL bState) const
{
	assert(this->IsWindow());
	// BM_SETSTATE
	// wParam = 指定按鈕的狀態值
	// lParam = 不使用，必須為零
	return ::SendMessage(*this, BM_SETSTATE, static_cast<WPARAM>(bState), 0);
}

/**
 *	@brief 設定按鈕樣式(風格)
 *	@param[in] dwStyle	按鈕的樣式(風格)
 *	@param[in] bRepaint	使否立即重繪 TRUE/FALSE
 *	@return	此函數沒有返回值
 */
void DmButton::SetButtonStyle(DWORD dwStyle, BOOL bRepaint) const
{
	assert(this->IsWindow());
	// BM_SETSTYLE
	// wParam = 按鈕的樣式(Style)
	// lParam = 是否進行重繪 TRUE/ FALSE
	::SendMessage(*this, BM_SETSTYLE, static_cast<WPARAM>(dwStyle), static_cast<LPARAM>(bRepaint));
}
