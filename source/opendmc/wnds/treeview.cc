/**************************************************************************//**
 * @file	treeview.cc
 * @brief	DmTreeView 樹狀圖示列表類別成員函數定義
 * @date	2000-10-10
 * @date	2018-09-12
 * @author	Swang
 *****************************************************************************/
#include "opendmc/wnds/treeview.hh"

/*
 *	@brief DmTreeView 建構式
 */
DmTreeView::DmTreeView() : DmWnds(EmCtrls::TreeView) { }

/*
 *	@brief DmTreeView 解構式
 *	@remark 解構時進行釋放(摧毀)控制項
 */
DmTreeView::~DmTreeView() { this->SafeWndsDestroy(); }

/**
 *	@brief [重載] 建立一個樹狀圖示列表 TreeView
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
BOOL DmTreeView::Create(const TCHAR* szCaptionPtr, int x, int y, int wd, int ht, HWND hWndParent, int nIDCItem)
{
	const DWORD dwStyle = TVS_HASLINES;
	const DWORD dwExStyle = 0;

	// TreeView 不需要 caption name
	UNREFERENCED_PARAMETER(szCaptionPtr);

	WNDSCTRLS smCtrl;
	::memset(&smCtrl, 0, sizeof(smCtrl));
	smCtrl.hInstance	= ::GetWinapp().GetInstanceHandle();
	smCtrl.hParent		= hWndParent;
	smCtrl.pszCaption	= NULL;	
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
 *	@brief 建立一個樹狀圖示列表樣本 (由繼承者發展)
 *	@return	<b>型別: BOOL</b> 若視窗建立成功返回值為非零值，若視窗建立失敗責返回值為零。
 */
BOOL DmTreeView::CreateSample() { return FALSE; }

/**
 *	@brief	建立樹狀目錄的項目拖曳點功能。
 *	@param[in] hItem 樹狀目錄項目操作代碼(Handle)。
 *	@return	<b>型別: HWND</b> \n 若成功返回值為非零值，值為樹狀目錄的項目操作代碼。 \n 若失敗則返回值為零。
 */
HWND DmTreeView::CreateDragImage(HTREEITEM hItem) const
{
	assert(this->IsWindow());
	// TVM_CREATEDRAGIMAGE
	// wParam = 不使用，必須為零
	// lParam = 樹狀目錄的項目操作代碼
	return reinterpret_cast<HWND>(::SendMessage(*this, TVM_CREATEDRAGIMAGE, 0, reinterpret_cast<LPARAM>(hItem)));
}

/**
 *	@brief	刪除樹狀目錄所有的項目
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmTreeView::DeleteAllItem() const
{
	assert(this->IsWindow());
	// TVM_DELETEITEM
	// wParam = 不使用，必須為零
	// lParam = 要刪除的狀目錄的項目，指定使用 TVI_ROOT 進行刪除所有項目。
	return ::SendMessage(*this, TVM_DELETEITEM, 0, reinterpret_cast<LPARAM>(TVI_ROOT)) != 0;
}

/**
 *	@brief	刪除指定的樹狀目錄的項目
 *	@param[in] hItem 樹狀目錄的項目操作代碼。
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmTreeView::DeleteItem(HTREEITEM hItem) const
{
	assert(this->IsWindow());
	// TVM_DELETEITEM
	// wParam = 不使用，必須為零
	// lParam = 項目操作代碼
	return ::SendMessage(*this, TVM_DELETEITEM, 0, reinterpret_cast<LPARAM>(hItem)) != 0;
}

/**
 *	@brief	進行編輯的指定樹狀目錄的項目文字
 *	@param[in] hItem 項目操作代碼。
 *	@return	<b>型別: HWND</b> \n 若成功返回值為非零值，值為編輯框操作代碼。 \n 若失敗則返回值為零。
 */
HWND DmTreeView::EditLabel(HTREEITEM hItem) const
{
	assert(this->IsWindow());
	// TVM_EDITLABEL
	// wParam = 不使用，必須為零
	// lParam = 項目操作代碼
	return reinterpret_cast<HWND>(::SendMessage(*this, TVM_EDITLABEL, 0, reinterpret_cast<LPARAM>(hItem)));
}

/**
 *	@brief	結束編輯樹狀目錄的項目文字
 *	@param[in] fCancelWithoutSave 指定結束編輯後是否保存，若參數值為 TRUE 表示不保存及結束編輯，若 FALSE 表示進行保存及結束編輯。
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmTreeView::EndEditLabelNow(BOOL fCancelWithoutSave) const
{
	assert(this->IsWindow());
	// TVM_ENDEDITLABELNOW
	// wParam = 指定結束編輯後是否保存
	// lParam = 不使用，必須為零
	return ::SendMessage(*this, TVM_ENDEDITLABELNOW, static_cast<WPARAM>(fCancelWithoutSave), 0) != 0;
}

/**
 *	@brief	指定樹狀目錄的項目顯示在可視範圍內。
 *	@param[in] hItem 樹狀目錄的項目操作代碼
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmTreeView::EnsureVisible(HTREEITEM hItem) const
{
	assert(this->IsWindow());
	// TVM_ENSUREVISIBLE
	// wParam = 不使用，必須為零
	// lParam = 項目操作代碼
	return ::SendMessage(*this, TVM_ENSUREVISIBLE, 0, reinterpret_cast<LPARAM>(hItem)) != 0;
}

/**
 *	@brief	展開或折疊與指定父項目旗下的子項目
 *	@param[in] hItem 樹狀目錄的項目操作代碼
 *	@param[in] nCode 採取的動作類型的旗標。
 *		- TVE_COLLAPSE		\n 摺疊清單。
 *		- TVE_COLLAPSERESET	\n 摺疊清單，並移除子項目。 TVIS_EXPANDEDONCE狀態旗標會重設。 這個旗標必須搭配TVE_COLLAPSE旗標。
 *		- TVE_EXPAND		\n 展開清單。
 *		- TVE_TOGGLE		\n 如果目前為展開狀態則將其摺疊，如果目前為摺疊狀態則將其展開。
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmTreeView::Expand(HTREEITEM hItem, UINT nCode) const
{
	assert(this->IsWindow());
	// TVM_EXPAND
	// wParam = 採取的動作類型的旗標
	// lParam = 項目操作代碼
	return ::SendMessage(*this, TVM_EXPAND, static_cast<WPARAM>(nCode), reinterpret_cast<LPARAM>(hItem)) != 0;
}

/**
 *	@brief	取得樹狀目錄的項目的背景顏色
 *	@return	<b>型別: COLORREF</b> \n 返回值表示當前背景顏色的 COLORREF 值。 \n 如果此值為 (-1) 則表示使用系統預設背景顏色。
 */
COLORREF DmTreeView::GetBkColor() const
{
	assert(this->IsWindow());
	// TVM_GETBKCOLOR
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<COLORREF>(::SendMessage(*this, TVM_GETBKCOLOR, 0, 0));
}

/**
 *	@brief	取得樹狀目錄的項目下第一個子項目
 *	@param[in] hItem 樹狀目錄的項目操作代碼
 *	@return	<b>型別:HTREEITEM </b> \n 若成功返回值為非零值，值為樹狀目錄的項目操作代碼。 \n 若函數操作失敗返回值為零。
 *	@note	此函數直接呼叫(調用) GetNextItem 函數進行處理
 */
HTREEITEM DmTreeView::GetChildItem(HTREEITEM hItem) const
{
	return this->GetNextItem(hItem, TVGN_CHILD);
}

/**
 *	@brief	取得目前樹狀目錄的項目數量
 *	@return	<b>型別: UINT</b> \n 返回值為樹狀目錄的項目數量。
 */
UINT DmTreeView::GetCount() const
{
	assert(this->IsWindow());
	// TVM_GETCOUNT
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<UINT>(::SendMessage(*this, TVM_GETCOUNT, 0, 0));
}

/**
 *	@brief	取得拖放操作目標的樹狀目錄的項目。
 *	@return	<b>型別:HTREEITEM </b> \n 若成功返回值為非零值，值為樹狀項目操作代碼。 \n 若失敗返回值為零。
 *	@note	此函數直接呼叫(調用) GetNextItem 函數進行處理
 */
HTREEITEM DmTreeView::GetDropHiLightItem() const
{
	return this->GetNextItem(NULL, TVGN_DROPHILITE);
}

/**
 *	@brief	取得樹狀目錄的項目，文字編輯框操作代碼
 *	@return	<b>型別: HWND</b> \n 若函數操作成功，返回值為非零值，值為編輯框操作代碼。 \n 若函數操作失敗，返回值為零。
 */
HWND DmTreeView::GetEditControl() const
{
	assert(this->IsWindow());
	// TVM_GETEDITCONTROL
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return reinterpret_cast<HWND>(::SendMessage(*this, TVM_GETEDITCONTROL, 0, 0));
}

/**
 *	@brief	取得樹狀目錄的擴展樣示
 *	@return	<b>型別: DWORD</b> \n 返回值為樹狀目錄的擴展項示
 *	@see	https://docs.microsoft.com/en-us/windows/desktop/controls/tree-view-control-window-extended-styles
 */
DWORD DmTreeView::GetExtendedStyle() const
{
	assert(this->IsWindow());
	// TVM_GETEXTENDEDSTYLE
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<DWORD>(::SendMessage(*this, TVM_GETEXTENDEDSTYLE, 0, 0));
}

/**
 *	@brief	取得樹狀目錄第一個可見的項目。
 *	@return	<b>型別:HTREEITEM </b> \n 若操作成功返回值為非零值，值為樹狀項目操作代碼。 \n 若操作失敗返回值為零。
 *	@note	此函數直接呼叫(調用) GetNextItem 函數進行處理
 */
HTREEITEM DmTreeView::GetFirstVisibleItem() const
{
	return this->GetNextItem(NULL, TVGN_FIRSTVISIBLE);
}

/**
 *	@brief	取得樹狀目錄標準或圖像列表控制項代碼。
 *	@param[in] nImageList	要取得的圖像列表的類型
 *		- TVSIL_NORMAL		\n 表示標準圖像列表，包含選定與未選定的圖像。
 *		- TVSIL_STATE		\n 表示狀態圖像列表，包含使用者定義樹狀目錄檢視項目。
 *	@return	<b>型別: HIMAGELIST</b> \n 返回值為指定圖像列表的操作代碼 (HIMAGELIST)。
 */
HIMAGELIST DmTreeView::GetImageList(UINT nImageList) const
{
	assert(this->IsWindow());
	// TVM_GETIMAGELIST
	// wParam = 要取得的圖像列表的類型
	// lParam = 不使用，必須為零
	return reinterpret_cast<HIMAGELIST>(::SendMessage(*this, TVM_GETIMAGELIST, static_cast<WPARAM>(nImageList), 0));
}

/**
 *	@brief	取得樹狀目錄項目與子項目縮排距離，單位像素(pixel)
 *	@return	<b>型別: UINT</b> \n 返回值為子項目與父項目間的縮牌距離，單位像素。
 */
UINT DmTreeView::GetIndent() const
{
	assert(this->IsWindow());
	// TVM_GETINDENT
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<UINT>(::SendMessage(*this, TVM_GETINDENT, 0, 0));
}

/**
 *	@brief	取得樹狀目錄插入標記的顏色
 *	@return	<b>型別: COLORREF</b> \n 返回值為當前插入標記顏色(COLORREF)值。
 */
COLORREF DmTreeView::GetInsertMarkColor() const
{
	assert(this->IsWindow());
	// TVM_GETINSERTMARKCOLOR
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<UINT>(::SendMessage(*this, TVM_GETINSERTMARKCOLOR, 0, 0));
}

/**
 *	@brief	取得樹狀目錄新增的搜索文字字串
 *	@param[in] strPtr 指向接收增量搜索字符串的緩衝區位址的指標。
 *	@return	<b>型別: UINT</b> \n 返回值為增量搜索的文字字串的文字數，單位 TCHAR
 */
UINT DmTreeView::GetISearchString(const TCHAR* strPtr) const
{
	assert(this->IsWindow());
	// TVM_GETISEARCHSTRING
	// wParam = 不使用，必須為零
	// lParam = 指向接收增量搜索字符串的緩衝區位址。
	return static_cast<UINT>(::SendMessage(*this, TVM_GETISEARCHSTRING, 0, reinterpret_cast<LPARAM>(strPtr)));
}

/**
 *	@brief	取得樹狀目錄的項目屬性
 *	@param[out]	tiPtr 指向 TVITEM 的緩衝區位址的指標，用來接收樹狀目錄的項目屬性。
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmTreeView::GetItem(TVITEM* tiPtr) const
{
	assert(this->IsWindow());
	// TVM_GETITEM
	// wParam = 不使用，必須為零
	// lParam = 指向 TVITEM 的緩衝區位址。
	return ::SendMessage(*this, TVM_GETITEM, 0, reinterpret_cast<LPARAM>(tiPtr)) != 0;
}

/**
 *	@brief	取得指定的樹狀目錄的項目相關聯的 32 位元應用程式特定值
 *	@param[in] hItem 樹狀目錄項目操作代碼
 *	@return <b></b> \n 指定的項目相關聯的 32 位元應用程式專屬值。
 */
DWORD_PTR DmTreeView::GetItemData(HTREEITEM hItem) const
{
	assert(this->IsWindow());

	TVITEM tvi;
	::memset(&tvi, 0, sizeof(tvi));
	tvi.mask = TVIF_PARAM;
	tvi.hItem = hItem;
	::SendMessage(*this, TVM_GETITEM, 0, reinterpret_cast<LPARAM>(&tvi));
	return tvi.lParam;
}

/**
 *	@brief	取得樹狀目錄的高度，單位像素 (in pixel)
 *	@return	<b>型別: int</b> \n 返回值為樹狀目錄的高度，單位像素 (in pixel)。
 */
int DmTreeView::GetItemHeight() const
{
	assert(this->IsWindow());
	// TVM_GETITEMHEIGHT
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<int>(::SendMessage(*this, TVM_GETITEMHEIGHT, 0, 0));
}

/**
 *	@brief	取得樹狀目錄的項目相關聯的點陣圖影像
 *	@param[in] hItem			樹狀目錄的項目操作代碼
 *	@param[in] nImage			用來接收樹狀目錄的影像清單中影像項目索引。
 *	@param[in] nSelectedImage	用來接收樹狀目錄的影像清單中選取的影像項目索引。
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmTreeView::GetItemImage(HTREEITEM hItem, int& nImage, int& nSelectedImage) const
{
	assert(this->IsWindow());

	BOOL bResult = FALSE;
	TVITEM tvi;
	::memset(&tvi, 0, sizeof(tvi));
	tvi.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvi.hItem = hItem;
	bResult = ::SendMessage(*this, TVM_GETITEM, 0, reinterpret_cast<LPARAM>(&tvi)) != 0;
	nImage = tvi.iImage;
	nSelectedImage = tvi.iSelectedImage;
	return bResult;
}

/**
 *	@brief	取得指定的樹狀目錄的項目組件的矩形邊界尺寸。
 *	@param[in] hItem 指定樹狀目錄的項目操作代碼
 *	@param[in] nPart 指定樹狀目錄的項目組件識別碼
 *	@param[in] rcPtr 指向 RECT 結構緩衝區位址的指標，用來接收指定樹狀目錄的項目組件矩形邊界尺寸。
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmTreeView::GetItemPartRect(HTREEITEM hItem, int nPart, RECT* rcPtr) const
{
	TVGETITEMPARTRECTINFO tvInfo;

	assert(this->IsWindow());
	tvInfo.hti = hItem;
	tvInfo.prc = rcPtr;
	tvInfo.partID = static_cast<TVITEMPART>(nPart);

	// TVM_GETITEMPARTRECT
	// wParam = 不使用，必須為零
	// lParam = 指向 TVGETITEMPARTRECTINFO 結構緩衝區位址
	return ::SendMessage(*this, TVM_GETITEMPARTRECT, 0, reinterpret_cast<LPARAM>(&tvInfo)) != 0;
}

/**
 *	@brief	取得指定項目矩形邊界尺寸資訊
 *	@param[in] hItem		樹狀目錄項目操作代碼
 *	@param[in] rcPtr		指向 RECT 結構緩衝區位址的指標，用來接收指定項目的矩形邊界尺寸資訊。
 *	@param[in] bTextOnly	指定是否僅取得文字區的尺寸資訊，若此值為 0, 則取得該項目所包含的所有範圍，若此值為 1 則僅紀錄文字區塊的尺寸。
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmTreeView::GetItemRect(HTREEITEM hItem, RECT* rcPtr, BOOL bTextOnly) const
{
	assert(this->IsWindow());
	// TVM_GETITEMRECT
	// wParam = 要取得邊界尺寸的方法
	// lParam = 指向 RECT 結構緩衝區位址，!!! 注意 !!! 在呼叫函數前，必須將此值先填入項目操作碼
	if (rcPtr != NULL) *(HTREEITEM*)rcPtr = hItem;
	return ::SendMessage(*this, TVM_GETITEMRECT, static_cast<WPARAM>(bTextOnly), reinterpret_cast<LPARAM>(rcPtr)) != 0;
}

/**
 *	@brief	取得指定項目的屬性
 *	@param[in] hItem		樹狀目錄項目操作代碼
 *	@param[in] nStateMask	要取得狀態的遮罩，可以參照 TVITEMEX 結構。
 *	@return	<b>型別: UINT</b> \n 返回值為指定樹狀目錄項目的狀態值，個狀態紀錄於各 Bit 中，可以參照 TVITEMEX 結構。
 *	@see	https://docs.microsoft.com/en-us/windows/desktop/api/Commctrl/ns-commctrl-tagtvitemexa
 */
UINT DmTreeView::GetItemState(HTREEITEM hItem, UINT nStateMask) const
{
	assert(this->IsWindow());
	// TVM_GETITEMSTATE
	// wParam = 項目操作代碼
	// lParam = 要取得狀態的遮罩
	return static_cast<UINT>(::SendMessage(*this, TVM_GETITEMSTATE, reinterpret_cast<WPARAM>(hItem), static_cast<LPARAM>(nStateMask)));
}

/**
 *	@brief	取得樹狀目錄的線條顏色
 *	@return	<b>型別: COLORREF</b> \n 返回值為樹狀目錄的線條顏色
 */
COLORREF DmTreeView::GetLineColor() const
{
	assert(this->IsWindow());
	// TVM_GETLINECOLOR
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<COLORREF>(::SendMessage(*this, TVM_GETLINECOLOR, 0, 0));
}

/**
 *	@brief	取得指定項目的文字內容
 *	@param[in]	hItem	樹狀目錄項目操作代碼
 *	@param[out] textPtr	要接收字串的緩衝區位址
 *	@param[in]	cchMax	要接收嘴大的字串文字數
 *	@return	<b>型別: BOOL</b> \n 若函數操作成功返回值為非零值。 \n 若函數操作失敗返回值為零。
 */
BOOL DmTreeView::GetItemText(HTREEITEM hItem, TCHAR* textPtr, int cchMax) const
{
	assert(IsWindow());

	BOOL bResult = FALSE;
	if (cchMax > 0)
	{
		TVITEM tvi;
		::memset(&tvi, 0, sizeof(tvi));
		tvi.hItem = hItem;
		tvi.mask = TVIF_TEXT;
		tvi.cchTextMax = cchMax;
		tvi.pszText = textPtr;
		bResult = ::SendMessage(*this, TVM_GETITEM, 0, reinterpret_cast<LPARAM>(&tvi)) != 0;
	}
	return bResult;
}

/**
 *	@brief	取得樹狀最後一個展開的項目。!!!「不是取得樹狀視窗可見的最後一個項目」!!!
 *	@return	<b>型別:HTREEITEM </b> \n 若函數操作成功返回值為非零值，值為樹狀項目操作代碼。 \n 若函數操作失敗返回值為零。
 */
HTREEITEM DmTreeView::GetLastVisibleItem() const
{
	return this->GetNextItem(NULL, TVGN_LASTVISIBLE);
}

/**
 *	@brief	取得樹狀目錄項目的關連性，依據指定的旗標值取得對應的關連。
 *	@param[in] hItem	樹狀項目操作代碼
 *	@param[in] nCode	要取得對應關連的旗標
 *		- TVGN_CARET		\n 取得當前選定的項目。也可以呼叫(調用) GetSelection 函數來取得。
 *		- TVGN_CHILD		\n 取得 hItem 參數指定的項目的第一個子項目。也可以呼叫(調用) GetChildItem 函數來取得。
 *		- TVGN_DROPHILITE	\n 取得為拖放操作目標的項目。也可以呼叫(調用) GetDropHilightItem 函數來取得。
 *		- TVGN_FIRSTVISIBLE \n 取得樹狀目錄的視窗第一個可見的項目。也可以呼叫(調用) GetFirstVisibleItem 函數來取得。
 *		- TVGN_LASTVISIBLE	\n 取得樹狀最後一個展開的項目。「不是取得樹狀視窗可見的最後一個項目」。也可以呼叫(調用) GetLastVisibleItem 函數來取得。
 *		- TVGN_NEXT			\n 取得下一個兄弟項目。也可以呼叫(調用) GetNextSiblingItem 函數來取得。
 *		- TVGN_NEXTSELECTED	\n 取得下一個選定的項目。也可以呼叫(調用) GetNextSelectedItem 函數來取得。
 *		- TVGN_NEXTVISIBLE	\n 取得指定項目的下一個可見項目。必須顯示指定的項目。可發送 TVM_GETITEMRECT 訊息來確定項目是否可見。也可以呼叫(調用) GetNextVisibleItem 函數來取得。
 *		- TVGN_PARENT		\n 取得指定項目的父項目。可以呼叫(調用) GetParentItem 函數來取得。
 *		- TVGN_PREVIOUS		\n 取得上一個兄弟項目。也可以呼叫(調用) GetPrevSiblingItem 函數來取得。
 *		- TVGN_PREVIOUSVISIBLE \n 取得指定項目之前的第一個可見項目(調用)。必須顯示指定的項目。可發送 TVM_GETITEMRECT 訊息確定項目是否可見。也可以呼叫 GetPrevVisibleItem 函數來取得。
 *		- TVGN_ROOT			\n 取得樹狀目錄最頂部或第一個項目。也可以呼叫(調用) GetRootItem 函數來取得。
 *	@return	<b>型別:HTREEITEM </b> \n 若成功返回值為非零值，值為樹狀項目操作代碼。 \n 若失敗則返回值為零。
 */
HTREEITEM DmTreeView::GetNextItem(HTREEITEM hItem, UINT nCode) const
{
	assert(this->IsWindow());
	// TVM_GETNEXTITEM
	// wParam = 要取得對應關連的旗標
	// lParam = 樹狀項目操作代碼
	return reinterpret_cast<HTREEITEM>(::SendMessage(*this, TVM_GETNEXTITEM, static_cast<WPARAM>(nCode), reinterpret_cast<LPARAM>(hItem)));
}

/**
 *	@brief	取得樹狀目錄的項目的下一個兄弟的項目
 *	@param[in] hItem 樹狀項目操作代碼
 *	@return	<b>型別:HTREEITEM </b> \n 若函數操作成功返回值為非零值，值為樹狀目錄的項目操作代碼。 \n 若函數操作失敗返回值為零。
 *	@note	此函數直接呼叫(調用) GetNextItem 函數進行處理
 */
HTREEITEM DmTreeView::GetNextSiblingItem(HTREEITEM hItem) const
{
	return this->GetNextItem(hItem, TVGN_NEXT);
}

/**
 *	@brief	取得下一個被選定的樹狀目錄的項目
 *	@param[in] hItem 樹狀項目操作代碼
 *	@return	<b>型別:HTREEITEM </b> \n 若函數操作成功返回值為非零值，值為樹狀項目操作代碼。 \n 若函數操作失敗返回值為零。
 *	@note	此函數直接呼叫(調用) GetNextItem 函數進行處理
 */
HTREEITEM DmTreeView::GetNextSelectedItem(HTREEITEM hItem) const
{
	return this->GetNextItem(hItem, TVGN_NEXTSELECTED);
}

/**
 *	@brief	取得樹狀項目的下一個可見的項目
 *	@param[in] hItem 樹狀項目操作代碼
 *	@return	<b>型別:HTREEITEM </b> \n 若函數操作成功返回值為非零值，值為樹狀項目操作代碼。 \n 若函數操作失敗返回值為零。
 *	@note	此函數直接呼叫(調用) GetNextItem 函數進行處理
 */
HTREEITEM DmTreeView::GetNextVisibleItem(HTREEITEM hItem) const
{
	return this->GetNextItem(hItem, TVGN_NEXTVISIBLE);
}

/**
 *	@brief	取得樹狀項目的父項目
 *	@param[in] hItem 樹狀項目操作代碼
 *	@return	<b>型別:HTREEITEM </b> \n 若函數操作成功返回值為非零值，值為樹狀項目操作代碼。 \n 若函數操作失敗返回值為零。
 *	@note	此函數直接呼叫(調用) GetNextItem 函數進行處理
 */
HTREEITEM DmTreeView::GetParentItem(HTREEITEM hItem) const
{
	return this->GetNextItem(hItem, TVGN_PARENT);
}

/**
 *	@brief	取得樹狀項目的上一個兄弟的項目
 *	@param[in] hItem 樹狀項目操作代碼
 *	@return	<b>型別:HTREEITEM </b> \n 若函數操作成功返回值為非零值，值為樹狀項目操作代碼。 \n 若函數操作失敗返回值為零。
 *	@note	此函數直接呼叫(調用) GetNextItem 函數進行處理
 */
HTREEITEM DmTreeView::GetPrevSiblingItem(HTREEITEM hItem) const
{
	return this->GetNextItem(hItem, TVGN_PREVIOUS);
}

/**
 *	@brief	取得樹狀項目的上一個可見的項目
 *	@param[in] hItem 樹狀項目操作代碼
 *	@return	<b>型別:HTREEITEM </b> \n 若函數操作成功返回值為非零值，值為樹狀項目操作代碼。 \n 若函數操作失敗返回值為零。
 *	@note	此函數直接呼叫(調用) GetNextItem 函數進行處理
 */
HTREEITEM DmTreeView::GetPrevVisibleItem(HTREEITEM hItem) const
{
	return this->GetNextItem(hItem, TVGN_PREVIOUSVISIBLE);
}

/**
 *	@brief	取得樹狀目錄最頂部或第一個項目
 *	@return	<b>型別:HTREEITEM </b> \n 若成功返回值為非零值，值為樹狀項目操作代碼。 \n 數失敗則返回值為零。
 *	@note	此函數直接呼叫(調用) GetNextItem 函數進行處理
 */
HTREEITEM DmTreeView::GetRootItem() const
{
	return this->GetNextItem(NULL, TVGN_ROOT);
}

/**
 *	@brief	取得樹狀目錄控制項卷軸捲動的最大時間
 *	@return	<b>型別: UINT</b> \n 返回值為最大的捲軸的時間，以毫秒(milliseconds)為單位。
 */
UINT DmTreeView::GetScrollTime() const
{
	assert(this->IsWindow());
	// TVM_GETSCROLLTIME
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<UINT>(::SendMessage(*this, TVM_GETSCROLLTIME, 0, 0));
}

/**
 *	@brief	取得當前被選定樹狀目錄的項目
 *	@return	<b>型別:HTREEITEM </b> \n 若函數操作成功返回值為非零值，值為樹狀項目操作代碼。 \n 若函數操作失敗返回值為零。
 *	@note	此函數直接呼叫(調用) GetNextItem 函數進行處理
 */
HTREEITEM DmTreeView::GetSelectionItem() const
{
	return this->GetNextItem(NULL, TVGN_CARET);
}

/**
 *	@brief	取得樹狀目錄當前被選取的樹狀目錄的項目數量
 *	@return	<b>型別: UINT</b> \n 選取的項目數量
 */
UINT DmTreeView::GetSelectedCount() const
{
	assert(this->IsWindow());
	// TVM_GETSELECTEDCOUNT
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<UINT>(::SendMessage(*this, TVM_GETSELECTEDCOUNT, 0, 0));
}

/**
 *	@brief	取得當前樹狀目錄的文字使用顏色
 *	@return	<b>型別: </b> \n 返回值為目前文字使用顏色值，若返回值為 (-1)，代表述狀目錄使用系統預設色彩的文字色彩。
 */
COLORREF DmTreeView::GetTextColor() const
{
	assert(this->IsWindow());
	// TVM_GETTEXTCOLOR
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<UINT>(::SendMessage(*this, TVM_GETTEXTCOLOR, 0, 0));
}

/**
 *	@brief	取得提示工具 (Tooltip) 操作代碼。
 *	@return	<b>型別: HWND</b> \n 若成功返回值為非零值，值為提示工具(Tooltip)操作代碼。 \n 若失敗則返回值為零。
 */
HWND DmTreeView::GetToolTips() const
{
	assert(this->IsWindow());
	// TVM_GETTOOLTIPS
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return reinterpret_cast<HWND>(::SendMessage(*this, TVM_GETTOOLTIPS, 0, 0));
}

/**
 *	@brief	檢查樹狀目錄是否採用 Unicode
 *	@return	<b>型別:</b> \n 若文字格式為 Unicode 返回值為非零值。 \n 若文字格式為 ANSI 返回值為零。
 */
BOOL DmTreeView::GetUnicodeFormat() const
{
	assert(this->IsWindow());
	// TVM_GETUNICODEFORMAT
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return ::SendMessage(*this, TVM_GETUNICODEFORMAT, 0, 0) != 0;
}

/**
 *	@brief	取得樹狀目錄可視的項目數量
 *	@return	<b>型別: UINT</b> \n 若成功返回值為樹狀目錄可視項目的數量。 \n 若失敗則返回值為 (-1)。
 */
UINT DmTreeView::GetVisibleCount() const
{
	assert(this->IsWindow());
	// TVM_GETVISIBLECOUNT
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<UINT>(::SendMessage(*this, TVM_GETVISIBLECOUNT, 0, 0));
}

/**
 *	@brief	測試點擊狀態
 *	@param[in]	ptPtr	指向 POINT 結構緩衝區位址，要測試的座標資訊。
 *	@param[out]	flagPtr	指向 UINT 資料緩衝區位址，用來接收點擊測試結果的相關資訊。
 *	@return	<b>型別: HTREEITEM</b>
 *		\n 若測試點擊坐落於樹狀目錄的項目上，返回值為非零值，值為項目操作代碼。
 *		\n 若沒有在測試點沒有項目，返回值為零 (NULL)
 */
HTREEITEM DmTreeView::HitTest(POINT* ptPtr, UINT* flagPtr) const
{
	TVHITTESTINFO tvhti;
	::memset(&tvhti, 0, sizeof(TVHITTESTINFO));
	tvhti.pt.x = ptPtr->x;
	tvhti.pt.y = ptPtr->y;
	::SendMessage(*this, TVM_HITTEST, 0, reinterpret_cast<LPARAM>(&tvhti));
	if (flagPtr != NULL) {
		*flagPtr = tvhti.flags;
	}
	return(tvhti.hItem);
}

/**
 *	@brief	測試點擊狀態
 *	@param[in, out] hitinfoPtr 指向 TVHITTESTINFO 結構緩衝區位址。
 *		\n (輸入) 內容為要點擊測試的的位置。
 *		\n (輸出) 接收點擊測試結果的相關資訊。
 *	@return	<b>型別: HTREEITEM</b>
 *		\n 若測試點擊坐落於樹狀目錄的項目上，返回值為非零值，值為項目操作代碼。
 *		\n 若沒有在測試點沒有項目，返回值為零 (NULL)
 */
HTREEITEM DmTreeView::HitTest(TVHITTESTINFO* hitinfoPtr) const
{
	assert(this->IsWindow());
	// TVM_HITTEST
	// wParam = 不使用，必須為零
	// lParam = 指向 TVHITTESTINFO 結構緩衝區位址
	return reinterpret_cast<HTREEITEM>(::SendMessage(*this, TVM_HITTEST, 0, reinterpret_cast<LPARAM>(hitinfoPtr)));
}

/**
 *	@brief	新增一個樹狀目錄的項目 (依據 nMask 建立指定項目)
 *	@param[in] nMask			指定要設定的屬性，參照 TVITEM
 *	@param[in] textPtr			指向文字字串緩衝區位址，內容為要新增的文字字串。
 *	@param[in] nImage			樹狀目錄圖形項目的徒刑索引值。
 *	@param[in] nSelectedImage	樹狀目錄被選擇項目的圖形索引值
 *	@param[in] nState			指定的項目狀態的值
 *	@param[in] nStateMask		指定要設定的狀態
 *	@param[in] lParam			項目的關聯 (32-bits)
 *	@param[in] hParent			插入項目的父視窗
 *	@param[in] hInsertAfter		插入新項目後之項目的控制代碼。
 *	@return	<b>型別: HTREEITEM</b>
 *		\n 若測試點擊坐落於樹狀目錄的項目上，返回值為非零值，值為項目操作代碼。
 *		\n 若沒有在測試點沒有項目，返回值為零 (NULL)
 */
HTREEITEM DmTreeView::InsertItem(UINT nMask
	, const TCHAR* textPtr
	, int nImage
	, int nSelectedImage
	, UINT nState
	, UINT nStateMask
	, LPARAM lParam
	, HTREEITEM hParent
	, HTREEITEM hInsertAfter) const {

	TVINSERTSTRUCT tvis;

	assert(this->IsWindow());
	::memset(&tvis, 0, sizeof(TVINSERTSTRUCT));
	tvis.hParent = hParent;
	tvis.hInsertAfter = hInsertAfter;
	tvis.item.mask = nMask;
	tvis.item.state = nState;
	tvis.item.stateMask = nStateMask;
	if (textPtr != NULL) {
		tvis.item.pszText = const_cast<LPTSTR>(textPtr);
		tvis.item.cchTextMax = static_cast<int>(_tcslen(textPtr));
	}
	tvis.item.iImage = nImage;
	tvis.item.iSelectedImage = nSelectedImage;
	tvis.item.lParam = lParam;
	// TVM_INSERTITEM
	// wParam = 不使用，必須為零
	// lParam = 指向 TVINSERTSTRUCT 結構緩衝區位址
	return reinterpret_cast<HTREEITEM>(::SendMessage(*this, TVM_INSERTITEM, 0, reinterpret_cast<LPARAM>(&tvis)));
}

/**
 *	@brief	新增一個樹狀目錄的項目 (文字項目)
 *	@param[in] textPtr		指向文字字串緩衝區位址，內容為要新增的文字字串。
 *	@param[in] hParent		插入項目的父視窗
 *	@param[in] hInsertAfter	插入新項目後之項目的控制代碼。
 *	@return	<b>型別: HTREEITEM</b>
 *		\n 若測試點擊坐落於樹狀目錄的項目上，返回值為非零值，值為項目操作代碼。
 *		\n 若沒有在測試點沒有項目，返回值為零 (NULL)
 */
HTREEITEM DmTreeView::InsertItem(const TCHAR* textPtr, HTREEITEM hParent, HTREEITEM hInsertAfter) const
{
	TVINSERTSTRUCT tvis;

	assert(this->IsWindow());
	::memset(&tvis, 0, sizeof(TVINSERTSTRUCT));
	tvis.hParent = hParent;
	tvis.hInsertAfter = hInsertAfter;
	tvis.item.mask = TVIF_TEXT;
	if (textPtr != NULL) {
		tvis.item.pszText = const_cast<LPTSTR>(textPtr);
		tvis.item.cchTextMax = static_cast<int>(_tcslen(textPtr));
	}
	// TVM_INSERTITEM
	// wParam = 不使用，必須為零
	// lParam = 指向 TVINSERTSTRUCT 結構緩衝區位址
	return reinterpret_cast<HTREEITEM>(::SendMessage(*this, TVM_INSERTITEM, 0, reinterpret_cast<LPARAM>(&tvis)));
}

/**
 *	@brief	新增一個樹狀目錄的項目 (圖形項目)
 *	@param[in] textPtr			指向文字字串緩衝區位址，內容為要新增的文字字串。
 *	@param[in] nImage			樹狀目錄圖形項目的徒刑索引值。
 *	@param[in] nSelectedImage	樹狀目錄被選擇項目的圖形索引值
 *	@param[in] hParent			插入項目的父視窗
 *	@param[in] hInsertAfter		插入新項目後之項目的控制代碼。
 *	@return	<b>型別: HTREEITEM</b>
 *		\n 若測試點擊坐落於樹狀目錄的項目上，返回值為非零值，值為項目操作代碼。
 *		\n 若沒有在測試點沒有項目，返回值為零 (NULL)
 */
HTREEITEM DmTreeView::InsertItem(const TCHAR* textPtr, int nImage, int nSelectedImage, HTREEITEM hParent, HTREEITEM hInsertAfter) const
{
	TVINSERTSTRUCT tvis;

	assert(this->IsWindow());
	::memset(&tvis, 0, sizeof(TVINSERTSTRUCT));
	tvis.hParent = hParent;
	tvis.hInsertAfter = hInsertAfter;
	tvis.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	if (textPtr != NULL) {
		tvis.item.pszText = const_cast<LPTSTR>(textPtr);
		tvis.item.cchTextMax = static_cast<int>(_tcslen(textPtr));
	}
	tvis.item.iImage = nImage;
	tvis.item.iSelectedImage = nSelectedImage;
	// TVM_INSERTITEM
	// wParam = 不使用，必須為零
	// lParam = 指向 TVINSERTSTRUCT 結構緩衝區位址
	return reinterpret_cast<HTREEITEM>(::SendMessage(*this, TVM_INSERTITEM, 0, reinterpret_cast<LPARAM>(&tvis)));
}

/**
 *	@brief	判斷是否樹狀目錄的項目是否有子項目
 *	@param[in] hItem 樹狀目錄的項目操作代碼
 *	@return	<b>型別: BOOL</b> \n 若擁有子項目返回值為非零值，若無子項目返回值為零。
 */
BOOL DmTreeView::ItemHasChildren(HTREEITEM hItem) const
{
	TVITEM tvi;

	assert(this->IsWindow());
	::memset(&tvi, 0, sizeof(TVITEM));
	tvi.mask = TVIF_HANDLE | TVIF_CHILDREN;
	tvi.hItem = hItem;
	::SendMessage(*this, TVM_GETITEM, 0, reinterpret_cast<LPARAM>(&tvi));
	return tvi.cChildren != 0;
}

/**
 *	@brief	將輔助工具映射到樹狀目錄的項目。
 *	@param[in] uAccId 輔助(協助)工具功能識別碼(ID)
 *	@return	<b>型別: HTREEITEM</b>\n 返回值為已映射輔助工具的樹狀目錄項目操作碼。
 */
HTREEITEM DmTreeView::MapAccIdToItem(UINT uAccId) const
{
	assert(this->IsWindow());
	// TVM_MAPACCIDTOHTREEITEM
	// wParam = 輔助工具功能識別碼
	// lParam = 不使用，必須為零
	return reinterpret_cast<HTREEITEM>(::SendMessage(*this, TVM_MAPACCIDTOHTREEITEM, static_cast<WPARAM>(uAccId), 0));
}

/**
 *	@brief	取得樹狀目錄的項目所對應的輔助工具
 *	@param[in] hItem 樹狀目錄的項目操作代碼
 *	@return	<b>型別: UINT</b> \n 返回值為已映射的輔助工具識別碼。
 */
UINT DmTreeView::MapItemToAccID(HTREEITEM hItem) const
{
	assert(this->IsWindow());
	// TVM_MAPHTREEITEMTOACCID
	// wParam = 輔助工具功能識別碼
	// lParam = 不使用，必須為零
	return static_cast<UINT>(::SendMessage(*this, TVM_MAPHTREEITEMTOACCID, reinterpret_cast<WPARAM>(hItem), 0));
}

/**
 *	@brief	指定項目，樹狀目錄將捲動列表到指定項目為可視狀態 (直到滿足 nCode 所設定的動型)
 *	@param[in] hItem 樹狀目錄的項目操作碼
 *	@param[in] nCode 要執行的動作類型。這個參數可以是下列值之一︰
 *		<pre>
 *		TVGN_CARET			將選取項目設定為指定的項目。
 *		TVGN_DROPHILITE		在樣式中用來表示拖放作業的目標來重新繪製指定的項目。
 *		TVGN_FIRSTVISIBLE	垂直捲動樹狀檢視，使指定的項目是第一個可見的項目。
 *		TVSI_NOSINGLEEXPAND	選擇單個項目時，確保樹視圖不會展開該項目的子項。僅在與 TVGN_CARET 標誌一起使用時才有效。
 *		</pre>
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmTreeView::SelectItem(HTREEITEM hItem, UINT nCode) const
{
	assert(this->IsWindow());
	// TVM_SELECTITEM
	// wParam = 要執行的動作類型
	// lParam = 樹狀目錄的項目操作碼
	return ::SendMessage(*this, TVM_SELECTITEM, static_cast<WPARAM>(nCode), reinterpret_cast<LPARAM>(hItem)) != 0;
}

/**
 *	@brief	設定目前 Tree View 捲動條、滾動條 (scroll bar) 捲動速率。
 *	@param[in] uPixelsPerSec	每秒捲動像素 (pixel) 量
 *	@param[in] uUpdateTime		控制項更新的間隔時間，單位為秒 (sec)
 *	@return	<b>型別: BOOL</b> \n 此函數始終返回 TRUE
 */
BOOL DmTreeView::SetAutoscrollInfo(UINT uPixelsPerSec, UINT uUpdateTime) const
{
	assert(this->IsWindow());
	// TVM_SETAUTOSCROLLINFO
	// wParam = 每秒捲動像素 (pixel) 量
	// lParam = 控制項更新的間隔時間
	return ::SendMessage(*this, TVM_SETAUTOSCROLLINFO, static_cast<WPARAM>(uPixelsPerSec), static_cast<LPARAM>(uUpdateTime)) != 0;
}

/**
 *	@brief	設定樹狀目錄的背景顏色 (COLORREF 格式)
 *	@param[in] clr 顏色值，若值為 (-1) 表示恢復使用系統預設背景顏色。
 *		\n (24-bits RGB color = #### #### RRRR RRRR GGGG GGGG BBBB BBBB)
 *	@return	<b>型別: COLORREF</b> \n 返回改變前的背景顏色，若返回值為 (-1) 表示使用系統預設的背景顏色。
 */
COLORREF DmTreeView::SetBkColor(COLORREF clr)
{
	assert(this->IsWindow());
	// TVM_SETBKCOLOR
	// wParam = 不使用，必須為零
	// lParam = 顏色值
	return static_cast<COLORREF>(::SendMessage(*this, TVM_SETBKCOLOR, static_cast<WPARAM>(0), static_cast<LPARAM>(clr)));
}

/**
 *	@brief	設定樹狀目錄邊界條尺寸
 *	@param[in] dwFlags	動作旗標
 *		- TVSBF_XBORDER	\n Applies the specified border size to the left side of the items in the tree-view control.
 *		- TVSBF_YBORDER	\n Applies the specified border size to the top of the items in the tree-view control.
 *	@param[in] xBorder	左邊界
 *	@param[in] yBorder	上邊界
 *	@return	<b>型別: DWORD</b> \n 設定前的邊界條尺寸，LOWORD 左邊界，HIWORD 上邊界。(單位: pixel)
 */
DWORD DmTreeView::SetBorder(DWORD dwFlags, int xBorder, int yBorder) const
{
	assert(this->IsWindow());
	// TVM_SETBORDER
	// wParam = 動作旗標
	// lParam = LOWORD 左邊界，HIWORD 上邊界。(單位: pixel)
	return static_cast<DWORD>(::SendMessage(*this, TVM_SETBORDER, static_cast<WPARAM>(dwFlags), static_cast<LPARAM>(MAKELPARAM(xBorder, yBorder))));
}

/**
 *	@brief	設定擴充 (延伸) 樣式
 *	@param[in] dwExMask		必定被設定的樣式遮罩
 *	@param[in] dwExStyles	指定新的擴充樣式
 *	@return	<b>型別: DWORD</b> \n 變更前的擴充 (延伸) 樣式
 *	@remark	實際變更的擴充樣式為 = dwExMask 與 dwExStyles 進行 OR 計算的結果。
 */
DWORD DmTreeView::SetExtendedStyle(DWORD dwExMask, DWORD dwExStyles) const
{
	assert(this->IsWindow());
	// TVM_SETEXTENDEDSTYLE
	// wParam = 必定被設定的樣式遮罩
	// lParam = 指定新的擴充樣式
	return static_cast<DWORD>(::SendMessage(*this, TVM_SETEXTENDEDSTYLE, static_cast<WPARAM>(dwExMask), static_cast<LPARAM>(dwExStyles)));
}

/**
 *	@brief	設定樹狀目錄為 Hot 狀態
 *	@param[in] hHotItem Hot 樹狀目錄的項目的操作代碼
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 *	@remark	供系統內部使用，不推薦用於應用程式。將來的 Windows 版本中可能不再支援此訊息。
 */
BOOL DmTreeView::SetHot(HTREEITEM hHotItem) const
{
	assert(this->IsWindow());
	// TVM_SETHOT
	// wParam = 不使用，必須為零
	// lParam = Hot 項目的控制代碼
	return ::SendMessage(*this, TVM_SETHOT, static_cast<WPARAM>(0), reinterpret_cast<LPARAM>(hHotItem)) != 0;
}

/**
 *	@brief	設定一般或樹狀目錄的圖像清單，並且重繪使用新的圖像。
 *	@param[in] hImageList		圖像清單操作代碼
 *	@param[in] nImageListType	設定圖像類型
 *		- TVSIL_NORMAL	\n	表示正常圖像列表，其中包含樹狀目錄的項目選定，未選定和疊加圖像。
 *		- TVSIL_STATE	\n	表示狀態圖像列表。您可以使用狀態圖像來指示應用程序定義的項目狀態。狀態圖像顯示在項目的選定或未選擇圖像的左側
 *	@return	<b>型別: HIMAGELIST</b> \n 改變前的影像清單操作代碼。
 */
HIMAGELIST DmTreeView::SetImageList(HIMAGELIST hImageList, int nImageListType) const
{
	assert(this->IsWindow());
	// TVM_SETIMAGELIST
	// wParam = 設定圖像類型
	// lParam = 圖像清單操作代碼
	return reinterpret_cast<HIMAGELIST>(::SendMessage(*this, TVM_SETIMAGELIST, static_cast<WPARAM>(nImageListType), reinterpret_cast<LPARAM>(hImageList)));
}

/**
 *	@brief	設定樹狀目錄縮排寬度，單位: 像素 (pixel)
 *	@param[in] nIndent 縮排寬度，單位為 pixel
 *	@return	此函數沒有返回值。
 */
void DmTreeView::SetIndent(UINT nIndent) const
{
	assert(this->IsWindow());
	// TVM_SETINDENT
	// wParam = 縮排寬度
	// lParam = 不使用，必須為零
	::SendMessage(*this, TVM_SETINDENT, static_cast<WPARAM>(nIndent), static_cast<LPARAM>(0));
}

/**
 *	@brief	在樹狀目錄插入一個標記
 *	@param[in] hItem	要放置標記的樹狀目錄的項目操作代碼。
 *	@param[in] fAfter	標記放置位置，指定項目前或後 (預設為 TRUE，放置於指定項目後)
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmTreeView::SetInsertMark(HTREEITEM hItem, BOOL fAfter) const
{
	assert(this->IsWindow());
	// TVM_SETINSERTMARK
	// wParam = 標記放置位置，指定項目前或後。
	// lParam = 要放置標記的項目操作代碼
	return ::SendMessage(*this, TVM_SETINSERTMARK, static_cast<WPARAM>(fAfter), reinterpret_cast<LPARAM>(hItem)) != 0;
}

/**
 *	@brief	設定插入標記的顏色
 *	@param[in] clrNew 新的顏色值
 *	@return	<b>型別: COLORREF</b> \n 改變前的顏色值。
 */
COLORREF DmTreeView::SetInsertMarkColor(COLORREF clrNew) const
{
	assert(this->IsWindow());
	// TVM_SETINSERTMARKCOLOR
	// wParam = 不使用，必須為零
	// lParam = 新的顏色值
	return static_cast<COLORREF>(::SendMessage(*this, TVM_SETINSERTMARKCOLOR, static_cast<WPARAM>(0), static_cast<LPARAM>(clrNew)));
}

/**
 *	@brief	設定樹狀目錄的項目屬性
 *	@param[in] tviPtr 指向結構 TVITEM 緩衝區位址，用來敘述要設定新的項目屬性。
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmTreeView::SetItem(TVITEM* tviPtr) const
{
	assert(this->IsWindow());
	// TVM_SETITEM
	// wParam = 不使用，必須為零
	// lParam = 指向結構 TVITEM 緩衝區位址
	return ::SendMessage(*this, TVM_SETITEM, static_cast<WPARAM>(0), reinterpret_cast<LPARAM>(tviPtr)) != 0;
}

/**
 *	@brief	設定樹狀目錄的項目屬性
 *	@param[in] hItem			樹狀項目操作代碼
 *	@param[in] uMask			遮罩
 *	@param[in] szItemPtr		文字內容
 *	@param[in] nImage			圖像
 *	@param[in] nSelectedImage	選擇圖像
 *	@param[in] uState			狀態
 *	@param[in] uStateMask		狀態遮罩
 *	@param[in] lParam			其他參數
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmTreeView::SetItem(HTREEITEM hItem, UINT uMask, const TCHAR* szItemPtr, int nImage, int nSelectedImage, UINT uState, UINT uStateMask, LPARAM lParam) const
{
	TVITEM tvi;

	::memset(&tvi, 0, sizeof(TVITEM));
	tvi.mask = uMask;
	tvi.hItem = hItem;
	tvi.state = uState;
	tvi.stateMask = uStateMask;
	tvi.pszText = const_cast<LPTSTR>(szItemPtr);
	tvi.cchTextMax = static_cast<int>(::_tcslen(szItemPtr));
	tvi.iImage = nImage;
	tvi.iSelectedImage = nSelectedImage;
	tvi.cChildren = 0;
	tvi.lParam = lParam;
	return this->SetItem(&tvi);
}

/**
 *	@brief	設定樹狀目錄的項目相關聯的 32 位元應用程式特定值。
 *	@param[in] hItem	樹狀目錄的項目操作代碼
 *	@param[in] dwData	樹狀目錄的項目相關聯的 32 位元應用程式專屬值
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmTreeView::SetItemData(HTREEITEM hItem, DWORD_PTR dwData) const
{
	TVITEM tvi;
	::memset(&tvi, 0, sizeof(tvi));
	tvi.hItem = hItem;
	tvi.mask = TVIF_PARAM;
	tvi.lParam = static_cast<LPARAM>(dwData);
	return this->SetItem(&tvi);
}

/**
 *	@brief	設定樹狀目錄的項目新高度 (in pixel)
 *	@param[in] nHeight 新的高度 (單位: pixel)
 *	@return	<b>型別: int</b> \n 返回值為先前設定高度 (in pixel)
 */
int DmTreeView::SetItemHeight(int nHeight) const
{
	assert(this->IsWindow());
	// TVM_SETITEMHEIGHT
	// wParam = 新的高度
	// lParam = 不使用，必須為零
	return static_cast<int>(::SendMessage(*this, TVM_SETITEMHEIGHT, static_cast<WPARAM>(nHeight), static_cast<LPARAM>(0)));
}

/**
 *	@brief	設定樹狀目錄的項目相關聯的點陣圖影像
 *	@param[in] hItem			樹狀目錄的項目操作代碼
 *	@param[in] nImage			樹狀目錄的影像清單中影像項目索引。
 *	@param[in] nSelectedImage	樹狀目錄的影像清單中選取的影像項目索引。
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmTreeView::SetItemImage(HTREEITEM hItem, int nImage, int nSelectedImage) const
{
	TVITEM tvi;
	::memset(&tvi, 0, sizeof(tvi));
	tvi.hItem = hItem;
	tvi.iImage = nImage;
	tvi.iSelectedImage = nSelectedImage;
	tvi.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	return this->SetItem(&tvi);
}

/**
 *	@brief	設定樹狀目錄的項目的文字內容
 *	@param[in]	hItem	樹狀目錄的項目操作代碼
 *	@param[out] textPtr	指向要設定的文字緩衝區位址
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmTreeView::SetItemText(HTREEITEM hItem, const TCHAR* textPtr) const
{
	TVITEM tvi;
	::memset(&tvi, 0, sizeof(tvi));
	tvi.hItem = hItem;
	tvi.pszText = const_cast<LPTSTR>(textPtr);
	tvi.mask = TVIF_TEXT;
	return this->SetItem(&tvi);
}

/**
 *	@brief	設定樹狀目錄目前的線條的色彩 (僅線條顏色，不含 + - 符號)
 *	@param[in] clrNew 新的色彩值 (COLORREF)，若值為 CLR_DEFAULT 代表使用系統預設顏色。
 *	@return	<b>型別: COLORREF</b> \n 先前設定的色彩值
 */
COLORREF DmTreeView::SetLineColor(COLORREF clrNew) const
{
	assert(this->IsWindow());
	// TVM_SETLINECOLOR
	// wParam = 不使用，必須為零
	// lParam = 新的色彩值
	return static_cast<COLORREF>(::SendMessage(*this, TVM_SETLINECOLOR, static_cast<WPARAM>(0), static_cast<LPARAM>(clrNew)));
}

/**
 *	@brief	設定樹狀目錄的捲動軸最大滾動時間 (毫秒，若小於 100 將無條件為 100)
 *	@param[in] uScrollTime 新的捲動軸最大滾動時間
 *	@return	<b>型別: UINT</b> \n 先前設定的最大捲動時間
 *	@remark	最大捲動時間為卷軸滾動可用的最大時間，時間越大捲動越細微。
 */
UINT DmTreeView::SetScrollTime(UINT uScrollTime) const
{
	assert(this->IsWindow());
	// TVM_SETSCROLLTIME
	// wParam = 新的捲動軸最大滾動時間
	// lParam = 不使用，必須為零
	return static_cast<UINT>(::SendMessage(*this, TVM_SETSCROLLTIME, static_cast<UINT>(uScrollTime), static_cast<LPARAM>(0)));
}

/**
 *	@brief	設定樹狀目錄文字色彩
 *	@param[in] clr 新的文字色彩值
 *	@return	<b>型別: COLORREF</b> \n 先前設定的文字色彩值
 */
COLORREF DmTreeView::SetTextColor(COLORREF clr) const
{
	assert(this->IsWindow());
	// TVM_SETTEXTCOLOR
	// wParam = 不使用，必須為零
	// lParam = 新的文字色彩值
	return static_cast<COLORREF>(::SendMessage(*this, TVM_SETTEXTCOLOR, static_cast<WPARAM>(0), static_cast<LPARAM>(clr)));
}

/**
 *	@brief	設定提示工具 (Tips)
 *	@param[in] hWndTip 提示工具 (Tips) 操作代碼。
 *	@return	<b>型別: HWND</b> \n 返回值為先前的提示工具 (Tips) 操作代碼，若返回值為 NULL 則表示先前沒有設定提示工具。
 */
HWND DmTreeView::SetToolTips(HWND hWndTip) const
{
	assert(this->IsWindow());
	// TVM_SETTOOLTIPS
	// wParam = 欲設定的提示工具操作代碼。
	// lParam = 不處理，必須為零
	return reinterpret_cast<HWND>(::SendMessage(*this, TVM_SETTOOLTIPS, reinterpret_cast<WPARAM>(hWndTip), static_cast<LPARAM>(0)));
}

/**
 *	@brief	設樹狀目錄使用 Unicode 字符集 (不用重新建立樹狀目錄即可變更字符集)
 *	@param[in] bUnicode 使用 Unicode 字符集旗標，若值為 TRUE 為使用 Unicode 字符集，否則為不使用。(預設值為 TRUE)
 *	@return	<b>型別: BOOL</b> \n 先前設定的的字符集使用狀態，若值為 TRUE 為使用 Unicode 字符集，否則為不使用。
 */
BOOL DmTreeView::SetUnicodeFormat(BOOL bUnicode) const
{
	assert(this->IsWindow());
	// TVM_SETUNICODEFORMAT
	// wParam = 使用 Unicode 字符集旗標
	// lParam = 不使用，必須為零
	return ::SendMessage(*this, TVM_SETUNICODEFORMAT, static_cast<WPARAM>(bUnicode), static_cast<LPARAM>(0)) != 0;
}

/**
 *	@brief	指定樹狀目錄的項目訊息提示 (一般情況，不需要使用此方式，訊息會自動顯示)
 *	@param[in] hItem 樹狀列表項目控制代碼
 *	@return	此函數沒有返回值
 */
void DmTreeView::ShowInfoTip(HTREEITEM hItem) const
{
	assert(this->IsWindow());
	// TVM_SHOWINFOTIP - 此訊息處理後返回值永遠為零
	// wParam = 不使用，必須為零
	// lParam = 樹狀目錄的項目操作代碼
	::SendMessage(*this, TVM_SHOWINFOTIP, 0, reinterpret_cast<LPARAM>(hItem));
}

/**
 *	@brief	指定父項的子項進行排序
 *	@param[in] hItem 要排序的父項目操作代碼，若值為 NULL 將會以樹狀跟目錄作為父項目對底下以項目進行排序。
 *	@param[in] bRecursive 使用遞迴方式處理旗標。(預設為 FALSE)
 *		\n 若為 TRUE 則表示處理旗下所有級別的子項目進行排序。
 *		\n 若為 FALSE  表示僅對直接級別子項目進行排序。
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmTreeView::SortChildren(HTREEITEM hItem, BOOL bRecursive) const
{
	assert(this->IsWindow());
	// TVM_SORTCHILDREN
	// wParam = 使用遞迴方式處理旗標
	// lParam = 要排序的父項目操作代碼
	return ::SendMessage(*this, TVM_SORTCHILDREN, static_cast<WPARAM>(bRecursive), reinterpret_cast<LPARAM>(hItem)) != 0;
}

/**
 *	@brief	使用的應用程式定義的回調函數來比較項目
 *	@param[in] tvsPtr 指向結構 TVSORTCB 緩衝區位址，成員 lpfnCompare 應用程序定義的回調函數。
 *	@return	<b>型別: BOOL</b> \n 若成功返回值為非零值。 \n 若失敗則返回值為零。
 */
BOOL DmTreeView::SortChildrenCB(TVSORTCB* tvsPtr)
{
	// TVM_SORTCHILDRENCB
	// wParam = 不使用，必須為零
	// lParam = 指向結構 TVSORTCB 緩衝區位址
	return ::SendMessage(*this, TVM_SORTCHILDREN, static_cast<WPARAM>(0), reinterpret_cast<LPARAM>(tvsPtr)) != 0;
}
