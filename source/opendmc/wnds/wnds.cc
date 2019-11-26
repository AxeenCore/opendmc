/**************************************************************************//**
 * @file	wnds.cc
 * @brief	DmWnds 視窗基底類別成員函數定義
 * @date	2000-10-10
 * @date	2018-08-05
 * @author	Swang
 *****************************************************************************/
#include "opendmc/wnds/wnds.hh"

/**
 *	@brief DmWnds 建構式
 */
DmWnds::DmWnds()
	: DmWndsObject()
	, m_eType(EmCtrls::Empty)
	, m_hWnd(NULL)
	, m_lPrevCallback(0)
	, m_hFont(NULL)
	, m_bModal(FALSE) {

	this->InitCommControls();
}

/**
 *	@brief DmWnds 建構式
 *	@param[in] eType 控制項種類，參照 EmCtrls 列舉，若非控制項則填入 EmCtrls::ExEmpty。
 */
DmWnds::DmWnds(EmCtrls eType)
	: DmWndsObject()
	, m_eType(eType)
	, m_hWnd(NULL)
	, m_lPrevCallback(0)
	, m_hFont(NULL)
	, m_bModal(FALSE) {

	this->InitCommControls();
}

/**
 *	@brief DmWnds 解構式
 *	@remark 解構時，釋放(銷毀)控制項
 */
DmWnds::~DmWnds() { this->SafeWndsDestroy(); }

/**
 *	@brief [虛擬函數] 連接已存在的視窗或控制項
 *	@param[in] hWnd 要進行連接的視窗或控制項操作代碼
 *	@return <b>型別: BOOL</b> \n 若連接成功返回值為非零值，若連接失敗則返回值為零。
 */
BOOL DmWnds::Attach(HWND hWnd)
{
	assert(hWnd);
	auto bResult = this->BindWindow(hWnd);

	if (bResult) {
		this->PostUserCreateMessage();
	}
	return bResult;
}

/**
 *	@brief <b>[虛擬函數]</b> 連接已存在視窗或控制項
 *	@param[in] hWndParent	父視窗操作代碼
 *	@param[in] nIDCItem		視窗或控制項識別碼
 *	@return <b>型別: BOOL</b> \n 若連接成功返回值為非零值，若連接失敗則返回值為零。
 */
BOOL DmWnds::AttachDlgItem(HWND hWndParent, int nIDCItem)
{
	assert(hWndParent);
	auto hWnd = ::GetDlgItem(hWndParent, nIDCItem);
	auto bResult = this->BindWindow(hWnd);

	if (bResult) {
		this->PostUserCreateMessage();
	}
	return bResult;
}

/**
 *	@brief 斷開連接，配合 Attach 函數使用。
 *	@return 此函數沒有返回值
 */
void DmWnds::Detach() { this->SafeWndsRelease(); }

/**
 *	@brief [虛擬函數] 建立控制項，內容由繼承者發展，基底永遠返回零。
 *	@param[in] szPtr		指向控制項名稱字串緩衝區位址，用來設定控制項的視窗名稱。
 *	@param[in] x			起始位置 (左上角) X
 *	@param[in] y			起始最至 (左上角) Y
 *	@param[in] wd			寬度
 *	@param[in] ht			高度
 *	@param[in] hWndParent	父視窗操作代碼
 *	@param[in] nIDCItem		項目識別碼
 *	@param[in] hInstance	視窗應用程式實例操作代碼
 *	@return <b>型別: BOOL</b> \n 若函數操作成功返回值為非零值，值為視窗或控制項操作代碼。 \n 若函數操作失敗返回值為零。
 */
BOOL DmWnds::Create(const TCHAR* szPtr, int x, int y, int wd, int ht, HWND hWndParent, int nIDCItem)
{
	const TCHAR* szCtrlPtr = this->GetControlsClassName();
	const HINSTANCE hInstance = ::GetWinapp().GetInstanceHandle();
	DWORD dwStyle;
	DWORD dwExStyle;

	if (szCtrlPtr == NULL) {
		// 建立視窗
		WNDSFRAME smFrame;
		// 確保視窗必須具備樣式，若視窗變更大小，必須重繪工作區。
		dwStyle = static_cast<DWORD>(CS_HREDRAW | CS_VREDRAW) | static_cast<DWORD>(WNDS_DEFAULT_STYLE);
		dwExStyle = static_cast<DWORD>(WNDS_DEFAULT_EXSTYLE);

		// 輸入 WINSFRAME 結構內容
		::memset((void*)&smFrame, 0, sizeof(WNDSFRAME));
		smFrame.hInstance = hInstance;
		smFrame.hWndParent = hWndParent;
		smFrame.pszClass = TEXT(WNDS_DEFAULT_CLASSNAME);
		smFrame.pszCaption = szPtr;
		smFrame.nPosx = x;
		smFrame.nPosy = y;
		smFrame.nWidth = wd;
		smFrame.nHeight = ht;
		smFrame.uClassStyle = 0;
		smFrame.hBackground = (HBRUSH)(COLOR_BTNFACE + 1);
		smFrame.hIcon = NULL;
		smFrame.hIconSm = NULL;
		smFrame.hCursor = NULL;
		smFrame.dwStyle = dwStyle;
		smFrame.dwExStyle = dwExStyle;
		smFrame.hMenuOrItemID = reinterpret_cast<HMENU>(static_cast<LONG_PTR>(nIDCItem));
		smFrame.vParam = NULL;
		return this->CreateFrame(&smFrame);
	} else {
		// 建立控制項
		auto type = this->GetSafeType();

		// 控制項判別
		switch (type)
		{
		case EmCtrls::Button:
			dwStyle = BS_PUSHBUTTON;
			dwExStyle = 0;
			break;
		case EmCtrls::Edit:
			dwStyle = ES_AUTOHSCROLL;
			dwExStyle = 0;
			break;
		case EmCtrls::List:
			dwStyle = WS_VSCROLL | ES_AUTOVSCROLL;
			dwExStyle = 0;
			break;
		case EmCtrls::Combo:
			dwStyle = CBS_DROPDOWN;
			dwExStyle = 0;
			break;
		case EmCtrls::ListView:
			dwStyle = LVS_REPORT | LVS_SHOWSELALWAYS;
			dwExStyle = 0;
			break;
		case EmCtrls::TreeView:
			dwStyle = TVS_HASLINES;
			dwExStyle = 0;
			break;
		case EmCtrls::TabControl:
			dwStyle = WS_CLIPSIBLINGS | TCS_FIXEDWIDTH;	// | TCS_MULTILINE
			dwExStyle = 0;
			break;
		default:
			// error handling
			return FALSE;
		}

		WNDSCTRLS smCtrl;
		::memset(&smCtrl, 0, sizeof(WNDSCTRLS));
		smCtrl.hInstance	= hInstance;
		smCtrl.hParent		= hWndParent;
		smCtrl.pszCaption	= szPtr;
		smCtrl.dwStyle		= dwStyle;
		smCtrl.dwExStyle	= dwExStyle;
		smCtrl.nPosx		= x;
		smCtrl.nPosy		= y;
		smCtrl.nWidth		= wd;
		smCtrl.nHeight		= ht;
		smCtrl.nIDCItem		= nIDCItem;
		return this->CreateControls(&smCtrl);
	}
}

/**
 *	@brief 建立一個視窗
 *	@param[in] dwExStyle		視窗擴展式樣式
 *	@param[in] szClassPtr		在系統註冊視窗 Class 名稱，使用 NULL 結尾的文字字串，使用 RegisterClass 或 RegisterClassEx 向系統註冊。
 *								\n 若為控制項請填入 NULL
 *	@param[in] szCaptionPtr		視窗顯示名稱，使用 NULL 結尾的文字字串。
 *	@param[in] dwStyle			視窗樣式
 *	@param[in] x				視窗顯示於螢幕的起始位置，左上角 X 座標，若為子視窗(項目) 則以父視窗的工作區作為顯示區。
 *	@param[in] y				視窗顯示於螢幕的起始位置，左上角 Y 座標，若為子視窗(項目) 則以父視窗的工作區作為顯示區。
 *	@param[in] wd				視窗的寬度
 *	@param[in] ht				視窗的高度
 *	@param[in] hWndParent		父視窗的操作代碼 (如果有的話，若沒有填入 NULL)
 *	@param[in] hMenuOrIDDItem	視窗選單操作代碼，或子視窗(項目)的識別碼(identifier)
 *	@param[in] vParamPtr		額外的參數，使用指標型別，當視窗建立後於 WM_CREATE 訊息觸發 由參數 LPARAM 時取得。
 *	@return <b>型別: BOOL</b> \n	若視窗建立成功則返回值為非零值。\n 若視窗建立失敗則返回零。
 */
BOOL DmWnds::CreateEx(DWORD dwExStyle, const TCHAR* szClassPtr, const TCHAR* szCaptionPtr, DWORD dwStyle, int x, int y, int wd, int ht, HWND hWndParent, HMENU hMenuOrIDDItem, void* vParamPtr)
{
	const TCHAR* szCtrlPtr = this->GetControlsClassName();
	const HINSTANCE hInstance = ::GetWinapp().GetInstanceHandle();

	if (szCtrlPtr == NULL) {
		// 確保視窗必須具備樣式，若視窗變更大小，必須重繪工作區。
		dwStyle |= static_cast<DWORD>(CS_HREDRAW | CS_VREDRAW);

		// 輸入 WINSFRAME 結構內容
		WNDSFRAME smFrame;
		::memset((void*)&smFrame, 0, sizeof(WNDSFRAME));
		smFrame.hInstance = hInstance;
		smFrame.hWndParent = hWndParent;
		smFrame.pszClass = szClassPtr;
		smFrame.pszCaption = szCaptionPtr;
		smFrame.nPosx = x;
		smFrame.nPosy = y;
		smFrame.nWidth = wd;
		smFrame.nHeight = ht;
		smFrame.uClassStyle = 0;
		smFrame.hBackground = (HBRUSH)(COLOR_BTNFACE + 1);
		smFrame.hIcon = NULL;
		smFrame.hIconSm = NULL;
		smFrame.hCursor = NULL;
		smFrame.dwStyle = dwStyle;
		smFrame.dwExStyle = dwExStyle;
		smFrame.hMenuOrItemID = hMenuOrIDDItem;
		smFrame.vParam = vParamPtr;
		return this->CreateFrame(&smFrame);
	}

	WNDSCTRLS smCtrl;
	::memset(&smCtrl, 0, sizeof(WNDSCTRLS));
	smCtrl.hInstance	= hInstance;
	smCtrl.hParent		= hWndParent;
	smCtrl.pszCaption	= szCaptionPtr;
	smCtrl.dwStyle		= dwStyle;
	smCtrl.dwExStyle	= dwExStyle;
	smCtrl.nPosx		= x;
	smCtrl.nPosy		= y;
	smCtrl.nWidth		= wd;
	smCtrl.nHeight		= ht;
	smCtrl.nIDCItem		= static_cast<int>(reinterpret_cast<LONG_PTR>(hMenuOrIDDItem));
	return this->CreateControls(&smCtrl);
}

/**
 *	@brief 建立一個樣本視窗框架
 *	@return	<b>型別: BOOL</b> 若視窗建立成功返回值為非零值，若視窗建立失敗責返回值為零。
 */
#if 0
BOOL DmWnds::CreateSample()
{
	const TCHAR* szClassPtr = this->GetControlsClassName();

	if (szClassPtr == NULL) {
		// 填寫 WINSFRAME 結構內容
		WNDSFRAME ws;
		::memset((void*)&ws, 0, sizeof(WNDSFRAME));
		ws.hInstance = ::GetWinapp().GetInstanceHandle();
		ws.hWndParent = NULL;
		ws.szClassPtr = const_cast<TCHAR*>(TEXT(WNDS_DEFAULT_CLASSNAME));
		ws.szCaptionPtr = const_cast<TCHAR*>(TEXT(WNDS_DEFAULT_CAPTION));
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
#endif

/**
 *	@brief	取得控制項種類
 *	@return	<b>型別: EmCtrls</b> \n 返回控制項種類代碼
 *	@see	EmCtrls
 */
EmCtrls DmWnds::GetSafeType() const { return m_eType; }

/**
 *	@brief	取得控制項操作代碼
 *	@return	<b>型別: HWND</b>
 *		\n 若控制項已存在返回值為非零值，返回值為控制項操作代碼。
 *		\n 若控制項不存在返回值為零。
 */
HWND DmWnds::GetSafeHwnd() const { return m_hWnd; }

/**
 *	@brief	取得使用者定義字型操作代碼
 *	@return	<b>型別: HFONT</b>
 *		\n 若使用者字型已建立則返回值為非零值，返回值為自行操作代碼。
 *		\n 若使用者字型未建立則返回值為零。
 */
HFONT DmWnds::GetSafeFont() const { return m_hFont; }

/**
 *	@brief	取得控制項訊息處理 Callback 函數位址
 *	@return	<b>型別: LONG_PTR</b> \n 返回值為控制項訊息處理 Callback 函數位址。
 */
LONG_PTR DmWnds::GetSafeCallback() const { return reinterpret_cast<LONG_PTR>(DmWnds::SafeWndProc); }

/**
 *	@brief	取得變更前的控制項訊息處理 Callback 函數位址
 *	@return	<b>型別: LONG_PTR</b> \n 返回值為變更前的控制項訊息處理 Callback 函數位址
 */
LONG_PTR DmWnds::GetSafePrevCallback() const { return m_lPrevCallback; }

/**
 *	@brief 建立視窗框架
 *	@param[in] smPtr 指向結構 WNDSFRAME 資料緩衝區位址，指定要建立視窗框架內容。
 *	@return	<b>型別: BOOL</b> 若視窗建立成功返回值為非零值，若視窗建立失敗責返回值為零。
 */
BOOL DmWnds::CreateFrame(const WNDSFRAME* smPtr)
{
	HWND hWnd = NULL;

	for (;;) {
		// 註冊視窗
		if (!this->SafeRegisterClass(smPtr)) {
			break;
		}
		// 建立視窗
		hWnd = this->SafeCreateWindows(smPtr);
		if (hWnd == NULL) {
			break;
		}
		break;
	}
	return this->IsWindow();
}

/**
 *	@brief 建立控制項
 *	@param[in] smPtr 指向結構 WINSFRAME 資料緩衝區位址，指定要建立視窗框架內容。
 *	@return	<b>型別: BOOL</b> 若視窗建立成功返回值為非零值，若視窗建立失敗責返回值為零。
 */
BOOL DmWnds::CreateControls(const WNDSCTRLS* smPtr)
{
	const TCHAR* szClassPtr = this->GetControlsClassName();
	HINSTANCE hInstance = ::GetWinapp().GetInstanceHandle();
	HWND	hCtrl = NULL;
	HMENU	hMenu = NULL;
	DWORD	dwExStyle, dwStyle;

	for (;;) {
		// 物件已存在視窗或控制項
		if (this->IsWindow()) {
			// error handleing
			break;
		}
		// 無效的結構位址 ?
		if (smPtr == NULL) {
			// error handling
			break;
		}
		// 視窗應用程式實例代碼是否就緒
		if (hInstance == NULL) {
			// error handling
			break;
		}
		// 驗證父視窗是否無效，控制項必定要有父視窗。
		if (smPtr->hParent == NULL || !::IsWindow(smPtr->hParent)) {
			// error handling
			break;
		}
		// 控制項識別碼是否在合法範圍
		if (smPtr->nIDCItem < MIN_WINC_IDITEM || smPtr->nIDCItem > MAX_WINC_IDITEM) {
			// error handling
			break;
		}
		// 取得控制項的註冊 class 名稱
		if (szClassPtr == NULL) {
			// error handling
			break;
		}

		// 設定視窗擴展 ExStyle
		dwStyle = smPtr->hParent ? static_cast<DWORD>(WNDS_COMMON_CHILD_STYLE) : static_cast<DWORD>(WNDS_COMMON_STYLE);
		dwExStyle = smPtr->dwExStyle | static_cast<DWORD>(WNDS_COMMON_CHILD_EXSTYLE);
		dwStyle |= smPtr->dwStyle;

		// 建立控制項，使用 Win32 API CreateWindowEx
		hMenu = reinterpret_cast<HMENU>(static_cast<LONG_PTR>(smPtr->nIDCItem));
		hCtrl = CreateWindowEx(
			dwExStyle,
			szClassPtr,
			smPtr->pszCaption,
			dwStyle,
			smPtr->nPosx,
			smPtr->nPosy,
			smPtr->nWidth,
			smPtr->nHeight,
			smPtr->hParent,
			hMenu,
			hInstance,
			reinterpret_cast<LPVOID>(this));
		if (hCtrl == NULL) {
			// error handling
			break;
		}

		// 綁定控制項到此物件
		if (!this->BindWindow(hCtrl)) {
			// error handling
			break;
		}

		// 發出綁定後，建立視窗訊息
		this->PostUserCreateMessage();
		break;
	}
	return hCtrl != NULL;
}

/**
 *	@brief 發出 WM_USERCREATE 訊息，用來告知建立視窗內容。
 *	@param[in] wParam 未使用，必須為零。
 *	@param[in] lParam 未使用，必須為零。
 *	@return <b>型別: BOOL</b> \n 若運作成功返回值為非零值，若運作失敗則返回值為零。
 */
BOOL DmWnds::PostUserCreateMessage(WPARAM wParam, LPARAM lParam) const
{
	assert(this->IsWindow());
	return ::PostMessage(*this, WM_USERCREATE, wParam, lParam);
}

/**
 *	@brief 建立使用者自定義字型於綁定視窗使用
 *	@param[in] szFacePtr	指向字型名稱緩衝區位址
 *	@param[in] nSize		字型尺寸
 *	@param[in] bBlod		使用粗體 (Yes = TRUE, No = FALSE)
 *	@param[in] nCharset		使用字符集
 *	@return <b>型別: HFONT</b> \n 若運作成功則返回值為非零值，值為字型操作代碼，若運作失敗則返回值為零。
 *	@see DmWnds::CreateFont
 */
BOOL DmWnds::CreateSafeFont(const TCHAR* szFacePtr, int nSize, BOOL bBlod, int nCharset)
{
	auto hFont = this->CreateFont(szFacePtr, nSize, bBlod, nCharset);
	if (hFont != NULL) {
		this->SetFont(hFont);
		this->SetSafeFont(hFont);
	}
	return hFont != NULL;
}

/**
 *	@brief 刪除使用者自定義字型
 *	@return <b>型別: BOOL</b> \n 若運作成功則返回值為非零值，若運作失敗則返回值為零。
 */
void DmWnds::DeleteSafeFont()
{
	if (m_hFont != NULL) {
		this->DeleteFont(m_hFont);
		this->SetSafeFont(NULL);
	}
}

/**
 *	@brief 準備對於指定視窗進行繪製，繪製方式參照 PAINTSTRUCT 結構定義內容。
 *	@param[out] psPtr 指向 PAINTSTRUCT 結構資料位址
 *	@return	<b>型別: HDC</b>
 *		\n 若運作成功，返回值為非零值，指定視窗的設備的 context 操作代碼 (Handle)。
 *		\n 若運作失敗，返回值為零，表示沒有可以使用於顯示的 context。
 *	@see https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-beginpaint
 */
HDC DmWnds::BeginPaint(PAINTSTRUCT* psPtr) const
{
	assert(this->IsWindow());
	return ::BeginPaint(*this, psPtr);
}

/**
 *	@brief 將指定的視窗移動倒 Z 軸的最頂端。
 *		\n 若指定視窗已是最上層，那麼將激活指定視窗. 若指定視窗為子視窗，那麼會將此視窗的父視窗移動到 Z 軸最頂端
 *	@return <b>型別: BOOL</b> \n	如果運作成功，則返回值為非零。 \n 如果函數失敗，則返回值為零。要取得錯誤信息，調用 GetLastError()
 */
BOOL DmWnds::BringWindowToTop() const
{
	assert(this->IsWindow());
	return ::BringWindowToTop(*this);
}

/**
 *	@brief 將視窗顯示未置定於父視窗 Client 區的中央位置, 若沒有父視窗則以桌面上中央位置顯示
 *	@return <b>型別: BOOL</b> \n	如果運作成功，則返回值為非零值。 \n 如果函數失敗，則返回值為零。要取得錯誤信息，調用 GetLastError()
 *	@remark 若有父視窗, 將以父視窗的 Client 區塊為基準. 若無父視窗將以桌面做為基準.
 */
BOOL DmWnds::CenterWindow() const
{
	assert(this->IsWindow());

	BOOL bResult = FALSE;
	HWND hWnd = this->GetSafeHwnd();
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

	if (this->GetWindowRect(&rc)) {
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
 *	@brief 改變按鈕控制項的檢查狀態
 *	@param[in] nIDButton	要變更的按鈕控制項 ID.
 *	@param[in] uCheck		要變更的狀態
 *	@return <b>型別: BOOL</b>
 *		\n 若運作成功，返回值為非零值。
 *		\n 若運作失敗，則返回值為零值。錯誤發生調用 GetLastError() 取得錯誤信息，
 *	@remark <b>UINT uCheck 狀態表</b>
 *		- BST_CHECKED		\n 設定按鈕成為已核對狀態
 *		- BST_INDETERMINATE	\n 設定按鈕承灰色，表示為不確定狀態，這一個狀態值僅支援 BS_3STATE 或 BS_AUTO3STATE 樣式。
 *		- BST_UNCHECKED		\n 清除按鈕狀態設定
 */
BOOL DmWnds::CheckDlgButton(int nIDButton, UINT uCheck) const
{
	assert(this->IsWindow());
	return ::CheckDlgButton(*this, nIDButton, uCheck);
}

/**
 *	@brief 給一組單選按鈕中的一個指定按鈕加上選中標示，並且清除組中其他按鈕的選中標示。
 *	@param[in] nIDFirstButton	指定組中第一個單選按鈕識別碼(ID)
 *	@param[in] nIDLastButton	指定組中最後一個單選按鈕識別碼(ID)
 *	@param[in] nIDCheckButton	指出要選訂哪一個單選按鈕識別碼(ID)
 *	@return <b>型別: BOOL</b>
 *		\n 如果運作成功返回值為非零值
 *		\n 如果運作失敗返回值為零。錯誤發生調用 GetLastError() 取得錯誤訊息。
 */
BOOL DmWnds::CheckRadioButton(int nIDFirstButton, int nIDLastButton, int nIDCheckButton) const
{
	assert(this->IsWindow());
	return ::CheckRadioButton(*this, nIDFirstButton, nIDLastButton, nIDCheckButton);
}

/**
 *	@brief 取得做標點上的子視窗操作代碼 (不包含子孫視窗)
 *	@param[in] Point 指向 POINT 結構資料參考, (於父視窗 Client 區域)
 *	@return <b>型別: HWND</b>
 *		\n 返回指定的座標點的子視窗操作代碼(handle)，若該座標點超出父視窗 client 邊界則傳回 NULL。
 *		\n 若該視窗內無任何子視窗，則傳回父視窗操作代碼。
 */
HWND DmWnds::ChildWindowFromPoint(POINT Point) const
{
	assert(this->IsWindow());
	return ::ChildWindowFromPoint(*this, Point);
}

/**
 *	@brief 將指定的 client 區域作標轉換成螢幕(桌面)座標。
 *	@param[in,out] ptPtr 指向 POINT 結構資料位址，做為資料進行轉換的存取位址。
 *	@return <b>型別: BOOL</b> \n 若運作成功則返回值為非零值。 \n 若運作失敗則返回值為零。
 */
BOOL DmWnds::ClientToScreen(POINT* ptPtr) const
{
	assert(this->IsWindow());
	return ::ClientToScreen(*this, ptPtr);
}

/**
 *	@brief 將指定的 Client 區域作標轉換成螢幕 (桌面) 座標。
 *	@param[in,out] rcPtr 指向 RECT 結構資料位址，做為資料進行轉換的存取位址.
 *	@return <b>型別: BOOL</b> \n 若運作成功則返回值為非零值。 \n 若運作失敗則返回值為零。
 */
BOOL DmWnds::ClientToScreen(RECT* rcPtr) const
{
	assert(this->IsWindow());
	return static_cast<BOOL>(::MapWindowPoints(*this, NULL, reinterpret_cast<LPPOINT>(rcPtr), 2));
}

/**
 *	@brief 最小化視窗，注意僅是將視窗最小化，不是銷毀視窗。
 *	@return	<b>型別: BOOL</b> \n 如果運作成功，則返回值為非零。 \n 如果函數失敗，則返回值為零。要取得錯誤信息，調用 GetLastError()
 */
BOOL DmWnds::CloseWindow() const
{
	assert(this->IsWindow());
	return ::CloseWindow(*this);
}

/**
 *	@brief 建立一個使用者自定義字型
 *	@param[in] szFacePtr	指向字型名稱緩衝區位址
 *	@param[in] nSize		字型尺寸
 *	@param[in] bBlod		使用粗體 (Yes = TRUE, No = FALSE)
 *	@param[in] nCharset		使用字符集
 *	@return <b>型別: HFONT</b> \n 若運作成功則返回值為非零值，值為字型操作代碼，若運作失敗則返回值為零。
 *	@remark <b>int nCharset 字符集參數說明</b>
 *		- ANSI_CHARSET
 *		- BALTIC_CHARSET
 *		- CHINESEBIG5_CHARSET
 *		- DEFAULT_CHARSET
 *		- EASTEUROPE_CHARSET
 *		- GB2312_CHARSET
 *		- GREEK_CHARSET
 *		- HANGUL_CHARSET
 *		- MAC_CHARSET
 *		- OEM_CHARSET
 *		- RUSSIAN_CHARSET
 *		- SHIFTJIS_CHARSET
 *		- SYMBOL_CHARSET
 *		- TURKISH_CHARSET
 *		- VIETNAMESE_CHARSET
 */
HFONT DmWnds::CreateFont(const TCHAR* szFacePtr, int nSize, BOOL bBlod, int nCharset)
{
	assert(this->IsWindow());

	HDC	hDC = NULL;
	int	iHeight, iBold;
	LOGFONT	lf;

	hDC = ::GetDC(*this);
	iHeight = 0 - ::MulDiv(nSize, ::GetDeviceCaps(hDC, LOGPIXELSY), 72);
	::ReleaseDC(*this, hDC);
	iBold = bBlod ? FW_BOLD : FW_NORMAL;

	// 設定 LOGFONT 結構內容
	::memset((void*)&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = iHeight;                      // 字體高度
	lf.lfWidth = 0;                             // 字體闊度
	lf.lfEscapement = 0;                        // 字體斜度
	lf.lfOrientation = 0;                       // 底線斜度
	lf.lfWeight = iBold;                        // 字體粗細
	lf.lfItalic = FALSE;                        // 設定字體為斜體
	lf.lfUnderline = FALSE;                     // 設定字體底線
	lf.lfStrikeOut = FALSE;                     // 設定刪線
	lf.lfCharSet = static_cast<BYTE>(nCharset); // 設定字符集
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;     // 字體輸出解析度
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;   // 字型擷取解析度
	lf.lfQuality = DEFAULT_QUALITY;             // 字體輪廓質素
	lf.lfPitchAndFamily = 0;                    // 字體的外觀參考(沒有所需字體時用)
	::_tcscpy(lf.lfFaceName, szFacePtr);		// 字體名稱
	return ::CreateFontIndirect(&lf);
}

/**
 *	@brief 刪除 DmWnds::CreateFont() 建立之字型
 *	@param[in] hFont 字型操作代碼
 *	@return <b>型別: BOOL</b> \n 若刪除字型成功返回值為非零值，若刪除自行失敗則返回值為零。
 */
BOOL DmWnds::DeleteFont(HFONT hFont) {

	if (hFont != NULL) {
		return ::DeleteObject(reinterpret_cast<HGDIOBJ>(hFont));
	}
	return FALSE;
}

/**
 *	@brief 啟用或禁用，視窗或控制項接收滑鼠或鍵盤等輸入訊息
 *	@return <b>型別: BOOL</b>
 *		\n 若先前視窗或控制項是禁用狀態，返回值為非零值。
 *		\n 若先前視窗或控制項非禁用狀態，返回值為零。
 *	@see DmWnds::EnableWindow
 */
BOOL DmWnds::DisableWindow() const
{
	assert(this->IsWindow());
	return ::EnableWindow(*this, FALSE);
}

/**
 *	@brief 將指定目錄下的子目錄、文件、檔案名稱等，顯示在指定的列表控制項
 *	@param[in] szPathSpecPtr	以 NULL 結尾的字串緩衝區位址，此字串指定絕對路徑，相對路徑或文件名稱
 *	@param[in] nIDListBox		ListBox 控制項 ID (identifier) 若此值為零，代表 DirList 將不會將子目錄或檔案名稱輸出於指定列表.
 *	@param[in] nIDStaticPath	視窗或對話框中的靜態控制項 ID。 (ListBox 此項為 0)
 *	@param[in] uFileType		要新增於列表的文件檔案或目錄的屬性。
 *	@return <b>型別: int</b>
 *		\n	若運作成功，返回值為非零值。 \n 若運作失敗，返回值為零。
 *		\n	若 szPathSpecPtr 為一個無效的目錄或檔案名稱，將視為錯誤，返回值為零，可使用 GetError() 取得錯誤訊息。
 *	@remark		<b>uFileType 參數詳細說明</b>
 *		- DDL_ARCHIVE	\n 包含檔案名稱.
 *		- DDL_DIRECTORY	\n 包含子目錄。子目錄名稱括在方括號（[]）中。
 *		- DDL_DRIVES	\n 所有映射的驅動器都將添加到列表中。磁碟以 [ - x - ] 的形式列出 ，其中 x 磁碟代碼。
 *		- DDL_EXCLUSIVE	\n 僅包含具有指定屬性的文件。默認情況下，即使未指定DDL_READWRITE，也會列出讀/寫文件。
 *		- DDL_HIDDEN	\n 包含隱藏檔案。
 *		- DDL_READONLY	\n 包含唯讀檔案。
 *		- DDL_READWRITE	\n 包含沒有其他屬性的 讀/寫 檔案，這是預設值設置。
 *		- DDL_SYSTEM	\n 包含系統檔案.
 *		- DDL_POSTMSGS	\n 若被設置 DlgDirList 使用 PostMessage 函數將消息發送到列錶框。如果未設置，則 DlgDirList 使用 SendMessage 函數。
 */
int DmWnds::DlgDirList(TCHAR* szPathSpecPtr, int nIDListBox, int nIDStaticPath, UINT uFileType) const
{
	assert(this->IsWindow());
	return ::DlgDirList(*this, szPathSpecPtr, nIDListBox, nIDStaticPath, uFileType);
}

/**
 *	@brief 將指定目錄下的子目錄、文件、檔案名稱等，顯示在指定的組合式列表控制項
 *	@param[in] szPathSpecPtr	以 NULL 結尾的字串緩衝區位址，此字串指定絕對路徑，相對路徑或文件名稱
 *	@param[in] nIDComboBox		nIDComboBox 控制項 ID (identifier)
 *	@param[in] nIDStaticPath	視窗或對話框中的靜態控制項 ID。
 *	@param[in] uFileType		要新增於列表的文件檔案或目錄的屬性。
 *	@return <b>型別: int</b>
 *		\n 若運作成功，返回值為非零值，若運作失敗返回值為零。
 *		\n 若 szPathSpecPtr 為一個無效的目錄或檔俺名稱，視為錯誤，返回值為零，可使用 GetError() 取得錯誤訊息。
 *	@see DmWnds::DlgDirList
 */
int DmWnds::DlgDirListComboBox(TCHAR* szPathSpecPtr, int nIDComboBox, int nIDStaticPath, UINT uFileType) const
{
	assert(this->IsWindow());
	return ::DlgDirListComboBox(*this, szPathSpecPtr, nIDComboBox, nIDStaticPath, uFileType);
}

/**
 *	@brief 從單選列表框(ListBox)中檢索當前選擇。假定列表框(ListBox)，由 DlgDirList 函數填充，並且選擇是磁碟符號，檔案名或目錄名。
 *	@param[in] strPtr		指向接收所選路徑的緩衝區位址。
 *	@param[in] chCount		strPtr 於緩衝區中的長度，單位 TCHAR
 *	@param[in] idListBox	列表框(ListBox)於對話框的 ID (identifier).
 *	@return <b>型別: BOOL</b>
 *		\n 若當前選擇的是目錄名稱返回值為非零值。若當前選擇的非目錄名稱返回值為零。
 *		\n 若 szPathSpecPtr 為一個無效的目錄或檔俺名稱，視為錯誤，返回值為零，可使用 GetError() 取得錯誤訊息。
 */
BOOL DmWnds::DlgDirSelectEx(TCHAR* strPtr, int chCount, int idListBox) const
{
	assert(this->IsWindow());
	return ::DlgDirSelectEx(*this, strPtr, chCount, idListBox);
}

/**
 *	@brief 從單選組合框(ComboBox)中檢索當前選擇。假定組合框(ComboBox)，由 DlgDirListComboBox 函數填充，並且選擇是磁碟符號，檔案名或目錄名。 \n
 *	@param[in] strPtr		指向接收所選路徑的緩衝區位址。
 *	@param[in] cchOut		strPtr 於緩衝區中的長度，單位 TCHAR
 *	@param[in] idComboBox	組合框(ComboBox)，於對話框的 ID (identifier).
 *	@return <b>型別: BOOL</b>
 *		\n 若當前選擇的是目錄名稱，返回值為非零值。若當前選擇的非目錄名稱，返回值為零。
 *		\n 若 szPathSpecPtr 為一個無效的目錄或檔俺名稱，視為錯誤，返回值為零，可使用 GetError() 取得錯誤訊息。
 */
BOOL DmWnds::DlgDirSelectComboBoxEx(TCHAR* strPtr, int cchOut, int idComboBox) const
{
	assert(this->IsWindow());
	return ::DlgDirSelectComboBoxEx(*this, strPtr, cchOut, idComboBox);
}

/**
 *	@brief 指示視窗或控制項是否接受刪除文件。
 *	@param[in] fAccept 是否接受已刪除的文件。
 *	@return 此函數沒有返回值。
 *	@remark <b>fAccept 參數值</b>
 *		- TRUE	\n 接受已刪除的文件
 *		- FALSE	\n 停止接受已刪除的文件。
 */
void DmWnds::DragAcceptFiles(BOOL fAccept) const
{
	assert(this->IsWindow());
	::DragAcceptFiles(*this, fAccept);
}

/**
 *	@brief 函數繪製一個線框 rectangle 並對它的進行動畫處理以指示打開 icon，或者最小化窗口的最小化或者最大化
 *	@param[in] idAni		動畫的類型，這必須是 IDANI_CAPTION。使用 IDANI_CAPTION 動畫類型。
 *							\n 視窗標題將從 rcFromPtr 指定的位置動畫到 rcToPtr 指定的位置。效果類似於最小化或最大化視窗。
 *	@param[in] rcFromPtr	指向結構 RECT 資料位址，指示圖示或最小化視窗的位置和尺寸。
 *	@param[in] rcToPtr		指向結構 RECT 資料位址，指示還原視窗的位置和尺寸。
 *	@return <b>型別: BOOL</b> \n 若運作成功返回值為非零值，若運作失敗返回值為零。
 */
BOOL DmWnds::DrawAnimatedRects(int idAni, const RECT* rcFromPtr, const RECT* rcToPtr) const
{
	assert(this->IsWindow());
	return ::DrawAnimatedRects(*this, idAni, rcFromPtr, rcToPtr);
}

/**
 *	@brief DrawCaption 用來針對視窗標題進行繪製
 *	@param[in] hDC		設備 context 的控制代碼。該函數將視窗標題繪製到此設備 context。
 *	@param[in] rcPtr	RECT 結構資料位址，指定邏輯座標中視窗標題的矩形邊界。
 *	@param[in] dwFlags	繪圖參數。此參數可以是以下值中的零個或多個。
 *	@return <b>型別: BOOL</b> \n 若運作成功，返回值為非零值。 \n 若運作失敗，返回值為零。
 *	@remark <b>dwFlags 繪圖參數說明</b>
 *		- DC_ACTIVE		\n 該函數使用表示活動標題的顏色。
 *		- DC_BUTTONS	\n 如果設置，該功能將繪製標題欄中的按鈕（以最小化，恢復或關閉應用程序）。
 *		- DC_GRADIENT	\n 設置此標誌時，該函數使用COLOR_GRADIENTACTIVECAPTION（如果已設置DC_ACTIVE標誌）或 COLOR_GRADIENTINACTIVECAPTION 作為標題欄顏色。
 *						\n 如果未設置此標誌，則該函數對兩種顏色使用COLOR_ACTIVECAPTION或COLOR_INACTIVECAPTION。
 *		- DC_ICON		\n 該函數在標題上繪製圖示
 *		- DC_INBUTTON	\n 該功能能在標題上繪製按鈕
 *		- DC_SMALLCAP	\n 該功能繪製小型標題使用目前小型標題的字型。
 *		- DC_TEXT		\n 該功能在標題列上繪製文字。
 */
BOOL DmWnds::DrawCaption(HDC hDC, const RECT* rcPtr, UINT dwFlags) const
{
	assert(this->IsWindow());
	return ::DrawCaption(*this, hDC, rcPtr, dwFlags);
}

/**
 *	@brief 重繪指定視窗的選單。如果在系統建立視窗後選單發生變更，則必須調用此函數去繪製變更的選單。
 *	@return <b>型別: BOOL</b> \n 若運作成功返回值為非零值。 \n 若運作失敗返回值為零。
 */
BOOL DmWnds::DrawMenuBar() const
{
	assert(this->IsWindow());
	return ::DrawMenuBar(*this);
}

/**
 *	@brief 啟用或禁用指定視窗或控制項的滑鼠與鍵盤的輸入
 *	@param[in] bEnable 指示啟用或禁用，若參數值為 TRUE 表示啟用視窗用控制項，若參數為 FALSE 表示禁用視窗或控制項。
 *	@return <b>型別: BOOL</b> \n 若視窗或控制項先前為禁用，返回值為非零值。 \n 若視窗或控制項非禁用，返回值為零
 */
BOOL DmWnds::EnableWindow(BOOL bEnable) const
{
	assert(this->IsWindow());
	return ::EnableWindow(*this, bEnable);
}

/**
 *	@brief 指定視窗繪製結束標記，此函數用來結束 BeginPaint 視窗開始繪製函數。
 *	@param[in] psPtr 指向結構 PAINTSTRUCT 資料緩衝區位址，紀錄 BeginPaint 相關資訊。
 *	@return <b>型別: BOOL</b> \n此函數永遠返回非零值。
 */
BOOL DmWnds::EndPaint(const PAINTSTRUCT* psPtr) const
{
	assert(this->IsWindow());
	return ::EndPaint(*this, psPtr);
}

/**
 *	@brief Retrieves a handle to the top-level window whose class name and window name match the specified strings
 *	@param[in] szClassNamePtr	The class name or a class atom created by a previous call to the RegisterClass or RegisterClassEx function
 *	@param[in] szWindowNamePtr	The window name (the window's title). If this parameter is NULL, all window names match.
 *	@return <b>型別: HWND</b>
 *		\n If the function succeeds, the return value is a handle to the window that has the specified class name and window name.
 *		\n If the function fails, the return value is NULL. To get extended error information, call GetLastError.
 */
HWND DmWnds::FindWindow(const TCHAR* szClassNamePtr, const TCHAR* szWindowNamePtr) const
{
	return ::FindWindow(szClassNamePtr, szWindowNamePtr);
}

/**
 *	@brief Retrieves a handle to a window whose class name and window name match the specified strings.
 *	@param[in] hWndParent		A handle to the parent window whose child windows are to be searched.
 *	@param[in] hWndChildAfter	A handle to a child window. The search begins with the next child window in the Z order.
 *	@param[in] szClassNamePtr	The class name or a class atom created by a previous call to the RegisterClass or RegisterClassEx function
 *	@param[in] szWindowNamePtr	The window name (the window's title). If this parameter is NULL, all window names match.
 *	@return <b>型別: HWND</b>
 *		\n If the function succeeds, the return value is a handle to the window that has the specified class name and window name.
 *		\n If the function fails, the return value is NULL. To get extended error information, call GetLastError.
 */
HWND DmWnds::FindWindowEx(HWND hWndParent, HWND hWndChildAfter, const TCHAR* szClassNamePtr, const TCHAR* szWindowNamePtr) const
{
	return ::FindWindowEx(hWndParent, hWndChildAfter, szClassNamePtr, szWindowNamePtr);
}

/**
 *	@brief Retrieves the window handle to the active window attached to the calling thread's message queue.
 *	@return <b>型別: HWND</b>
 *		\n The return value is the handle to the active window attached to the calling thread's message queue.
 *		\n Otherwise, the return value is NULL.
 */
HWND DmWnds::GetActiveWindow() const
{
	return ::GetActiveWindow();
}

/**
 *	@brief Retrieves the handle to the ancestor of the specified window.
 *	@param[in] uFlag The ancestor to be retrieved.
 *		- GA_PARENT		\n Value=1, Retrieves the parent window. This does not include the owner, as it does with the GetParent function.
 *		- GA_ROOT		\n Value=2, Retrieves the root window by walking the chain of parent windows.
 *		- GA_ROOTOWNER	\n Value=3, Retrieves the owned root window by walking the chain of parent and owner windows returned by GetParent.
 *	@return <b>型別: HWND</b> \n The return value is the handle to the ancestor window.
 */
HWND DmWnds::GetAncestor(UINT uFlag) const
{
	assert(this->IsWindow());
	return ::GetAncestor(*this, uFlag);
}

/**
 *	@brief Retrieves the window border size (in pixel).
 *	@return <b>型別: int</b> \n return the value is board size
 */
int DmWnds::GetBorderSize() const
{
	assert(this->IsWindow());
	int nBorder = 0;

	if (this->IsZoomed()) {
		RECT rcw, rcc;
		long bww, bwc;
		this->GetWindowRect(&rcw);
		this->GetClientRect(&rcc);
		bww = rcw.right - rcw.left;
		bwc = rcc.right - rcc.left;
		nBorder = static_cast<int>((bww - bwc) >> 1);
	}
	else {
		RECT rcw = { 0 };
		::AdjustWindowRectEx(&rcw, this->GetStyle(), ::GetMenu(*this) != NULL, this->GetExStyle());
		nBorder = rcw.right - rcw.left;
	}

	return nBorder;
}

/**
 *	@brief Retrieves a handle to the window (if any) that has captured the mouse.
 *		\n Only one window at a time can capture the mouse; this window receives mouse input whether or not the cursor is within its borders.
 *	@return <b>型別: HWND</b>
 *		\n The return value is a handle to the capture window associated with the current thread.
 *		\n If no window in the thread has captured the mouse, the return value is NULL.
 */
HWND DmWnds::GetCapture() const
{
	return ::GetCapture();
}

/**
 *	@brief Copies the caret's position to the specified POINT structure.
 *	@param[in] ptPtr A pointer to the POINT structure that is to receive the client coordinates of the caret.
 *	@return <b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero. To get extended error information, call GetLastError.
 */
BOOL DmWnds::GetCaretPos(POINT* ptPtr) const
{
	return ::GetCaretPos(ptPtr);
}

/**
 *	@brief Retrieves the name of the class to which the specified window belongs.
 *	@param[out] szClassNamePtr	The class name string.
 *	@param[in]	nMaxCount		The length
 *	@return <b>型別: int</b>
 *		\n If the function succeeds, the return value is the number of characters copied to the buffer, not including the terminating null character.
 *		\n If the function fails, the return value is zero. To get extended error information, call GetLastError.
 */
int DmWnds::GetClassName(TCHAR* szClassNamePtr, int nMaxCount) const
{
	assert(this->IsWindow());
	return ::GetClassName(*this, szClassNamePtr, nMaxCount);
}

/**
 *	@brief 取得視窗關聯的 WNDCLASSEX 結構中要檢索內容, 視窗向系統註冊的資訊 WNDCLASSEX, 利用此函數能夠調閱當前於系統中的視窗class內容.
 *	@param[in] nIndex 檢索項目
 *		- GCW_ATOM				\n 值 = (-32)，檢索唯一標識視窗類的 ATOM值。這與 RegisterClassEx 函數返回的值相同。
 *		- GCL_CBCLSEXTRA		\n 值 = (-20)，檢索與類關聯的額外記憶體的大小（以 WORD 為單位）。
 *		- GCL_CBWNDEXTRA		\n 值 = (-18)，檢索與類中每個視窗關聯的額外視窗記憶體的大小（以字節為單位）。有關如何訪問此記憶體的訊息，請參閱GetWindowLongPtr。
 *		- GCLP_HBRBACKGROUND	\n 值 = (-10)，檢索與類關聯的背景畫筆的句柄。
 *		- GCLP_HCURSOR			\n 值 = (-12)，檢索與類關聯的游標的句柄。
 *		- GCLP_HICON			\n 值 = (-14)，檢索與類關聯的圖標的句柄。
 *		- GCLP_HICONSM			\n 值 = (-34)，檢索與該類關聯的小圖標的句柄。
 *		- GCLP_HMODULE			\n 值 = (-16)，檢索註冊該類的模塊的句柄。
 *		- GCLP_MENUNAME			\n 值 = ( -8)，檢索指向菜單名稱字符串的指針。該字符串標識與該類關聯的菜單資源。
 *		- GCL_STYLE				\n 值 = (-26)，檢索視窗類樣式位。
 *		- GCLP_WNDPROC			\n 值 = (-24)，檢索視窗過程的地址，或表示視窗過程地址的句柄。您必須使用CallWindowProc函數來調用視窗過程。
 * @return <b>型別: ULONG_PTR</b> \n 若運作成功返回值檢索項目資訊。 \n 若操作失敗返回零，使用 GetLastError() 取得錯誤碼
 */
ULONG_PTR DmWnds::GetClassLongPtr(int nIndex) const
{
	assert(this->IsWindow());
	return ::GetClassLongPtr(*this, nIndex);
}

/**
 *	@brief 取得視窗的工作區座標資訊 (左上座標及右下座標)
 *	@param[out] rcPtr 指向結構 RECT 緩衝區位址，用來接收工作區邊界資訊。
 *	@return <b>型別: BOOL</b>
 *		\n 若運作成功返回值為非零值。
 *		\n 若運作失敗俺回值為零，使用 GetLastError 取得失敗原因資訊。
 */
BOOL DmWnds::GetClientRect(RECT* rcPtr) const
{
	assert(this->IsWindow());
	return ::GetClientRect(*this, rcPtr);
}

/**
 *	@brief The GetDC function retrieves a handle to a device context (DC) for the client area of a specified window or for the entire screen.
 *		\n You can use the returned handle in subsequent GDI functions to draw in the DC.
 *		\n The device context is an opaque data structure, whose values are used internally by GDI.
 *	@return <b>型別: HDC</b>
 *		\n If the function succeeds, the return value is a handle to the DC for the specified window's client area.
 *		\n If the function fails, the return value is NULL.
 */
HDC DmWnds::GetDC() const
{
	assert(this->IsWindow());
	return ::GetDC(*this);
}

/**
 *	@brief The GetDCEx function retrieves a handle to a device context (DC) for the client area of a specified window or for the entire screen.
 *		\n You can use the returned handle in subsequent GDI functions to draw in the DC.
 *		\n The device context is an opaque data structure, whose values are used internally by GDI.
 *	@param[in] hrgnClip	A clipping region that may be combined with the visible region of the DC.
 *	@param[in] dwFlags	Specifies how the DC is created.
 *	@return <b>型別: HDC</b>
 *		\n If the function succeeds, the return value is the handle to the DC for the specified window.
 *		\n If the function fails, the return value is NULL. An invalid value for the hWnd parameter will cause the function to fail.
 */
HDC DmWnds::GetDCEx(HRGN hrgnClip, DWORD dwFlags) const
{
	assert(this->IsWindow());
	return ::GetDCEx(*this, hrgnClip, dwFlags);
}

/**
 *	@brief 取得桌面視窗操作代碼(handle of window)
 *	@return <b>型別: HWND</b> \n 返回值為桌面視窗操作代碼(handle of window)
 */
HWND DmWnds::GetDesktopWindow() const
{
	return ::GetDesktopWindow();
}

/**
 *	@brief 取得此控制項ID (identifier)
 *	@return <b>型別: int</b>
 *		\n 若運作成功則返回值為非零值，值為控制項ID (identifier)
 *		\n 若運作失敗則返回值為零，失敗時調用 GetLastError() 取得錯誤訊息
 */
int DmWnds::GetDlgCtrlID() const
{
	assert(this->IsWindow());
	return ::GetDlgCtrlID(*this);
}

/**
 *	@brief 取得控制項操作代碼 (handle of window)
 *	@param[in] nIDDlgItem 要取得控制項操作代碼的控制項 ID (identifier)
 *	@return <b>型別: HWND</b>
 *		\n 若運作成功則返回值為非零值，值為控制項操作代碼(handle of window)
 *		\n 若運作失敗則返回值為零, 表示無效的對話框(Dialog) 或無效的控制項ID。
 *		\n 若運作失敗可調用 GetLastError 取得錯誤訊息。
 */
HWND DmWnds::GetDlgItem(int nIDDlgItem) const
{
	assert(this->IsWindow());
	return ::GetDlgItem(*this, nIDDlgItem);
}

/**
 *	@brief 將指定控制項的文字內容轉化為整數數值
 *	@param[in]		nIDDlgItem		要進行轉化的控制項識別碼
 *	@param[in,out]	bTranslatedPtr	指向 BOOL 型態的資料位址，用來接收運作狀態（TRUE表示成功，FALSE表示失敗），若此參數為 NULL，則不會返回有關成功或失敗的信息。
 *	@param[in]		bSigned			指定轉化值帶正負號，值為 TRUE 使用正負號，值為 FALSE 不使用正負號。
 *	@return <b>型別: UINT</b>
 *		\n 如果成功，則 bTranslatedPtr 指向的變數值將設被為 TRUE，返回值是控制項文字內容轉換值。
 *		\n 如果失敗，則 bTranslatedPtr 指向的變數值將設被為 FALSE，返回值為零。注意！因為零是可能的轉換值，返回值零並不一定表示失敗。
 *		\n 如果 bTranslatedPtr 為 NULL，則函數將不返回有關成功或失敗的訊息。
 *		\n 注意！如果 bSigned 參數為 TRUE 且文本開頭有減號（ - ），則 GetDlgItemInt 會將文本轉換為有符號整數值。否則將建立無符號整數值。要在這種情況下獲取正確的值，請將返回值強制轉換為 int 類型。
 *		\n 若運作失敗，請調用 GetLastError 取得錯誤訊息。
 */
UINT DmWnds::GetDlgItemInt(int nIDDlgItem, BOOL* bTranslatedPtr, BOOL bSigned) const
{
	assert(this->IsWindow());
	return ::GetDlgItemInt(*this, nIDDlgItem, bTranslatedPtr, bSigned);
}

/**
 *	@brief 將指定控制項的文字內容轉化為整數數值
 *	@param[in] nIDDlgItem	要進行轉化的控制項識別碼
 *	@param[in] bSigned		指定轉化值帶正負號，值為 TRUE 使用正負號，值為 FALSE 不使用正負號。
 *	@return <b>型別: UINT</b>
 *		\n 若運作成功返回值為非零值，值為轉化後數值。
 *		\n 若運作失敗返回值將為零，不過！返回值為零不代表就是失敗，因為有可能轉化值就是零。
 *	@see DmWnds::GetDlgItemInt
 */
UINT DmWnds::GetDlgItemInt(int nIDDlgItem, BOOL bSigned) const
{
	assert(this->IsWindow());
	// 不接收成功或失敗運作狀態
	return ::GetDlgItemInt(*this, nIDDlgItem, NULL, bSigned);
}

/**
 *	@brief Retrieves the title or text associated with a control in a dialog box.
 *	@param[in]	nIDDlgItem	The identifier of the control whose title or text is to be retrieved.
 *	@param[out] szStringPtr	The buffer to receive the title or text.
 *	@param[in]	cchMax		The maximum length, in characters, of the string to be copied to the buffer pointed to by szStringPtr.
 *							If the length of the string, including the null character, exceeds the limit, the string is truncated.
 *	@return <b>型別: UINT</b>
 *		\n If the function succeeds, the return value specifies the number of characters copied to the buffer, not including the terminating null character.
 *		\n If the function fails, the return value is zero. To get extended error information, call GetLastError.
 */
UINT DmWnds::GetDlgItemText(int nIDDlgItem, TCHAR* szStringPtr, int cchMax) const
{
	assert(this->IsWindow());
	return ::GetDlgItemText(*this, nIDDlgItem, szStringPtr, cchMax);
}

/**
 *	@brief 取得視窗延伸擴展風格.
 *	@return <b>型別: DWORD</b>
 *		\n 若運作成功則返回值為非零值, 值為視窗延伸擴展風格。
 *		\n 若運作失敗則返回值為零, 運作失敗可調用 GetLastError() 取得錯誤碼。
 */
DWORD DmWnds::GetExStyle() const
{
	assert(this->IsWindow());
	::SetLastError(ERROR_SUCCESS);
	return static_cast<DWORD>(::GetWindowLongPtr(*this, GWL_EXSTYLE));
}

/**
 *	@brief Retrieves the handle to the window that has the keyboard focus, if the window is attached to the calling thread's message queue.
 *	@return <b>型別: HWND</b>
 *		\n The return value is the handle to the window with the keyboard focus.
 *		\n If the calling thread's message queue does not have an associated window with the keyboard focus, the return value is NULL.
 */
HWND DmWnds::GetFocus() const { return ::GetFocus(); }

/**
 *	@brief 取得當前視窗或控制項使用的字型
 *	@return <b>型別: HFONT</b>
 *		\n 若返回值為正在使用中的字型操作代碼(handle)。
 *		\n 若返回值為 NULL 代表該視窗或控制項正在使用系統預設字型。
 */
HFONT DmWnds::GetFont() const
{
	assert(this->IsWindow());
	// WM_GETFONT
	// wParam = 未使用，必須為零
	// lParam = 未使用，必須為零
	return reinterpret_cast<HFONT>(::SendMessage(*this, WM_GETFONT, 0, 0));
}

/**
 *	@brief Retrieves a handle to the foreground window (the window with which the user is currently working).
 *		\n The system assigns a slightly higher priority to the thread that creates the foreground window than it does to other threads.
 *	@return <b>型別: HWND</b>
 *		\n The return value is a handle to the foreground window. \n
 *		\n The foreground window can be NULL in certain circumstances, such as when a window is losing activation.
 */
HWND DmWnds::GetForegroundWindow() const { return ::GetForegroundWindow(); }

/**
 *	@brief Sent to a window to retrieve a handle to the large or small icon associated with a window.
 *	@param[in] bBigIcon	The type of icon being retrieved
 *		- ICON_BIG		\n value=1, Retrieve the large icon for the window.
 *		- ICON_SMALL	\n value=0, Retrieve the small icon for the window.
 *		- ICON_SMALL2	\n value=2, Retrieves the small icon provided by the application. If the application does not provide one, the system uses the system-generated icon for that window.
 *	@return <b>型別: HICON</b>
 *		\n The return value is a handle to the large or small icon, depending on the value of wParam.
 *		\n When an application receives this message, it can return a handle to a large or small icon, or pass the message to the DefWindowProc function.
 */
HICON DmWnds::GetIcon(BOOL bBigIcon) const
{
	assert(this->IsWindow());
	// WM_GETICON message
	// wParam = The type of icon being retrieved
	// lParam = The DPI of the icon being retrieved. This can be used to provide different icons depending on the icon size.
	return reinterpret_cast<HICON>(::SendMessage(*this, WM_GETICON, static_cast<WPARAM>(bBigIcon), static_cast<LPARAM>(0)));
}

/**
 *	@brief Determines which pop-up window owned by the specified window was most recently active.
 *	@return <b>型別: HWND</b>
 *		\n The return value identifies the most recently active pop-up window.
 *		\n The return value is the same as the hWnd parameter, if any of the following conditions are met:
 *		- The window identified by hWnd was most recently active.
 *		- The window identified by hWnd does not own any pop-up windows.
 *		- The window identifies by hWnd is not a top-level window, or it is owned by another window.
 */
HWND DmWnds::GetLastActivePopup() const
{
	assert(this->IsWindow());
	return ::GetLastActivePopup(*this);
}

/**
 *	@brief Retrieves a handle to the menu assigned to the specified window.
 *	@return <b>型別: HMENU</b>
 *		\n The return value is a handle to the menu.
 *		\n If the specified window has no menu, the return value is NULL.
 *		\n If the window is a child window, the return value is undefined.
 */
HMENU DmWnds::GetMenu() const
{
	assert(this->IsWindow());
	return ::GetMenu(*this);
}

/**
 *	@brief Retrieves a module handle for the specified module. The module must have been loaded by the calling process.
 *	@return <b>型別: HMODULE</b>
 *		\n If the function succeeds, the return value is a handle to the specified module.
 *		\n If the function fails, the return value is NULL. To get extended error information, call GetLastError.
 */
HMODULE DmWnds::GetModuleHandle() const { return ::GetModuleHandle(NULL); }

/**
 *	@brief Retrieves a handle to the first control in a group of controls that precedes (or follows) the specified control in a dialog box.
 *	@param[in] hCtrl		A handle to the control to be used as the starting point for the search. If this parameter is NULL, the function uses the last (or first) control in the dialog box as the starting point for the search.
 *	@param[in] bPrevious	Indicates how the function is to search the group of controls in the dialog box. If this parameter is TRUE, the function searches for the previous control in the group. If it is FALSE, the function searches for the next control in the group.
 *	@return <b>型別: HWND</b>
 *		\n If the function succeeds, the return value is a handle to the previous (or next) control in the group of controls.
 *		\n If the function fails, the return value is NULL. To get extended error information, call GetLastError.
 */
HWND DmWnds::GetNextDlgGroupItem(HWND hCtrl, BOOL bPrevious) const
{
	assert(this->IsWindow());
	return ::GetNextDlgGroupItem(*this, hCtrl, bPrevious);
}

/**
 *	@brief Retrieves a handle to the first control that has the WS_TABSTOP style that precedes (or follows) the specified control.
 *	@param[in] hCtrl A handle to the control to be used as the starting point for the search.
 *		\n If this parameter is NULL, the function fails.
 *	@param[in] bPrevious Indicates how the function is to search the dialog box.
 *		\n If this parameter is TRUE, the function searches for the previous control in the dialog box.
 *		\n If this parameter is FALSE, the function searches for the next control in the dialog box.
 *	@return <b>型別: HWND</b>
 *		\n If the function succeeds, the return value is the window handle of the previous (or next) control that has the WS_TABSTOP style set.
 *		\n If the function fails, the return value is NULL. To get extended error information, call GetLastError.
 */
HWND DmWnds::GetNextDlgTabItem(HWND hCtrl, BOOL bPrevious) const
{
	assert(this->IsWindow());
	return ::GetNextDlgTabItem(*this, hCtrl, bPrevious);
}

/**
 *	@brief Retrieves a handle to the specified window's parent or owner.
 *		\n Retrieves a handle to the specified window's parent or owner.
 *		\n To retrieve a handle to a specified ancestor, use the GetAncestor function.
 *	@return <b>型別: HWND</b>
 *		\n If the window is a child window, the return value is a handle to the parent window.
 *		\n If the window is a top-level window with the WS_POPUP style, the return value is a handle to the owner window.
 *		\n If the function fails, the return value is NULL. To get extended error information, call GetLastError.
 */
HWND DmWnds::GetParent() const
{
	assert(this->IsWindow());
	return ::GetParent(*this);
}

/**
 *	@brief Retrieves information about the specified scroll bar.
 *	@param[in] idObject Specifies the scroll bar object.
 *		- OBJID_CLIENT	\n The hwnd parameter is a handle to a scroll bar control.
 *		- OBJID_HSCROLL	\n The horizontal scroll bar of the hwnd window.
 *		- OBJID_VSCROLL	\n The vertical scroll bar of the hwnd window.
 *	@param[in,out] sbiPtr Pointer to a SCROLLBARINFO structure to receive the information.
 *		\n Before calling GetScrollBarInfo, set the cbSize member to sizeof(SCROLLBARINFO).
 *	@return <b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero. To get extended error information, call GetLastError.
 */
BOOL DmWnds::GetScrollBarInfo(LONG idObject, SCROLLBARINFO* sbiPtr) const
{
	assert(this->IsWindow());
	return ::GetScrollBarInfo(*this, idObject, sbiPtr);
}

/**
 *	@brief The GetScrollInfo function retrieves the parameters of a scroll bar, including the minimum and maximum scrolling positions, the page size, and the position of the scroll box (thumb).
 *	@param[in] nBar Specifies the type of scroll bar for which to retrieve parameters.
 *		- SB_CTL	\n Sets the parameters of a scroll bar control. The hwnd parameter must be the handle to the scroll bar control.
 *		- SB_HORZ	\n Sets the parameters of the window's standard horizontal scroll bar.
 *		- SB_VERT	\n Sets the parameters of the window's standard vertical scroll bar.
 *	@param[in,out] siPtr Pointer to a SCROLLINFO structure. Before calling GetScrollInfo.
 *	@return <b>型別: BOOL</b>
 *		\n If the function retrieved any values, the return value is nonzero.
 *		\n If the function does not retrieve any values, the return value is zero. To get extended error information, call GetLastError.
 */
BOOL DmWnds::GetScrollInfo(int nBar, SCROLLINFO* siPtr) const
{
	assert(this->IsWindow());
	return ::GetScrollInfo(*this, nBar, siPtr);
}

/**
 *	@brief The GetScrollPos function retrieves the current position of the scroll box (thumb) in the specified scroll bar.
 *		\n The current position is a relative value that depends on the current scrolling range.
 *		\n For example, if the scrolling range is 0 through 100 and the scroll box is in the middle of the bar, the current position is 50.
 *	@param[in] nBar Specifies the scroll bar to be examined.
 *	@return <b>型別: int</b>
 *		\n If the function succeeds, the return value is the current position of the scroll box.
 *		\n If the function fails, the return value is zero. To get extended error information, call GetLastError.
 *	@see DmWnds::GetScrollInfo
 */
int DmWnds::GetScrollPos(int nBar) const
{
	assert(this->IsWindow());
	return ::GetScrollPos(*this, nBar);
}

/**
 *	@brief The GetScrollRange function retrieves the current minimum and maximum scroll box (thumb) positions for the specified scroll bar.
 *	@param[in]	nBar		Specifies the scroll bar from which the positions are retrieved.
 *	@param[out]	nMinPosPtr	Pointer to the integer variable that receives the minimum position.
 *	@param[out]	nMaxPosPtr	Pointer to the integer variable that receives the maximum position.
 *	@return <b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero. To get extended error information, call GetLastError.
 */
BOOL DmWnds::GetScrollRange(int nBar, int* nMinPosPtr, int* nMaxPosPtr) const
{
	assert(this->IsWindow());
	return ::GetScrollRange(*this, nBar, nMinPosPtr, nMaxPosPtr);
}

/**
 *	@brief Retrieves the window styles.
 *	@return <b>型別: DWORD</b>
 *		\n If the function succeeds, the return value is the window style value.
 *		\n If the function fails, the return value is zero. To get extended error information, call GetLastError.
 */
DWORD DmWnds::GetStyle() const
{
	assert(this->IsWindow());
	::SetLastError(ERROR_SUCCESS);
	return static_cast<DWORD>(::GetWindowLongPtr(*this, GWL_STYLE));
}

/**
 *	@brief Enables the application to access the window menu (also known as the system menu or the control menu) for copying and modifying.
 *	@param[in] bRevert The action to be taken. If this parameter is FALSE, GetSystemMenu returns a handle to the copy of the window menu currently in use.
 *	@return <b>型別: HMENU</b>
 *		\n If the bRevert parameter is FALSE, the return value is a handle to a copy of the window menu.
 *		\n If the bRevert parameter is TRUE, the return value is NULL.
 */
HMENU DmWnds::GetSystemMenu(BOOL bRevert) const
{
	assert(this->IsWindow());
	return ::GetSystemMenu(*this, bRevert);
}

/**
 *	@brief Examines the Z order of the child windows associated with the specified parent window and retrieves a handle to the child window at the top of the Z order.
 *	@return <b>型別: HWND</b>
 *		\n If the function succeeds, the return value is a handle to the child window at the top of the Z order.
 *		\n If the specified window has no child windows, the return value is NULL.
 *		\n To get extended error information, use the GetLastError function.
 */
HWND DmWnds::GetTopWindow() const
{
	assert(this->IsWindow());
	return ::GetTopWindow(*this);
}

/**
 *	@brief The GetUpdateRect function retrieves the coordinates of the smallest rectangle that completely encloses the update region of the specified window.
 *		\n GetUpdateRect retrieves the rectangle in logical coordinates.
 *		\n If there is no update region, GetUpdateRect retrieves an empty rectangle (sets all coordinates to zero).
 *	@param[out]	rcPtr	Pointer to the RECT structure that receives the coordinates, in device units, of the enclosing rectangle.
 *	@param[in]	bErase	Specifies whether the background in the update region is to be erased.
 *	@return <b>型別: BOOL</b> 若運作成功返回值為非零值。 \n 若運作失敗返回值為零。
 */
BOOL DmWnds::GetUpdateRect(RECT* rcPtr, BOOL bErase) const
{
	assert(this->IsWindow());
	return ::GetUpdateRect(*this, rcPtr, bErase);
}

/**
 *	@brief The GetUpdateRgn function retrieves the update region of a window by copying it into the specified region.
 *		\n The coordinates of the update region are relative to the upper-left corner of the window (that is, they are client coordinates).
 *	@param[in] hRgn		Handle to the region to receive the update region.
 *	@param[in] bErase	Specifies whether the window background should be erased and whether nonclient areas of child windows should be drawn.
 *		\n If this parameter is FALSE, no drawing is done.
 *	@return <b>TypeL int</b> \n The return value indicates the complexity of the resulting region.
 *		- COMPLEXREGION	\n Region consists of more than one rectangle.
 *		- ERROR			\n An error occurred.
 *		- NULLREGION	\n Region is empty.
 *		- SIMPLEREGION	\n Region is a single rectangle.
 */
int DmWnds::GetUpdateRgn(HRGN hRgn, BOOL bErase) const
{
	assert(this->IsWindow());
	return ::GetUpdateRgn(*this, hRgn, bErase);
}

/**
 *	@brief Retrieves a handle to a window that has the specified relationship (Z-Order or owner) to the specified window.
 *	@param[in] uCmd The relationship between the specified window and the window whose handle is to be retrieved.
 *		- GW_CHILD			\n 值 = 5, The retrieved handle identifies the child window at the top of the Z order, if the specified window is a parent window; otherwise, the retrieved handle is NULL. The function examines only child windows of the specified window. It does not examine descendant windows.
 *		- GW_ENABLEDPOPUP	\n 值 = 6, The retrieved handle identifies the enabled popup window owned by the specified window (the search uses the first such window found using GW_HWNDNEXT); otherwise, if there are no enabled popup windows, the retrieved handle is that of the specified window.
 *		- GW_HWNDFIRST		\n 值 = 0, The retrieved handle identifies the window of the same type that is highest in the Z order. If the specified window is a topmost window, the handle identifies a topmost window. If the specified window is a top-level window, the handle identifies a top-level window. If the specified window is a child window, the handle identifies a sibling window.
 *		- GW_HWNDLAST		\n 值 = 1, The retrieved handle identifies the window of the same type that is lowest in the Z order. If the specified window is a topmost window, the handle identifies a topmost window. If the specified window is a top-level window, the handle identifies a top-level window. If the specified window is a child window, the handle identifies a sibling window.
 *		- GW_HWNDNEXT		\n 值 = 2, The retrieved handle identifies the window below the specified window in the Z order. If the specified window is a topmost window, the handle identifies a topmost window. If the specified window is a top-level window, the handle identifies a top-level window. If the specified window is a child window, the handle identifies a sibling window.
 *		- GW_HWNDPREV		\n 值 = 3, The retrieved handle identifies the window above the specified window in the Z order. If the specified window is a topmost window, the handle identifies a topmost window. If the specified window is a top-level window, the handle identifies a top-level window. If the specified window is a child window, the handle identifies a sibling window.
 *		- GW_OWNER			\n 值 = 4, The retrieved handle identifies the specified window's owner window, if any. For more information, see Owned Windows.
 *	@return <b>型別: HWND</b>
 *		\n If the function succeeds, the return value is a window handle.
 *		\n If no window exists with the specified relationship to the specified window, the return value is NULL.
 *		\n To get extended error information, call GetLastError.
 */
HWND DmWnds::GetWindow(UINT uCmd) const
{
	assert(this->IsWindow());
	return ::GetWindow(*this, uCmd);
}

/**
 *	@brief The GetWindowDC function retrieves the device context (DC) for the entire window, including title bar, menus, and scroll bars.
 *		\n A window device context permits painting anywhere in a window, because the origin of the device context is the upper-left corner of the window instead of the client area.
 *	@return <b>型別: HDC</b>
 *		\n If the function succeeds, the return value is a handle to a device context for the specified window.
 *		\n If the function fails, the return value is NULL, indicating an error or an invalid hWnd parameter.
 */
HDC DmWnds::GetWindowDC() const
{
	assert(this->IsWindow());
	return ::GetWindowDC(*this);
}

/**
 *	@brief Retrieves information about the specified window.
 *		\n The function also retrieves the value at a specified offset into the extra window memory.
 *	@param[in]	nIndex The zero-based offset to the value to be retrieved.
 *		\n Valid values are in the range zero through the number of bytes of extra window memory, minus the size of a LONG_PTR.
 *		- GWL_EXSTYLE		\n 值 = -20	\n Retrieves the extended window styles.
 *		- GWLP_HINSTANCE	\n 值 = -6	\n Retrieves a handle to the application instance.
 *		- GWLP_HWNDPARENT	\n 值 = -8	\n Retrieves a handle to the parent window, if there is one.
 *		- GWLP_ID			\n 值 = -12	\n Retrieves the identifier of the window.
 *		- GWL_STYLE			\n 值 = -16	\n Retrieves the window styles.
 *		- GWLP_USERDATA		\n 值 = -21	\n Retrieves the user data associated with the window. This data is intended for use by the application that created the window. Its value is initially zero.
 *		- GWLP_WNDPROC		\n 值 = -4	\n Retrieves the pointer to the window procedure, or a handle representing the pointer to the window procedure. You must use the CallWindowProc function to call the window procedure.
 *	@return <b>型別: LONG_PTR</b>
 *		\n If the function succeeds, the return value is the requested value.
 *		\n If the function fails, the return value is zero. To get extended error information, call GetLastError.
 */
LONG_PTR DmWnds::GetWindowLongPtr(int nIndex) const
{
	assert(this->IsWindow());
	return ::GetWindowLongPtr(*this, nIndex);
}

/**
 *	@brief Retrieves information about the specified window.
 *	@param[in,out] wiPtr A pointer to a WINDOWINFO structure to receive the information.
 *		\n Note that you must set the cbSize member to sizeof(WINDOWINFO) before calling this function.
 *	@return <b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero. To get extended error information, call GetLastError.
 */
BOOL DmWnds::GetWindowInfo(WINDOWINFO* wiPtr) const
{
	assert(this->IsWindow());
	return ::GetWindowInfo(*this, wiPtr);
}

/**
 *	@brief Retrieves the show state and the restored, minimized, and maximized positions of the specified window.
 *	@param[in,out] wpmPtr A pointer to the WINDOWPLACEMENT structure that receives the show state and position information.
 *		\n Before calling GetWindowPlacement, set the length member to sizeof(WINDOWPLACEMENT).
 *		\n GetWindowPlacement fails if lpwndpl-> length is not set correctly.
 *	@return <b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero. To get extended error information, call GetLastError.
 */
BOOL DmWnds::GetWindowPlacement(WINDOWPLACEMENT* wpmPtr) const
{
	assert(this->IsWindow());
	return ::GetWindowPlacement(*this, wpmPtr);
}

/**
 *	@brief 取得視窗邊界座標資訊 (左上座標及右下座標)
 *	@param[out] rcPtr 指向結構 RECT 緩衝區位址，用來接收視窗邊界座標資訊。
 *	@return <b>型別: BOOL</b>
 *		\n 若運作成功返回值為非零值。
 *		\n 若運作失敗則返回值為零。可使用 GetLastError 取得運作失敗原因。
 */
BOOL DmWnds::GetWindowRect(RECT* rcPtr) const
{
	assert(this->IsWindow());
	return ::GetWindowRect(*this, rcPtr);
}

/**
 *	@brief The GetWindowRgn function obtains a copy of the window region of a window.
 *	@param[in] hRgn Handle to the region which will be modified to represent the window region.
 *	@return <b>型別: int</b> \n The return value specifies the type of the region that the function obtains. It can be one of the following values.
 *		- NULLREGION	\n The region is empty.
 *		- SIMPLEREGION	\n The region is a single rectangle.
 *		- COMPLEXREGION	\n The region is more than one rectangle.
 *		- ERROR			\n The specified window does not have a region, or an error occurred while attempting to return the region.
 */
int DmWnds::GetWindowRgn(HRGN hRgn) const
{
	assert(this->IsWindow());
	return ::GetWindowRgn(*this, hRgn);
}

/**
 *	@brief Copies the text of the specified window's title bar (if it has one) into a buffer.
 *	@param[in] textPtr The buffer that will receive the text. If the string is as long or longer than the buffer, the string is truncated and terminated with a null character.
 *	@param[in] nMaxCount The maximum number of characters to copy to the buffer, including the null character. If the text exceeds this limit, it is truncated.
 *	@return <b>型別: int</b>
 *		\n If the function succeeds, the return value is the length, in characters, of the copied string, not including the terminating null character.
 *		\n If the window has no title bar or text, if the title bar is empty, or if the window or control handle is invalid, the return value is zero. To get extended error information, call GetLastError.
 */
int DmWnds::GetWindowText(TCHAR* textPtr, int nMaxCount) const
{
	assert(this->IsWindow());
	return ::GetWindowText(*this, static_cast<LPTSTR>(textPtr), static_cast<int>(nMaxCount));
}

/**
 *	@brief Retrieves the length, in characters, of the specified window's title bar text (if the window has a title bar).
 *	@return <b>型別: int</b>
 *		\n If the function succeeds, the return value is the length, in characters, of the text. Under certain conditions, this value may actually be greater than the length of the text. For more information, see the following Remarks section.
 *		\n If the window has no text, the return value is zero. To get extended error information, call GetLastError.
 */
int DmWnds::GetWindowTextLength() const
{
	assert(this->IsWindow());
	return ::GetWindowTextLength(*this);
}

/**
 *	@brief Removes the caret from the screen. Hiding a caret does not destroy its current shape or invalidate the insertion point.
 *	@return <b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero. To get extended error information, call GetLastError.
 */
BOOL DmWnds::HideCaret() const
{
	assert(this->IsWindow());
	return ::HideCaret(*this);
}

/**
 *	@brief Sets the specified window's show state.
 *	@return <b>型別: BOOL</b>
 *		\n If the window was previously visible, the return value is nonzero.
 *		\n If the window was previously hidden, the return value is zero.
 *	@see DmWnds::ShowWindow
 */
BOOL DmWnds::HideWindow() const
{
	assert(this->IsWindow());
	return ::ShowWindow(*this, SW_HIDE);
}

/**
 *	@brief Adds or removes highlighting from an item in a menu bar.
 *	@param[in] hMenu			A handle to the menu bar that contains the item.
 *	@param[in] uIDHiliteItem	The menu item. This parameter is either the identifier of the menu item or the offset of the menu item in the menu bar, depending on the value of the uHilite parameter.
 *	@param[in] uHilite			Controls the interpretation of the uItemHilite parameter and indicates whether the menu item is highlighted. This parameter must be a combination of either MF_BYCOMMAND or MF_BYPOSITION and MF_HILITE or MF_UNHILITE.
 *	@return <b>型別: BOOL</b>
 *		\n If the menu item is set to the specified highlight state, the return value is nonzero.
 *		\n If the menu item is not set to the specified highlight state, the return value is zero.
 */
BOOL DmWnds::HiliteMenuItem(HMENU hMenu, UINT uIDHiliteItem, UINT uHilite) const
{
	assert(this->IsWindow());
	return ::HiliteMenuItem(*this, hMenu, uIDHiliteItem, uHilite);
}

/**
 *	@brief The update region represents the portion of the window's client area that must be redrawn.
 *	@param[in] bErase pecifies whether the background within the update region is to be erased when the update region is processed.
 *	@return 此函數沒有返回值
 */
void DmWnds::Invalidate(BOOL bErase) const
{
	assert(this->IsWindow());
	::InvalidateRect(*this, NULL, bErase);
}

/**
 *	@brief The InvalidateRect function adds a rectangle to the specified window's update region.
 *		\n The update region represents the portion of the window's client area that must be redrawn.
 *	@param[in] rectPtr	A pointer to a RECT structure that contains the client coordinates of the rectangle to be added to the update region.
 *		\n If this parameter is NULL, the entire client area is added to the update region.
 *	@param[in] bErase	Specifies whether the background within the update region is to be erased when the update region is processed.
 *		\n If this parameter is TRUE, the background is erased when the BeginPaint function is called.
 *		\n If this parameter is FALSE, the background remains unchanged.
 *	@return <b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero.
 */
BOOL DmWnds::InvalidateRect(const RECT* rectPtr, BOOL bErase) const
{
	assert(this->IsWindow());
	return ::InvalidateRect(*this, rectPtr, bErase);
}

/**
 *	@brief The InvalidateRect function adds a rectangle to the specified window's update region.
 *		\n The update region represents the portion of the window's client area that must be redrawn.
 *	@param[in] bErase	Specifies whether the background within the update region is to be erased when the update region is processed.
 *		\n If this parameter is TRUE, the background is erased when the BeginPaint function is called.
 *		\n If this parameter is FALSE, the background remains unchanged.
 *	@return <b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero.
 */
BOOL DmWnds::InvalidateRect(BOOL bErase) const
{
	assert(this->IsWindow());
	return ::InvalidateRect(*this, NULL, bErase);
}

/**
 *	@brief The InvalidateRgn function invalidates the client area within the specified region by adding it to the current update region of a window.
 *		\n The invalidated region, along with all other areas in the update region, is marked for painting when the next WM_PAINT message occurs.
 *	@param[in] hRgn	A handle to the region to be added to the update region. The region is assumed to have client coordinates.
 *		\n If this parameter is NULL, the entire client area is added to the update region.
 *	@param[in] bErase Specifies whether the background within the update region is to be erased when the update region is processed.
 *	@return <b>型別: BOOL</b> \n The return value is always nonzero.
 *	@see DmWnds::InvalidateRect
 */
BOOL DmWnds::InvalidateRgn(HRGN hRgn, BOOL bErase) const
{
	assert(this->IsWindow());
	return ::InvalidateRgn(*this, hRgn, bErase);
}

/**
 *	@brief Determines whether a window is a child window or descendant window of a specified parent window.
 *		\n A child window is the direct descendant of a specified parent window if that parent window is in the chain of parent windows,
 *		the chain of parent windows leads from the original overlapped or pop-up window to the child window.
 *	@return <b>型別: BOOL</b>
 *		\n If the window is a child or descendant window of the specified parent window, the return value is nonzero.
 *		\n If the window is not a child or descendant window of the specified parent window, the return value is zero.
 */
BOOL DmWnds::IsChild() const
{
	assert(this->IsWindow());
	return ::IsChild(::GetParent(*this), *this);
}

/**
 *	@brief Determines whether a message is intended for the specified dialog box and, if it is, processes the message.
 *	@param[in] MsgPtr A pointer to an MSG structure that contains the message to be checked.
 *	@return <b>型別: BOOL</b>
 *		\n If the message has been processed, the return value is nonzero.
 *		\n If the message has not been processed, the return value is zero.
 */
BOOL DmWnds::IsDialogMessage(MSG* MsgPtr) const
{
	assert(this->IsWindow());
	return ::IsDialogMessage(*this, MsgPtr);
}

/**
 *	@brief The IsDlgButtonChecked function determines whether a button control is checked or whether a three-state button control is checked, unchecked, or indeterminate.
 *	@param[in] nIDButton The identifier of the button control.
 *	@return <b>型別: UINT</b>
 *		\n The return value from a button created with the BS_AUTOCHECKBOX, BS_AUTORADIOBUTTON,
 *		BS_AUTO3STATE, BS_CHECKBOX, BS_RADIOBUTTON, or BS_3STATE styles can be one of the values in the following table.
 *		\n If the button has any other style, the return value is zero.
 */
UINT DmWnds::IsDlgButtonChecked(int nIDButton) const
{
	assert(this->IsWindow());
	return ::IsDlgButtonChecked(*this, nIDButton);
}

/**
 *	@brief Determines whether the specified window is minimized (iconic).
 *	@return <b>型別: BOOL</b>
 *		\n If the window is iconic, the return value is nonzero.
 *		\n If the window is not iconic, the return value is zero.
 */
BOOL DmWnds::IsIconic() const
{
	assert(this->IsWindow());
	return ::IsIconic(*this);
}

/**
 *	@brief Checks whether a specified window is touch-capable and, optionally,
 *		\n retrieves the modifier flags set for the window's touch capability.
 *	@param[out] uFlagsPtr The address of the ULONG variable to receive the modifier flags for the specified window's touch capability.
 *	@return <b>型別: BOOL</b>
 *		\n Returns @c TRUE if the window supports Windows Touch.
 *		\n Returns @c FALSE if the window does not support Windows Touch.
 */
BOOL DmWnds::IsTouchWindow(ULONG * uFlagsPtr) const
{
	assert(this->IsWindow());
	return ::IsTouchWindow(*this, uFlagsPtr);
}

/**
 *	@brief Determines whether the specified window is enabled for mouse and keyboard input.
 *	@return <b>型別: BOOL</b>
 *		\n If the window is enabled, the return value is nonzero.
 *		\n If the window is not enabled, the return value is zero.
 */
BOOL DmWnds::IsWindowEnabled() const
{
	assert(this->IsWindow());
	return ::IsWindowEnabled(*this);
}

/**
 *	@brief Determines the visibility state of the specified window.
 *	@return <b>型別: BOOL</b>
 *		\n If the specified window, its parent window, its parent's parent window, and so forth, have the WS_VISIBLE style,
 *		the return value is nonzero. Otherwise, the return value is zero.
 *		\n Because the return value specifies whether the window has the WS_VISIBLE style,
 *		it may be nonzero even if the window is totally obscured by other windows.
 */
BOOL DmWnds::IsWindowVisible() const
{
	assert(this->IsWindow());
	return ::IsWindowVisible(*this);
}

/**
 *	@brief 視窗是否處於最大化
 *	@return <b>型別: BOOL</b> \n 如果視窗為最大化狀態，返回值為非零值。 \n 如果視窗非最大化狀態，則返回值為零。
 */
BOOL DmWnds::IsZoomed() const
{
	assert(this->IsWindow());
	return ::IsZoomed(*this);
}

/**
 *	@brief 銷毀指定的計時器 (Timer)
 *	@param[in] uIDEvent 指定要被銷毀的計時器識別碼 (ID)
 *	@return <b>型別: BOOL</b>
 *		\n 如果運作成功，返回值為非零值。
 *		\n 如果運作失敗，返回值為零。可使用 GetLastError 取得錯誤的碼 (錯誤資訊)
 */
BOOL DmWnds::KillTimer(UINT_PTR uIDEvent) const
{
	assert(this->IsWindow());
	return ::KillTimer(*this, uIDEvent);
}

/**
 *	@brief The LockWindowUpdate function disables or enables drawing in the specified window. Only one window can be locked at a time.
 *	@return <b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero, indicating that an error occurred or another window was already locked.
 */
BOOL DmWnds::LockWindowUpdate() const
{
	assert(this->IsWindow());
	return ::LockWindowUpdate(*this);
}

/**
 *	@brief The MapWindowPoints function converts (maps) a set of points from a coordinate space relative to one window to a coordinate space relative to another window.
 *	@param[in]	hWndTo	A handle to the window to which points are converted. If this parameter is NULL or HWND_DESKTOP, the points are converted to screen coordinates.
 *	@param[out]	ptPtr	A pointer to an array of POINT structures that contain the set of points to be converted.
 *		\n The points are in device units. This parameter can also point to a RECT structure, in which case the cPoints parameter should be set to 2.
 *	@param[in] uPoints	The number of POINT structures in the array pointed to by the lpPoints parameter.
 *	@return 此函數沒有返回值
 */
void DmWnds::MapWindowPoints(HWND hWndTo, POINT* ptPtr, UINT uPoints) const
{
	assert(this->IsWindow());
	::MapWindowPoints(*this, hWndTo, ptPtr, uPoints);
}

/**
 *	@brief MapWindowPoints function converts (maps) a set of points from a coordinate space relative to one window to a coordinate space relative to another window.
 *	@param[in]	hWndTo	A handle to the window to which points are converted.
 *	@param[out]	ptPtr	A pointer to an array of POINT structures that contain the set of points to be converted.
 *	@return 此函數沒有返回值
 */
void DmWnds::MapWindowPoints(HWND hWndTo, POINT* ptPtr) const
{
	assert(this->IsWindow());
	::MapWindowPoints(*this, hWndTo, ptPtr, 1);
}

/**
 *	@brief MapWindowPoints function converts (maps) a set of points from a coordinate space relative to one window to a coordinate space relative to another window.
 *	@param[in]	hWndTo	A handle to the window to which points are converted.
 *	@param[out]	rcPtr	A pointer to an array of RECT structures that contain the set of points to be converted.
 *	@return 此函數沒有返回值
 */
void DmWnds::MapWindowPoints(HWND hWndTo, RECT* rcPtr) const
{
	assert(this->IsWindow());
	::MapWindowPoints(*this, hWndTo, reinterpret_cast<POINT*>(rcPtr), 2);
}

/**
 *	@brief Displays a modal dialog box that contains a system icon, a set of buttons, and a brief application-specific message, such as status or error information.
 *		\n The message box returns an integer value that indicates which button the user clicked.
 *	@param[in] textPtr		Points to a null-terminated string containing the message to be displayed.
 *	@param[in] szCaptionPtr	Points to a null-terminated string to be used for the message-box caption.  If lpszCaption is NULL, the default caption "Error" is used.
 *	@param[in] uType		Specifies the contents and behavior of the message box.
 *	@return <b>型別: int</b>
 *		\n If a message box has a Cancel button, the function returns the IDCANCEL value if either the ESC key is pressed or the Cancel button is selected. If the message box has no Cancel button, pressing ESC has no effect.
 *		\n If the function fails, the return value is zero. To get extended error information, call GetLastError.
 *		\n If the function succeeds, the return value is one of the following menu-item values.
 */
int DmWnds::MessageBox(const TCHAR* textPtr, const TCHAR* szCaptionPtr, UINT uType) const
{
	assert(this->IsWindow());
	return ::MessageBox(*this, textPtr, szCaptionPtr, uType);
}

/**
 *	@brief 修改視窗或控制項的樣式。
 *	@param[in] dwRemove	要移除的樣式
 *	@param[in] dwAdd	要增加的樣式
 *	@param[in] uFlags	擴展後是否進行調用 SetWindowPos 函數的標記，若此標記為零，代表不調用 SetWindowPos 函數。(此值預設為零)
 *	@return <b>型別: BOOL</b> \n 若樣式修改成功返回值為非零值，否則將返回零。
 *	@see DmWnds::SetStyle
 */
BOOL DmWnds::ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT uFlags) const
{
	BOOL bResult = FALSE;
	assert(this->IsWindow());

	for (;;) {
		DWORD dwStyle = this->GetStyle();
		dwStyle &= ~dwRemove;
		dwStyle |= dwAdd;
		bResult = ::SetWindowLongPtr(*this, GWL_STYLE, dwStyle) != 0;
		if (!bResult) break;
		if (uFlags) {
			RECT rc;
			::GetWindowRect(*this, &rc);
			bResult = this->SetWindowSize(static_cast<int>(rc.right - rc.left), static_cast<int>(rc.bottom - rc.top));
		}
		break;
	}
	return bResult;
}

/**
 *	@brief 修改視窗或控制項的擴展樣式。
 *	@param[in] dwRemove	要移除的擴展樣式
 *	@param[in] dwAdd	要增加的擴展樣式
 *	@param[in] uFlags	擴展後是否進行調用 SetWindowPos 函數的標記，若此標記為零，代表不調用 SetWindowPos 函數。(此值預設為零)
 *	@return <b>型別: BOOL</b> \n 若樣式修改成功返回值為非零值，否則將返回零。
 *	@see DmWnds::SetExStyle
 */
BOOL DmWnds::ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT uFlags) const
{
	BOOL bResult = FALSE;
	assert(this->IsWindow());

	for (;;) {
		DWORD dwStyle = this->GetExStyle();
		dwStyle &= ~dwRemove;
		dwStyle |= dwAdd;
		bResult = ::SetWindowLongPtr(*this, GWL_EXSTYLE, dwStyle) != 0;
		if (!bResult) break;
		if (uFlags) {
			RECT rc;
			::GetWindowRect(*this, &rc);
			bResult = this->SetWindowSize(static_cast<int>(rc.right - rc.left), static_cast<int>(rc.bottom - rc.top));
		}
		break;
	}
	return bResult;
}

/**
 *	@brief Changes the position and dimensions of the specified window.
 *		\n For a top-level window, the position and dimensions are relative to the upper-left corner of the screen.
 *		\n For a child window, they are relative to the upper-left corner of the parent window's client area.
 *	@param[in] x		The new position of the left side of the window.
 *	@param[in] y		The new position of the top of the window.
 *	@param[in] nWidth	The new position of the top of the window.
 *	@param[in] nHeight	The new height of the window.
 *	@param[in] bRepaint	Indicates whether the window is to be repainted.
 *		- TRUE	\n The window receives a message.
 *		- FALSE	\n No repainting of any kind occurs.
 *	@return	<b>型別: BOOL</b> \n If the function succeeds, the return value is nonzero
 */
BOOL DmWnds::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint) const
{
	assert(this->IsWindow());
	return ::MoveWindow(*this, x, y, nWidth, nHeight, bRepaint);
}

/**
 *	@brief Changes the position and dimensions of the specified window.
 *		\n For a top-level window, the position and dimensions are relative to the upper-left corner of the screen.
 *		\n For a child window, they are relative to the upper-left corner of the parent window's client area.
 *	@param[in] rcPtr	Pointer of RECT structure
 *	@param[in] bRepaint	Indicates whether the window is to be repainted.
 *		- If this parameter is TRUE, the window receives a message.
 *		- If the parameter is FALSE, no repainting of any kind occurs.
 *	@return <b>型別: BOOL</b> \n If the function succeeds, the return value is nonzero
 */
BOOL DmWnds::MoveWindow(const RECT* rcPtr, BOOL bRepaint) const
{
	assert(this->IsWindow());
	return ::MoveWindow(*this
		, static_cast<int>(rcPtr->left)
		, static_cast<int>(rcPtr->top)
		, static_cast<int>(rcPtr->right - rcPtr->left)
		, static_cast<int>(rcPtr->bottom - rcPtr->top)
		, bRepaint);
}

/**
 *	@brief Signals the system that a predefined event occurred.
 *		\n If any client applications have registered a hook function for the event, the system calls the client's hook function.
 *	@param[in] dwEvent	Specifies the event that occurred. This value must be one of the event constants.
 *	@param[in] idObject	Identifies the object that generated the event.
 *		\n This value is either one of the predefined object identifiers or a custom object ID value.
 *	@param[in] idChild	Identifies whether the event was generated by an object or by a child element of the object.
 *		\n If this value is CHILDID_SELF, the event was generated by the object itself.
 *		\n If not CHILDID_SELF, this value is the child ID of the element that generated the event.
 *	@return This function does not return a value.
 */
void DmWnds::NotifyWinEvent(DWORD dwEvent, LONG idObject, LONG idChild) const
{
	assert(this->IsWindow());
	::NotifyWinEvent(dwEvent, *this, idObject, idChild);
}

/**
 *	@brief Places (posts) a message in the message queue associated with the thread that created the specified window and returns without waiting for the thread to process the message.
 *	@param[in] uMessage	The message to be sent.
 *	@param[in] wParam	Additional message-specific information.
 *	@param[in] lParam	Additional message-specific information.
 *	@return <b>型別: LRESULT</b> \n If the function succeeds, the return value is nonzero.
 */
BOOL DmWnds::PostMessage(UINT uMessage, WPARAM wParam, LPARAM lParam) const
{
	assert(this->IsWindow());
	return ::PostMessage(*this, uMessage, wParam, lParam);
}

/**
 *	@brief Call this member function to draw the current window in the specified device context, which is most commonly in a printer device context.
 *	@param[in] hDC		A pointer to a device context.
 *	@param[in] dwFlags	Specifies the drawing options.
 *		- PRF_CHECKVISIBLE	\n Draw the window only if it is visible.
 *		- PRF_CHILDREN		\n Draw all visible children windows.
 *		- PRF_CLIENT		\n Draw the client area of the window.
 *		- PRF_ERASEBKGND	\n Erase the background before drawing the window.
 *		- PRF_NONCLIENT		\n Draw the nonclient area of the window.
 *		- PRF_OWNED			\n Draw all owned windows.
 *	@return	This function does not return a value.
 */
void DmWnds::Print(HDC hDC, DWORD dwFlags) const
{
	assert(this->IsWindow());
	::SendMessage(*this, WM_PRINT, reinterpret_cast<WPARAM>(hDC), dwFlags);
}

/**
 *	@brief The RedrawWindow function updates the specified rectangle or region in a window's client area.
 *	@param[in] rcUpdatePtr A pointer to a RECT structure containing the coordinates, in device units, of the update rectangle.
 *		\n This parameter is ignored if the hrgnUpdate parameter identifies a region.
 *	@param[in] uFlags One or more redraw flags.
 *		\n This parameter can be used to invalidate or validate a window, control repainting, and control which windows are affected by RedrawWindow.
 *	@return <b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero.
 */
BOOL DmWnds::RedrawWindow(const RECT* rcUpdatePtr, UINT uFlags) const
{
	assert(this->IsWindow());
	return ::RedrawWindow(*this, rcUpdatePtr, NULL, uFlags);
}

/**
 *	@brief The RedrawWindow function updates the specified rectangle or region in a window's client area.
 *	@param[in] hRgn	A handle to the update region.
 *		\n If both the hrgnUpdate and lprcUpdate parameters are NULL, the entire client area is added to the update region.
 *	@param[in] flags One or more redraw flags.
 *		\n This parameter can be used to invalidate or validate a window, control repainting, and control which windows are affected by RedrawWindow.
 *	@return <b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero.
 */
BOOL DmWnds::RedrawWindow(HRGN hRgn, UINT flags) const
{
	assert(this->IsWindow());
	return ::RedrawWindow(*this, NULL, hRgn, flags);
}

/**
 *	@brief The RedrawWindow function updates the specified rectangle or region in a window's client area.
 *	@param[in] flags One or more redraw flags.
 *		\n This parameter can be used to invalidate or validate a window, control repainting, and control which windows are affected by RedrawWindow.
 *	@return <b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero.
 *	@see https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-redrawwindow
 */
BOOL DmWnds::RedrawWindow(UINT flags) const
{
	assert(this->IsWindow());
	return ::RedrawWindow(*this, NULL, NULL, flags);
}

/**
 *	@brief The ReleaseDC function releases a device context (DC),
 *		freeing it for use by other applications. The effect of the ReleaseDC function depends on the type of DC.
 *		\n It frees only common and window DCs. It has no effect on class or private DCs.
 *	@param[in] hDC A handle to the DC to be released.
 *	@return <b>型別: int</b>
 *		\n The return value indicates whether the DC was released.
 *		\n If the DC was released, the return value is 1.
 *		\n If the DC was not released, the return value is zero.
 */
int DmWnds::ReleaseDC(HDC hDC) const
{
	assert(this->IsWindow());
	return ::ReleaseDC(*this, hDC);
}

/**
 *	@brief The @c ScreenToClient function converts the screen coordinates of a specified point on the screen to client-area coordinates.
 *	@param[in,out] pointPtr A pointer to a POINT structure that specifies the screen coordinates to be converted.
 *	@return <b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero.
 */
BOOL DmWnds::ScreenToClient(POINT* pointPtr) const
{
	assert(this->IsWindow());
	return ::ScreenToClient(*this, pointPtr);
}

/**
 *	@brief The @c ScreenToClient function converts the screen coordinates of a specified point on the screen to client-area coordinates.
 *	@param[in,out] rectPtr A pointer to a RECT structure that specifies the screen coordinates to be converted.
 *	@return <b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero.
 */
BOOL DmWnds::ScreenToClient(RECT* rectPtr) const
{
	assert(this->IsWindow());
	return static_cast<BOOL>(::MapWindowPoints(NULL, *this, reinterpret_cast<POINT*>(rectPtr), 2));
}

/**
 *	@brief The @c ScrollWindow function scrolls the contents of the specified window's client area.
 *	@param[in] xAmount		Specifies the amount, in device units, of horizontal scrolling.
 *	@param[in] yAmount		Specifies the amount, in device units, of vertical scrolling.
 *	@param[in] rcPtr		Pointer to the RECT structure specifying the portion of the client area to be scrolled.
 *	@param[in] rcClipPtr	Pointer to the RECT structure containing the coordinates of the clipping rectangle.
 *	@return <b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero. To get extended error information, call GetLastError.
 */
BOOL DmWnds::ScrollWindow(int xAmount, int yAmount, const RECT* rcPtr, const RECT* rcClipPtr) const
{
	assert(this->IsWindow());
	return ::ScrollWindow(*this, xAmount, yAmount, rcPtr, rcClipPtr);
}

/**
 *	@brief The @c ScrollWindowEx function scrolls the contents of the specified window's client area.
 *	@param[in] dx			Specifies the amount, in device units, of horizontal scrolling. This parameter must be a negative value to scroll to the left.
 *	@param[in] dy			Specifies the amount, in device units, of vertical scrolling. This parameter must be a negative value to scroll up.
 *	@param[in] rcScrollPtr	Pointer to a RECT structure that specifies the portion of the client area to be scrolled. If this parameter is NULL, the entire client area is scrolled.
 *	@param[in] rcClipPtr	Pointer to a RECT structure that contains the coordinates of the clipping rectangle.
 *	@param[in] hrgnUpdate	Handle to the region that is modified to hold the region invalidated by scrolling. This parameter may be NULL.
 *	@param[in] rcUpdatePtr	Pointer to a RECT structure that receives the boundaries of the rectangle invalidated by scrolling. This parameter may be NULL.
 *	@param[in] flags		Specifies flags that control scrolling. This parameter can be a combination of the following values.
 *		- SW_ERASE			\n Erases the newly invalidated region by sending a WM_ERASEBKGND message to the window when specified with the SW_INVALIDATE flag.
 *		- SW_INVALIDATE		\n Invalidates the region identified by the hrgnUpdate parameter after scrolling.
 *		- SW_SCROLLCHILDREN	\n Scrolls all child windows that intersect the rectangle pointed to by the prcScroll parameter. The child windows are scrolled by the number of pixels specified by the dx and dy parameters. The system sends a WM_MOVE message to all child windows that intersect the prcScroll rectangle, even if they do not move.
 *		- SW_SMOOTHSCROLL	\n Scrolls using smooth scrolling. Use the HIWORD portion of the flags parameter to indicate how much time, in milliseconds, the smooth-scrolling operation should take.
 *	@return <b>型別: int</b>
 *		\n If the function succeeds, the return value is SIMPLEREGION (rectangular invalidated region),
 *		COMPLEXREGION (nonrectangular invalidated region; overlapping rectangles), or NULLREGION (no invalidated region).
 *		\n If the function fails, the return value is ERROR. To get extended error information, call GetLastError.
 */
int DmWnds::ScrollWindowEx(int dx, int dy, const RECT* rcScrollPtr, const RECT* rcClipPtr, HRGN hrgnUpdate, RECT* rcUpdatePtr, UINT flags) const
{
	assert(this->IsWindow());
	return ::ScrollWindowEx(*this, dx, dy, rcScrollPtr, rcClipPtr, hrgnUpdate, rcUpdatePtr, flags);
}

/**
 *	@brief Sends a message to the specified control in a dialog box.
 *	@param[in] nIDDlgItem	The identifier of the control that receives the message.
 *	@param[in] uMessage		The message to be sent.
 *	@param[in] wParam		Additional message-specific information.
 *	@param[in] lParam		Additional message-specific information.
 *	@return <b>型別: LRESULT</b> \n The return value specifies the result of the message processing and depends on the message sent.
 */
LRESULT DmWnds::SendDlgItemMessage(int nIDDlgItem, UINT uMessage, WPARAM wParam, LPARAM lParam) const
{
	assert(this->IsWindow());
	return ::SendDlgItemMessage(*this, nIDDlgItem, uMessage, wParam, lParam);
}

/**
 *	@brief Sends the specified message to a window or windows.
 *		\n The SendMessage function calls the window procedure for the specified window
 *		and does not return until the window procedure has processed the message.
 *	@param[in] uMessage	The message to be sent.
 *	@param[in] wParam	Additional message-specific information.
 *	@param[in] lParam	Additional message-specific information.
 *	@return <b>型別: LRESULT</b> \n The return value specifies the result of the message processing; it depends on the message sent.
 */
LRESULT DmWnds::SendMessage(UINT uMessage, WPARAM wParam, LPARAM lParam) const
{
	assert(this->IsWindow());
	return ::SendMessage(*this, uMessage, wParam, lParam);
}

/**
 *	@brief Sends the specified message to a window or windows.
 *		\n If the window was created by the calling thread,
 *		SendNotifyMessage calls the window procedure for the window and does not return until the window procedure has processed the message.
 *		\n If the window was created by a different thread,
 *		SendNotifyMessage passes the message to the window procedure and returns immediately,
 *		it does not wait for the window procedure to finish processing the message.
 *
 *	@param[in] uMessage	The message to be sent.
 *	@param[in] wParam	Additional message-specific information.
 *	@param[in] lParam	Additional message-specific information.
 *	@return <b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero. To get extended error information, call GetLastError.
 */
BOOL DmWnds::SendNotifyMessage(UINT uMessage, WPARAM wParam, LPARAM lParam) const
{
	assert(this->IsWindow());
	return static_cast<BOOL>(::SendNotifyMessage(*this, uMessage, wParam, lParam));
}

/**
 *	@brief Activates a window. The window must be attached to the calling thread's message queue.
 *	@return <b>型別: HWND</b>
 *		\n If the function succeeds, the return value is the handle to the window that was previously active.
 *		\n If the function fails, the return value is NULL. To get extended error information, call GetLastError.
 */
HWND DmWnds::SetActiveWindow() const
{
	assert(this->IsWindow());
	return ::SetActiveWindow(*this);
}

/**
 *	@brief Sets the mouse capture to the specified window belonging to the current thread.
 *		SetCapture captures mouse input either when the mouse is over the capturing window,
 *		or when the mouse button was pressed while the mouse was over the capturing window and the button is still down.
 *		Only one window at a time can capture the mouse.
 *		\n\n If the mouse cursor is over a window created by another thread,
 *		the system will direct mouse input to the specified window only if a mouse button is down.
 *	@return <b>型別: HWND</b>
 *		\n The return value is a handle to the window that had previously captured the mouse.
 *		\n If there is no such window, the return value is NULL
 */
HWND DmWnds::SetCapture() const
{
	assert(this->IsWindow());
	return ::SetCapture(*this);
}

/**
 *	@brief Moves the caret to the specified coordinates.
 *		\n If the window that owns the caret was created with the CS_OWNDC class style,
 *		then the specified coordinates are subject to the mapping mode of the device context associated with that window.
 *	@param[in] pointPtr Pointer of POINT structure, Specifies the new x and y coordinates (in client coordinates) of the caret.
 *	@return <b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero. To get extended error information, call GetLastError.
 */
BOOL DmWnds::SetCaretPos(const POINT* pointPtr) const
{
	assert(this->IsWindow());
	return ::SetCaretPos(static_cast<int>(pointPtr->x), static_cast<int>(pointPtr->y));
}

/**
 *	@brief Moves the caret to the specified coordinates.
 *		\n If the window that owns the caret was created with the CS_OWNDC class style,
 *		then the specified coordinates are subject to the mapping mode of the device context associated with that window.
 *	@param[in] nPosx The new x-coordinate of the caret.
 *	@param[in] nPosy The new y-coordinate of the caret.
 *	@return <b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero. To get extended error information, call GetLastError.
 */
BOOL DmWnds::SetCaretPos(int nPosx, int nPosy) const { return ::SetCaretPos(nPosx, nPosy); }

/**
 *	@brief 替換原先註冊視窗的資訊，參照 WNDCLASSEX 結構
 *	@param[in] nIndex		指定項目的索引值，參照 DmWnds::GetClassLongPtr
 *	@param[in] dwNewLong	變更內容
 *	@return <b>型別: UNLONG_PTR</b> \n 若運作成功返回值為變更前項目資訊。 \n 若操作失敗返回零，使用 GetLastError() 取得錯誤碼。
 *	@see DmWnds::GetClassLongPtr
 */
ULONG_PTR DmWnds::SetClassLongPtr(int nIndex, LONG_PTR dwNewLong) const
{
	assert(this->IsWindow());
	return ::SetClassLongPtr(*this, nIndex, dwNewLong);
}

/**
 *	@brief 設定工作區邊界尺寸
 *	@param[in] nWidth	寬度
 *	@param[in] nHeight	高度
 *	@return <b>型別: BOOL</b> \n 若運作成功返回值為非零值。 \n 若運作失敗則返回值為零。
 */
BOOL DmWnds::SetClientSize(int nWidth, int nHeight) const
{
	const UINT flag = SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE;
	HWND hWnd = *this;
	RECT rc;

	assert(this->IsWindow());
	if (!::SetRect(&rc, 0, 0, nWidth, nHeight)) return FALSE;
	if (!::AdjustWindowRectEx(&rc, ::GetWindowLong(hWnd, GWL_STYLE), ::GetMenu(hWnd) != NULL, 0)) return FALSE;

	nWidth = static_cast<int>(rc.right - rc.left);
	nHeight = static_cast<int>(rc.bottom - rc.top);
	return ::SetWindowPos(hWnd, NULL, 0, 0, nWidth, nHeight, flag);
}

/**
 *	@brief 設定視窗新的識別碼 (Identifier)
 *	@param[in] nID 新的視窗識別碼 (Identifier)
 *	@return <b>型別: int</b> \n 如果運作成功返回值為改變前的識別碼 (ID)。 \n 如果運作失敗返回值為 (0)，使用 GetLastError 取得失敗訊息。
 */
int DmWnds::SetDlgCtrlID(int nID) const
{
	assert(this->IsWindow());
	// 清空錯誤訊息
	::SetLastError(ERROR_SUCCESS);
	return static_cast<int>(::SetWindowLongPtr(*this, GWLP_ID, nID));
}

/**
 *	@brief Sets the text of a control in a dialog box to the string representation of a specified integer value.
 *	@param[in] nIDDlgItem	The control to be changed.
 *	@param[in] uValue		The integer value used to generate the item text.
 *	@param[in] bSigned		Indicates whether the uValue parameter is signed or unsigned.
 *		\n If this parameter is TRUE, uValue is signed.
 *		\n If this parameter is TRUE and uValue is less than zero, a minus sign is placed before the first digit in the string.
 *		\n If this parameter is FALSE, uValue is unsigned.
 *	@return <b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero. To get extended error information, call GetLastError.
 */
BOOL DmWnds::SetDlgItemInt(int nIDDlgItem, UINT uValue, BOOL bSigned)
{
	assert(this->IsWindow());
	return ::SetDlgItemInt(*this, nIDDlgItem, uValue, bSigned);
}

/**
 *	@brief Sets the title or text of a control in a dialog box.
 *	@param[in] nIDDlgItem	The control with a title or text to be set.
 *	@param[in] szTextPtr	The text to be copied to the control.
 *	@return <b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero. To get extended error information, call GetLastError.
 */
BOOL DmWnds::SetDlgItemText(int nIDDlgItem, const TCHAR* szTextPtr)
{
	assert(this->IsWindow());
	return ::SetDlgItemText(*this, nIDDlgItem, szTextPtr);
}

/**
 *	@brief 設定視窗、控制項擴展樣式
 *	@param[in] dwExStyle 視窗、控制項擴展樣式
 *	@return	<b>型別: DWORD</b>
 *		\n 若運作成功返回值為非零值，值為設定前的擴展樣式。
 *		\n 若運作失敗則返回值為零，失敗請調用 GetLastError 取得錯誤訊息。
 */
DWORD DmWnds::SetExStyle(DWORD dwExStyle) const
{
	assert(this->IsWindow());
	return static_cast<DWORD>(::SetWindowLongPtr(*this, GWL_EXSTYLE, static_cast<LONG_PTR>(dwExStyle)));
}

/**
 *	@brief Sets the keyboard focus to the specified window. \n The window must be attached to the calling thread's message queue.
 *	@param[in] hWnd A handle to the window that will receive the keyboard input. \n If this parameter is NULL, keystrokes are ignored.
 *	@return <b>型別: HWND</b>
 *		\n If the function succeeds, the return value is the handle to the window that previously had the keyboard focus.
 *		\n If the hWnd parameter is invalid or the window is not attached to the calling thread's message queue, the return value is NULL. To get extended error
 */
HWND DmWnds::SetFocus(HWND hWnd) const
{
	HWND hWndPrev = NULL;
	if (hWnd == reinterpret_cast<HWND>(-1)) {
		hWnd = this->GetSafeHwnd();
	}

	if (::IsWindow(hWnd)) {
		hWndPrev = ::SetFocus(hWnd);
	}
	return hWndPrev;
}

/**
 *	@brief Sets the font that a control is to use when drawing text.
 *	@param[in] hFont A handle to the font (HFONT).
 *		\n If this parameter is NULL, the control uses the default system font to draw text.
 *	@param[in] bRedraw The low-order word of lParam specifies whether the control should be redrawn immediately upon setting the font.
 *		\n If this parameter is TRUE, the control redraws itself. Default value is TRUE
 *	@return This function does not return a value.
 */
void DmWnds::SetFont(HFONT hFont, BOOL bRedraw) const
{
	assert(this->IsWindow());
	// WM_SETFONT
	// wParam = 字型操作代碼
	// lParam = 是否立即重繪
	auto wParam = reinterpret_cast<WPARAM>(hFont);
	auto lParam = static_cast<LPARAM>(bRedraw);
	::SendMessage(*this, WM_SETFONT, wParam, lParam);
}

/**
 *	@brief Brings the thread that created the specified window into the foreground and activates the window.
 *		\n Keyboard input is directed to the window, and various visual cues are changed for the user.
 *		\n The system assigns a slightly higher priority to the thread that created the foreground window than it does to other threads.
 *	@return <b>型別: BOOL</b>
 *		\n If the window was brought to the foreground, the return value is nonzero.
 *		\n If the window was not brought to the foreground, the return value is zero.
 */
BOOL DmWnds::SetForegroundWindow() const
{
	assert(this->IsWindow());
	return ::SetForegroundWindow(*this);
}

/**
 *	@brief Associates a new large or small icon with a window.
 *		\n The system displays the large icon in the ALT+TAB dialog box, and the small icon in the window caption.
 *	@param[in] hIcon	The type of icon to be set. This parameter can be one of the following values.
 *	@param[in] bBigIcon	A handle to the new large or small icon. If this parameter is NULL, the icon indicated by wParamis removed.
 *	@return <b>型別: HICON</b>
 *		\n The return value is a handle to the previous large or small icon, depending on the value of wParam.
 *		\n It is NULL if the window previously had no icon of the type indicated by wParam.
 */
HICON DmWnds::SetIcon(HICON hIcon, BOOL bBigIcon) const
{
	assert(this->IsWindow());
	// WM_SETICON
	// wParam = 是否使用大圖示 (TRUE / FALSE)
	// lParam = 圖示的操作代碼
	return reinterpret_cast<HICON>(::SendMessage(*this
		, WM_SETICON
		, static_cast<WPARAM>(bBigIcon)
		, reinterpret_cast<LPARAM>(hIcon)));
}

/**
 *	@brief Assigns a new menu to the specified window.
 *	@param[in] hMenu A handle to the new menu. If this parameter is NULL, the window's current menu is removed.
 *	@return <b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero. To get extended error information, call GetLastError.
 */
BOOL DmWnds::SetMenu(HMENU hMenu) const
{
	assert(this->IsWindow());
	return ::SetMenu(*this, hMenu);
}

/**
 *	@brief Changes the parent window of the specified child window.
 *	@param[in] hWndNewParent A handle to the new parent window. If this parameter is NULL, \n the desktop window becomes the new parent window.
 *	@return <b>型別: HWND</b>
 *		\n If the function succeeds, the return value is a handle to the previous parent window.
 *		\n If the function fails, the return value is NULL. To get extended error information, call GetLastError.
 */
HWND DmWnds::SetParent(HWND hWndNewParent) const
{
	assert(this->IsWindow());
	return ::SetParent(*this, hWndNewParent);
}

/**
 *	@brief Changes in that window to be redrawn or to prevent changes in that window from being redrawn.
 *	@param[in] bRedraw The redraw state.
 *		- If this parameter is TRUE, the content can be redrawn after a change.
 *		- If this parameter is FALSE, the content cannot be redrawn after a change.
 *	@return <b>型別: BOOL</b> \n An application returns zero
 */
BOOL DmWnds::SetRedraw(BOOL bRedraw) const
{
	assert(this->IsWindow());
	// WM_SETREDRAW
	// wParam = 是否重繪狀態
	// lParam = 未使用，必須為零
	return static_cast<BOOL>(::SendMessage(*this
		, WM_SETREDRAW
		, static_cast<WPARAM>(bRedraw)
		, 0));
}

/**
 *	@brief sets the parameters of a scroll bar. \n
 *		Including the minimum and maximum scrolling positions, the page size, and the position of the scroll box (thumb). \n
 *		The function also redraws the scroll bar, if requested.
 *	@param[in] nBar Specifies the type of scroll bar for which to set parameters
 *		<pre>
 *		SB_CTL		Sets the parameters of a scroll bar control. The hwnd parameter must be the handle to the scroll bar control.
 *		SB_HORZ		Sets the parameters of the window's standard horizontal scroll bar.
 *		SB_VERT		Sets the parameters of the window's standard vertical scroll bar.
 *		</pre>
 *	@param[in] siPtr Pointer to a SCROLLINFO structure. \n Set the cbSize member of the structure to sizeof(SCROLLINFO), Set the fMask member to indicate the parameters to set, and specify the new parameter values in the appropriate members.
 *	@param[in] bRedraw Specifies whether the scroll bar is redrawn to reflect the changes to the scroll bar. \n If this parameter is TRUE, the scroll bar is redrawn, otherwise, it is not redrawn.
 *	@return <b>型別: int</b> \n The return value is the current position of the scroll box.
 */
int DmWnds::SetScrollInfo(int nBar, const SCROLLINFO* siPtr, BOOL bRedraw) const
{
	assert(this->IsWindow());
	return ::SetScrollInfo(*this, nBar, siPtr, bRedraw);
}

/**
 *	@brief Sets the position of the scroll box (thumb) in the specified scroll bar and, if requested, redraws the scroll bar to reflect the new position of the scroll box.
 *	@param[in] nBar		Specifies the scroll bar to be set
 *	@param[in] nPos		Specifies the new position of the scroll box. The position must be within the scrolling range. For more information about the scrolling range
 *	@param[in] bRedraw	Specifies whether the scroll bar is redrawn to reflect the new scroll box position. If this parameter is TRUE, the scroll bar is redrawn. If it is FALSE, the scroll bar is not redrawn.
 *	@return <b>型別: int</b>
 *		\n If the function succeeds, the return value is the previous position of the scroll box.
 *		\n If the function fails, the return value is zero. To get extended error information, call GetLastError.
 *	@see DmWnds::SetScrollInfo
 */
int DmWnds::SetScrollPos(int nBar, int nPos, BOOL bRedraw) const
{
	assert(this->IsWindow());
	return ::SetScrollPos(*this, nBar, nPos, bRedraw);
}

/**
 *	@brief Sets the minimum and maximum scroll box positions for the specified scroll bar.
 *	@param[in] nBar Specifies the scroll bar to be set.
 *	@param[in] nMinPos Specifies the minimum scrolling position.
 *	@param[in] nMaxPos Specifies the maximum scrolling position.
 *	@param[in] bRedraw Specifies whether the scroll bar should be redrawn to reflect the change. If this parameter is TRUE, the scroll bar is redrawn. If it is FALSE, the scroll bar is not redrawn.
 *	@return <b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero. To get extended error information, call GetLastError.
 *	@see DmWnds::SetScrollInfo
 */
BOOL DmWnds::SetScrollRange(int nBar, int nMinPos, int nMaxPos, BOOL bRedraw) const
{
	assert(this->IsWindow());
	return ::SetScrollRange(*this, nBar, nMinPos, nMaxPos, bRedraw);
}

/**
 *	@brief 設定視窗、控制項樣式
 *	@param[in] dwStyle 視窗或控制項樣式
 *	@return	<b>型別: DWORD</b>
 *		\n 若運作成功返回值為非零值，值為設定前的視窗樣式。
 *		\n 若運作失敗則返回值為零，失敗請調用 GetLastError 取得錯誤訊息。
 */
DWORD DmWnds::SetStyle(DWORD dwStyle) const
{
	assert(this->IsWindow());
	::SetLastError(ERROR_SUCCESS);
	return static_cast<DWORD>(::SetWindowLongPtr(*this, GWL_STYLE, static_cast<LONG_PTR>(dwStyle)));
}

/**
 *	@brief Creates a timer with the specified time-out value.
 *	@param[in] nIDEvent		A nonzero timer identifier.
 *	@param[in] uElapse		The time-out value, in milliseconds.
 *	@param[in] fnTimerFunc	A pointer to the function to be notified when the time-out value elapses.
 *	@return	<b>型別: UINT_PTR</b> \n
 *		\n If the function succeeds and the hWnd parameter is NULL, the return value is an integer identifying the new timer. An application can pass this value to the KillTimer function to destroy the timer.
 *		\n If the function succeeds and the hWnd parameter is not NULL, then the return value is a nonzero integer. An application can pass the value of the nIDEvent parameter to the KillTimer function to destroy the timer.
 *		\n If the function fails to create a timer, the return value is zero. To get extended error information, call GetLastError.
 *	@see https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nc-winuser-timerproc
 */
UINT_PTR DmWnds::SetTimer(UINT_PTR nIDEvent, UINT uElapse, TIMERPROC fnTimerFunc) const
{
	assert(this->IsWindow());
	return ::SetTimer(*this, nIDEvent, uElapse, fnTimerFunc);
}

/**
 *	@brief Changes an attribute of the specified window. The function also sets a value at the specified offset in the extra window memory.
 *	@param[in] nIndex		The zero-based offset to the value to be set.
 *	@param[in] dwNewLong	The replacement value.
 *	@return <b>型別: LONG_PTR</b>
 *		\n If the function succeeds, the return value is the previous value of the specified offset.
 *		\n If the function fails, the return value is zero. To get extended error information, call GetLastError.
 *	@see DmWnds::GetWindowLongPtr
 */
LONG_PTR DmWnds::SetWindowLongPtr(int nIndex, LONG_PTR dwNewLong) const
{
	assert(this->IsWindow());
	::SetLastError(ERROR_SUCCESS);
	return ::SetWindowLongPtr(*this, nIndex, dwNewLong);
}

/**
 *	@brief Sets the show state and the restored, minimized, and maximized positions of the specified window.
 *	@param[in] wndplPtr A pointer to a WINDOWPLACEMENT structure that specifies the new show state and window positions.
 *	@return <b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero. To get extended error information, call GetLastError.
 */
BOOL DmWnds::SetWindowPlacement(const WINDOWPLACEMENT* wndplPtr) const
{
	assert(this->IsWindow());
	return ::SetWindowPlacement(*this, wndplPtr);
}

/**
 *	@brief Changes the size, position, and Z order of a child, pop-up, or top-level window.
 *		\n These windows are ordered according to their appearance on the screen.
 *		The topmost window receives the highest rank and is the first window in the Z order.
 *	@param[in] hWndInsertAfter A handle to the window to precede the positioned window in the Z order.
 *	@param[in] x	The new position of the left side of the window, in client coordinates.
 *	@param[in] y	The new position of the top of the window, in client coordinates.
 *	@param[in] cx	The new width of the window, in pixels.
 *	@param[in] cy	The new height of the window, in pixels.
 *	@param[in] uFlags	The window sizing and positioning flags.
 *	@return <b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero. To get extended error information, call GetLastError.
 *	@remark <b>hWndInsertAfter 參數值額外說明</b>
 *		\n 若 uFlags 參數中設置了 SWP_NOZORDER 標記則本參數將被忽略。參數必須是視窗 Handle 或 參數值。
 *		- HWND_BOTTOM		\n 將視窗置於 Z 順序的底部如果參數的 hWnd 標識了一個頂層視窗，則視窗失去頂級位置，並且被置在其他視窗的底部。
 *		- HWND_NOTOPMOST	\n 將視窗置於所有非頂層視窗之上（即在所有頂層視窗之後）如果視窗已經是非頂層視窗則該標誌不起作用。
 *		- HWND_TOP			\n 將視窗置於 Z 順序的頂部。
 *		- HWND_TOPMOST		\n 將視窗置於所有非頂層視窗之上即使視窗未被激活視窗也將保持頂級位置。
 *	@remark		<b>uFlags 標示內容 (Google 機器翻譯，尚未修正)</b>
 *		- SWP_ASYNCWINDOWPOS	\n 如果調用進程不擁有視窗，系統會向擁有視窗的線程發出需求這就防止調用線程在其他線程處理需求的時候發生死鎖。
 *		- SWP_DEFERERASE		\n 防止產生WM_SYNCPAINT訊息。
 *		- SWP_DRAWFRAME			\n 在視窗周圍畫一個邊框（定義在視窗類描述中）。
 *		- SWP_FRAMECHANGED		\n 給視窗發送 WM_NCCALCSIZE 訊息，即使視窗尺寸沒有改變也會發送該訊息如果未指定這個標誌，只有在改變了視窗尺寸時才發送 WM_NCCALCSIZE。
 *		- SWP_HIDEWINDOW		\n 隱藏視窗。
 *		- SWP_NOACTIVATE		\n 不激活視窗如果未設置標誌，則視窗被激活，並被設置到其他最高級視窗或非最高級組的頂部（根據參數hWndlnsertAfter設置）。
 *		- SWP_NOCOPYBITS		\n 清除客戶區的所有內容如果未設置該標誌，客戶區的有效內容被保存並且在視窗尺寸更新和重定位後拷貝回客戶區。
 *		- SWP_NOMOVE			\n 維持當前位置（忽略X和Ÿ參數）。
 *		- SWP_NOOWNERZORDER		\n 不改變 Z 順序中的所有者視窗的位置。
 *		- SWP_NOREDRAW			\n 不重畫改變的內容如果設置了這個標誌，則不發生任何重畫動作適用於客戶區和非客戶區（包括標題欄和滾動條）和任何由於窗回移動而露出的父視窗的所有部分。
 *		 						\n 如果設置了這個標誌，應用程序必須明確地使視窗無效並區重畫視窗的任何部分和父視窗需要重畫的部分。
 *		- SWP_NOREPOSITION		\n 與 SWP_NOOWNERZORDER 標誌相同。
 *		- SWP_NOSENDCHANGING	\n 防止視窗接收WM_WINDOWPOSCHANGING訊息。
 *		- SWP_NOSIZE			\n 維持當前尺寸（忽略CX和賽揚參數）。
 *		- SWP_NOZORDER			\n 維持當前ž序（忽略hWndlnsertAfter參數）。
 *		- SWP_SHOWWINDOW		\n 顯示視窗。
 */
BOOL DmWnds::SetWindowPos(HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT uFlags) const
{
	assert(this->IsWindow());
	return ::SetWindowPos(*this, hWndInsertAfter, x, y, cx, cy, uFlags);
}

/**
 *	@brief 設定視窗顯示位置
 *	@param[in] x 於父視窗 client-area 座標 x (左邊界)
 *	@param[in] y 於父視窗 client-area 座標 y (上邊界)
 *	@return <b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero. To get extended error information, call GetLastError.
 *	@see DmWnds::SetWindowPos
 */
BOOL DmWnds::SetWindowPosition(int x, int y) const
{
	assert(this->IsWindow());
	const UINT flag = SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE;
	return ::SetWindowPos(*this, NULL, x, y, 0, 0, flag);
}

/**
 *	@brief 設定視窗大小
 *	@param[in] nWidth	寬度
 *	@param[in] nHeight	高度
 *	@return <b>型別: BOOL</b>
 *		\n 若運作成功返回值為非零值。
 *		\n 若運作失敗則返回值為零，失敗呼叫 GetLastError 取得錯誤訊息。
 *	@see DmWnds::SetWindowPos
 */
BOOL DmWnds::SetWindowSize(int nWidth, int nHeight) const
{
	assert(this->IsWindow());
	const UINT uFlag = SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE;
	return ::SetWindowPos(*this, NULL, 0, 0, nWidth, nHeight, uFlag);
}

/**
 *	@brief The @c SetWindowRgn function sets the window region of a window.
 *	@param[in] hRgn	A handle to a region. The function sets the window region of the window to this region.
 *		\n If hRgn is NULL, the function sets the window region to NULL.
 *	@param[in]	bRedraw	Specifies whether the system redraws the window after setting the window region.
 *		\n If bRedraw is TRUE, the system does so; otherwise, it does not.
 *	@return <b>型別: int</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero.
 */
int DmWnds::SetWindowRgn(HRGN hRgn, BOOL bRedraw) const
{
	assert(this->IsWindow());
	return ::SetWindowRgn(*this, hRgn, bRedraw);
}

/**
 *	@brief Changes the text of the specified window's title bar (if it has one).
 *		\n If the specified window is a control, the text of the control is changed. However,
 *		SetWindowText cannot change the text of a control in another application.
 *	@param[in] textPtr The new title or control text.
 *	@return <b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero. To get extended error information, call GetLastError.
 */
BOOL DmWnds::SetWindowText(const TCHAR* textPtr) const
{
	assert(this->IsWindow());
	return ::SetWindowText(*this, textPtr);
}

/**
 *	@brief Makes the caret visible on the screen at the caret's current position.
 *		When the caret becomes visible, it begins flashing automatically.
 *	@return <b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero. To get extended error information, call GetLastError.
 */
BOOL DmWnds::ShowCaret() const
{
	assert(this->IsWindow());
	return ::ShowCaret(*this);
}

/**
 *	@brief Shows or hides all pop-up windows owned by the specified window.
 *	@param[in] fShow If this parameter is TRUE, all hidden pop-up windows are shown. If this parameter is FALSE, all visible pop-up windows are hidden.
 *	@return <b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero. To get extended error information, call GetLastError.
 */
BOOL DmWnds::ShowOwnedPopups(BOOL fShow) const
{
	assert(this->IsWindow());
	return ::ShowOwnedPopups(*this, fShow);
}

/**
 *	@brief The ShowScrollBar function shows or hides the specified scroll bar.
 *	@param[in] wBar Specifies the scroll bar(s) to be shown or hidden. This parameter can be one of the following values.
 *		- SB_BOTH	\n	Shows or hides a window's standard horizontal and vertical scroll bars.
 *		- SB_CTL	\n	Shows or hides a scroll bar control. The hwnd parameter must be the handle to the scroll bar control.
 *		- SB_HORZ	\n	Shows or hides a window's standard horizontal scroll bars.
 *		- SB_VERT	\n	Shows or hides a window's standard vertical scroll bar.
 *	@param[in] bShow Specifies whether the scroll bar is shown or hidden.
 *		\n If this parameter is TRUE, the scroll bar is shown; otherwise, it is hidden.
 *	@return <b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero. To get extended error information, call GetLastError.
 */
BOOL DmWnds::ShowScrollBar(int wBar, BOOL bShow)
{
	assert(this->IsWindow());
	return ::ShowScrollBar(*this, wBar, bShow);
}

/**
 *	@brief 設定視窗顯視狀態
 *	@param[in] nCmdShow 要如何顯示視窗
 *	@return <b>型別: BOOL</b>
 *		\n 如果視窗為可見狀態，返回值為非零值。
 *		\n 如果視窗為不可見狀態 (如隱藏) 返回值為零。
 *	@remark <b>nCmdShow 參數說明</b>
 *		- SW_HIDE				\n	隱藏視窗
 *		- SW_SHOWNORMAL			\n	顯示視窗，如果視窗被最小化或最大化，系統將其恢復到原來的尺寸和大小 (初始建立時的大小)
 *		- SW_SHOWMINIMIZED		\n	顯示視窗並以最小化顯示
 *		- SW_SHOWMAXIMIZED		\n	顯示視窗並以最大化顯示
 *		- SW_MAXIMIZE			\n	最大化顯示視窗
 *		- SW_SHOWNOACTIVATE		\n	以視窗最近一次的大小和狀態顯示，活耀狀態視窗仍維活耀狀態。
 *		- SW_SHOW				\n	以原來大小及位置顯示視窗
 *		- SW_MINIMIZE			\n	顯示視窗並最小化，交出活耀狀態給下一個 Z 順位
 *		- SW_SHOWMINNOACTIVE	\n	視窗最小化，但仍保持活耀狀態
 *		- SW_SHOWNA				\n	以原來的狀態顯示視窗，活耀狀態視窗仍維活耀狀態
 *		- SW_RESTORE			\n	顯示視窗並呈現活耀狀態，如果視窗最小化或最大化，則係統將視窗恢復到原來的尺寸和位置，在恢復最小化視窗時，應該指定使用這個方式。
 *		- SW_SHOWDEFAULT		\n	依據在 STARTUPINFO 結構中指定的 FLAG 標誌設定顯示狀態,STARTUPINFO 結構是由啟動應用程序的程序傳遞給 CreateProcess 函數。
 *		- SW_FORCEMINIMIZE		\n	在 Windows NT5.0 中最小化視窗,即使擁有視窗的線程被激活也會最小化。在從其他線程最小化視窗時才使用這個參數。
 */
BOOL DmWnds::ShowWindow(int nCmdShow) const
{
	assert(this->IsWindow());
	return ::ShowWindow(*this, nCmdShow);
}

/**
 *	@brief Updates the client area of the specified window.
 *		\n The UpdateWindow function updates the client area of the specified window by sending a WM_PAINT message to the window
 *		if the window's update region is not empty.
 *		\n The function sends a WM_PAINT message directly to the window procedure of the specified window,
 *		bypassing the application queue. If the update region is empty, no message is sent.
 *	@return <b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero.
 */
BOOL DmWnds::UpdateWindow() const
{
	assert(this->IsWindow());
	return ::UpdateWindow(*this);
}

/**
 *	@brief The ValidateRect function validates the client area within a rectangle by removing the rectangle from the update region of the specified window.
 *	@param[in] rcPtr Pointer to a RECT structure that contains the client coordinates of the rectangle to be removed from the update region. \n If this parameter is NULL, the entire client area is removed.
 *	@return <b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero.
 */
BOOL DmWnds::ValidateRect(const RECT* rcPtr) const
{
	assert(this->IsWindow());
	return ::ValidateRect(*this, rcPtr);
}

/**
 *	@brief The ValidateRect function Validates the entire client area of the window.
 *	@return	<b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero
 */
BOOL DmWnds::ValidateRect() const
{
	assert(this->IsWindow());
	return ::ValidateRect(*this, NULL);
}

/**
 *	@brief The ValidateRgn function validates the client area within a region by removing the region from the current update region of the specified window.
 *	@param[in] hRgn Handle to a region that defines the area to be removed from the update region. If this parameter is NULL, the entire client area is removed.
 *	@return <b>型別: BOOL</b>
 *		\n If the function succeeds, the return value is nonzero.
 *		\n If the function fails, the return value is zero.
 */
BOOL DmWnds::ValidateRgn(HRGN hRgn) const
{
	assert(this->IsWindow());
	return ::ValidateRgn(*this, hRgn);
}

/**
 *	@brief 取得指定位置(點)，所指的視窗操作代碼(HWND)。
 *	@param[in] stPoint 結構 POINT 緩衝區參考
 *	@return <b>型別: HWND</b>
 *		\n 返回值是指定位置的視窗的操作代碼。如果指定點上不存在視窗，則返回值為NULL。
 *		\n 如果該點位於靜態文本控制項之上，則返回值是靜態文本控件項下視窗操作代碼 (HWND)
 */
HWND DmWnds::WindowFromPoint(POINT stPoint) const { return ::WindowFromPoint(stPoint); }





///////////////////////////////////////////////////////////////////////////////
//
// Message dispose
// 
///////////////////////////////////////////////////////////////////////////////

/**
 *	@brief	視窗或控制像訊息處理函數 (轉發給原訊息處理函數)
 *	@param[in] uMessage	訊息碼
 *	@param[in] wParam	訊息參數
 *	@param[in] lParam	訊息參數
 *	@return <b>型別: LRESULT</b> \n 返回值為訊息處理結果
 */
LRESULT DmWnds::PassToNextWndProc(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	// 呼叫原視窗處理函數
	auto fnCallback = reinterpret_cast<WNDPROC>(this->GetSafePrevCallback());
	if (fnCallback != NULL) {
		return ::CallWindowProc(fnCallback, *this, uMessage, wParam, lParam);
	}
	// 使用系統預設處理函數
	return ::DefWindowProc(*this, uMessage, wParam, lParam);
}

/**
 *	@brief 視窗或控制項訊息處理函數 (預設處理函數)
 *	@param[in] uMessage	訊息碼
 *	@param[in] wParam	訊息參數
 *	@param[in] lParam	訊息參數
 *	@return <b>型別: LRESULT</b> \n 返回值為訊息處理結果
 */
LRESULT DmWnds::DefaultWndProc(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	auto lResult = static_cast< LRESULT>(0);	// 多此一舉? 直接用 LRESULT 定義不就好? 嘿嘿~ 我爽~ 就是想用用 auto 語法

	switch (uMessage)
	{
	/** !!! 注意 WM_NCCREATE, WM_NCDESTROY 還是必須交回原系統處理 */
	case WM_NCCREATE:
		this->WmNcCreate(wParam, lParam);
		return this->PassToNextWndProc(WM_NCCREATE, wParam, lParam);
		break;
	case WM_CREATE:
		lResult = this->WmCreate(wParam, lParam);
		break;
	case WM_DESTROY:
		this->WmDestroy(wParam, lParam);
		break;
	case WM_NCDESTROY:
		this->WmNcDestroy(wParam, lParam);
		return this->PassToNextWndProc(WM_NCDESTROY, wParam, lParam);
		break;
	case WM_CLOSE:
		this->WmClose(wParam, lParam);
		break;
	case WM_USERCREATE:
		this->WmUserCreate(wParam, lParam);
		break;
	default:
		return this->PassToNextWndProc(uMessage, wParam, lParam);
	}

	return lResult;
}

/**
 *	@brief [虛擬函數] WM_NCCREATE 訊息處理，此訊息為 WM_CREATE 首個視窗建立後訊息。
 *	@param[in] wParam 不使用
 *	@param[in] lParam 指向 CREATESTRUCT 結構資料位址, 內容為正在建立的視窗或控制項的訊息.
 *	@return <b>型別: LRESULT</b>
 *		\n 若進行處理此訊息則必須返回 TRUE，以便繼續完成視窗建立過程。
 *		\n 若返回 FALSE 值, 則視為窗建立失敗，返回後視窗將被銷毀，使用 CreateWindowEx 或 CreateWindow 建立視窗的函數將收到 NULL 返回值。
 */
LRESULT DmWnds::WmNcCreate(WPARAM wParam, LPARAM lParam)
{
	// 有甚麼要在視窗各項功能啟動前先定義嗎? 重載這個函數。
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	return TRUE;
}

/**
 *	@brief [虛擬函數] WM_CREATE 訊息處理，視窗或控制項使用 CreateWindow 或 CreateWindowEx API 建立成功時通知，衍生類別應該重載複寫此函數。
 *	@param[in] wParam	此參數未使用
 *	@param[in] lParam	指向 CREATESTRUCT 結構資料位址, 內容為正在建立的視窗或控制項的訊息.
 *	@return <b>型別: LRESULT</b>
 *		\n 若進行處理此訊息則必須返回零，以便繼續完成視窗建立過程。
 *		\n 若返回 (-1) 值, 則視為窗建立失敗，返回後視窗將被銷毀，使用 CreateWindowEx 或 CreateWindow 建立視窗的函數將收到 NULL 返回值。
 */
LRESULT DmWnds::WmCreate(WPARAM wParam, LPARAM lParam)
{
	// 什麼事都不做，留給建立者補足。
	//UNREFERENCED_PARAMETER(wParam);
	//UNREFERENCED_PARAMETER(lParam);
	this->PostUserCreateMessage(wParam, lParam);
	return 0;
}

/**
 *	@brief [虛擬函數] WM_DESTROY 訊息處理，視窗已被銷毀通知，可在衍生類別可重載複寫此函數。
 *	@param[in] wParam 未使用。
 *	@param[in] lParam 未使用。
 *	@return 此函數沒有返回值
 */
void DmWnds::WmDestroy(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
}

/**
 *	@brief [虛擬函數] WM_NCDESTROY 訊息處理，Window 被銷毀最後會收到的訊息。
 *	@param[in] wParam 不使用
 *	@param[in] lParam 不使用
 *	@return 此函數沒有返回值。
 */
void DmWnds::WmNcDestroy(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	// 釋放所有資源與物件，恢復所有預設。
	this->SafeWndsRelease();
}

/**
 *	@brief [虛擬函數] WM_CLOSE 訊息處理，視窗被關閉通知。可在衍生類別可重載複寫此函數。
 *	@param[in] wParam 未使用。
 *	@param[in] lParam 未使用。
 *	@return 此函數沒有返回值
 */
void DmWnds::WmClose(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	this->SafeWndsDestroy(static_cast<int>(wParam));
}

/**
 *	@brief [虛擬函數] 使用者訊息 UWM_WINCCREATE，建立視窗內容
 *	@param[in] wParam 未使用
 *	@param[in] lParam 未使用
 *	@return 此函數沒有返回值
 */
void DmWnds::WmUserCreate(WPARAM wParam, LPARAM lParam)
{
	// 由繼承者繼成後發展。
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
}

/**
 *	@brief [虛擬函數] 視窗或控制項訊息處理函數，衍生類別重載複寫此函數以執行對應任務。
 *	@param[in]	uMessage	訊息碼
 *	@param[in]	wParam		訊息參數
 *	@param[in]	lParam		訊息參數
 *	@return <b>型別: LRESULT</b> \n 返回值為訊息處理結果
 */
LRESULT DmWnds::WndProc(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	// 呼叫預設處理
	return this->DefaultWndProc(uMessage, wParam, lParam);
}

/**
 *	@brief 設定 (保存) 對話框種類
 *	@return 此函數沒有返回值
 */
void DmWnds::SetSafeType(EmCtrls eType) { m_eType = eType; }

/**
 *	@brief 保存使用者自定義字型
 *	@param[in] hFont 字型操作代碼
 *	@return 此函數沒有返回值
 */
void DmWnds::SetSafeFont(HFONT hFont) { m_hFont = hFont; }

/**
 *	@brief 保存綁定的視窗操作代碼
 *	@param[in] hWnd 視窗代碼
 *	@retuen 此函數沒有返回值
 */
void DmWnds::SetSafeHwnd(HWND hWnd) { m_hWnd = hWnd; }

/**
 *	@brief 保存改變前的視窗訊息處理 Callback 函數位址
 *	@param[in] lCallback 指向 Callback 函數位址
 */
void DmWnds::SetSafePrevCallback(LONG_PTR lCallback) { m_lPrevCallback = lCallback; }

/**
 *	@brief [虛擬函數] 綁定視窗於物件
 *	@return <b>型別: BOOL</b> \n 若綁定成功返回值為非零值。 \n 若綁定失敗則返回值為零。
 */
BOOL DmWnds::BindWindow(HWND hWnd)
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
		auto dmPrev = ::GetWindowLongPtr(hWnd, GWLP_WNDPROC);
		auto dmProc = this->GetSafeCallback();

		::SetWindowLongPtr(*this, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
		if (dmPrev != dmProc) {
			// 設定新的訊息處理 Callback 函數位址
			dmPrev = ::SetWindowLongPtr(hWnd, GWLP_WNDPROC, dmProc);
			this->SetSafePrevCallback(dmPrev);
		}
		break;
	}
	return this->GetSafeHwnd() != NULL;
}

/**
 *	@brief [虛擬函數] 解除綁定中視窗或控制項
 *	@return 此函數沒有返回值
 */
void DmWnds::LooseWindow()
{
	// 恢復原始訊息處理 Callback
	auto fnCallback = this->GetSafePrevCallback();
	if (fnCallback != 0) {
		::SetWindowLongPtr(*this, GWLP_USERDATA, 0);
		::SetWindowLongPtr(*this, GWLP_WNDPROC, fnCallback);
		m_lPrevCallback = 0;
	}
}

/**
 *	@brief <b>[虛擬函數]</b> 請求銷毀視窗，該請求執行後被銷毀後會得到 WM_DESTROY 訊息通知。
 *	@param[in] nExitCode	結束程式返回碼
 *	@return 此函數沒有返回值
 */
void DmWnds::SafeWndsDestroy(int nExitCode)
{
	if (this->IsWindow()) {

		// Modal's Dialog ??
		if (m_bModal) {
			::EndDialog(*this, static_cast<INT_PTR>(nExitCode));
			return;
		}

		// 要求銷毀綁定中的控制項(視窗)
		::DestroyWindow(*this);
		this->SafeWndsWaiting();

		// 是主視窗嗎? 若是主視窗則送出結束訊息迴圈訊息。
		if (::GetWinapp().GetMainframe() == reinterpret_cast<LONG_PTR>(this)) {
			::PostQuitMessage(nExitCode);
		}
	}
}

/**
 *	@brief [虛擬函數] 釋放所有物件與資源
 *	@return 此函數沒有返回值
 */
void DmWnds::SafeWndsRelease()
{
	this->SafeUserRelease();	// 呼叫 '純' 虛擬函數，繼承者釋放函數。
	this->LooseWindow();		// 恢復原先的訊息處理 Callback 函數, 恢復使用者訊息
	this->DeleteSafeFont();		// 刪除使用者自定義字型

	this->SetSafePrevCallback(0);
	this->SetSafeFont(NULL);
	this->SetSafeHwnd(NULL);
}

/**
 *	@brief 等待視窗結束
 *	@return	此函數沒有返回值
 */
void DmWnds::SafeWndsWaiting()
{
	// 等待視窗被終結
	for (int i = 0; i < WNDS_DESTORY_OVERTIME; i++) {
		if (!this->IsWindow()) break;
		::Sleep(WNDS_DESTROY_WAITING);
	}
}

/**
 *	@brief 向系統註冊視窗，註冊後可用 CreateWindow() 或 CreateWindowEx() 建立視窗
 *	@param[in] smPtr 指向結構 WINSFRAME 結構緩衝區位址，內容為描述要建立的視窗資訊。
 *	@return <b>型別: BOOL</b>
 *		\n 若運作成功返回值為非零值，若運作失敗則返回值為零。
 *		\n 運作失敗可調用 GetLastError 取得錯誤訊息。
 */
BOOL DmWnds::SafeRegisterClass(const WNDSFRAME* smPtr)
{
	BOOL		bResult = FALSE;
	UINT		uStyle = CS_HREDRAW | CS_VREDRAW; // | CS_DBLCLKS;
	HINSTANCE	hInst = NULL;
	HCURSOR		hCusr = NULL;
	HICON		hIcon = NULL;
	HBRUSH		hBrsh = NULL;
	auto		fnWndProc = reinterpret_cast<WNDPROC>(this->GetSafeCallback());
	WNDCLASSEX	wcex;

	for (;;) {
		// 此物件已有視窗連接
		if (this->IsWindow()) {
			// error handling
			break;
		}
		// 無效的 WINSFRAME 結構指標
		if (smPtr == NULL) {
			// error handling
			break;
		}
		// 無效的視窗應用程式實例代碼
		hInst = smPtr->hInstance;
		if (hInst == NULL || hInst != ::GetWinapp().GetInstanceHandle()) {
				break;
		}
		// 無效的視窗 Class 名稱
		if (smPtr->pszClass == NULL) {
			// error handling
			break;
		}

		// 設定預設參數值
		uStyle |= smPtr->uClassStyle;
		hCusr = smPtr->hCursor;
		hIcon = smPtr->hIcon;
		hBrsh = smPtr->hBackground;

		// 檢視游標
		if (hCusr == NULL) {
			hCusr = ::LoadCursor(NULL, IDC_ARROW);
		}

		// 檢視圖示
		if (hIcon == NULL) {
			hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
		}

		// 檢視視窗背景顏色
		if (hBrsh == NULL) {
			// hBrsh = (HBRUSH)(COLOR_WINDOW + 1);
			hBrsh = reinterpret_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));
		}

		::memset(static_cast<void*>(&wcex), 0, sizeof(wcex));
		wcex.cbSize = sizeof(WNDCLASSEX);			// size of the WNDCLASSEX structure
		wcex.style = smPtr->uClassStyle;			// the window style
		wcex.lpfnWndProc = fnWndProc;				// the callback function
		wcex.cbClsExtra = 0;						// nothing to do, always 0
		wcex.cbWndExtra = 0;						// nothing to do, always 0
		wcex.hInstance = hInst;						// handle of instance
		wcex.hIcon = hIcon;							// handle of icon
		wcex.hCursor = hCusr;						// handle of cursor
		wcex.hbrBackground = hBrsh;					// handle of background
		wcex.lpszMenuName = NULL;					// pointer of menu name (string)
		wcex.lpszClassName = smPtr->pszClass;		// pointer of class name (string)
		wcex.hIconSm = smPtr->hIconSm;				// pointer of icon for small icon using
		bResult = ::RegisterClassEx(&wcex) != 0;	// 返回 ATOM 值，若成功返回非零值，註冊視窗唯一識別碼，若失敗將返回值為零。
		break;
	}
	return bResult;
}

/**
 *	@brief 建立視窗或控制項
 *	@param[in] smPtr 指向結構 WNDSFRAME 資料緩衝區位址，內容為描述要建立的視窗資訊。
 *	@return <b>型別: HWND</b>
 *		\n 若函數操作成功則返回值為非零值，值為視窗或控制項操作代碼。
 *		\n 若函數操作失敗則返回值為零。
 */
HWND DmWnds::SafeCreateWindows(const WNDSFRAME* smPtr)
{
	HINSTANCE hInst = NULL;
	HWND hWnd = NULL;

	for (;;) {
		// 已存在視窗連接，發出錯誤。
		if (this->IsWindow()) {
			// error handling
			break;
		}
		// 無效的 WINSFRAME 結構指標
		if (smPtr == NULL) {
			// error handling
			break;
		}
		// 無效的視窗應用程式實例代碼
		if ((hInst = smPtr->hInstance) == NULL) {
			hInst = ::GetWinapp().GetInstanceHandle();
			if (hInst == NULL) {
				// error handling
				break;
			}
		}
		// 無效的視窗 Class 名稱
		if (smPtr->pszClass == NULL) {
			// error handling
			break;
		}

		WNDSPARAM smParam;
		::memset(&smParam, 0, sizeof(smParam));
		smParam.lParam = reinterpret_cast<LPARAM>(this);
		smParam.wParam = reinterpret_cast<WPARAM>(smPtr->vParam);

		// 建立視窗，呼叫 Win32 API - CreatewindowEx
		hWnd = ::CreateWindowEx(
			smPtr->dwExStyle,		// window extern style
			smPtr->pszClass,		// window class name
			smPtr->pszCaption,		// window name
			smPtr->dwStyle,			// window style, "WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX"
			smPtr->nPosx,			// window left-top position, coordinate x
			smPtr->nPosy,			// window left-top position, coordinate y
			smPtr->nWidth,			// window width
			smPtr->nHeight,			// window height
			smPtr->hWndParent,		// handle of parent window
			smPtr->hMenuOrItemID,	// handle of menu or Item ID
			hInst,					// handle of instance
			reinterpret_cast<LPVOID>(&smParam));	// pointer of parameter
		break;
	}
	return hWnd;
}

/**
 *	@brief 取得控制項註冊 Class 名稱
 *	@param[in] uType 控制項種類代碼
 *	@return <b>型別: TCHAR*</b> \n 控制項註冊 Class 名稱
 */
const TCHAR* DmWnds::GetControlsClassName(UINT uType) const
{
	static const TCHAR* static_controls_class[] = {
		NULL,						// 空值 // TEXT("Empty")
		TEXT("Button"),				// Button
		TEXT("ComboBox"),			// Combo Box
		TEXT("Edit"),				// Edit control.
		TEXT("ListBox"),			// List box.
		TEXT("MDIClient"),			// MDI client window.
		TEXT("ScrollBar"),			// Scroll bar.
		TEXT("Static"),				// Static control.

		TEXT("ComboLBox"),			// List box contained in a combo box.
		TEXT("DDEMLEvent"),			// Dynamic Data Exchange Management Library (DDEML) events.
		TEXT("Message"),			// Message-only window.
		TEXT("#32768"),				// Menu.
		TEXT("#32769"),				// Desktop window.
		TEXT("#32770"),				// Dialog box. (32770)
		TEXT("#32771"),				// Task switch window.
		TEXT("#32772"),				// Icon titles.

		TEXT("ToolbarWindow32"),	// Toolbar
		TEXT("tooltips_class32"),	// Tooltip
		TEXT("msctls_statusbar32"), // Status bar
		TEXT("SysTreeView32"),		// Tree view
		TEXT("SysListView32"),		// List view
		TEXT("SysAnimate32"),		// Animation
		TEXT("SysHeader32"),		// Header
		TEXT("msctls_hotkey32"),	// Hot-key
		TEXT("msctls_progress32"),	// Progress bar
		TEXT("RICHEDIT"),			// Rich edit
		TEXT("msctls_updown32"),	// Up-Down
		TEXT("SysTabControl32"),	// Tab
		NULL						// 錯誤 // TEXT("Error!! Over list")
	};

	const auto len = static_cast<UINT>(sizeof(static_controls_class) / sizeof(static_controls_class[0]));
	return uType >= len ? NULL : static_controls_class[uType];
}

/**
 *	@brief 取得當前物件的控制項註冊 Class 名稱
 *	@return <b>型別: TCHAR*</b> \n 控制項註冊 Class 名稱
 */
const TCHAR* DmWnds::GetControlsClassName() const
{
	return this->GetControlsClassName(static_cast<UINT>(m_eType));
}

/**
 *	@brief 使用擴充控制項，加載通用控制項 DLL（Comctl32.dll），並從註冊通用控制項類。
 *		\n 應用程序必須在建立任何通用控制項前調用此函數
 *	@return 這個函數沒有返回值
 */
void DmWnds::InitCommControls() const
{
	// 在此設定一個靜態識別旗標，令所有繼承者，不會重複指定通用控制項
	static BOOL static_common_controls_flag = FALSE;
	INITCOMMONCONTROLSEX icce;

	if (!static_common_controls_flag) {
		// 指定啟用全部控制項
		icce.dwSize = sizeof(INITCOMMONCONTROLSEX);
		icce.dwICC = ICC_WIN95_CLASSES;
		if (::InitCommonControlsEx(&icce)) {
			static_common_controls_flag = TRUE;
		}
	}
}

/**
 *	@brief 視窗或控制項訊息處理 Callback 函數
 *	@param[in] hWnd		視窗或控制項操作代碼 (Handle)
 *	@param[in] uMessage	視窗訊息
 *	@param[in] wParam	訊息參數
 *	@param[in] lParam	訊息參數
 *	@return <b>型別: LRESULT</b> \n 訊息處理後返回值
 */
LRESULT CALLBACK DmWnds::SafeWndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	DmWnds* dmWnds = reinterpret_cast<DmWnds*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));

	// 控制項不存在
	if (dmWnds == NULL) {
		// 若使用 Win32 API CreateWindow() 或 CreateWindowEx() 建立的視窗或控制項，必定會產生此 WM_NCCREATE, WM_CREATE 訊息
		if (uMessage == WM_NCCREATE) {
			// 取得自定義額外參數
			WNDSPARAM* cParam = reinterpret_cast<WNDSPARAM*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
			if (cParam != NULL) {
				dmWnds = reinterpret_cast<DmWnds*>(cParam->lParam);
				if (dmWnds != NULL) {
					if (!dmWnds->BindWindow(hWnd)) {
						// 綁定失敗，返回建立視窗失敗。
						return -1;
					}
					return dmWnds->WndProc(uMessage, wParam, lParam);
				}
			}
			::PostMessage(hWnd, WM_CLOSE, 0, 0);
		}
		return ::DefWindowProc(hWnd, uMessage, wParam, lParam);
	}

	// 控制項已存在
	return dmWnds->WndProc(uMessage, wParam, lParam);
}
