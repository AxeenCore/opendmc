/**************************************************************************//**
 * @file	tabview.cc
 * @brief	DmTabView 標籤類別成員函數定義
 * @date	2000-10-10
 * @date	2018-09-15
 * @author	Swang
 *****************************************************************************/
#include "opendmc/wnds/tabview.hh"

/*
 *	@brief DmTabView 建構式
 */
DmTabView::DmTabView() : DmWnds(EmCtrls::TabControl) { }

/*
 *	@brief DmTabView 解構式
 *	@remark 解構時進行釋放(摧毀)控制項 
 */
DmTabView::~DmTabView() { this->DestroyMine(); }

/**
 *	@brief [重載] 建立一個組合框 Combo Box
 *	@param[in] szCaptionPtr	指向控制項名稱字串緩衝區位址，請直接填上 NULL。
 *	@param[in] x			起始位置 (左上角) X
 *	@param[in] y			起始最至 (左上角) Y
 *	@param[in] wd			寬度
 *	@param[in] ht			高度
 *	@param[in] hWndParent	父視窗操作代碼
 *	@param[in] nIDCItem		項目識別碼
 *	@param[in] hInstance	視窗應用程式實例操作代碼
 *	@return <b>型別: BOOL</b> \n 若函數操作成功返回值為非零值，值為視窗或控制項操作代碼。 \n 若函數操作失敗返回值為零。
 */
BOOL DmTabView::Create(const TCHAR* szCaptionPtr, int x, int y, int wd, int ht, HWND hWndParent, int nIDCItem)
{
	const DWORD dwStyle = TCS_FIXEDWIDTH;
	const DWORD dwExStyle = 0;
	WNDSCTRLS smCtrl;

	szCaptionPtr = NULL;
	::memset(&smCtrl, 0, sizeof(smCtrl));
	smCtrl.hInstance	= ::GetWinapp().GetInstanceHandle();
	smCtrl.hParent		= hWndParent;
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
 *	@brief 建立一個組合框樣本 (由繼承者發展)
 *	@return	<b>型別: BOOL</b> 若視窗建立成功返回值為非零值，若視窗建立失敗責返回值為零。
 */
BOOL DmTabView::CreateSample() { return FALSE; }

/**
 *	@breif 建立一個標籤卡與標籤頁面
 *	@param[in] dmPtr		指向 DmWnds 類別物件位址。(物件指標)
 *	@param[in] textPtr		指向標籤卡顯示名稱字串緩衝區位址
 *	@param[in] hIcon		標籤卡上顯示的圖示操作代碼
 *	@param[in] uIDTabItem	加入的項目識別碼
 *	@return <b>型別: BOOL</b> \n 若新增標籤成功返回值為非零值。 \n 若新增標籤失敗則返回值為零。
 */
BOOL DmTabView::AddTabPage(DmWnds* dmPtr, const TCHAR* textPtr, HICON hIcon, UINT uIDTabItem)
{
	BOOL bResult = FALSE;

	for (;;) {
		if (this->IsWindow()) {
			break;
		}

		if (dmPtr == NULL) {
			break;
		}

		if (textPtr == NULL || ::_tcslen(textPtr) > MAX_STRING_MENU) {
			break;
		}

		TABPAGEINFO smPage;
		::memset(&smPage, 0, sizeof(smPage));
		smPage.cObject = dmPtr;
		smPage.nIcon = hIcon == NULL ? -1 : -1;
		smPage.uIDTabItem = uIDTabItem;
		::_tcscpy(smPage.szText, textPtr);

		TCITEM smTci;
		::memset(&smTci, 0, sizeof(smTci));
		smTci.mask = TCIF_TEXT | TCIF_IMAGE;
		smTci.iImage = smPage.nIcon;
		smTci.pszText = reinterpret_cast<LPTSTR>(smPage.szText);
		if (this->InsertItem(static_cast<int>(m_vTabView.size()), &smTci) == -1) {
			break;
		}

		bResult = TRUE;
		m_vTabView.push_back(smPage);

		//SetTabSize();
		//SelectPage(iNewPage);
		//NotifyChanged();
		break;
	}

	return bResult;
}

/**
 *	@breif 建立一個標籤卡與標籤頁面
 *	@param[in] dmPtr		指向 DmWnds 類別物件位址。(物件指標)
 *	@param[in] textPtr		指向標籤卡顯示名稱字串緩衝區位址
 *	@param[in] uIDIcon		Icon 資源識別碼
 *	@param[in] uIDTabItem	加入的項目識別碼
 *	@return <b>型別: BOOL</b> \n 若新增標籤成功返回值為非零值。 \n 若新增標籤失敗則返回值為零。
 */
BOOL DmTabView::AddTabPage(DmWnds* dmPtr, const TCHAR* textPtr, UINT uIDIcon, UINT uIDTabItem)
{
	auto hIcon = reinterpret_cast<HICON>(::LoadImage(
		::GetWinapp().GetResourceHandle()
		, MAKEINTRESOURCE(uIDIcon)
		, IMAGE_ICON, 0, 0, LR_SHARED));
	return this->AddTabPage(dmPtr, textPtr, hIcon, uIDTabItem);
}

/**
 *	@brief 調整標籤工作區塊尺寸
 *	@param[in,out]	rcPtr	指向 RECT 結構緩衝區位址，用來指矩形尺寸並接收計算後的矩形尺寸。
 *	@param[in]		bMode	使用模式，預設值為 FALSE
 *	@return 此函數沒有返回值
 *	@remark bMode 預設值為設定為 FALSE，採用視窗相應的矩形區域。
 *		\n Set TRUE, specifies a display rectangle and receives the corresponding window rectangle
 *		\n Set FALSE, specifies a window rectangle and receives the corresponding display area.
 */
void DmTabView::AdjustRect(RECT* rcPtr, BOOL bMode) const
{
	assert(this->IsWindow());
	// TCM_ADJUSTRECT
	// wParam = 使用模式
	// lParam = 指向 RECT 結構緩衝區位址
	::SendMessage(*this, TCM_ADJUSTRECT, static_cast<WPARAM>(bMode), reinterpret_cast<LPARAM>(rcPtr));
}

/**
 *	@brief 刪除所有標籤的選項卡
 *	@return <b>型別: BOOL</b> \n 若函數操作成功返回值為非零值，若函數操作失敗則返回值為零。
 */
BOOL DmTabView::DeleteAllItem() const
{
	assert(this->IsWindow());
	// TCM_DELETEALLITEMS
	// wParam = 未使用，必須為零
	// lParam = 未使用，必須為零
	return static_cast<BOOL>(::SendMessage(*this, TCM_DELETEALLITEMS, 0, 0));
}

/**
 *	@brief 刪除一個標籤的選項卡
 *	@param[in] nCards 指定要刪除標籤的選項卡索引值，索引值從 0 起算。
 *	@return <b>型別: BOOL</b> \n 若函數操作成功返回值為非零值，若函數操作失敗則返回值為零。
 */
BOOL DmTabView::DeleteItem(int nCards) const
{
	assert(this->IsWindow());
	// TCM_DELETEITEM
	// wParam = 指定要刪除選項卡索引值，索引值從 0 起算。
	// lParam = 未使用，必須為零
	return static_cast<BOOL>(::SendMessage(*this, TCM_DELETEITEM, static_cast<WPARAM>(nCards), 0));
}

/**
 *	@brief 重置標籤的選項卡狀態
 *	@param[in] bDeSel 指定要重置的標籤方式，TRUR 表示重置當前被選取的選項卡狀態，FALSE 重置所有的選項卡狀態。
 *	@return 此函數沒有返回值
 *	@remark MSDN 說明 \n 重製選項卡的選項，清除任何狀態可用 TCIS_BUTTONPRESSED。
 */
void DmTabView::DeSelectAll(BOOL bDeSel) const
{
	assert(this->IsWindow());
	// TCM_DESELECTALL
	// wParam = 指定要重置的標籤方式
	// lParam = 未使用，必須為零
	::SendMessage(*this, TCM_DESELECTALL, static_cast<WPARAM>(bDeSel), 0);
}

/**
 *	@brief 取得目前被選定的選項卡焦點
 *	@return <b>型別: int</b> \n 若函數操作操作成功返回值為選項卡的索引值，若函數操作失敗返回值為 (-1)
 */
int DmTabView::GetCurselFocus() const
{
	assert(this->IsWindow());
	// TCM_GETCURFOCUS
	// wParam = 未使用，必須為零
	// lParam = 未使用，必須為零
	return static_cast<int>(::SendMessage(*this, TCM_GETCURFOCUS, 0, 0));
}

/**
 *	@brief 取得被選定標籤的選項卡
 *	@return <b>型別: int</b> \n 若函數操作操作成功返回值為選項卡的索引值，若函數操作失敗返回值為 (-1)
 */
int DmTabView::GetCursel() const
{
	assert(this->IsWindow());
	// TCM_GETCURSEL
	// wParam = 未使用，必須為零
	// lParam = 未使用，必須為零
	return static_cast<int>(::SendMessage(*this, TCM_GETCURSEL, 0, 0));
}

/**
 *	@brief 取得當前正在使用的標籤使用的擴展樣式。
 *	@return <b>型別: DWORD</b> \n 返回值為 DWORD 值，表示當前用於標籤的擴展樣式。
 */
DWORD DmTabView::GetExtendedStyle() const
{
	assert(this->IsWindow());
	// TCM_GETEXTENDEDSTYLE
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<DWORD>(::SendMessage(*this, TCM_GETEXTENDEDSTYLE, 0, 0));
}

/**
 *	@brief 取得與標籤關聯的圖像列表操作代碼
 *	@return <b>型別: HIMAGELIST</b> \n 若函數操作成功返回值為非零值，值為圖像列表操作代碼(HIMAGELIST)。 \n 若函數操作失敗則返回值為零(NULL)。
 */
HIMAGELIST DmTabView::GetImageList() const
{
	assert(this->IsWindow());
	// TCM_GETIMAGELIST
	// wParam = 不使用，必須為零
	// wParam = 不使用，必須為零
	return reinterpret_cast<HIMAGELIST>(::SendMessage(*this, TCM_GETIMAGELIST, 0, 0));
}

/**
 * @brief 取得指定標籤的選項卡資訊
 * @param[in]	nCards	指定選項卡索引值，索引值從 0 起算。
 * @param[out]	tciPtr	指向 TCITEM 結構緩衝區位址，用於設定和接收有關選項卡的的訊息。
 * @return <b>型別: BOOL</b> \n 若函數操作成功返回值為非零值，若函數操作失敗則返回值為零。
 */
BOOL DmTabView::GetItem(int nCards, TCITEM* tciPtr) const
{
	assert(this->IsWindow());
	// TCM_GETITEM
	// wParam = 指定選項卡索引值，從 0 算起。
	// lParam = 指向 TCITEM 結構緩衝區位址
	return static_cast<BOOL>(::SendMessage(*this, TCM_GETITEM, static_cast<WPARAM>(nCards), reinterpret_cast<LPARAM>(tciPtr)));
}

/**
 * @brief 取得標籤的選項卡數量
 * @return <b>型別: int</b> \n 若函數操作成功返回值為選項卡數量，若函數操作失敗則返回值為零。
 */
int DmTabView::GetItemCount() const
{
	assert(this->IsWindow());
	// TCM_GETITEMCOUNT
	// wParam 未使用，必須為零
	// lParam 未使用，必須為零
	return static_cast<int>(::SendMessage(*this, TCM_GETITEMCOUNT, 0, 0));
}

/**
 * @brief 取得標籤的選項卡邊界尺寸
 * @param[in]		nCards	指定選項卡索引值，索引值從 0 起算。
 * @param[in,out]	rcPtr	指向 RECT 結構緩衝區位址，用來接收標籤編輯框尺寸資料。
 * @return <b>型別: BOOL</b> \n 若函數操作成功返回值為非零值，若函數操作失敗則返回值為零。
 */
BOOL DmTabView::GetItemRect(int nCards, RECT* rcPtr) const
{
	assert(this->IsWindow());
	// TCM_GETITEMRECT
	// wParam = 指定選項卡索引值
	// lParam = 指向 RECT 結構緩衝區位址
	return static_cast<BOOL>(::SendMessage(*this, TCM_GETITEMRECT, static_cast<WPARAM>(nCards), reinterpret_cast<LPARAM>(rcPtr)));
}

/**
 *	@brief 取得當前標籤的多行選項卡列數
 *	@return <b>型別: int</b> \n 返回值為選項卡的行數
 *	@remark 只有具有 TCS_MULTILINE 樣式的的標籤才能有多行選項卡。
 */
int DmTabView::GetRowCount() const
{
	assert(this->IsWindow());
	// TCM_GETROWCOUNT
	// wParam = 0
	// lParam = 0
	return static_cast<int>(::SendMessage(*this, TCM_GETROWCOUNT, 0, 0));
}

/**
 *	@brief 取得標籤的提示工具操作代碼
 *	@return <b>型別: HWND</b> \n 若函數操作成功返回值為非零值，值為提示工具視窗操作代碼。 \n 若操作失敗則返回值為零。
 *	@remark 如果選項卡控件具有 TCS_TOOLTIPS 樣式，會創建提示工具控件。也可以使用 TCM_SETTOOLTIPS 訊息將提示工具控件分配給標籤。
 */
HWND DmTabView::GetToolTips() const
{
	assert(this->IsWindow());
	// TCM_GETTOOLTIPS
	// wParam = 0
	// lParam = 0
	return reinterpret_cast<HWND>(::SendMessage(*this, TCM_GETTOOLTIPS, 0, 0));
}

/**
 *	@brief 取得標籤使用 Unicode 狀態
 *	@return <b>型別: BOOL</b> \n 若標籤使用 Unicode 返回值為 TRUE，若使用 ANSI 返回值為 FALSE
 */
BOOL DmTabView::GetUnicodeFormat() const
{
	assert(this->IsWindow());
	// TCM_GETUNICODEFORMAT
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return ::SendMessage(*this, TCM_GETUNICODEFORMAT, 0, 0) != 0;
}

/**
 *	@brief 設定標籤項目成反白顯示狀態
 *	@param[in] nCards		標籤項目的索引值
 *	@param[in] bHighlight	指定反白顯示狀態。如果參數值為 TRUE 表示設為反白，若參數值為 FALSE 表示取消反白回預設狀態。
 *	@return <b>型別: BOOL</b> \n 若函數操作成功返回值為非零值。 \n 若函數操作失敗返回值為零。
 */
BOOL DmTabView::HighlightItem(int nCards, BOOL bHighlight) const
{
	assert(this->IsWindow());
	// TCM_HIGHLIGHTITEM
	// wParam = 標籤項目的索引值
	// lParam = 指定反白顯示狀態
	return static_cast<BOOL>(::SendMessage(*this, TCM_HIGHLIGHTITEM, static_cast<WPARAM>(nCards), static_cast<LPARAM>(MAKELPARAM(bHighlight, 0))));
}

/**
 *	@brief 檢查當前滑鼠游標在標籤的選項卡位置
 *	@param[in, out] hitPtr 指向 TCHITTESTINFO 結構緩衝區位址，用於指定要測試的螢幕位置。
 *	@return <b>型別: int</b> \n 若函數操作成功返回值為選項卡的索引值，若標選項卡不存在則返回值為 (-1)
 *	@remark 函數操作後，返回的狀態表 TCHITTESTINFO 結構成員 flags
 *		- TCHT_NOWHERE		\n 指定的位置超出標籤的範圍
 *		- TCHT_ONITEM		\n 指定的位置處於標籤的選項卡範圍內，但不在標籤的選項卡圖示或文字上。
 *		- TCHT_ONITEMICON	\n 指定的位置處於標籤的選項卡圖示上
 *		- TCHT_ONITEMLABEL	\n 指定的位置處於標籤的選項卡文字上
 */
int DmTabView::HitTest(TCHITTESTINFO* hitPtr) const
{
	assert(this->IsWindow());
	// TCM_HITTEST
	// wParam = 未使用，必須為零
	// lParam = 指向 TCHITTESTINFO 結構緩衝區位址
	return static_cast<int>(::SendMessage(*this, TCM_HITTEST, static_cast<WPARAM>(0), reinterpret_cast<LPARAM>(hitPtr)));
}

/**
 * @brief 插入(新增)一個標籤的選項卡
 * @param[in] nCards 要插入(新增)標籤的選項卡索引值，索引值從 0 起算。
 * @param[in] tciPtr 指向 TCITEM 結構緩衝區位址，用於指定選項卡的屬性
 * @return <b>型別: int</b>
 *		\n 若運作成功返回值為 (-1) 以外任意值，值為標籤的選項卡索引值。
 *		\n 若運作失敗返回值為 (-1)
 */
int DmTabView::InsertItem(int nCards, TCITEM* tciPtr) const
{
	assert(this->IsWindow());
	// TCM_INSERTITEM
	// wParam = 選項卡索引值
	// lParam = 指向 TCITEM 結構緩衝區位址
	return static_cast<int>(::SendMessage(*this, TCM_INSERTITEM, static_cast<WPARAM>(nCards), reinterpret_cast<LPARAM>(tciPtr)));
}

/**
 *	@brief 插入一個文字標籤的選項卡
 *	@param[in] nCards	指定插入(新增)標籤的選項卡索引值，索引值從 0 起算。
 *	@param[in] textPtr	指向文字字串緩衝區位址，用於指定選項卡的名稱文字字串。
 * @return <b>型別: int</b>
 *		\n 若運作成功返回值為 (-1) 以外任意值，值為標籤的選項卡索引值。
 *		\n 若運作失敗返回值為 (-1)
 */
int DmTabView::InsertItem(int nCards, const TCHAR* textPtr) const
{
	const BOOL bResult = FALSE;

	TCITEM tci;
	TCHAR szText[MAX_PATH];

	if (textPtr != NULL) {
		::_tcscpy(szText, textPtr);
		tci.mask = TCIF_TEXT | TCIF_IMAGE;
		tci.iImage = (-1);	// 不使用圖示
		tci.pszText = szText;
		return this->InsertItem(nCards, &tci);
	}
	return bResult;
}

/**
 *	@brief 移除標籤的影像列表中刪除圖像。
 *	@param[in] nImage 要移除的圖像索引值，索引值從 0 算起。
 *	@return 此函數沒有返回值
 */
void DmTabView::RemoveImage(int nImage) const
{
	// TCM_REMOVEIMAGE
	// wParam = 要刪除圖像的索引值
	// lParam = 0
	::SendMessage(*this, TCM_REMOVEIMAGE, static_cast<WPARAM>(nImage), 0);
}

/**
 *	@brief 設定標籤的選項卡成為焦點(被選定狀態)
 *	@param[in] nCards 指定要成為焦點的選項卡索引值，索引值從 0 起算。
 *	@return 此函數沒有返回值
 */
void DmTabView::SetCurselFocus(int nCards) const
{
	assert(this->IsWindow());
	// TCM_SETCURFOCUS
	// wParam = 指定要成為焦點的選項卡索引值
	// lParam = 未使用、必須為零
	::SendMessage(*this, TCM_SETCURFOCUS, static_cast<WPARAM>(nCards), 0);
}

/**
 * @brief 設定標籤的選項卡成為被選定狀態
 * @param[in] nCards 指定要成為被選定狀態標籤的選項卡索引值，索引值從 0 起算。
 * @return <b>型別: int</b> \n 若函數操作成功返回值為選項卡索引值，若函數操作失敗則返回值為 (-1)
 */
int DmTabView::SetCursel(int nCards) const
{
	assert(this->IsWindow());
	// TCM_SETCURSEL
	// wParam = 指定要成為被選定狀態標籤的項目索引值
	// lParam = 未使用、必須為零
	return static_cast<int>(::SendMessage(*this, TCM_SETCURSEL, static_cast<WPARAM>(nCards), 0));
}

/**
 *	@brief 設定標籤的擴充樣式
 *	@param[in] dwNewStyle	新的樣式
 *	@param[in] dwExMask		遮罩值，會進行與 dwNewStyle 遮罩動作，若此值為 0 將影響 dwNewStyle 全部設定。
 *	@return <b>型別: DWORD</b> \n 返回值為變更前的擴展樣式。
 */
DWORD DmTabView::SetExtendedStyle(DWORD dwNewStyle, DWORD dwExMask)
{
	assert(this->IsWindow());
	// TCM_SETEXTENDEDSTYLE
	// wParam = 遮罩值
	// lParam = 樣式
	return static_cast<DWORD>(::SendMessage(*this, TCM_SETEXTENDEDSTYLE, static_cast<WPARAM>(dwExMask), static_cast<LPARAM>(dwNewStyle)));
}

/**
 *	@brief 設定標籤的圖像列表操作代碼
 *	@param[in] hImageList 圖像列表的操作代碼
 *	@return <b>型別: HWND</b> \n 若函數操作成功返回非零值，值為改變設定前的圖像操作代碼。 \n 若函數操作失敗則返回值為零(NULL)
 */
HWND DmTabView::SetImageList(HWND hImageList) const
{
	assert(this->IsWindow());
	// TCM_SETIMAGELIST
	// wParam = 0
	// lParam = 圖像列表的操作代碼
	return reinterpret_cast<HWND>(::SendMessage(*this, TCM_SETIMAGELIST, 0, reinterpret_cast<LPARAM>(hImageList)));
}

/**
 * @brief 設定標籤的選項卡屬性，設定部分或全部的屬性於 TCITEM 結構。
 * @param[in]	nCards	指定標籤的選項卡索引值，索引值從 0 起算。
 * @param[out]	tciPtr	指向 TCITEM 結構緩衝區位址
 * @return <b>型別: BOOL</b> \n 若函數操作成功返回值為非零值，若函數操作失敗則返回值為零。
 */
BOOL DmTabView::SetItem(int nCards, TCITEM* tciPtr) const
{
	assert(this->IsWindow());
	// TCM_SETITEM
	// wParam = 指定選項卡索引值
	// lParam = 指向 TCITEM 結構緩衝區位址
	return static_cast<BOOL>(::SendMessage(*this, TCM_SETITEM, static_cast<WPARAM>(nCards), reinterpret_cast<LPARAM>(tciPtr)));
}

/**
 *	@brief 設定選項卡額外保留的空間
 *	@param[in] nBytes 空間大小
 *	@return <b>型別: BOOL</b> \n若函數操作成功返回值為非零值。 \n 若還數操作失敗返回值為零。
 */
BOOL DmTabView::SetItemExtra(int nBytes)
{
	// TCM_SETITEMEXTRA
	// wParam = 空間大小
	// lParam = 不使用，必須為零
	return ::SendMessage(*this, TCM_SETITEMEXTRA, static_cast<WPARAM>(nBytes), 0) != 0;
}

/**
 * @brief 設定標籤的尺寸大小，在固定的寬度或使用者繪製下，設定標籤的寬度與高度。
 * @param[in] wd 標籤寬度
 * @param[in] ht 標籤高度
 * @return <b>型別: LRESULT</b> \n 返回值為改變前的標籤尺寸，HIWORD 部分為高度、LOWORD 部分為寬度。
 */
LRESULT DmTabView::SetItemSize(int wd, int ht) const
{
	assert(this->IsWindow());
	// TCM_SETITEMSIZE
	// wParam = 未使用、必須為零
	// lParam = 標籤尺寸 (HIWORD = 高度) (LOWORD = 寬度)
	return ::SendMessage(*this, TCM_SETITEMSIZE, 0, MAKELPARAM(wd, ht));
}

/**
 * @brief 設定標籤最小寬度
 * @param[in] wd 標籤的寬度 (in pixel)，若此參數為 (-1), 表示使用系統預設的寬度。
 * @return <b>型別L INT</b> \n 返回值為改變前的標籤寬度
 */
INT DmTabView::SetMinTabWidth(int wd) const
{
	assert(this->IsWindow());
	// TCM_SETMINTABWIDTH
	// wParam = 未使用、必須為零
	// lParam = 標籤最小寬度，如果此參數為(-1)，則使用預設寬度。
	return static_cast<INT>(::SendMessage(*this, TCM_SETMINTABWIDTH, 0, static_cast<LPARAM>(wd)));
}

/**
 *	@brief 設置標籤的每個選項卡的圖標和標籤周圍的空間大小(填充距)
 *	@param[in] px 指定水平的填充量，以像素(pixel)為單位
 *	@param[in] py 指定垂直的填充量，以像素(pixel)為單位
 *	@return 此函數沒有返回值
 */
void DmTabView::SetPadding(int px, int py) const
{
	assert(this->IsWindow());
	// TCM_SETPADDING
	// wParam = 不使用，必須為零
	// lParam = 填充量
	::SendMessage(*this, TCM_SETPADDING, 0, MAKELPARAM(px, py));
}

/**
 *	@brief 設定選項卡使用的提示工具(ToolsTips)
 *	@param[in] hTips 提示工具操作代碼
 *	@return 此函數沒有返回值
 */
void DmTabView::SetToolTips(HWND hTips) const
{
	assert(this->IsWindow());
	// TCM_SETTOOLTIPS
	// wParam = 提示工具操作代碼
	// lParam = 不使用，必須為零
	::SendMessage(*this, TCM_SETTOOLTIPS, reinterpret_cast<WPARAM>(hTips), 0);
}

/**
 *	@brief 設定標籤使用 Unicode
 *	@param[in] bFormat 使用格式，若參數值為 TRUE 標示使用 Unicode，若參數值為 FALSE 表示使用 ANSI。
 *	@return 此函數沒有返回值
 */
void DmTabView::SetUnicodeFormat(BOOL bFormat) const
{
	assert(this->IsWindow());
	// TCM_SETUNICODEFORMAT
	// wParam = 使用格式
	// lParam = 不使用，必須為零
	::SendMessage(*this, TCM_SETUNICODEFORMAT, static_cast<WPARAM>(bFormat), 0);
}
