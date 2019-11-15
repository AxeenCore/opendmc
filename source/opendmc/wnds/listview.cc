/**************************************************************************//**
 * @file	listview.cc
 * @brief	DmListView 圖示列表類別成員函數定義
 * @date	2000-10-10
 * @date	2018-09-08
 * @author	Swang
 *****************************************************************************/
#include "opendmc/wnds/listview.hh"

/**
 *	@brief	DmListView 建構式
 *	@return	此函數沒有返回值
 */
DmListView::DmListView() : DmWnds(EmCtrls::ListView), m_nSelectItemCount(0), m_nSelectItemIndex(0) { }

/**
 *	@brief	DmListView 解構式
 *	@return	此函數沒有返回值
 */
DmListView::~DmListView() { if (this->IsWindow()) { this->RequestToDestroy(0); } }

/**
 *	@brief	[重載] 建立一個 ListView (Report 樣式)
 *	@param[in] szCaptionPtr	指向控制項名稱字串緩衝區位址，請直接填上 NULL。
 *	@param[in] x			起始位置 (左上角) X
 *	@param[in] y			起始最至 (左上角) Y
 *	@param[in] wd			寬度
 *	@param[in] ht			高度
 *	@param[in] hWndParent	父視窗操作代碼
 *	@param[in] nIDCItem		項目識別碼
 *	@param[in] hInstance	視窗應用程式實例操作代碼
 *	@return	<b>型別: BOOL</b> \n 若函數操作成功返回值為非零值，值為視窗或控制項操作代碼。 \n 若函數操作失敗返回值為零。
 */
BOOL DmListView::Create(const TCHAR* szCaptionPtr, int x, int y, int wd, int ht, HWND hWndParent, int nIDCItem)
{
	const DWORD dwStyle = LVS_REPORT | LVS_SHOWSELALWAYS;
	WNDSCTRLS smCtrl;

	szCaptionPtr = NULL;
	::memset(&smCtrl, 0, sizeof(smCtrl));
	smCtrl.hInstance	= ::GetWinapp().GetInstanceHandle();
	smCtrl.hParent		= hWndParent;
	smCtrl.pszCaption	= szCaptionPtr;
	smCtrl.dwStyle		= dwStyle;
	smCtrl.dwExStyle	= 0;	// 延伸擴展樣式，ListView 無法直接在 CreateWindowEx 設定。
	smCtrl.nPosx		= x;
	smCtrl.nPosy		= y;
	smCtrl.nWidth		= wd;
	smCtrl.nHeight		= ht;
	smCtrl.nIDCItem		= nIDCItem;

	if (this->CreateControls(&smCtrl)) {
		this->DefaultReportStyle();
		return TRUE;
	}
	return FALSE;
}

/**
 *	@brief 建立一個 ListView 樣本 (由繼承者發展)
 *	@return	<b>型別: BOOL</b> 若視窗建立成功返回值為非零值，若視窗建立失敗責返回值為零。
 */
BOOL DmListView::CreateSample() { return FALSE; }

/**
 *	@brief	計算 ListView 的項目數量，顯示時所需的寬度和高度(近似值)
 *	@param[in]	wd	寬度，以像素為單位(in pixel)，若值為 -1 表示使用當前寬度 (預設 -1)
 *	@param[in]	ht	高度，以像素為單位(in pixel)，若值為 -1 表示使用當前高度 (預設 -1)
 *	@param[in]	nCount	 ListView 的項目要顯示的數量，若值為 -1 表示使用當前列表的項目全部數量 (預設 -1)
 *	@return	<b>型別: DWORD</b> \n 返回一個 DWORD 值，保存顯示項目所需的近似寬度（LOWORD）和高度（HIWORD），以像素為單位(in pixel)。
 */
DWORD DmListView::ApproximateViewRec(int wd, int ht, int nCount) const
{
	assert(this->IsWindow());
	// LVM_APPROXIMATEVIEWRECT
	// wParam	顯示數量
	// lParam	LOWORD 部分為 X 維度，HIWORD 部分為 Y 維度。
	return static_cast<DWORD>(::SendMessage(*this, LVM_APPROXIMATEVIEWRECT, static_cast<WPARAM>(nCount), static_cast<LPARAM>(MAKELPARAM(wd, ht))));
}

/**
 *	@brief	圖示模式的項目排列
 *	@param[in] uAlign 指定對齊方式
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值，否則返回值為零。
 *	@remark	<b>uAlign 對齊方式</b>
 *		- LVA_ALIGNLEFT		\n 未實現。改為應用 LVS_ALIGNLEFT 樣式。
 *		- LVA_ALIGNTOP		\n 未實現。改為應用 LVS_ALIGNTOP 樣式。
 *		- LVA_DEFAULT		\n 根據列表的當前對齊樣式（預設值）對齊項目。
 *		- LVA_SNAPTOGRID	\n 將所有圖示移動到最近的網格位置。
 */
BOOL DmListView::Arrange(UINT uAlign) const
{
	assert(this->IsWindow());
	// LVM_ARRANGE
	// wParam = 指定對齊方式
	// lParam = 不使用，必須為零
	return ::SendMessage(*this, LVM_ARRANGE, static_cast<WPARAM>(uAlign), static_cast<LPARAM>(0)) != 0;
}

/**
 *	@brief	取消 ListView 的項目文字操作
 *	@return	此函數沒有返回值
 */
void DmListView::CancelEditLable() const
{
	assert(this->IsWindow());
	// LVM_CANCELEDITLABEL
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	::SendMessage(*this, LVM_CANCELEDITLABEL, 0, 0);
}

/**
 *	@brief	建立一個可以拖拉的 ImageList
 *	@param[in]	nItem		項目索引值
 *	@param[out] pointPtr	指向 POINT 結構資料位址，用來接收圖示項目起始座標(左上位置)。
 *	@return	<b>型別: HIMAGELIST</b>
 *		\n 若成功返回值為非零值，值為可拖動的 ImageList 操作代碼。
 *		\n 若失敗返回值為零。
 */
HIMAGELIST DmListView::CreateDragImage(int nItem, POINT* pointPtr) const
{
	assert(this->IsWindow());
	// LVM_CREATEDRAGIMAGE
	// wParam = 項目索引值
	// lParam = 指向 POINT 結構資料位址
	return reinterpret_cast<HIMAGELIST>(::SendMessage(*this, LVM_CREATEDRAGIMAGE, static_cast<WPARAM>(nItem), reinterpret_cast<LPARAM>(pointPtr)));
}

/**
 *	@brief	刪除 ListView 所有的項目
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值，否則返回值為零。
 */
BOOL DmListView::DeleteAllItem() const
{
	assert(this->IsWindow());
	// LVM_DELETEALLITEMS
	// wParam = 未使用，必須為零
	// lParam = 未使用，必須為零
	return ::SendMessage(*this, LVM_DELETEALLITEMS, 0, 0) != 0;
}

/**
 *	@brief	刪除一個欄位(Column)
 *	@param[in] nColumn 欄位索引值，所以值從 0 算起。
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值，否則返回值為零。
 */
BOOL DmListView::DeleteColumn(int nColumn) const
{
	assert(this->IsWindow());
	// LVM_DELETECOLUMN
	// wParam = 欄位索引值
	// lParam = 未使用，必須為零
	return ::SendMessage(*this, LVM_DELETECOLUMN, static_cast<WPARAM>(nColumn), static_cast<LPARAM>(0)) != 0;
}

/**
 *	@brief	刪除所有的欄位(Column)
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值，否則返回值為零。
 */
BOOL DmListView::DeleteAllColumn() const
{
	auto count = this->GetColumnCount();
	while (count > 0) {
		this->DeleteColumn(0);
		count--;
	}
	return count == 0;
}

/**
 *	@brief	刪除一個項目(Item)
 *	@param[in] nItem 項目索引值，索引值從 0 算起。
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值，否則返回值為零。
 */
BOOL DmListView::DeleteItem(int nItem) const
{
	assert(this->IsWindow());
	// LVM_DELETEITEM
	// wParam = 項目索引值
	// lParam = 未使用，必須為零
	return ::SendMessage(*this, LVM_DELETEITEM, static_cast<WPARAM>(nItem), static_cast<LPARAM>(0)) != 0;
}

/**
 *	@brief	直接於項目的欄位中，直接編輯文字(like editbox)
 *	@param[in] nItem 項目索引值，若要取消編輯則輸入 (-1)，索引值從 0 算起。
 *	@return	此函數沒有返回值
 */
void DmListView::EditLabel(int nItem) const
{
	assert(this->IsWindow());
	// LVM_EDITLABEL
	// wParam = 列表的項目索引值
	// lParam = 未使用，必須為零
	::SendMessage(*this, LVM_EDITLABEL, static_cast<WPARAM>(nItem), static_cast<LPARAM>(0));
}

/**
 *	@brief	啟用或禁用項目以群組顯示
 *	@param[in] bView 指定啟用或禁用
 *	@return	<b>型別: int</b> \n 指定動作返回值如下:
 *		- 返回值為 = 0	\n	已啟用或禁用將 ListView 項目顯示為組的功能。
 *		- 返回值為 = 1	\n	控制狀態已成功更改。
 *		- 返回值為 = -1	\n	操作失敗。
 */
int DmListView::EnableGroupView(BOOL bView) const
{
	assert(this->IsWindow());
	// LVM_ENABLEGROUPVIEW
	// wParam = 指定啟用或禁用
	// lParam = 不使用，必須為零
	return static_cast<int>(::SendMessage(*this, LVM_ENABLEGROUPVIEW, static_cast<WPARAM>(bView), static_cast<LPARAM>(0)));
}

/**
 *	@brief	確保 ListView 的項目為可視狀態
 *	@param[in]	nItem		項目的索引值
 *	@param[in]	bPartial	指定項目是否完全可見。(TRUE / FALSE)
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值，否則返回值為零。
 */
BOOL DmListView::EnsureVisible(int nItem, BOOL bPartial) const
{
	assert(this->IsWindow());
	// LVM_ENSUREVISIBLE
	// wParam = 列表項目的索引值
	// lParam = 指定列表的項目是否完全可見
	return ::SendMessage(*this, LVM_ENSUREVISIBLE, static_cast<WPARAM>(nItem), static_cast<LPARAM>(bPartial)) != 0;
}

/**
 *	@brief	找尋項目，指定特徵條件進行找尋。
 *	@param[in]	nStart		指定開始尋找項目的索引值，或指定 (-1) 從最開頭開始搜尋，指定的項目本身會被排除。
 *	@param[in]	findInfoPtr	指向 LVFINDINFO 結構資料位址，內容為尋找項目的訊息。
 *	@return	<b>型別: int</b> \n 若成功返回值為項目的索引值，否則返回值為 (-1)。
 */
int DmListView::FindItem(int nStart, LVFINDINFO* findInfoPtr) const
{
	assert(this->IsWindow());
	// LVM_FINDITEM
	// wParam = 指定開始搜尋的項目索引值
	// lParam = 指向 LVFINDINFO 結構資料位址
	return static_cast<int>(::SendMessage(*this, LVM_FINDITEM, static_cast<WPARAM>(nStart), reinterpret_cast<LPARAM>(findInfoPtr)));
}

/**
 *	@brief	取得列表背景顏色
 *	@return	<b>型別: COLORREF</b> \n 返回值為 ListView 的背景顏色。
 */
COLORREF DmListView::GetBkColor() const
{
	assert(this->IsWindow());
	// LVM_GETBKCOLOR
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<COLORREF>(::SendMessage(*this, LVM_GETBKCOLOR, 0, 0));
}

/**
 *	@brief	取得 ListView 背景圖案
 *	@param[out] lvbkiPtr 指向 LVBKIMAGE 結構資料位址，用來接收背景圖形。
 *	@return	<b>型別: BOOL</b> \ 若成功返回值為非零值，否則返回值為零。
 */
BOOL DmListView::GetBkImage(LVBKIMAGE* lvbkiPtr) const
{
	assert(this->IsWindow());
	// LVM_GETBKIMAGE
	// wParam = 不使用，必須為零
	// lParam = 指向 LVBKIMAGE 結構資料位址
	return ::SendMessage(*this, LVM_GETBKIMAGE, 0, reinterpret_cast<LPARAM>(lvbkiPtr)) != 0;
}

/**
 *	@brief	取得列表的 callback mask
 *	@return	<b>型別: UINT</b> \n 返回值為列表的 callback mask
 */
UINT DmListView::GetCallbackMask() const
{
	assert(this->IsWindow());
	// LVM_GETCALLBACKMASK
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<int>(::SendMessage(*this, LVM_GETCALLBACKMASK, 0, 0));
}

/**
 *	@brief	取得指定欄位(Column)文字內容
 *	@param[in]	nColumn		欄位索引值，索引值從 0 算起。
 *	@param[out]	szColumnPtr	指向文字存放緩衝區位址
 *	@param[in]	ccMax		欲取得文字內容長度 (in TCHAR)
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值，否則返回值為零。
 */
BOOL DmListView::GetColumn(int nColumn, LPTSTR szColumnPtr, int ccMax) const
{
	LVCOLUMN lvc;

	assert(this->IsWindow());

	if (NULL == szColumnPtr) return FALSE;
	::memset((void*)&lvc, 0, sizeof(LVCOLUMN));
	lvc.mask = LVCF_TEXT;
	lvc.pszText = szColumnPtr;
	lvc.cchTextMax = ccMax;

	// LVM_GETCOLUMN
	// wParam = 欄位索引值
	// lParam = 指向 LVCOLUMN 結構資料位址
	return ::SendMessage(*this, LVM_GETCOLUMN, static_cast<WPARAM>(nColumn), reinterpret_cast<LPARAM>(&lvc)) != 0;
}

/**
 *	@brief	取得欄位(Column)數量
 *	@return	<b>型別: int</b> \n 若成功返回值為欄位(Column)數量，否則為失敗返回值為 (-1)。
 *	@remark	Win32 API 並無提供直接刪除所有欄位(Column)的功能，所以必須先取得 ListView 的 HDM 然後去取得 Header 數量。
 */
int DmListView::GetColumnCount() const
{
	assert(this->IsWindow());

	// 取得 HDM Handle
	auto hWnd = reinterpret_cast<HWND>(::SendMessage(*this, LVM_GETHEADER, 0, 0));
	if (hWnd != NULL) {
		// HDM_GETITEMCOUNT
		// wParam = 未使用，必須為零
		// lParam = 未使用，必須為零
		return static_cast<int>(::SendMessage(*this, HDM_GETITEMCOUNT, 0, 0));
	}
	return -1;
}

/**
 *	@brief	取得欄位當前的順序(由左至右的順序)
 *	@param[in]	nCount	指定欄位的數量
 *	@param[out]	intPtr	指向整數數組緩衝區位址，用來接收列表欄位(Column)的索引值。緩衝區必須足夠大以容納 nCount 指定的數量。
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值，否則返回值為零。
 */
BOOL DmListView::GetColumnOrderArray(int nCount, int* intPtr) const
{
	assert(this->IsWindow());
	// LVM_GETCOLUMNORDERARRAY
	// wParam = 指定列表欄位的數量
	// lParam = 指向整數數組緩衝區位址
	return ::SendMessage(*this, LVM_GETCOLUMNORDERARRAY, static_cast<WPARAM>(nCount), reinterpret_cast<LPARAM>(intPtr)) != 0;
}

/**
 *	@brief	取得欄位(Column)寬度
 *	@param[in] nColumn 欄位索引值，索引值從 0 算起。
 *	@return	<b>型別: int</b> \n 若成功返回值為寬度數據。 \n 若失敗則返回值為零。
 */
int DmListView::GetColumnWidth(int nColumn) const
{
	assert(this->IsWindow());
	// LVM_GETCOLUMNWIDTH
	// wParam = 欄位的項目索引值
	// lParam = 未使用，必須為零
	return static_cast<int>(::SendMessage(*this, LVM_GETCOLUMNWIDTH, static_cast<WPARAM>(nColumn), static_cast<LPARAM>(0)));
}

/**
 *	@brief	取得目前可視範圍的項目(item)數量
 *	@return	<b>型別: int</b>
 *		\n 若成功，返回值為當前可視範圍列表的項目(item)數量
 *		\n 若為圖示或小圖示模式，返回值為所有於 ListView 的圖示數量。
 */
int DmListView::GetCountPerPage() const
{
	assert(this->IsWindow());
	// LVM_GETCOUNTPERPAGE
	// wParam = 未使用，必須為零
	// lParam = 未使用，必須為零
	return static_cast<int>(::SendMessage(*this, LVM_GETCOUNTPERPAGE, 0, 0));
}

/**
 *	@brief	取得項目編輯框(EditBox)的操作代碼。
 *	@return	<b>型別: HWND</b> \n 若成功返回值為非零值，值為編輯框的操作代碼。 \n 若失敗返回值為零。
 *	@remark	要具備可編輯動作功能，必須先向父視窗發出 LVN_BEGINLABELEDIT 訊息通知，並實現 LVN_BEGINLABELEDIT 作業方式。
 */
HWND DmListView::GetEditControl() const
{
	assert(this->IsWindow());
	// LVM_GETEDITCONTROL
	// wParam = 未使用，必須為零
	// lParam = 未使用，必須為零
	return reinterpret_cast<HWND>(::SendMessage(*this, LVM_GETEDITCONTROL, 0, 0));
}

/**
 *	@brief	取得 ListView 空白文字？搞不清楚這是要做啥，先完成函數實作。
 *	@param[in]		ccLen	指向文字字串緩衝區大小，包含 null 結尾，單位 TCHAR。
 *	@param[in,out]	textPre	指向文字字串緩衝區位址， 用來接收文字字串。
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值，否則返回值為零。
 */
BOOL DmListView::GetEmptyText(size_t ccLen, TCHAR* textPre) const
{
	assert(this->IsWindow());
	// LVM_GETEMPTYTEXT
	// wParam = 指定字串長度
	// lParam = 指向文字字串緩衝區位址
	return ::SendMessage(*this, LVM_GETEMPTYTEXT, static_cast<WPARAM>(ccLen), reinterpret_cast<LPARAM>(textPre)) != 0;
}

/**
 *	@brief	取得 ListView 的擴展樣式
 *	@return	<b>型別: DWORD</b> \n 返回值為一個 DWORD，表示當前 ListView 的擴展樣式，擴展樣式參照 Extended List-View Styles
 *	@see	https://docs.microsoft.com/en-us/windows/desktop/controls/extended-list-view-styles
 */
DWORD DmListView::GetExtendedListViewStyle() const
{
	assert(this->IsWindow());
	// LVM_GETEXTENDEDLISTVIEWSTYLE
	// wParam = 未使用，必須為零
	// lParam = 未使用，必須為零
	return static_cast<DWORD>(::SendMessage(*this, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0));
}

/**
 *	@brief	取得 ListView 當前具有輸入焦點的群組(Group)
 *	@return	<b>型別: int</b> \n 返回值為 LVGS_FOCUSED 群組的索引，如果沒有群組，則返回值為 (-1)。
 */
int DmListView::GetFocusedGroup() const
{
	// LVM_GETFOCUSEDGROUP
	// wParam = 未使用，必須為零
	// lParam = 未使用，必須為零
	return static_cast<int>(::SendMessage(*this, LVM_GETFOCUSEDGROUP, 0, 0));
}

/**
 *	@brief	Contains information on a footer in a list-view control.
 *	@param[out]	lvfInfoPtr	指向 LVFOOTERINFO 結構資料存放位址
 *	@return	<b>型別: BOOL</b> \n 返回值永遠為 TRUE
 *	@remark	若執行成功，會將結果存放於指定的緩衝區。
 */
BOOL DmListView::GetFooterInfo(LVFOOTERINFO* lvfInfoPtr) const
{
	assert(this->IsWindow());
	// LVM_GETFOOTERINFO
	// wParam = 不使用，必須為零
	// lParam = 指向 LVFOOTERINFO 結構緩衝區位址
	return static_cast<BOOL>(::SendMessage(*this, LVM_GETFOOTERINFO, static_cast<WPARAM>(0), reinterpret_cast<LPARAM>(lvfInfoPtr)));
}

/**
 *	@brief	Gets information on a footer item for a specified list-view control.
 *	@param[in]	nItem		index of the item
 *	@param[out]	lvfItemPtr	指向 LVFOOTERITEM 結構資料區位址
 *	@return	<b>型別: BOOL</b>
 *		\n 若成功返回值為非零值 (TRUE)，結果存放於 LVFOOTERITEM 結構中。
 *		\n 若失敗返回值為零 (FALSE)
 *	@remark	若執行成功，會將結果存放於指定的緩衝區。
 */
BOOL DmListView::GetFooterItem(int nItem, LVFOOTERITEM* lvfItemPtr) const
{
	assert(this->IsWindow());
	// LVM_GETGROUPCOUNT
	// wParam = 項目索引值 (zero base)
	// lParam = 指向 LVFOOTERITEM 結構資料區位址
	auto wParam = static_cast<WPARAM>(nItem);
	auto lParam = reinterpret_cast<LPARAM>(lvfItemPtr);
	return static_cast<BOOL>(::SendMessage(*this, LVM_GETFOOTERITEM, wParam, lParam));
}

/**
 *	@brief	Gets the coordinates of a footer for a specified item in a list-view control.
 *	@param[in]	nItem	Index of then item.
 *	@param[out]	rcPtr	指向 RECT 結構緩衝區位址。
 *	@return	<b>型別: BOOL</b>
 *		\n 若成功返回值為非零值 (TRUE)，結果存放於 LVFOOTERITEM 結構中。
 *		\n 若失敗返回值為零 (FALSE)
 *	@remark	若執行成功，會將結果存放於指定的緩衝區。
 */
BOOL DmListView::GetFooterItemRect(int nItem, RECT* rcPtr) const
{
	assert(this->IsWindow());
	// LVM_GETFOOTERITEMRECT
	// wParam = 項目索引值 (zero base)
	// lParam = 指向 RECT 結構緩衝區位址。
	auto wParam = static_cast<WPARAM>(nItem);
	auto lParam = reinterpret_cast<LPARAM>(rcPtr);
	return static_cast<BOOL>(::SendMessage(*this, LVM_GETFOOTERITEMRECT, wParam, lParam));
}

/**
 *	@brief	Retrieves the coordinates of the footer for a list-view control.
 *	@param[out]	指向 RECT 結構緩衝區位址。
 *	@return	<b>型別: BOOL</b>
 *		\n 若成功返回值為非零值 (TRUE)，結果存放於 LVFOOTERITEM 結構中。
 *		\n 若失敗返回值為零 (FALSE)
 *	@remark	若執行成功，會將結果存放於指定的緩衝區。
 */
BOOL DmListView::GetFooterRect(RECT* rcPtr)
{
	assert(this->IsWindow());
	// LVM_GETFOOTERITEMRECT
	// wParam = 不使用，必須為零
	// lParam = 指向 RECT 結構緩衝區位址。
	return static_cast<BOOL>(::SendMessage(*this, LVM_GETFOOTERRECT, static_cast<WPARAM>(0), reinterpret_cast<LPARAM>(rcPtr)));
}

/**
 *	@brief	取得群組(Group)數量
 *	@return	<b>型別: int</b> \n 返回值為群組數量
 */
int DmListView::GetGroupCount() const
{
	assert(this->IsWindow());
	// LVM_GETGROUPCOUNT
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<int>(::SendMessage(*this, LVM_GETGROUPCOUNT, 0, 0));
}

/**
 *	@brief	取得指定群組資訊
 *	@param[in]	nIDGroup	群組識別碼(ID)
 *	@param[out]	lvgPtr		指向 LVGROUP 結構資料位址，用來接收群組資訊，其成員 cbSize 需設定為 sizeof(LVGROUP)
 *	@return	<b>型別: int</b> \n 若成功返回值為群組的ID，若失敗返回值為 (-1)。
 */
int DmListView::GetGroupInfo(int nIDGroup, LVGROUP* lvgPtr) const
{
	assert(this->IsWindow());
	// LVM_GETGROUPINFO
	// wParam = 群組識別碼
	// lParam = 指向 LVGROUP 結構資料位址
	return static_cast<int>(::SendMessage(*this, LVM_GETGROUPINFO, static_cast<WPARAM>(nIDGroup), reinterpret_cast<LPARAM>(lvgPtr)));
}

/**
 *	@brief	取得指定群組資訊
 *	@param[in]	nGroupIndex	群組索引值
 *	@param[out]	lvgPtr 指向 LVGROUP 結構資料位址，用來接收群組資訊，其成員 cbSize 需設定為 sizeof(LVGROUP)
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值，否則返回值為零。
 */
BOOL DmListView::GetGroupInfoByIndex(int nGroupIndex, LVGROUP* lvgPtr) const
{
	assert(this->IsWindow());
	// LVM_GETGROUPINFOBYINDEX
	// wParam = 群組索引值
	// lParam = 指向 LVGROUP 結構資料位址
	return static_cast<int>(::SendMessage(*this, LVM_GETGROUPINFOBYINDEX, static_cast<WPARAM>(nGroupIndex), reinterpret_cast<LPARAM>(lvgPtr)));
}

/**
 *	@brief	取得群組顯示相關資訊
 *	@param[out] lvgmPtr 指向 LVGROUPMETRICS 結構資料位址，用於接收群組顯示相關資訊。
 *	@return	此函數沒有返回值
 */
void DmListView::GetGroupMetrics(LVGROUPMETRICS* lvgmPtr) const
{
	assert(this->IsWindow());
	// LVM_GETGROUPMETRICS
	// wParam = 不使用，必須為零
	// lParam = 指向 LVGROUPMETRICS 結構資料位址
	::SendMessage(*this, LVM_GETGROUPINFOBYINDEX, static_cast<WPARAM>(0), reinterpret_cast<LPARAM>(lvgmPtr));
}

/**
 *	@brief	取得指定群組的矩形邊界資訊
 *	@param[in]	nIDGroup	群組的識別碼(ID)
 *	@param[in]	nType		要取得群組的類型
 *	@param[out] rcPtr		指向 RECT 結構資料位址，用來接收指定群組的矩形邊界資訊，傳入時 top 成員內容為要取得群組的類型。
 *	@return	<b>型別: BOOL</b> \n \n 若成功返回值為非零值，否則返回值為零。
 */
BOOL DmListView::GetGroupRect(int nIDGroup, LONG nType, RECT* rcPtr) const
{
	assert(this->IsWindow());
	// LVM_GETGROUPRECT
	// wParam = 群組的識別碼
	// lParam = 指向 RECT 結構資料位址
	if (rcPtr != NULL) rcPtr->top = nType;
	return ::SendMessage(*this, LVM_GETGROUPRECT, static_cast<WPARAM>(nIDGroup), reinterpret_cast<LPARAM>(rcPtr)) != 0;
}

/**
 *	@brief	取得指定群組狀態
 *	@param[in] nIDGroup	群組識別碼(ID)
 *	@param[in] dwMask	要取得的狀態值
 *	@return	<b>型別: UINT</b> \n 返回值為已設置的狀態值的組合，如果找不到該群組則返回值為零。
 */
UINT DmListView::GetGroupState(int nIDGroup, DWORD dwMask) const
{
	assert(this->IsWindow());
	// LVM_GETGROUPSTATE
	// wParam = 群組識別碼
	// lParam = 要取得的狀態值
	return static_cast<UINT>(::SendMessage(*this, LVM_GETGROUPSTATE, static_cast<WPARAM>(nIDGroup), static_cast<LPARAM>(dwMask)));
}

/**
 *	@brief	取的 ListView Header 操作代碼
 *	@return	<b>型別: HWND</b> \n 返回值為 Header 控制像操作代碼
 */
HWND DmListView::GetHeader() const
{
	assert(this->IsWindow());
	// LVM_GETHEADER
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return reinterpret_cast<HWND>(::SendMessage(*this, LVM_GETHEADER, 0, 0));
}

/**
 *	@brief	Retrieves the HCURSOR value used when the pointer is over an item while hot tracking is enabled.
 *	@return	<b>型別: HCURSOR</b> Returns an HCURSOR value that is the handle to the cursor that the list-view control uses when hot tracking is enabled.
 */
HCURSOR DmListView::GetHotCursor() const
{
	assert(this->IsWindow());
	// LVM_GETHOTCURSOR
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return reinterpret_cast<HCURSOR>(::SendMessage(*this, LVM_GETHOTCURSOR, 0, 0));
}

/**
 *	@brief	取得 Hot 項目索引值
 *	@return	<b>型別: int</b> \n 返回 Hot 項目索引值
 *	@remark	Hot 為滑鼠游標指到的項目
 */
int DmListView::GetHotItem() const
{
	assert(this->IsWindow());
	// LVM_GETHOTITEM
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<int>(::SendMessage(*this, LVM_GETHOTITEM, 0, 0));
}

/**
 *	@brief	取得滑鼠游標在 ListView 項目上選中前須停留的時間，單位毫秒。
 *	@return	<b>型別: DWORD</b> \n 返回值為取滑鼠游標在 ListView 項目上停留的時間。
 */
DWORD DmListView::GetHoverTime() const
{
	assert(this->IsWindow());
	// LVM_GETHOVERTIME
	// wParam = 不是用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<DWORD>(::SendMessage(*this, LVM_GETHOVERTIME, 0, 0));
}

/**
 *	@brief	取得 ListView 操作代碼
 *	@param[in] iImageList 要取得 Image list 的模式
 *	@return	<b>型別: HIMAGELIST</b> \n 若成功返回值為非零值，值為圖示的操作代碼。 \n 若失敗返回值為零。
 *	@remark	<b>iImageList 參數說明</b>
 *		- LVSIL_NORMAL		\n 大圖示的圖示列表
 *		- LVSIL_SMALL		\n 小圖示的圖像列表
 *		- LVSIL_STATE		\n 帶狀態的圖示列表
 *		- LVSIL_GROUPHEADER	\n 有群組的圖像列表。
 */
HIMAGELIST DmListView::GetImageList(int iImageList) const
{
	assert(this->IsWindow());
	// LVM_GETIMAGELIST
	// wParam = 使用模式
	// lParam = 不使用，必須為零
	return reinterpret_cast<HIMAGELIST>(::SendMessage(*this, LVM_GETIMAGELIST, static_cast<WPARAM>(iImageList), static_cast<LPARAM>(0)));
}

/**
 *	@brief 取得插入點的位置
 *	@param[out] lvimPtr 指向 LVINSERTMARK 結構資料位址，用來接收插入點的位置資訊
 *	@return	<b>型別: BOOL</b>
 *		\n 若成功返回值為非零值。
 *		\n 若失敗返回值為零，如果 LVINSERTMARK 結構的 cbSize 成員的大小不等於結構的實際大小，則返回值為零。
 */
BOOL DmListView::GetInsertMark(LVINSERTMARK* lvimPtr) const
{
	assert(this->IsWindow());
	// LVM_GETINSERTMARK
	// wParam = 不使用，必須為零
	// lParam = 指向 LVINSERTMARK 結構資料位址
	return ::SendMessage(*this, LVM_GETINSERTMARK, static_cast<WPARAM>(0), reinterpret_cast<LPARAM>(lvimPtr)) != 0;
}

/**
 *	@brief	取得插入點的顏色
 *	@return	<b>型別: COLORREF</b> \n 返回值為顏色
 */
COLORREF DmListView::GetInsertMarkColor() const
{
	assert(this->IsWindow());
	// LVM_GETINSERTMARKCOLOR
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<COLORREF>(::SendMessage(*this, LVM_GETINSERTMARKCOLOR, 0, 0));
}

/**
 *	@brief	取得插入點的矩形範圍
 *	@param[out] rectPtr	指向 RECT 結構資料位址，用來接收插入點的矩形範圍資訊。
 *	@return	<b>型別: int</b> \n 若已找到插入點則返回值為 (1)，若找不到插入點則返回值為 (0)
 */
int DmListView::GetInsertMarkRect(RECT* rectPtr) const
{
	assert(this->IsWindow());
	// LVM_GETINSERTMARKRECT	- 訊息處理返回值，0 = 找不到插入點， 1 = 找到了插入點
	// wParam = 不使用，必須為零
	// lParam = 指向 RECT 結構資料位址
	return static_cast<int>(::SendMessage(*this, LVM_GETINSERTMARKRECT, static_cast<WPARAM>(0), reinterpret_cast<LPARAM>(rectPtr)));
}

/**
 *	@brief	取得 ListView 增加的搜索文字字串
 *	@param[out]	textPtr 指向文字字串緩衝區位址，用來接收 ListView 增加的搜索文字字串。
 *	@return	<b>型別: int</b>
 *		\n 返回值為 ListView 增加的搜索文字字串長度，單位 TCHAR 不含 NULL 結尾。
 *		\n 如果 ListView 不處於增量搜索模式狀態，則返回值為零。
 */
int DmListView::GetISearchString(TCHAR* textPtr) const
{
	assert(this->IsWindow());
	// LVM_GETISEARCHSTRING
	// wParam = 不使用，必須為零
	// lParam = 向文字字串緩衝區位址
	return static_cast<int>(::SendMessage(*this, LVM_GETISEARCHSTRING, 0, reinterpret_cast<LPARAM>(textPtr)));
}

/**
 *	@brief	取得列表的項目資訊
 *	@param[in,out] lviPtr 指向 LVITEM 結構資料位址，用來指定與接收項目的資訊。
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmListView::GetItem(LVITEM * lviPtr) const
{
	assert(this->IsWindow());
	// LVM_GETITEM
	// wParam = 不使用，必須為零
	// lParam = 指向 LVITEM 結構資料位址
	return ::SendMessage(*this, LVM_GETITEM, static_cast<WPARAM>(0), reinterpret_cast<LPARAM>(lviPtr)) != 0;
}

/**
 *	@brief	取得項目(Item)數量
 *	@return	<b>型別: int</b> \n 返回值為項目(Item)數量。
 */
int DmListView::GetItemCount() const
{
	assert(this->IsWindow());
	// LVM_GETITEMCOUNT
	// wParam = 未使用，必須為零
	// lParam = 未使用，必須為零
	return static_cast<int>(::SendMessage(*this, LVM_GETITEMCOUNT, 0, 0));
}

/**
 *	@param[in]		lviiPtr		指向 LVITEMINDEX 結構資料位址，此參數不得為 NULL
 *	@param[in]		nSubItem	子項目索引值
 *	@param[in]		nCode		資料取得方式
 *	@param[in,out]	rcPtr		指向 RECT 結構資料位址，用來接收位置資訊。調用時成員 top 填入子項目的索引值，調用時成員 left 填入取得方式。
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 *	@remark	<b>nCode 資料取得方式</b>
 *		<pre>
 *		LVIR_BOUNDS	返回整個子項的矩形邊界，包括圖示和書籤。
 *		LVIR_ICON	返回子項的圖示或小圖示的矩形邊界。
 *		LVIR_LABEL	返回子項文本的矩形邊界。
 *		</pre>
 */
BOOL DmListView::GetItemIndexRect(LVITEMINDEX* lviiPtr, LONG nSubItem, LONG nCode, RECT* rcPtr) const
{
	assert(this->IsWindow());

	if (rcPtr != NULL) {
		rcPtr->top = nSubItem;
		rcPtr->left = nCode;
	}

	// LVM_GETITEMINDEXRECT
	// wParam = 指向 LVITEMINDEX 結構資料位址
	// lParam = 指向 RECT 結構資料位址
	return ::SendMessage(*this, LVM_GETITEMINDEXRECT, reinterpret_cast<WPARAM>(lviiPtr), reinterpret_cast<LPARAM>(rcPtr)) != 0;
}

/**
 *	@brief	取得 ListView 指定的項目座標位置
 *	@param[in]	nItem	項目索引值，索引值從 0 算起。
 *	@param[out]	ptPtr	指向 POINT 結構資料位址
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值，若失敗返回值為零。
 */
BOOL DmListView::GetItemPosition(int nItem, POINT* ptPtr) const
{
	assert(this->IsWindow());
	// LVM_GETITEMPOSITION
	// wParam = 列表的項目索引值
	// lParam = 指向 POINT 結構資料位址
	return ::SendMessage(*this, LVM_GETITEMPOSITION, static_cast<WPARAM>(nItem), reinterpret_cast<LPARAM>(ptPtr)) != 0;
}

/**
 *	@brief	取得 ListView 的項目全部或部分的邊界值。
 *	@param[in]		nItem	列表的項目索引值，索引值從 0 算起。
 *	@param[in,out]	rcPtr	指向 RECT 結構資料位址，必須在結構的 left 成員填入返回方式
 *		- LVIR_BOUNDS		\n	要求返回整個項目的矩形邊界，包含圖示與書籤。
 *		- LVIR_ICON			\n	要求返回圖示或小圖示(icon)的矩形邊界。
 *		- LVIR_LABEL		\n	要求返回文字區的矩形邊界。
 *		- LVIR_SELECTBOUNDS	\n	要求返回圖示與文字區(LVIR_ICON + LVIR_LABEL)矩形邊界，但不包含報表模式下(Report)的欄位區塊。
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值，若失敗則返回值為零。 \n 若成功 RECT 存放指定要求的內容。
 */
BOOL DmListView::GetItemRect(int nItem, RECT* rcPtr) const
{
	assert(this->IsWindow());
	// LVM_GETITEMRECT
	// wParam = 列表的項目索引值
	// lParam = 指向 RECT 結構資料位址
	return ::SendMessage(*this, LVM_GETITEMRECT, static_cast<WPARAM>(nItem), reinterpret_cast<LPARAM>(rcPtr)) != 0;
}

/**
 *	@brief	取得 ListView 的項目全部或部分矩形邊界範圍
 *	@param[in]		nItem	項目索引 (zero-base)
 *	@param[in]		nMode	要取得返回的模式
 *	@param[in,out]	rcPtr	指向 RECT 結構資料位址，用來接收邊界資訊。
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值，若失敗則返回值為零。 \n 若成功 RECT 存放指定要求的內容。
 *	@remark	<b>nMode 可用設定值如下</b>
 *		- LVIR_BOUNDS		\n	要求返回整個項目的矩形邊界，包含圖示與書籤。
 *		- LVIR_ICON			\n	要求返回圖示或小圖示(icon)的矩形邊界。
 *		- LVIR_LABEL		\n	要求返回文字區的矩形邊界。
 *		- LVIR_SELECTBOUNDS	\n	要求返回圖示與文字區(LVIR_ICON + LVIR_LABEL)矩形邊界，但不包含報表模式下(Report)的欄位區塊。
 */
BOOL DmListView::GetItemRect(int nItem, int nMode, RECT* rcPtr) const
{
	assert(this->IsWindow());
	if (rcPtr != NULL) {
		// 將指定的模式填寫入 rcPtr 的 left 成員。
		rcPtr->left = nMode;
		return this->GetItemRect(nItem, rcPtr);
	}
	return FALSE;
}

/**
 *	@brief 確定 ListView 圖示項目之間的間距。
 *	@param[in]	bSmall 要取得何種圖示的間距，小圖示(TRUE)，標準圖示(FALSE)
 *	@param[out] nHorzSpacingPtr	指向一個整數的緩衝區位址，用來接收水平的間距值
 *	@param[out] nVertSpacingPtr	指向一個整數的緩衝區位址，用來接收垂直的間距值
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值，若失敗則返回值為零。
 */
BOOL DmListView::GetItemSpacing(BOOL bSmall, int* nHorzSpacingPtr, int* nVertSpacingPtr) const
{
	assert(this->IsWindow());
	// LVM_GETITEMSPACING		- LOWORD(返回值) = 水平間距，HIWORD(返回值) = 垂直間距。
	// wParam = 要取得的項目間距
	// lParam = 不使用，必須為零
	auto lResult = ::SendMessage(*this, LVM_GETITEMSPACING, static_cast<WPARAM>(bSmall), static_cast<LPARAM>(0));

	if (nHorzSpacingPtr != NULL && nVertSpacingPtr != NULL) {
		*nHorzSpacingPtr = static_cast<int>(LOWORD(lResult));
		*nVertSpacingPtr = static_cast<int>(HIWORD(lResult));
		return TRUE;
	}
	return FALSE;
}

/**
 *	@brief	取得 ListView 項目的狀態
 *	@param[in] nItem	項目索引值，索引值從 0 算起。
 *	@param[in] nMode	指定要取得的方式 (預設 LVIS_STATEIMAGEMASK)
 *	@return	<b>型別: int</b> \n 返回值為當前狀態，狀態值依據 nMode 指定要取得方式
 *	@remark	<b>nMode 參數值如下</b>
 *		LVIS_CUT			\n	該項目標記為剪下和貼上操作。
 *		LVIS_DROPHILITED	\n	該項目突出顯示為拖放目標。
 *		LVIS_FOCUSED		\n	該項目具有焦點，因此它被標準焦點矩形包圍。雖然可以選擇多個項目，但只有一個項目可以具有焦點。
 *		LVIS_SELECTED		\n	該項目已被選中。所選項目的外觀取決於它是否具有焦點以及用於選擇的系統顏色。
 *		LVIS_OVERLAYMASK	\n	使用此蒙版可以檢索項目的疊加圖像索引。
 *		LVIS_STATEIMAGEMASK	\n	使用此掩碼檢索項目的狀態圖像索引。
 */
int DmListView::GetItemState(int nItem, int nMode) const
{
	assert(this->IsWindow());
	// LVM_GETITEMSTATE
	// wParam = 項目索引值
	// lParam = 指定要取得的方式
	return static_cast<int>(::SendMessage(*this, LVM_GETITEMSTATE, static_cast<WPARAM>(nItem), static_cast<LPARAM>(nMode)));
}

/**
 *	@brief	取得指定項目(Item)文字內容
 *	@param[in]	nItem		項目索引值，索引值從 0 算起。
 *	@param[in]	nSubItem	子項目索引值，索引值從 0 算起。
 *	@param[out]	szTextPtr	指向文字字串緩衝區位址
 *	@param[in]	ccLen		要取得文字字串的長度 (in TCHAR)
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmListView::GetItemText(int nItem, int nSubItem, TCHAR* szTextPtr, int ccLen) const
{
	LVITEM lvi;

	assert(this->IsWindow());
	if (szTextPtr == NULL) return FALSE;

	// Setting properties Of members
	::memset((void*)&lvi, 0, sizeof(LVITEM));
	lvi.mask = LVIF_TEXT | LVIF_IMAGE;	// LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvi.iItem = nItem;			// choose item
	lvi.iSubItem = nSubItem;	// choose sub item
	lvi.pszText = szTextPtr;	// push string pointer
	lvi.cchTextMax = ccLen;		// the string length

	// LVM_GETITEMTEXT
	// wParam = 列表的項目索引值
	// lParam = 指向 LVITEM 結構資料位址
	return ::SendMessage(*this, LVM_GETITEMTEXT, static_cast<WPARAM>(nItem), reinterpret_cast<LPARAM>(&lvi)) != 0;
}

/**
 *	@brief	在複選狀態下，取得下一個被選取的項目索引值。
 *	@return	<b>型別: int</b> \n 若成功返回值為項目(Item)索引值，若失敗則返回值為 (-1)。
 */
int DmListView::GetNextSelectItem()
{
	assert(this->IsWindow());
	auto nNext = m_nSelectItemIndex;
	// LVM_GETNEXTITEM
	// wParam = 起始項目所引值，若值為(-1)，則以查找與指定標誌匹配的第一個項目，指定項目本身將被排除在搜索之外。
	// lParam = 指定項目搜尋方式
	nNext = static_cast<int>(::SendMessage(*this, LVM_GETNEXTITEM, static_cast<WPARAM>(nNext), static_cast<LPARAM>(LVNI_ALL)));

	// 保存
	if (nNext != -1) {
		m_nSelectItemIndex = nNext;
	}
	return nNext;
}

/**
 *	@brief	複選狀態下，取得第一個被選取項目索引值 (選取累計，配合 GetNextSelectItem 可繼續取得下一個被選項目)。
 *	@return	<b>型別: int</b> \n 若成功返回值項目(Item)索引值，若失敗則返回值為 (-1)
 */
int DmListView::GetFirstSelectItem()
{
	assert(this->IsWindow());
	// LVM_GETNEXTITEM
	// wParam = 用於開始搜索項目的索引，使用參數值 (-1) 以查找與指定標誌匹配的第一個列表的項目，指定列表的項目本身將被排除在搜索之外。
	// lParam = 指定項目搜尋方式
	m_nSelectItemCount = this->GetSelectCount();
	m_nSelectItemIndex = static_cast<int>(::SendMessage(*this, LVM_GETNEXTITEM, static_cast<WPARAM>(-1), static_cast<LPARAM>(LVNI_ALL)));
	return m_nSelectItemIndex;
}

/**
 *	@brief	取得第一個被選取項目索引值 (無累計，永遠取得第一個)
 *	@return	<b>型別: int</b> \n 若成功返回值為 (-1) 以外的任意值，值為列表的項目(Item)的索引值, 若失敗返回值為 (-1)
 */
int DmListView::GetSelectItem() const
{
	assert(this->IsWindow());
	// LVM_GETNEXTITEM
	// wParam = 用於開始搜索項目的索引，(-1) 以查找與指定標誌匹配的第一個列表的項目，指定列表的項目本身將被排除在搜索之外。
	// lParam = 指定項目搜尋方式
	return static_cast<int>(::SendMessage(*this, LVM_GETNEXTITEM, static_cast<WPARAM>(-1), static_cast<LPARAM>(LVNI_SELECTED)));
}

/**
 *	@brief 取得下一個符合指定屬性的項目索引值
 *	@param[in,out]	lviiPtr	指項 LVITEMINDEX 結構緩衝區位址，描述開始搜尋內容為 (-1)，表示找第一個吻合者。
 *	@param[in]		nFlags	指定屬性搜尋，可組合。
 *		- LVNI_ALL				\n Searches for a subsequent item by index, the default value.
 *								\n Searches by physical relationship to the index of the item where the search is to begin.
 *		- LVNI_ABOVE			\n Searches for an item that is above the specified item.
 *		- LVNI_BELOW			\n Searches for an item that is below the specified item.
 *		- LVNI_TOLEFT			\n Searches for an item to the left of the specified item.
 *		- LVNI_PREVIOUS			\n Windows Vista and later: Searches for an item that is ordered before the item specified in wParam. The LVNI_PREVIOUS flag is not directional (LVNI_ABOVE will find the item positioned above, while LVNI_PREVIOUS will find the item ordered before.) The LVNI_PREVIOUS flag basically reverses the logic of the search performed by the LVM_GETNEXTITEM or LVM_GETNEXTITEMINDEX messages.
 *		- LVNI_TORIGHT			\n Searches for an item to the right of the specified item.
 *		- LVNI_DIRECTIONMASK	\n Windows Vista and later: A directional flag mask with value as follows: LVNI_ABOVE | LVNI_BELOW | LVNI_TOLEFT | LVNI_TORIGHT.
 *								\n The state of the item to find can be specified with one or a combination of the following values:
 *		- LVNI_CUT				\n The item has the LVIS_CUT state flag set.
 *		- LVNI_DROPHILITED		\n The item has the LVIS_DROPHILITED state flag set
 *		- LVNI_FOCUSED			\n The item has the LVIS_FOCUSED state flag set.
 *		- LVNI_SELECTED			\n flag set.
 *		- LVNI_STATEMASK		\n Windows Vista and later: A state flag mask with value as follows: LVNI_FOCUSED | LVNI_SELECTED | LVNI_CUT | LVNI_DROPHILITED.
 *								\n Searches by appearance of items or by group.
 *		- LVNI_VISIBLEORDER		\n Windows Vista and later: Search the visible order.
 *		- LVNI_VISIBLEONLY		\n Windows Vista and later: Search the visible items.
 *		- LVNI_SAMEGROUPONLY	\n Windows Vista and later: Search the current group.
 *	@return	<b>型別: BOOL</b>	\n 若成功返回值為非零值。 \n 若失敗返回值零。
 */
BOOL DmListView::GetNextItemIndex(LVITEMINDEX* lviiPtr, int nFlags) const
{
	assert(this->IsWindow());
	// LVM_GETNEXTITEMINDEX
	// wParam = 指項 LVITEMINDEX 結構緩衝區位址
	// lParam = 指定屬性搜尋
	return ::SendMessage(*this, LVM_GETNEXTITEMINDEX, reinterpret_cast<WPARAM>(lviiPtr), static_cast<LPARAM>(nFlags)) != 0;
}

/**
 *	@brief	取得工作區範圍內的項目數量
 *	@return	<b>型別: UINT</b> \n 返回值為項目數量
 */
UINT DmListView::GetNumberOfWorkAreas() const
{
	assert(this->IsWindow());
	UINT workAreas = 0;
	// LVM_GETNUMBEROFWORKAREAS
	// wParam = 不使用，必須為零
	// lParam = 指向 UINT 型別的緩衝區位址，用來接收工作區內的項目數量，此參數不得為 NULL
	::SendMessage(*this, LVM_GETNUMBEROFWORKAREAS, 0, reinterpret_cast<LPARAM>(&workAreas));
	return workAreas;
}

/**
 *	@brief	Retrieves the current view origin for a list-view control.
 *	@param[out] pointPtr 指向結構 POINT 緩衝區位址，用來接收原始位置資訊
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值，否則返回值為零。若 ListView 正處於列表狀態，則永遠返回零。
 */
BOOL DmListView::GetOrigin(POINT* pointPtr) const
{
	assert(this->IsWindow());
	// LVM_GETORIGIN
	// wParam = 不使用，必須為零
	// lParam = 指向結構 POINT 緩衝區位址
	return ::SendMessage(*this, LVM_GETORIGIN, static_cast<WPARAM>(0), reinterpret_cast<LPARAM>(pointPtr)) != 0;
}

/**
 *	@brief	取得當前 ListView 外框線顏色
 *	@return	<b>型別: COLORREF</b> \n 返回值為 外框線的 COLORREF 色彩值。
 */
COLORREF DmListView::GetOutlineColor() const
{
	assert(this->IsWindow());
	// LVM_GETOUTLINECOLOR
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<COLORREF>(::SendMessage(*this, LVM_GETOUTLINECOLOR, 0, 0));
}

/**
 *	@brief	取得被選中的 column 索引值
 *	@return	<b>型別: UINT</b> \n 返回值為當前被選中的 column 索引值。
 */
UINT DmListView::GetSelectedColumn() const
{
	assert(this->IsWindow());
	// LVM_GETSELECTEDCOLUMN
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<int>(::SendMessage(*this, LVM_GETSELECTEDCOLUMN, 0, 0));
}

/**
 *	@brief	取得被選中項目(Item)數量，即為項目被標記(反白)的數量。
 *	@return	<b>型別: int</b> \n 被選中(反白)的項目的數量。
 */
int DmListView::GetSelectCount() const
{
	assert(this->IsWindow());
	// LVM_GETSELECTEDCOUNT
	// wParam = 未使用，必須為零
	// lParam = 未使用，必須為零
	return static_cast<int>(::SendMessage(*this, LVM_GETSELECTEDCOUNT, 0, 0));
}

/**
 *	@brief	取得選取範圍標記
 *	@return	<b>型別: int</b> \n 返回值為選取範圍標記，若返回值為 (-1) 標示沒有選取標記。
 */
int DmListView::GetSelectionMark() const
{
	assert(this->IsWindow());
	// LVM_GETSELECTIONMARK
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<int>(::SendMessage(*this, LVM_GETSELECTIONMARK, 0, 0));
}

/**
 *	@brief	取得指定文字字串所需最小寬度，(僅針對文字與字型所需寬度，不包含圖示或圖像)
 *	@param[in] textPtr 指向文字字串緩衝區位址，內容指定的文字字串
 *	@return	<b>型別: int</b> \n 返回值為指定的文字字串所需的最小寬度 (in pixel)
 */
int DmListView::GetStringWidth(const TCHAR* textPtr) const
{
	assert(this->IsWindow());
	// LVM_GETSTRINGWIDTH
	// wParam = 不使用，必須為零
	// lParam = 指向文字字串緩衝區位址
	return static_cast<int>(::SendMessage(*this, LVM_GETSTRINGWIDTH, static_cast<WPARAM>(0), reinterpret_cast<LPARAM>(textPtr)));
}

/**
 *	@brief	取得子項目的邊界資訊
 *	@param[in] nItem	要獲取子項目的父項目索引值
 *	@param[in] nSubItem	子項項索引值
 *	@param[in] nArea	取得方式 (可以使用 OR 進行組合)
 *		- LVIR_BOUNDS	\n 傳回整個項目，包括圖示和標籤的週框矩形。
 *		- LVIR_ICON		\n 傳回圖示或小圖示的週框矩形。
 *		- LVIR_LABEL	\n 傳回整個項目，包括圖示和標籤的週框矩形。 這是 LVIR_BOUNDS 完全相同。
 *	@param[in,out] rcPtr 指向結構 RECT 緩衝區位址，用來接收子項目邊界資訊。
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmListView::GetSubItemRect(int nItem, int nSubItem, int nArea, RECT* rcPtr) const
{
	assert(this->IsWindow());
	// LVM_GETSUBITEMRECT
	// wParam = 要獲取子項目的父項目索引值
	// lParam = 指向結構 RECT 緩衝區位址
	if (rcPtr != NULL) {
		rcPtr->top = static_cast<LONG>(nSubItem);
		rcPtr->left = static_cast<LONG>(nArea);
	}
	return ::SendMessage(*this, LVM_GETSUBITEMRECT, static_cast<WPARAM>(nItem), reinterpret_cast<LPARAM>(rcPtr)) != 0;
}

/**
 *	@brief	取得 ListView 文字背景色彩
 *	@return	<b>型別: COLORREF</b> \n 返回值為文字背景色彩
 */
COLORREF DmListView::GetTextBkColor() const
{
	assert(this->IsWindow());
	// LVM_GETTEXTBKCOLOR
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<COLORREF>(::SendMessage(*this, LVM_GETTEXTBKCOLOR, 0, 0));
}

/**
 *	@brief	取得 ListView 文字色彩
 *	@return	<b>型別: COLORREF</b> \n 返回值為文字色彩
 */
COLORREF DmListView::GetTextColor() const
{
	assert(this->IsWindow());
	// LVM_GETTEXTCOLOR
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<COLORREF>(::SendMessage(*this, LVM_GETTEXTCOLOR, 0, 0));
}

/**
 *	@brief	取得 tile 資訊
 *	@param[out] lvtiPtr	指向結構 LVTILEINFO 緩衝區位址，用來接收 tile 資訊
 *	@return	此函數沒有返回值
 */
void DmListView::GetTileInfo(LVTILEINFO * lvtiPtr) const
{
	assert(this->IsWindow());
	// LVM_GETTILEINFO
	// wParam
	// lParam
	::SendMessage(*this, LVM_GETTILEINFO, static_cast<WPARAM>(0), reinterpret_cast<LPARAM>(lvtiPtr));
}

/**
 *	@brief	取得 tile view 資訊
 *	@param[out] lvtviPtr 指向結構 LVTILEVIEWINFO 緩衝區位址，用來接收 tile view 資訊
 */
void DmListView::GetTileViewInfo(LVTILEVIEWINFO* lvtviPtr) const
{
	assert(this->IsWindow());
	// LVM_GETTILEVIEWINFO
	// wParam = 不使用，必須為零。
	// lParam = 指向結構 LVTILEVIEWINFO 緩衝區位址
	::SendMessage(*this, LVM_GETTILEVIEWINFO, static_cast<WPARAM>(0), reinterpret_cast<LPARAM>(lvtviPtr));
}

/**
 *	@brief	取得提示工具操作代碼
 *	@return	<b>型別: HWND</b> 返回值為提示工具的操作代碼。
 */
HWND DmListView::GetToolTips() const
{
	assert(this->IsWindow());
	// LVM_GETTOOLTIPS
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return reinterpret_cast<HWND>(::SendMessage(*this, LVM_GETTOOLTIPS, 0, 0));
}

/**
 *	@brief	取得可視範圍內的最上層的項目索引值
 *	@return	<b>型別: int</b>
 *		\n 若成功返回值為非零值，值為可視的圍內最上層項目的索引值。
 *		\n 如果 ListView 在圖示或小圖示的模式下，或者如果 ListView 在群組的詳細訊息則返回零。
 */
int DmListView::GetTopIndex() const
{
	assert(this->IsWindow());
	// LVM_GETTOPINDEX
	// wParam = 未使用，必須為零
	// lParam = 未使用，必須為零
	return static_cast<int>(::SendMessage(*this, LVM_GETTOPINDEX, 0, 0));
}

/**
 *	@brief	此列表是否使用 Unicode
 *	@return	<b>型別: BOOL</b> 若使用 Unicode 字元返回值為非零值。 \n 若不使用 Unicode 字元則返回值為零。
 */
BOOL DmListView::GetUnicodeFormat() const
{
	assert(this->IsWindow());
	// LVM_GETUNICODEFORMAT
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return ::SendMessage(*this, LVM_GETUNICODEFORMAT, 0, 0) != 0;
}

/**
 *	@brief	取得目前顯示型態
 *	@return	<b>型別: DWORD</b> \n 返回值為目前列表的顯示型態。
 *		- LV_VIEW_DETAILS
 *		- LV_VIEW_ICON
 *		- LV_VIEW_LIST
 *		- LV_VIEW_SMALLICON
 *		- LV_VIEW_TILE
 */
DWORD DmListView::GetView() const
{
	assert(this->IsWindow());
	// LVM_GETVIEW
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<DWORD>(::SendMessage(*this, LVM_GETVIEW, 0, 0));
}

/**
 *	@brief	取得 ListView 所有項目邊界值。
 *	@param[out] rcPtr 指向 RECT 結構資料位址，用來接收 ListView 所有項目的邊界。
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值，若失敗返回值為零。
 */
BOOL DmListView::GetViewRect(RECT* rcPtr) const
{
	assert(this->IsWindow());
	// LVM_GETVIEWRECT
	// wParam = 不使用，必須為零
	// lParam = 指向 RECT 結構資料位址
	return ::SendMessage(*this, LVM_GETVIEWRECT, static_cast<WPARAM>(0), reinterpret_cast<WPARAM>(rcPtr)) != 0;
}

/**
 *	@brief	取得工作區範圍尺寸
 *	@param[in]	nWorkAreas	RECT 結構陣列的組數
 *	@param[out]	rcPtr		指向 RECT 結構陣列的位址，用來接收工作區範圍尺寸
 *	@return	此函數沒有返回值
 */
void DmListView::GetWorkAreas(int nWorkAreas, RECT* rcPtr) const
{
	assert(this->IsWindow());
	// LVM_GETWORKAREAS
	// wParam = RECT 結構陣列的組數
	// lParam = 指向 RECT 結構陣列的位址
	::SendMessage(*this, LVM_GETWORKAREAS, static_cast<WPARAM>(nWorkAreas), reinterpret_cast<LPARAM>(rcPtr));
}

/**
 *	@brief	判斷列表是否具有指定的群組。
 *	@param[in] nIDGroup	群組識別碼
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmListView::HasGroup(int nIDGroup) const
{
	assert(this->IsWindow());
	// LVM_HASGROUP
	// wParam = 群組識別碼
	// lParam = 不使用必須為零
	return ::SendMessage(*this, LVM_HASGROUP, static_cast<WPARAM>(nIDGroup), static_cast<LPARAM>(0)) != 0;
}

/**
 *	@brief	點擊測試 (若有項目存在的話，將得到項目索引值)
 *	@param[in] lvhPtr 指向 LVHITTESTINFO 結構資料位址，填入測試位置資訊並接收測試結果資訊。
 *	@return	<b>型別: int</b>
 *		\n 若成功返回值為項目索引值 (點擊位置所在的項目索引值)
 *		\n 若失敗返回值為 (-1)
 */
int DmListView::HitTest(LVHITTESTINFO* lvhPtr) const
{
	assert(this->IsWindow());
	// LVM_HITTEST
	// wParam = 不使用，必須為零
	// lParam = 指向 LVHITTESTINFO 結構資料位址
	return static_cast<int>(::SendMessage(*this, LVM_HITTEST, static_cast<WPARAM>(0), reinterpret_cast<LPARAM>(lvhPtr)));
}

/**
 *	@brief	點擊測試 (若有項目存在的話，將得到項目索引值)
 *	@param[in]	ptPtr	指向 POINT 結構資料位址，要測試點擊的位置(座標)
 *	@param[out]	flagPtr	指向 UINT 型別緩衝區位址，用來接收點擊資訊。
 *	@return	<b>型別: int</b>
 *		\n 若成功返回值為項目索引值 (點擊位置所在的項目索引值)
 *		\n 若失敗返回值為 (-1)
 */
int DmListView::HitTest(POINT* ptPtr, UINT* flagPtr) const
{
	LVHITTESTINFO hti;
	::memset(&hti, 0, sizeof(LVHITTESTINFO));
	hti.flags = *flagPtr;
	hti.pt = *ptPtr;
	return this->HitTest(&hti);
}

/**
 *	@brief	新增一個欄位(Column)
 *	@param[in] nColumn	欄位索引值
 *	@param[in] colPtr	指向 LVCOLUMN 結構資料位址，內容為要新增的欄位資訊。
 *	@return	<b>型別: int</b> \n 若成功返回值為欄位索引值。 \n 若失敗返回值為(-1)。
 */
int DmListView::InsertColumn(int nColumn, const LVCOLUMN* colPtr) const
{
	assert(this->IsWindow());
	// LVM_INSERTCOLUMN
	// wParam = 指定欄位索引值
	// lParam = LVCOLUMN 結構資料位址
	return static_cast<int>(::SendMessage(*this, LVM_INSERTCOLUMN, static_cast<WPARAM>(nColumn), reinterpret_cast<LPARAM>(colPtr)));
}

/**
 *	@brief	新增一個欄位 (Column)
 *	@param[in] nColumn	欄位索引值
 *	@param[in] wd		指定欄位(Column)寬度
 *	@param[in] nAlign	指定對齊方式
 *	@param[in] textPtr	指向文字字串緩衝區位址
 *	@return	<b>型別: int</b> \n 若成功返回值為欄位索引值。 \n 若失敗返回值為(-1)。
 */
BOOL DmListView::InsertColumn(int nColumn, int wd, int nAlign, const TCHAR* textPtr) const
{
	const static int aiAssign[] = { LVCFMT_LEFT, LVCFMT_RIGHT, LVCFMT_CENTER, LVCFMT_LEFT };
	LVCOLUMN lvc;
	assert(this->IsWindow());

	// zero struct's members
	::memset((void*)&lvc, 0, sizeof(LVCOLUMN));
	if (nAlign >= static_cast<int>(EmLVColumn::AlignPassend)) {
		nAlign = static_cast<int>(EmLVColumn::AlignLife);
	}

	// Setting properties of members
	lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT; // LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt = aiAssign[nAlign];
	lvc.pszText = const_cast<LPTSTR>(textPtr);
	lvc.cchTextMax = static_cast<int>(_tcslen(lvc.pszText));
	lvc.cx = wd;

	return this->InsertColumn(nColumn, &lvc);
}

/**
 *	@brief	新增(插入)一個群組
 *	@param[in] nGroup	要插入位置的群組索引值，若值為 (-1) 代表插入最底端(最後)。
 *	@param[in] grpPtr	指向 LVGROUP 資料緩衝區位址，內容為群組資訊。
 *	@return	<b>型別: int</b> \n 若新增群組成功返回值為群組索引。 \n 若新增群組失敗則返回值為零。
 */
int DmListView::InsertGroup(int nGroup, LVGROUP* grpPtr)
{
	assert(this->IsWindow());
	// LVM_INSERTGROUP
	// wParam = 要插入位置的群組索引值
	// lParam = 指向 LVGROUP 資料緩衝區位址
	return static_cast<int>(::SendMessage(*this, LVM_INSERTGROUP, static_cast<WPARAM>(nGroup), reinterpret_cast<LPARAM>(grpPtr)));
}

/**
 *	@brief	使用排序方式插入指定群組。
 *	@param[in] lvigPtr 指向結構 LVINSERTGROUPSORTED 緩衝區的位址，用來擲定要插入 (新增) 的群組內容。
 *	@return	此函數沒有返回值
 */
void DmListView::InsertGroupSorted(LVINSERTGROUPSORTED* lvigPtr) const
{
	assert(this->IsWindow());
	// LVM_INSERTGROUPSORTED
	// wParam = 指向結構 LVINSERTGROUPSORTED 緩衝區的位址
	// lParam = 不使用，必須為零
	::SendMessage(*this, LVM_INSERTGROUPSORTED, reinterpret_cast<WPARAM>(lvigPtr), static_cast<LPARAM>(0));
}

/**
 *	@brief	新增項目
 *	@param[in] lviPtr 指向結構 LVITEM 的換衝區位址，內容為指定要新增的項目。
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值，若失敗返回值為零。
 */
BOOL DmListView::InsertItem(const LVITEM* lviPtr) const
{
	assert(this->IsWindow());

	if (lviPtr == NULL) { return FALSE; }

	// LVM_INSERTITEM, 若運作失敗將傳回 -1
	// wParam = 未使用，必須為零
	// lParam = 指向 LVITEM 結構資料位址
	return ::SendMessage(*this, LVM_INSERTITEM, static_cast<WPARAM>(0), reinterpret_cast<LPARAM>(lviPtr)) != -1;
}

/**
 *	@brief		新增項目 (文字項目)
 *	@param[in]	nItem	指定列表的項目索引值，索引值從 0 算起
 *	@param[in]	textPtr	指向文字字串緩衝區位址
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值，若失敗返回值為零。
 */
BOOL DmListView::InsertItem(int nItem, const TCHAR* textPtr) const
{
	assert(this->IsWindow());
	if (textPtr == NULL) {
		return FALSE;
	}

	// Setting properties of members
	LVITEM lvi;
	::memset((void*)&lvi, 0, sizeof(LVITEM));
	lvi.mask = LVIF_TEXT;	// 遮罩項目 (要使用的模式) LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvi.iItem = nItem;		// 項目索引值
	lvi.pszText = const_cast<LPTSTR>(textPtr);	// set text
	lvi.cchTextMax = static_cast<int>(_tcslen(lvi.pszText));

	// LVM_INSERTITEM, 若運作失敗將傳回 -1
	// wParam = 未使用，必須為零
	// lParam = 指向 LVITEM 結構資料位址
	return ::SendMessage(*this, LVM_INSERTITEM, static_cast<WPARAM>(0), reinterpret_cast<LPARAM>(&lvi)) != -1;
}

/**
 *	@brief		新增項目 (圖示項目)
 *	@param[in]	nItem	指定列表的項目索引值，索引值從 0 算起
 *	@param[in]	textPtr	指向文字字串緩衝區位址
 *	@param[in]	nImage	指定的圖示索引值
 *	@return		<b>型別: BOOL</b> \n 若成功返回值為非零值，若失敗返回值為零。
 */
BOOL DmListView::InsertItem(int nItem, const TCHAR* textPtr, int nImage) const
{
	assert(this->IsWindow());
	if (textPtr == NULL) {
		return FALSE;
	}

	// Setting properties of members
	LVITEM lvi;
	::memset((void*)&lvi, 0, sizeof(LVITEM));
	lvi.mask = LVIF_TEXT | LVIF_IMAGE;	// 遮罩項目 (要使用的模式) LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvi.iItem = nItem;					// 項目索引值
	lvi.iImage = nImage;				// 圖示索引值
	lvi.pszText = const_cast<LPTSTR>(textPtr);	// set text
	lvi.cchTextMax = static_cast<int>(_tcslen(lvi.pszText));

	// LVM_INSERTITEM, 若運作失敗將傳回 -1
	// wParam = 未使用，必須為零
	// lParam = 指向 LVITEM 結構資料位址
	return ::SendMessage(*this, LVM_INSERTITEM, static_cast<WPARAM>(0), reinterpret_cast<LPARAM>(&lvi)) != -1;
}

/**
 *	@brief	擷取最接近指定的插入點。
 *	@param[in] pointPtr 指向 POINT 結構資料緩衝區位址，內容為點擊測試的座標。
 *	@param[in] lvimPtr	指向 LVINSERTMARK 結構資料緩衝區位址，用來接收結果。
 *	@return	<b>型別: int</b>
 *		\n 若運作成功返回值為非零值。
 *		\n 若運作失敗則返回值為零。若成員 cbSize 描述與結構 LVINSERTMARK 長度不一致也會造成失敗。
 */
BOOL DmListView::InsertMarkHitTest(POINT* pointPtr, LVINSERTMARK* lvimPtr) const
{
	assert(this->IsWindow());
	// 成員 cbSize 必須指定，否則必然會返回 FALSE
	lvimPtr->cbSize = sizeof(LVINSERTMARK);
	// LVM_INSERTMARKHITTEST
	// wParam = 指向 POINT 結構資料緩衝區位址
	// lParam = 指向 LVINSERTMARK 結構資料緩衝區位址
	return static_cast<BOOL>(::SendMessage(*this, LVM_INSERTMARKHITTEST, reinterpret_cast<WPARAM>(pointPtr), reinterpret_cast<LPARAM>(lvimPtr)));
}

/**
 *	@brief	判斷是否使用了 Group view
 *	@return	<b>型別: BOOL</b> \n 若已使用 Group View 返回值為非零值，否則返回值為零。
 */
BOOL DmListView::IsGroupViewEnabled() const
{
	assert(this->IsWindow());
	// LVM_ISGROUPVIEWENABLED
	// wParam = 不使用，必須為零。
	// lParam = 不使用，必須為零。
	return ::SendMessage(*this, LVM_ISGROUPVIEWENABLED, 0, 0) != 0;
}

/**
 *	@brief	指定項目是否為可視狀態
 *	@param[in] iItem 項目索引值
 *	@return	<b>型別: BOOL</b> \n 若為 Enable 狀態，返回值為非零值，否則返回零
 */
BOOL DmListView::IsItemVisible(int iItem) const
{
	assert(this->IsWindow());
	// LVM_ISITEMVISIBLE
	// wParam
	// lParam
	return ::SendMessage(*this, LVM_ISITEMVISIBLE, static_cast<WPARAM>(iItem), static_cast<LPARAM>(0)) != 0;
}

/**
 *	@brief	ListView 目前項目識別碼對應至當前索引值
 *	@param[in] uID 項目識別碼
 *	@return	<b>型別: int</b> 返回值為當前 ListView 項目的索引值。
 */
int DmListView::MapIDToIndex(UINT uID) const
{
	assert(this->IsWindow());
	// LVM_MAPIDTOINDEX
	// wParam = 項目識別碼
	// lParam = 不使用，必須為零。
	return static_cast<int>(::SendMessage(*this, LVM_MAPIDTOINDEX, static_cast<WPARAM>(uID), static_cast<LPARAM>(0)));
}

/**
 *	@brief	將目前 ListView 項目的索引值對應至唯一的識別碼。
 *	@param[in] iItem 項目索引值 (zero-base)
 *	@return	<b>型別: UINT</b> \n 返回值為項目唯一識別碼
 */
UINT DmListView::MapIndexToID(int iItem) const
{
	assert(this->IsWindow());
	// LVM_MAPINDEXTOID
	// wParam = 項目索引值
	// lParam = 不使用，必須為零。
	return static_cast<UINT>(::SendMessage(*this, LVM_MAPINDEXTOID, static_cast<WPARAM>(iItem), static_cast<LPARAM>(0)));
}

/**
 *	@brief	移動指定群組，來指定零起始的索引清單檢視控制項。
 *	@param[in] iGroupId	要移動的群組識別碼。
 *	@param[in] toIndex	要移動群組的索引值 (Zero-base)。
 *	@return	此函數沒有返回值。
 */
void DmListView::MoveGroup(int iGroupId, int toIndex) const
{
	assert(this->IsWindow());
	// LVM_MOVEGROUP
	// wParam = 要移動的群組識別碼
	// lParam = 要移動群組的索引值
	::SendMessage(*this, LVM_MOVEGROUP, static_cast<WPARAM>(iGroupId), static_cast<LPARAM>(toIndex));
}

/**
 *	@brief	將指定的項目移到指定的群組。
 *	@param[in]	idItemFrom	要移動項目的索引值
 *	@param[in]	idGroupTo	要前往的群組識別碼
 *	@return	此函數沒有返回值
 */
void DmListView::MoveItemToGroup(int idItemFrom, int idGroupTo) const
{
	assert(this->IsWindow());
	// LVM_MOVEITEMTOGROUP :: 成功返回非零值，失敗返回值為零。
	// wParam = 要移動項目的索引值
	// lParam = 要前往的群組識別碼
	::SendMessage(*this, LVM_MOVEITEMTOGROUP, static_cast<WPARAM>(idItemFrom), static_cast<WPARAM>(idGroupTo));
}

/**
 *	@brief	強制重繪 ListView 指定範圍的項目
 *	@param[in] nFirst	要重繪的開始項目索引值
 *	@param[in] nLast	要重繪的結束項目索引值
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值，若失敗返回值為零。
 */
BOOL DmListView::RedrawItems(int nFirst, int nLast) const
{
	assert(this->IsWindow());
	// LVM_REDRAWITEMS
	// wParam = 開始項目索引值
	// lParam = 結束項目索引值
	return ::SendMessage(*this, LVM_REDRAWITEMS, static_cast<WPARAM>(nFirst), static_cast<LPARAM>(nLast)) != 0;
}

/**
 *	@brief	移除所有的 ListView 群組
 *	@return	此函數沒有返回值
 */
void DmListView::RemoveAllGroups() const
{
	assert(this->IsWindow());
	// LVM_REMOVEALLGROUPS
	// wParam = 不使用，必須為零。
	// lParam = 不使用，必須為零。
	::SendMessage(*this, LVM_REMOVEALLGROUPS, 0, 0);
}

/**
 *	@brief	移除一個 ListView 群組
 *	@param[in] nGroupId 群組識別碼
 *	@return	<b>型別: int</b> 若成功返回值為被移除的群組識別碼，反則返回 (-1)。
 */
int DmListView::RemoveGroup(int nGroupId) const
{
	assert(this->IsWindow());
	// LVM_REMOVEGROUP
	// wParam
	// lParam
	return static_cast<int>(::SendMessage(*this, LVM_REMOVEGROUP, static_cast<WPARAM>(nGroupId), static_cast<LPARAM>(NULL)));
}

/**
 *	@brief	設定 ListView 卷軸捲動
 *	@param[in] xScroll 水平捲動量，相對當前位置的捲動量，單位像素(pixel)
 *	@param[in] yScroll 垂直捲動量，相對當前位置的捲動量，單位像素(pixel)
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若帆樹操作失敗則返回值為零。
 */
BOOL DmListView::Scroll(int xScroll, int yScroll) const
{
	assert(this->IsWindow());
	// LVM_SCROLL
	// wParam = 水平捲動量
	// lParam = 垂直捲動量
	return ::SendMessage(*this, LVM_SCROLL, static_cast<WPARAM>(xScroll), static_cast<LPARAM>(yScroll)) != 0;
}

/**
 *	@brief	設定 ListView 卷軸捲動
 *	@param[in] sizePtr 指向結構 SIZE 資料緩衝區位址，成員 cx 為水平捲動量，cy 為垂直捲動量。
 *	@return	<b>型別: BOOL</b> \n 若運作成功返回值為非零值。 \n 若運作失敗則返回值為零。
 */
BOOL DmListView::Scroll(const SIZE* sizePtr) const
{
	assert(this->IsWindow());
	assert(sizePtr);
	// LVM_SCROLL
	// wParam = 水平捲動量
	// lParam = 垂直捲動量
	return ::SendMessage(*this, LVM_SCROLL, static_cast<WPARAM>(sizePtr->cx), static_cast<LPARAM>(sizePtr->cy)) != 0;
}

/**
 *	@brief	設定 ListView 背景顏色
 *	@param[in] dwColor 顏色(RGB)，背景顏色 RGB(若使用 CLR_NONE 定義值，則為無被景色)
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmListView::SetBkColor(COLORREF dwColor) const
{
	assert(this->IsWindow());
	// LVM_SETBKCOLOR
	// wParam = 未使用，必須為零
	// lParam = 背景顏色值
	return ::SendMessage(*this, LVM_SETBKCOLOR, static_cast<WPARAM>(0), static_cast<LPARAM>(dwColor)) != 0;
}

/**
 *	@brief	在 ListView 設置背景圖。
 *	@param[in] lvbiPtr 指向結構 LVBKIMAGE 資料緩衝區位址，內容為背景圖相關資訊。
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmListView::SetBkImage(const LVBKIMAGE* lvbiPtr) const
{
	assert(this->IsWindow());
	// LVM_SETBKIMAGE
	// wParam = 不使用，必須為零
	// lParam = 指向結構 LVBKIMAGE 資料緩衝區位址
	return ::SendMessage(*this, LVM_SETBKIMAGE, static_cast<WPARAM>(0), reinterpret_cast<LPARAM>(lvbiPtr)) != 0;
}

/**
 *	@brief 設定 ListView callback 遮罩
 *	@param[in] nMask 新的遮罩值
 *		<pre>
 *		Value				Meaning
 *		LVIS_CUT			The item is marked for a cut-and-paste operation.
 *		LVIS_DROPHILITED	The item is highlighted as a drag-and-drop target.
 *		LVIS_FOCUSED		The item has the focus.
 *		LVIS_SELECTED		The item is selected.
 *		LVIS_OVERLAYMASK	The application stores the image list index of the current overlay image for each item.
 *		LVIS_STATEIMAGEMASK	The application stores the image list index of the current state image for each item.
 *		</pre>
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值，若失敗返回值為零。
 */
BOOL DmListView::SetCallbackMask(UINT nMask) const
{
	// LVM_SETCALLBACKMASK
	// wParam = 新的遮罩值
	// lParam = 不使用，必須為零。
	return ::SendMessage(*this, LVM_SETCALLBACKMASK, static_cast<WPARAM>(nMask), static_cast<LPARAM>(0)) != 0;
}

/**
 *	@brief	設定指定欄位(Column)文字內容
 *	@param[in] nColumn	欄位索引值，索引從 0 算起
 *	@param[in] textPtr	指向文字字串緩衝區位址
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmListView::SetColumn(int nColumn, const TCHAR* textPtr) const
{
	LVCOLUMN lvc;

	assert(this->IsWindow());
	if (textPtr == NULL) {
		return FALSE;
	}

	::memset((void*)&lvc, 0, sizeof(LVCOLUMN));
	lvc.mask = LVCF_TEXT;
	lvc.pszText = const_cast<LPTSTR>(textPtr);

	// LVM_SETCOLUMN
	// wParam = 欄位索引值
	// lParam = 指向 LVCOLUMN 結構資料位址
	return ::SendMessage(*this, LVM_SETCOLUMN, static_cast<WPARAM>(nColumn), reinterpret_cast<LPARAM>(&lvc)) != 0;
}

/**
 *	@brief	設定 Column 的順序 (由左至右)
 *	@param[in] nCount		指定要設定的 ListView 的 Column 數量
 *	@param[in] nArrayPtr	指向 INT 型別索引值陣列緩衝區位址。
 *		\n 該緩衝區包含 ListView Column 索引值（從左到右）。
 *		\n 緩衝區必須夠大，要能夠包含 ListView Column 總數。
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值，若失敗返回值為零。
 */
BOOL DmListView::SetColumnOrderArray(int nCount, INT* nArrayPtr) const
{
	// LVM_SETCOLUMNORDERARRAY
	// wParam = 指定要設定的 ListView 的 Column 數量
	// lParam = 指向 INT 型別陣列緩衝區位址
	return ::SendMessage(*this, LVM_SETCOLUMNORDERARRAY, static_cast<WPARAM>(nCount), reinterpret_cast<LPARAM>(nArrayPtr)) != 0;
}

/**
 *	@brief	設定欄位(Column)寬度
 *	@param[in] nColumn	欄位索引值，索引值從 0 算起
 *	@param[in] nPixel	欄位寬度，單位像素(pixel)
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmListView::SetColumnWidth(int nColumn, int nPixel) const
{
	assert(this->IsWindow());
	// LVM_GETCOLUMNWIDTH
	// wParam = 欄位索引值
	// lParam = 欄位寬度
	return ::SendMessage(*this, LVM_SETCOLUMNWIDTH, static_cast<WPARAM>(nColumn), static_cast<LPARAM>(nPixel)) != 0;
}

/**
 *	@brief	設定 ListView 擴展樣式
 *	@param[in] dwExStyle 擴展樣式， ListView 的呈現與操作的擴展與延伸樣式。
 *	@return	此函數沒有返回值
 *	@see	DmListView
 */
void DmListView::SetExtendedListViewStyleEx(DWORD dwExStyle) const
{
	assert(this->IsWindow());
	// LVM_SETEXTENDEDLISTVIEWSTYLE
	// wParam = 未使用，必須為零
	// lParam = 擴展樣式
	::SendMessage(*this, LVM_SETEXTENDEDLISTVIEWSTYLE, static_cast<WPARAM>(0), static_cast<LPARAM>(dwExStyle));
}

/**
 *	@brief	設定群組的資訊
 *	@param[in] iGroupId	指定要設置資訊的群組 ID
 *	@param[in] lvgPtr	指向 LVGROUP 結構資料位址，內容為群組的訊息。
 *	@return	<b>型別: int</b> \n 若成功返回值為群組的 ID。 \n 若失敗則返回值為 (-1)
 */
int DmListView::SetGroupInfo(int iGroupId, LVGROUP* lvgPtr) const
{
	assert(this->IsWindow());
	// LVM_SETGROUPINFO
	// wParam = 指定要設置資訊的群組 ID
	// lParam = 指向 LVGROUP 結構資料位址
	return static_cast<int>(::SendMessage(*this, LVM_SETGROUPINFO, static_cast<WPARAM>(iGroupId), reinterpret_cast<LPARAM>(lvgPtr)));
}

/**
 *	@brief	設定群組顯示資訊
 *	@param[in] lvgmtPtr 指向結構 LVGROUPMETRICS 定義緩衝區位址，結構內容為群組顯示資訊。
 *	@return	此函數沒有返回值
 */
void DmListView::SetGroupMetrics(LVGROUPMETRICS* lvgmtPtr) const
{
	assert(this->IsWindow());
	// LVM_SETGROUPMETRICS
	// wParam = 不使用，必須為零。
	// lParam = 指向結構 LVGROUPMETRICS 定義緩衝區位址
	::SendMessage(*this, LVM_SETGROUPMETRICS, static_cast<WPARAM>(0), reinterpret_cast<LPARAM>(lvgmtPtr));
}

/**
 *	@brief	設定 Hot tracking 樣式下的游標
 *	@param[in] hCursor 游標操作代碼
 *	@return	<b>型別: HCURSOR</b> \n 先前使用的游標操作代碼。
 */
HCURSOR DmListView::SetHotCursor(HCURSOR hCursor) const
{
	assert(this->IsWindow());
	// LVM_SETHOTCURSOR
	// wParam = 不使用，必須為零。
	// lParam = 游標操作代碼
	return reinterpret_cast<HCURSOR>(::SendMessage(*this, LVM_SETHOTCURSOR, static_cast<WPARAM>(0), reinterpret_cast<LPARAM>(hCursor)));
}

/**
 *	@brief	設定當前熱選(Hot)選中列表項目
 *	@param[in] nItem 要設定為 hot Item 的索引值 (從零開始算起)
 *	@return	<b>型別: int</b> \n 返回值為先前被設定為 Hot Item 的索引值。
 */
int DmListView::SetHotItem(int nItem) const
{
	assert(this->IsWindow());
	// LVM_SETHOTITEM
	// wParam = 要設定為 hot Item 的索引值
	// lParam = 不使用，必須為零。
	return static_cast<int>(::SendMessage(*this, LVM_SETHOTITEM, static_cast<WPARAM>(nItem), static_cast<LPARAM>(0)));
}

/**
 *	@brief	設定滑鼠游標在 ListView 項目上選中前須停留的時間，單位毫秒。
 *	@param[in] dwHoverTime 停留時間單位毫秒。
 *	@return	<b>型別: DWORD</b> \n 返回值為先前設定的時間。
 */
DWORD DmListView::SetHoverTime(DWORD dwHoverTime) const
{
	assert(this->IsWindow());
	// LVM_SETHOVERTIME
	// wParam = 不使用，必須為零
	// lParam = 停留時間
	return static_cast<DWORD>(::SendMessage(*this, LVM_SETHOVERTIME, static_cast<WPARAM>(0), static_cast<LPARAM>(dwHoverTime)));
}

/**
 *	@brief	設定圖示與圖示的間距，必須在大圖示模式或小圖示模式才有作用。
 *	@param[in] cx 水平間距
 *	@param[in] cy 垂直間距
 *	@return	<b>型別: DWORD</b>
 *		\n 返回值為先前設定的大圖示或小圖示模式下，圖示間的間距。
 *		\n LOWORD(返回值) 為水平間距，HIWORD(返回值) 為垂直間距。
 */
DWORD DmListView::SetIconSpacing(int cx, int cy) const
{
	assert(this->IsWindow());
	// LVM_SETICONSPACING
	// wParam = 不使用，必須為零
	// lParam = 圖示間距資料
	return static_cast<DWORD>(::SendMessage(*this, LVM_SETICONSPACING, static_cast<WPARAM>(0), MAKELPARAM(cx, cy)));
}

/**
 *	@brief	設定圖示與圖示的間距，必須在大圖示模式或小圖示模式才有作用。
 *	@param[in] sizePtr 指向 SIZE 結構資料位址，結構為間距定義資料。
 *	@return	<b>型別: DWORD</b>
 *		\n 返回值為先前設定的大圖示或小圖示模式下，圖示間的間距。
 *		\n LOWORD(返回值) 為水平間距，HIWORD(返回值) 為垂直間距。
 */
DWORD DmListView::SetIconSpacing(SIZE* sizePtr) const
{
	assert(this->IsWindow());
	// LVM_SETICONSPACING
	// wParam = 不使用，必須為零
	// lParam = 圖示間距資料
	return static_cast<DWORD>(::SendMessage(*this, LVM_SETICONSPACING, static_cast<WPARAM>(0), MAKELPARAM(sizePtr->cx, sizePtr->cy)));
}

/**
 *	@brief	將 ImageList 分配給 ListView
 *	@param[in] hImageList		ImageList 操作代碼
 *	@param[in] nImageListType	ImageList 使用類型
 *		- VSIL_NORMAL		\n 大圖示的 ImageList。
 *		- LVSIL_SMALL		\n 小圖示的 ImageList。
 *		- LVSIL_STATE		\n 狀態圖像的 ImageList。
 *		- LVSIL_GROUPHEADER	\n 給群組用的 ImageList，(Image list for group header.)。
 *	@return	<b>型別: HIMAGELIST</b> \n 如果成功返回值為設定前的 ImagList 操作代碼，否則返回值為 NULL。
 */
HIMAGELIST DmListView::SetImageList(HIMAGELIST hImageList, int nImageListType) const
{
	assert(this->IsWindow());
	// LVM_SETIMAGELIST
	// wParam = ImageList 使用類型
	// lParam = ImageList 操作代碼
	return reinterpret_cast<HIMAGELIST>(::SendMessage(*this, LVM_SETIMAGELIST, static_cast<WPARAM>(nImageListType), reinterpret_cast<LPARAM>(hImageList)));
}

/**
 *	@brief	設定文字提示工具
 *	@param[in] lvsitPtr 指向 LVSETINFOTIP 結構資料位址。提示工具相關內容。
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 否則返回值為零。
 */
BOOL DmListView::SetInfoTip(LVSETINFOTIP* lvsitPtr) const
{
	assert(this->IsWindow());
	// LVM_SETINFOTIP
	// wParam = 不使用，必須為零
	// lParam = 指向 LVSETINFOTIP 結構資料位址
	return ::SendMessage(*this, LVM_SETINFOTIP, static_cast<WPARAM>(0), reinterpret_cast<LPARAM>(lvsitPtr)) != 0;
}

/**
 *	@brief	設定插入點標記到定義位置
 *	@param[in] lvimPtr 指向 LVINSERTMARK 結構的緩衝區位址，定義插入點標記位置。
 *	@return	<b>型別: BOOL</b>
 *		\n	若成功返回值為非零值，否則返回值為零。
 *		\n	如果在 LVINSERTMARK 結構的 cbSize 成員指定值不等於結構的實際大小，或者插入點並不適用。
 */
BOOL DmListView::SetInsertMark(LVINSERTMARK* lvimPtr) const
{
	assert(this->IsWindow());
	// LVM_SETINSERTMARK
	// wParam = 不使用，必須為零
	// lParam = 指向 LVINSERTMARK 結構的緩衝區位址
	return ::SendMessage(*this, LVM_SETINSERTMARK, static_cast<WPARAM>(0), reinterpret_cast<LPARAM>(lvimPtr)) != 0;
}

/**
 *	@brief 設定插入標記的顏色
 *	@param[in] dwColor 插入(新增)標記顏色 (COLORREF 格式)
 *	@return	<b>型別: COLORREF</b> \n 返回值為設定前的插入標記顏色。
 */
COLORREF DmListView::SetInsertMarkColor(COLORREF dwColor) const
{
	assert(this->IsWindow());
	// LVM_SETINSERTMARKCOLOR
	// wParam = 不使用，必須為零
	// lParam = 標記顏色值
	return static_cast<COLORREF>(::SendMessage(*this, LVM_SETINSERTMARKCOLOR, static_cast<WPARAM>(0), static_cast<LPARAM>(dwColor)));
}

/**
 *	@brief	設置 ListView 部分或全部屬性，也可以透過此函數設定項目的子項目文字內容。
 *	@param[in] lviPtr 指向 LVITEM 結構資料位址
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmListView::SetItem(const LVITEM* lviPtr) const
{
	// 範例
	// LVITEM lvi = { 0 };
	// lvi.mask = LVIF_TEXT;
	// lvi.iItem = index;
	// lvi.iSubItem = subitem; // <-- nothing happens whether I add columns first or not
	// lvi.pszText = buffer;
	// ::SendMessage(hwnd, LVM_SETITEM, 0, (LPARAM)&lvi);

	assert(this->IsWindow());
	// LVM_SETITEM
	// wParam = 未使用，必須為零
	// lParam = 指向 LVITEM 結構資料位址
	return static_cast<BOOL>(::SendMessage(*this, LVM_SETITEM, static_cast<WPARAM>(0), reinterpret_cast<LPARAM>(lviPtr)));
}

/**
 *	@brief	預先指定 ListView 項目數量，預先分配項目數量所需的記憶體空間，(要載入大量的資料時非常適用)。
 *	@param[in] nItems		指定項目數量
 *	@param[in] nBehavior	指定變更後行為
 *		- LVSICF_NOINVALIDATEALL	\n 除非受影響的項目，當前在可視範圍，否則不會重新繪製。
 *		- LVSICF_NOSCROLL			\n 當項目計數更改時，ListView 不會更改卷軸滾動位置。
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值，否則返回值為零。
 */
BOOL DmListView::SetItemCount(int nItems, int nBehavior) const
{
	assert(this->IsWindow());
	// LVM_SETITEMCOUNT
	// wParam = 指定項目數量
	// lParam = 變更後行為
	return ::SendMessage(*this, LVM_SETITEMCOUNT, static_cast<WPARAM>(nItems), static_cast<LPARAM>(nBehavior)) != 0;
}

/**
 *	@brief 設定項目狀態
 *	@param[in] lviiPtr	指向 LVITEMINDEX 結構資料位址，紀錄項目狀態
 *	@param[in] dwState	設定 ListView 狀態標記。
 *	@param[in] dwMask	指定要改變狀態的成員
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值，否則返回值為零。
 *
 *	@remark	<b>dwState 狀態標記說明 (Google 機翻尚未修飾)</b>
 *		- LVIS_ACTIVATING		\n	目前不支持。
 *		- LVIS_CUT				\n	該項目標記為剪切和粘貼操作。
 *		- LVIS_DROPHILITED		\n	該項目突出顯示為拖放目標。
 *		- LVIS_FOCUSED			\n	該項目具有焦點，因此它被標準焦點矩形包圍。雖然可以選擇多個項目，但只有一個項目可以具有焦點。
 *		- LVIS_OVERLAYMASK		\n	通過蒙版檢索項目的疊加圖像索引。
 *		- LVIS_SELECTED			\n	該項目已被選中。所選項目的外觀取決於它是否具有焦點以及用於選擇的系統顏色。
 *		- LVIS_STATEIMAGEMASK	\n	通過掩碼檢索項目的狀態圖像索引。
 *
 *	@remark	<b>dwMask 成員說明 (Google 機翻尚未修飾)</b>
 *		- LVIF_COLFMT		\n	Windows Vista及更高版本。該 piColFmt 成員有效或必須設置。如果使用此標誌，則 cColumns 成員有效或必須設置。
 *		- LVIF_COLUMNS		\n	該 cColumns 成員有效或必須設置。
 *		- LVIF_DI_SETITEM	\n	操作系統應存儲請求的列表項信息，而不是再次請求它。此標誌僅用於 LVN_GETDISPINFO 通知代碼。
 *		- LVIF_GROUPID		\n	該 iGroupId 成員有效或必須設置。如果在發送 LVM_INSERTITEM 消息時未設置此標誌，則假定 iGroupId 的值為 I_GROUPIDCALLBACK。
 *		- LVIF_IMAGE		\n	該 IIMAGE 成員有效或必須設置。
 *		- LVIF_INDENT		\n	該 iIndent 成員有效或必須設置。
 *		- LVIF_NORECOMPUTE	\n	如果收到 LVM_GETITEM 消息 ，控件將不會生成 LVN_GETDISPINFO 以檢索文本信息。相反，pszText 成員將包含 LPSTR_TEXTCALLBACK。
 *		- LVIF_PARAM		\n	該 lParam 的成員有效或必須設置。
 *		- LVIF_STATE		\n	該狀態成員有效或必須設置。
 *		- LVIF_TEXT			\n	該 pszText 成員有效或必須設置。
 */
BOOL DmListView::SetItemIndexState(LVITEMINDEX* lviiPtr, DWORD dwState, DWORD dwMask) const
{
	LV_ITEM lvi;

	assert(this->IsWindow());
	if (lviiPtr == NULL) return FALSE;
	::memset(&lvi, 0, sizeof(LV_ITEM));
	lvi.stateMask = static_cast<UINT>(dwMask);
	lvi.state = static_cast<UINT>(dwState);
	// LVM_SETITEMINDEXSTATE
	// wParam
	// lParam
	return ::SendMessage(*this, LVM_SETITEMINDEXSTATE, reinterpret_cast<WPARAM>(lviiPtr), reinterpret_cast<LPARAM>(&lvi)) == S_OK;
}

/**
 *	@brief	設定項目新的位置，ListView 顯示模式必須為圖示或大圖示。
 *	@param[in] nItem 項目索引值，從 0 算起。
 *	@param[in] ptPtr 指向 POINT 結構資料位址，內容為位置資訊(座標)。
 *	@return	<b>型別: BOOL</b> \n 返回值為新的位置資訊。
 *		<pre>
 *		LOWORD	新的左上座標 X
 *		HIWORD	新的左上座標 Y
 *		</pre>
 */
BOOL DmListView::SetItemPosition(int nItem, POINT* ptPtr) const
{
	assert(this->IsWindow());

	if (ptPtr == NULL) return FALSE;
	// LVM_SETITEMPOSITION
	// wParam = 項目索引值
	// lParam = 指定位置
	return ::SendMessage(*this, LVM_SETITEMPOSITION, static_cast<WPARAM>(nItem), MAKELPARAM(ptPtr->x, ptPtr->y)) != 0;
}

/**
 *	@brief	設定項目新的位置，ListView 顯示模式必須為圖示或大圖示。
 *	@param[in] nItem 項目索引值，從 0 算起。
 *	@param[in] ptPtr 指向 POINT 結構資料位址，內容為位置資訊(座標)。
 *	@return	此函數沒有返回值。
 *	@remark	<b>SetItemPosition 與  SetItemPosition32 的差異</b>
 *		<pre>
 *		SetItemPosition		設定的座標值使用 16-bits 方式導入。
 *		SetItemPosition32	設定的座標值使用 32-bits 方式導入。
 *		</pre>
 */
void DmListView::SetItemPosition32(int nItem, POINT* ptPtr) const
{
	assert(this->IsWindow());
	// LVM_SETITEMPOSITION32
	// wParam = 項目索引值
	// lParam = 指向 POINT 結構資料位址
	::SendMessage(*this, LVM_SETITEMPOSITION32, static_cast<WPARAM>(nItem), reinterpret_cast<LPARAM>(ptPtr));
}

/**
 *	@brief	改變指定項目的狀態
 *	@param[in] nItem	項目索引值，索引值從 0 開始算起。如果此值為 (-1) 則改變所有項目的狀態。
 *	@param[in] lviPtr	指向 LVITEM 結構資料位址，stateMask 成員設定要改變的狀態。
 *		- LVIS_ACTIVATING	\n Not currently supported.
 *		- LVIS_CUT			\n The item is marked for a cut-and-paste operation.
 *		- LVIS_DROPHILITED	\n The item is highlighted as a drag-and-drop target.
 *		- LVIS_FOCUSED		\n The item has the focus, so it is surrounded by a standard focus rectangle. Although more than one item may be selected, only one item can have the focus.
 *		- LVIS_OVERLAYMASK	\n The item's overlay image index is retrieved by a mask.
 *		- LVIS_SELECTED		\n The item is selected. The appearance of a selected item depends on whether it has the focus and also on the system colors used for selection.
 *		- LVIS_STATEIMAGEMASK \n The item's state image index is retrieved by a mask.
 *	@return	<b>型別: BOOL</b> \n 若運作成功返回值為非零值。 \n 若運作失敗則返回值為零。
 */
BOOL DmListView::SetItemState(int nItem, const LVITEM* lviPtr) const
{
	assert(this->IsWindow());
	// LVM_SETITEMSTATE
	// wParam = 項目索引值
	// lParam = 指向 LVITEM 結構資料位址
	return ::SendMessage(*this, LVM_SETITEMSTATE, static_cast<WPARAM>(nItem), reinterpret_cast<LPARAM>(lviPtr)) != 0;
}

/**
 *	@brief	改變指定項目的狀態
 *	@param[in] nItem	項目索引值，索引值從 0 開始算起。如果此值為 (-1) 則改變所有項目的狀態。
 *	@param[in] uState	要變更的狀態，狀態表同 uMask 說明。
 *	@param[in] uMask	要遮罩的狀態，非遮罩部分 uState 不會被改變，僅針對遮罩部分進行變更。
 *		- LVIS_ACTIVATING	\n Not currently supported.
 *		- LVIS_CUT			\n The item is marked for a cut-and-paste operation.
 *		- LVIS_DROPHILITED	\n The item is highlighted as a drag-and-drop target.
 *		- LVIS_FOCUSED		\n The item has the focus, so it is surrounded by a standard focus rectangle. Although more than one item may be selected, only one item can have the focus.
 *		- LVIS_OVERLAYMASK	\n The item's overlay image index is retrieved by a mask.
 *		- LVIS_SELECTED		\n The item is selected. The appearance of a selected item depends on whether it has the focus and also on the system colors used for selection.
 *		- LVIS_STATEIMAGEMASK \n The item's state image index is retrieved by a mask.
 *	@return	<b>型別: BOOL</b> \n 若運作成功返回值為非零值。 \n 若運作失敗則返回值為零。
 */
BOOL DmListView::SetItemState(int nItem, UINT uState, UINT uMask) const
{
	LVITEM lvi;
	::memset((void*)&lvi, 0, sizeof(LVITEM));
	lvi.stateMask = uMask;
	lvi.state = uState;
	return this->SetItemState(nItem, &lvi);
}

/**
 *	@brief	設定或取消項目成為被選取狀態。
 *	@param	[in] nItem		指定項目索引值，索引值從 0 開始算起
 *	@param	[in] bSelect	選擇狀態 (TRUE、為設定被選擇，FALSE、為取消被選擇)
 *	@return	<b>型別: BOOL</b> \n 若運作成功返回值為非零值。 \n 若運作失敗則返回值為零。
 */
BOOL DmListView::SetItemSelected(int nItem, BOOL bSelect) const
{
	LVITEM lvi;
	::memset((void*)&lvi, 0, sizeof(LVITEM));
	lvi.stateMask = LVIS_SELECTED | LVIS_FOCUSED;
	lvi.state = bSelect ? LVIS_SELECTED | LVIS_FOCUSED : 0;
	return this->SetItemState(nItem, &lvi);
}

/**
 *	@brief	設定或取消項目成為焦點狀態。
 *	@param	[in] nItem	指定項目索引值，索引值從 0 開始算起
 *	@param	[in] bFocus	選擇狀態 (TRUE、為設定，FALSE、為取消)
 *	@return	<b>型別: BOOL</b> \n 若運作成功返回值為非零值。 \n 若運作失敗則返回值為零。
 */
BOOL DmListView::SetItemFocused(int nItem, BOOL bFocus) const
{
	LVITEM lvi;
	::memset((void*)&lvi, 0, sizeof(LVITEM));
	lvi.stateMask = LVIS_FOCUSED;
	lvi.state = bFocus ? LVIS_FOCUSED : 0;
	return this->SetItemState(nItem, &lvi);
}

/**
 *	@brief	設定指定項目文字內容
 *	@param	[in] nItem		指定項目索引值
 *	@param	[in] nSub		項目欄位索引值
 *	@param	[in] textPtr	指向文字字串緩衝區位址
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmListView::SetItemText(int nItem, int nSub, const TCHAR* textPtr) const
{
	LVITEM lvi;

	assert(this->IsWindow());
	if (NULL == textPtr) return FALSE;
	// Setting properties Of members
	::memset((void*)&lvi, 0, sizeof(LVITEM));
	lvi.mask = LVIF_TEXT | LVIF_IMAGE;		// LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvi.iItem = nItem;						// choose item
	lvi.iSubItem = nSub;					// choose sub item
	lvi.pszText = const_cast<LPTSTR>(textPtr);	// push the string
	lvi.cchTextMax = static_cast<int>(::_tcslen(lvi.pszText));

	// LVM_SETITEMTEXT
	// wParam = 指定項目索引值
	// lParam = 指向 LVITEM 結構資料位址
	return ::SendMessage(*this, LVM_SETITEMTEXT, static_cast<WPARAM>(nItem), reinterpret_cast<LPARAM>(&lvi)) != 0;
}

/**
 *	@brief	設定 ListView 邊框顏色
 *	@param[in] dwColor COLORREF 格式的顏色值。
 *	@return	<b>型別: COLORREF</b> \n 設定前的邊框顏色值。
 */
COLORREF DmListView::SetOutlineColor(COLORREF dwColor) const
{
	assert(this->IsWindow());
	// LVM_SETOUTLINECOLOR
	// wParam = 不使用，必須為零
	// lParam = 邊框顏色值
	return static_cast<COLORREF>(::SendMessage(*this, LVM_SETOUTLINECOLOR, static_cast<WPARAM>(0), static_cast<LPARAM>(dwColor)));
}

/**
 *	@brief	設定指定欄位選取狀態
 *	@param[in] nCol 欄位索引值。
 *	@return	此函數沒有返回值
 */
void DmListView::SetSelectedColumn(int nCol) const
{
	assert(this->IsWindow());
	// LVM_SETSELECTEDCOLUMN
	// wParam = 欄位索引值
	// lParam = 不使用，必須為零
	::SendMessage(*this, LVM_SETSELECTEDCOLUMN, static_cast<WPARAM>(nCol), 0);
}

/**
 *	@brief	設定指定項目選取標記
 *	@param[in] nItem 項目索引值，若為多選則會取第一個被選項目，若值為(-1)代表刪除標記。
 *	@return	<b>型別: int</b> \n 返回值為先前被設定標記的項目索引值。 \n 若返回值為(-1)代表先前沒有被設定標記的項目。
 */
int DmListView::SetSelectionMark(int nItem) const
{
	assert(this->IsWindow());
	// LVM_SETSELECTIONMARK
	// wParam = 不使用，必須為零
	// lParam = 項目索引值
	return static_cast<int>(::SendMessage(*this, LVM_SETSELECTIONMARK, 0, static_cast<int>(nItem)));
}

/**
 *	@brief	設定 ListView 文字背景顏色
 *	@param	[in] dwColor 顏色(RGB)，背景顏色 RGB(若使用 CLR_NONE 定義值，則為無被景色)
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmListView::SetTextBkColor(COLORREF dwColor) const
{
	assert(this->IsWindow());
	// LVM_SETTEXTBKCOLOR
	// wParam = 未使用，必須為零
	// lParam = 顏色值
	return ::SendMessage(*this, LVM_SETTEXTBKCOLOR, static_cast<WPARAM>(0), static_cast<LPARAM>(dwColor)) != 0;
}

/**
 *	@brief	設定 ListView 文字顏色
 *	@param	[in] dwColor 顏色(RGB)
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmListView::SetTextColor(COLORREF dwColor) const
{
	assert(this->IsWindow());
	// LVM_SETTEXTCOLOR
	// wParam = 未使用，必須為零
	// lParam = 顏色值
	return static_cast<BOOL>(::SendMessage(*this, LVM_SETTEXTCOLOR, static_cast<WPARAM>(0), static_cast<LPARAM>(dwColor)));
}

/**
 *	@brief	設定 ListView Tile 資訊
 *	@param[in] lvtiPtr 指向 LVTILEINFO 結構資料位址，設置 Tile 內容結構。
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmListView::SetTileInfo(LVTILEINFO* lvtiPtr) const
{
	assert(this->IsWindow());
	// LVM_SETTILEINFO
	// wParam = 不使用，必須為零
	// lParam = 指向 LVTILEINFO 結構資料位址
	return ::SendMessage(*this, LVM_SETTILEINFO, static_cast<WPARAM>(0), reinterpret_cast<LPARAM>(lvtiPtr)) != 0;
}

/**
 *	@brief	設定 ListView 使用並排顯示的資訊。
 *	@param[in] lvtviPtr 指向 LVTILEVIEWINFO 結構資料位址，內容為 ListView 的相關資訊。
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmListView::SetTileViewInfo(LVTILEVIEWINFO* lvtviPtr) const
{
	assert(this->IsWindow());
	// LVM_SETTILEVIEWINFO
	// wParam = 不使用，必須為零
	// lParam = 指向 LVTILEVIEWINFO 結構資料位址
	return ::SendMessage(*this, LVM_SETTILEVIEWINFO, static_cast<WPARAM>(0), reinterpret_cast<LPARAM>(lvtviPtr)) != 0;
}

/**
 *	@brief	設定提示工具
 *	@param[in] hWndTips 提示工具操作代碼
 *	@return	<b>型別: HWND</b> \n 若成功返回值為非零值，值為設定前的提示工具操作代碼。 \n 若失敗返回值為零。
 *	@remark	若不使用提示工具，建立 ListView 時需指定 LVS_NOTOOLTIPS 樣式。
 */
HWND DmListView::SetToolTips(HWND hWndTips) const
{
	assert(this->IsWindow());
	// LVM_SETTOOLTIPS
	// wParam = 提示工具視窗操作代碼
	// lParam = 不使用，必須為零
	return reinterpret_cast<HWND>(::SendMessage(*this, LVM_SETTOOLTIPS, reinterpret_cast<WPARAM>(hWndTips), static_cast<LPARAM>(0)));
}

/**
 *	@brief	設定 ListView 文字編碼為 Unicode
 *	@param[in] bUnicode	使否使用 Unicode 字符集
 *		\n 若值為 TRUE 表示使用 Unicode 字符集
 *		\n 若值為 FALSE	使用 ANSI 字符集
 *	@return	<b>型別: BOOL</b> \n 返回先前設定的的 Unicode 字符集使用狀態，若返回值為 TRUE 為表示使用 Unicode 字符集，否則為未使用。
 */
BOOL DmListView::SetUnicodeFormat(BOOL bUnicode) const
{
	// LVM_SETUNICODEFORMAT
	// wParam = 使否為 Unicode 編碼
	// lParam = 不使用，必須為零
	return ::SendMessage(*this, LVM_SETUNICODEFORMAT, static_cast<WPARAM>(bUnicode), static_cast<LPARAM>(0)) != 0;
}

/**
 *	@brief	設定 ListView 呈現方式，如大圖式、小圖示、詳細資料。。。等
 *	@param[in] dwShow 指定呈現的方式
 *	@return	<b>型別: int</b> \n 若成功返回值為 (1)，若失敗則返回值為 (-1)，如果呈現方式無效，則返回 (-1)。
 *	@remark	<b>dwShow 呈現方式</b>
 *		- LV_VIEW_DETAILS
 *		- LV_VIEW_ICON
 *		- LV_VIEW_LIST
 *		- LV_VIEW_SMALLICON
 *		- LV_VIEW_TILE
 */
int DmListView::SetView(DWORD dwShow) const
{
	assert(this->IsWindow());
	// LVM_SETVIEW
	// wParam = 呈現方式
	// lParam = 不使用，必須為零
	return static_cast<int>(::SendMessage(*this, LVM_SETVIEW, static_cast<WPARAM>(dwShow), static_cast<LPARAM>(0)));
}

/**
 *	@brief	設定 ListView 的工作區
 *	@param[in] nWorkAreas	指定 rcPtr 所使用的 RECT 結構數量。允許的最大工作區為 LV_MAX_WORKAREAS。
 *	@param[in] rcPtrArray	指向 RECT 結構陣列，用來指定新的工作區。
 *	@return	此函數沒有返回值
 */
void DmListView::SetWorkAreas(int nWorkAreas, const RECT* rcPtrArray) const
{
	assert(this->IsWindow());
	// LVM_SETWORKAREAS
	// wParam = 使用 RECT 結構數量
	// lParma = 指向 RECT 結構陣列
	::SendMessage(*this, LVM_SETWORKAREAS, static_cast<WPARAM>(nWorkAreas), reinterpret_cast<LPARAM>(rcPtrArray));
}

/**
 *	@brief	指定群組的排序比對函數位址。
 *	@param[in] fnGroupComparePtr 指向群組比對函式位址
 *	@param[in] vPtr 指向由應用程式定義的資料位址。
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值，若失敗返回值為零。
 */
BOOL DmListView::SortGroups(PFNLVGROUPCOMPARE fnGroupComparePtr, LPVOID vPtr) const
{
	assert(this->IsWindow());
	// LVM_SORTGROUPS
	// wParam = 指向群組比對函式位址
	// lParma = 指向由應用程式定義的資料位址
	return ::SendMessage(*this, LVM_SORTGROUPS, reinterpret_cast<WPARAM>(fnGroupComparePtr), reinterpret_cast<LPARAM>(vPtr)) != 0;
}

/**
 *	@brief	指定排序比對函數位址，排序 ListView 的項目順序。
 *	@param[in] fnComparePtr 指向 PFNLVCOMPARE 型別的 callback 函數位址，此函數為進行比對的函數。
 *		\n 排序作業呼叫的比對函式，每次需要判斷兩個清單項目的相對順序。
 *		\n 若在類別中，比對函式必須是靜態成員，或是獨立的函數，不屬於任何類別的成員。
 *	@param[in] dwData 應用程式定義的值傳遞至比對函數。
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 *	@remark	比對函數格式 \n int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
 */
BOOL DmListView::SortItems(PFNLVCOMPARE fnComparePtr, DWORD_PTR dwData) const
{
	assert(this->IsWindow());
	// LVM_SORTITEMS
	// wParam = 傳遞給比對函數的值
	// lParam = 指向 PFNLVCOMPARE 型別的 callback 函數位址
	return ::SendMessage(*this, LVM_SORTITEMS, static_cast<WPARAM>(dwData), reinterpret_cast<LPARAM>(fnComparePtr)) != 0;
}

/**
 *	@brief	使用應用程序定義的比對函數，對 ListView 的項目進行排序。每個項目的索引值都會改變以對應新的排序。
 *	@param[in] fnComparePtr 指向 PFNLVCOMPARE 型別的 callback 函數位址，此函數為進行比對的函數。
 *	@param[in] dwData 應用程式定義的值傳遞至比對函數。
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 *	@remark	比對函數格式 \n int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
 */
BOOL DmListView::SortItemsEx(PFNLVCOMPARE fnComparePtr, DWORD_PTR dwData) const
{
	assert(this->IsWindow());
	// LVM_SORTITEMSEX
	// wParam = 傳遞給比對函數的值
	// lParam = 指向 PFNLVCOMPARE 型別的 callback 函數位址
	return ::SendMessage(*this, LVM_SORTITEMSEX, static_cast<WPARAM>(dwData), reinterpret_cast<LPARAM>(fnComparePtr)) != 0;
}

/**
 *	@brief	子項目點擊測試，測試點擊位置是否子項目。
 *	@param[in, out] lvtiPtr	指向 LVHITTESTINFO 結構資料位址。
 *							\n 設置為要進行點擊測試的 ListView 客戶區(Client-area)座標。
 *							\n 用於接收點擊訊息相關訊息。
 *	@return	<b>型別: int</b>
 *		\n 返回值為測試子項目的索引值，否則返回值為 (-1)。
 *		\n 如果子項目位於測試點擊定義的座標處，則 LVHITTESTINFO 結構內容為點擊相關訊息。
 */
int DmListView::SubItemHitTest(LVHITTESTINFO* lvtiPtr) const
{
	assert(this->IsWindow());
	// LVM_SUBITEMHITTEST
	// wParam = 不使用，必須為零
	// lParam = 指向 LVHITTESTINFO 結構資料位址
	return static_cast<int>(::SendMessage(*this, LVM_SUBITEMHITTEST, static_cast<WPARAM>(0), reinterpret_cast<LPARAM>(lvtiPtr)));
}

/**
 *	@brief	更新具有 LVS_AUTOARRANGE 樣式的項目
 *	@param[in] nItem 指定項目索引值
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmListView::Update(int nItem) const
{
	assert(this->IsWindow());
	// LVM_UPDATE
	// wParam = 項目索引值
	// lParam = 不使用，必須為零
	return ::SendMessage(*this, LVM_UPDATE, static_cast<WPARAM>(nItem), static_cast<LPARAM>(0)) != 0;
}

/**
 *	@brief	[虛擬函數] 使用自訂 ListView Report 預設樣式
 *	@return	此函數沒有返回值
 *	@see	DmListView
 *	@see	https://docs.microsoft.com/en-us/windows/desktop/controls/extended-list-view-styles
 */
void DmListView::DefaultReportStyle() const
{
	// ListView 視窗樣式
	// LVS_REPORT				: 設定成 Report 樣式
	// LVS_SHOWSELALWAYS		: 非當前使用視窗時，仍始終顯示被選擇項目。
	// LVS_EDITLABELS			: 子項目欄位可編輯
	DWORD dwStyle = LVS_REPORT
		| LVS_SHOWSELALWAYS;

	// ListView 擴充樣式
	// LVS_EX_FULLROWSELECT     : 選擇時，整個項目會呈現被選擇狀態。
	// LVS_EX_GRIDLINES         : 使用網格
	// LVS_EX_CHECKBOXES        : 帶有 Check Box
	// LVS_EX_HEADERDRAGDROP    : 讓 Listview Header(表頭) 顯示位置甩曳交換
	// LVS_EX_ONECLICKACTIVATE  : 單擊觸發
	// LVS_EX_TWOCLICKACTIVATE  : 雙擊觸發
	// LVS_EX_MULTIWORKAREAS    : 多工作區 (可複選項目)
	// LVS_EX_SUBITEMIMAGES		: 允許子項目使用圖示
	DWORD dwExStyle = LVS_EX_FULLROWSELECT
		| LVS_EX_GRIDLINES
		| LVS_EX_HEADERDRAGDROP
		| LVS_EX_AUTOSIZECOLUMNS
		| LVS_EX_SUBITEMIMAGES;

	dwStyle |= this->GetStyle();
	dwExStyle |= this->GetExStyle();
	this->SetStyle(dwStyle);
	this->SetExtendedListViewStyleEx(dwExStyle);
}
