/**************************************************************************//**
 * @file	listbox.cc
 * @brief	DmListbox 列表框類別成員函數定義
 * @date	2000-10-10
 * @date	2018-09-05
 * @author	Swang
 *****************************************************************************/
#include "opendmc/wnds/listbox.hh"

/**
 *	@brief DmListbox 建構式
 */
DmListbox::DmListbox() : DmWnds(EmCtrls::List) { }

/**
 *	@brief DmListbox 解構式
 */
DmListbox::~DmListbox() { if (this->IsWindow()) { this->RequestToDestroy(0); } }

/**
 *	@brief [重載] 建立一個編輯框
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
BOOL DmListbox::Create(const TCHAR* szCaptionPtr, int x, int y, int wd, int ht, HWND hWndParent, int nIDCItem)
{
	const DWORD dwStyle = WS_VSCROLL | ES_AUTOVSCROLL;
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
 *	@brief 建立一個列表框樣本 (由繼承者發展)
 *	@return	<b>型別: BOOL</b> 若視窗建立成功返回值為非零值，若視窗建立失敗責返回值為零。
 */
BOOL DmListbox::CreateSample() { return FALSE; }

/**
 *	@brief	新增一個檔案項目
 *	@param[in] filePtr 指向檔案名稱緩衝區位址
 *	@return	<b>型別: int</b> \n 若函數操作成功返回值為新增的項目索引值。 \n 若函數操作失敗返回值為 LB_ERR。
 */
int DmListbox::AddFile(const TCHAR* filePtr) const
{
	assert(this->IsWindow());
	// LB_ADDFILE
	// wParam = 未使用，必須為零
	// lParam = 指向檔案名稱緩衝區位址
	return static_cast<int>(::SendMessage(*this, LB_ADDFILE, 0, reinterpret_cast<LPARAM>(filePtr)));
}

/**
 *	@brief	新增一個文字項目
 *	@param[in] strPtr 指向文字字串緩衝區位址，要新增的文字字串。
 *	@return	<b>型別: int</b> \n 若函數操作成功返回值為新增的項目索引值。 \n 若函數操作失敗返回值為 LB_ERR。
 */
int DmListbox::AddString(const TCHAR* strPtr) const
{
	assert(this->IsWindow());
	// LB_ADDSTRING
	// wParam = 未使用，必須為零
	// lParam = 指向文字字串緩衝區位址
	return static_cast<int>(::SendMessage(*this, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(strPtr)));
}

/**
 *	@brief	刪除一個項目
 *	@param[in] nItem 項目索引值，從 0 開始算起。
 *	@return	<b>型別: int</b> \n 若函數操作成功返回值為當前剩餘的項目數量。 \n 若函數操作失敗返回值為 LB_ERR。
 */
int DmListbox::DeleteString(int nItem) const
{
	assert(this->IsWindow());
	// LB_DELETESTRING
	// wParam = 項目索引值
	// lParam = 未使用，必須為零
	return static_cast<int>(::SendMessage(*this, LB_DELETESTRING, static_cast<WPARAM>(nItem), 0));
}

/**
 *	@brief	增加一個目錄下指定內容至列表
 *	@param[in] nAttrib	目錄屬性，目錄屬性如下
 *		- DDL_ARCHIVE	\n 包括存檔文件
 *		- DDL_DIRECTORY	\n 包括子目錄。子目錄名稱括在方括號（[]）中。
 *		- DDL_DRIVES	\n 所有映射的驅動器都將添加到列表中。驅動器以[ - x - ] 的形式列出，其中x是驅動器號。
 *		- DDL_EXCLUSIVE	\n 僅包含具有指定屬性的文件。默認情況下，即使未指定 DDL_READWRITE，也會列出讀/寫文件。
 *		- DDL_HIDDEN	\n 包含隱藏文件。
 *		- DDL_READONLY	\n 包括唯讀文件
 *		- DDL_READWRITE	\n 包括沒有其他屬性的讀/寫文件。這是預設設置。
 *		- DDL_SYSTEM	\n 包括系統文件。
 *	@param[in] pathPtr 指向目錄名稱緩衝區位址
 *		- 例如, 路徑名稱 "c:\\folder\\"
 *		- 例如, 萬用名稱 "c:\\folder\\*.exe"
 *	@return	<b>型別: int</b> \n 若函數操作成功返回值為新增項目項目的索引，若函數操作失敗則返回值為 LB_ERR
 */
int DmListbox::Dir(int nAttrib, TCHAR* pathPtr) const
{
	assert(this->IsWindow());
	// LB_DIR
	// wParam = 目錄屬性
	// lParam = 指向目錄名稱緩衝區位址
	return static_cast<int>(::SendMessage(*this, LB_DIR, static_cast<WPARAM>(nAttrib), reinterpret_cast<LPARAM>(pathPtr)));
}

/**
 *	@brief	於列表中找尋第一個與輸入字串相符的項目
 *	@param[in] nItem	指定起始項目索引值
 *	@param[in] strPtr	指向欲搜尋文字字串緩衝區位址
 *	@return	<b>型別: int</b> \n 若函數操作成功返回值為第一個吻合項目的索引值，若函數操作失敗返回值為 LB_ERR
 */
int DmListbox::FindString(int nItem, TCHAR* strPtr) const
{
	assert(this->IsWindow());
	// LB_FINDSTRING
	// wParam = 指定起始項目索引值
	// lParam = 指向欲搜尋文字字串緩衝區位址
	return static_cast<int>(::SendMessage(*this, LB_FINDSTRING, static_cast<WPARAM>(nItem), reinterpret_cast<LPARAM>(strPtr)));
}

/**
 *	@brief	於列表中找尋第一個與輸入字串完全相符的項目 (不區分大小寫)
 *	@param[in] nItem	指定起始項目索引值
 *	@param[in] strPtr	指向欲搜尋文字字串緩衝區位址
 *	@return	<b>型別: int</b> \n 若函數操作成功返回值為第一個完全相符項目的索引值, 若函數操作失敗則返回值為 LB_ERR
 */
int DmListbox::FindStringExact(int nItem, TCHAR* strPtr) const
{
	assert(this->IsWindow());
	// LB_FINDSTRINGEXACT
	// wParam = 指定起始項目索引值
	// lParam = 指向欲搜尋文字字串緩衝區位址
	return static_cast<int>(::SendMessage(*this, LB_FINDSTRINGEXACT, static_cast<WPARAM>(nItem), reinterpret_cast<LPARAM>(strPtr)));
}

/**
 *	@brief	取得滑鼠最後選點選項目的索引值
 *	@return	<b>型別: int</b> \n 若函數操作成功返回值為滑鼠最後點選的索引值，若函數操作失敗則返回值為 LB_ERR
 */
int DmListbox::GetAnchorIndex() const
{
	assert(this->IsWindow());
	// LB_GETANCHORINDEX
	// wParam = 未使用，必須為零
	// lParam = 未使用，必須為零
	return static_cast<int>(::SendMessage(*this, LB_GETANCHORINDEX, 0, 0));
}

/**
 *	@brief	取得多選的項目中，具有焦點項目的索引值，該項目可能是被選擇中也可能沒有被選擇。
 *	@return	<b>型別: int</b> \n 若函數操作成功返回值為滑鼠最後點選的索引值，若函數操作失敗則返回值為 LB_ERR
 */
int DmListbox::GetCaretIndex() const
{
	assert(this->IsWindow());
	// LB_GETCARETINDEX
	// wParam = 未使用，必須為零
	// lParam = 未使用，必須為零
	return static_cast<int>(::SendMessage(*this, LB_GETCARETINDEX, 0, 0));
}

/**
 *	@brief	取得目前列表中項目數量
 *	@return	<b>型別: int</b> \n 若函數操作成功返回值為現存像木數量, 若函數操作失敗則返回值為 LB_ERR
 */
int DmListbox::GetCount() const
{
	assert(this->IsWindow());
	// LB_GETCOUNT
	// wParam = 未使用，必須為零
	// lParam = 未使用，必須為零
	return static_cast<int>(::SendMessage(*this, LB_GETCOUNT, 0, 0));
}

/**
 *	@brief	取得列表選擇反白光棒項目的索引值 (當前被選取的項目索引值)
 *	@return	<b>型別: int</b>
 *		\n 若函數操作成功返回值為目前列表上光棒索引值，當前被選取項目索引值。
 *		\n 若函數操作失敗則返回值為 LB_ERR
 */
int DmListbox::GetCursel() const
{
	assert(this->IsWindow());
	// LB_GETCURSEL
	// wParam = 未使用，必須為零
	// lParam = 未使用，必須為零
	return static_cast<int>(::SendMessage(*this, LB_GETCURSEL, 0, 0));
}

/**
 *	@brief	若列表框有水平滾動軸的話，就取得列表上水平滾動軸的寬度 (像數單位 pixel)
 *	@return	<b>型別: int</b> \n
 *		\n 若函數操作成功返回值為水平滾動軸的寬度 (in pixel)
 *		\n 若函數操作失敗則返回值為 LB_ERR
 */
int DmListbox::GetHorizontalExtent() const
{
	assert(this->IsWindow());
	// LB_GETHORIZONTALEXTENT
	// wParam = 未使用，必須為零
	// lParam = 未此用，必須為零
	return static_cast<int>(::SendMessage(*this, LB_GETHORIZONTALEXTENT, 0, 0));
}

/**
 *	@brief	取得指定列表框的項目定義值
 *	@param[in] nItem 指定項目索引值
 *	@return	<b>型別: DWORD_PTR</b> \n 若函數操作成功返回值為項目關聯值。 \n 若函數操作發生錯誤則返回 LB_ERR。
 */
DWORD_PTR DmListbox::GetItemData(int nItem) const
{
	assert(this->IsWindow());
	// LB_GETITEMDATA
	// wParam = 指定項目索引值
	// lParam = 不使用，必須為零
	return static_cast<int>(::SendMessage(*this, LB_GETITEMDATA, static_cast<int>(nItem), 0));
}

/**
 *	@brief	取得指定列表框的項目定義值
 *	@param[in] nItem 指定項目索引值
 *	@return	<b>型別: void*</b> \n 若函數操作成功返回值為項目關聯值。 \n 若函數操作發生錯誤則返回 LB_ERR。
 */
void* DmListbox::GetItemDataPtr(int nItem) const
{
	assert(this->IsWindow());
	// LB_GETITEMDATA
	// wParam = 指定項目索引值
	// lParam = 不使用，必須為零
	return reinterpret_cast<void*>(::SendMessage(*this, LB_GETITEMDATA, static_cast<int>(nItem), 0));
}

/**
 *	@brief	取得列表框指定項目的高度，單位為像素(pixel)
 *	@param[in] nItem 指定項目索引值
 *	@return	<b>型別: int</b>
 *		\n 若函數操作成功返回值為列表框中的每個項目的高度（以像素為單位）。
 *		\n 如果列錶框具有 LBS_OWNERDRAWVARIABLE 樣式，則返回值是 wParam 參數指定的項目的高度。
 *		\n 如果發生錯誤，返回值為 LB_ERR。
 */
int DmListbox::GetItemHeight(int nItem) const
{
	assert(this->IsWindow());
	// LB_GETITEMDATA
	// wParam = 指定項目索引值
	// lParam = 不使用，必須為零
	return static_cast<int>(::SendMessage(*this, LB_GETITEMDATA, static_cast<WPARAM>(nItem), 0));
}

/**
 *	@brief	取得指定項目的矩形資料 (此項目必須於列表顯示中)
 *	@param[in]		nItem	指定項目的索引值
 *	@param[in,out]	rcPtr	指向 RECT 結構緩衝區位址的指標
 *	@return	<b>型別: int</b> \n 若函數操作成功返回值為 LB_ERR 以外的任意值，若函數操作失敗返回值為 LB_ERR
 */
int DmListbox::GetItemRect(int nItem, RECT* rcPtr) const
{
	assert(this->IsWindow());
	// LB_GETITEMRECT
	// wParam = 指定項目的索引值
	// lParam = 指向 RECT 結構緩衝區位址的指標
	return static_cast<int>(::SendMessage(*this, LB_GETITEMRECT, static_cast<WPARAM>(nItem), reinterpret_cast<LPARAM>(rcPtr)));
}

/**
 *	@brief	取指定列表框中每個列的項目數
 *	@return	<b>型別: int</b> \n 返回每列的項目數
 */
int DmListbox::GetListBoxInfo() const
{
	assert(this->IsWindow());
	// LB_GETITEMRECT
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<int>(::SendMessage(*this, LB_GETITEMRECT, 0, 0));
}

/**
 *	@brief	取得列表框當前區域設置，可以使用區域設置來確定顯示文字的正確排序順序。
 *	@return	<b>型別: DWORD</b> \n 返回值指定列表框的當前區域設置。
 *		- HIWORD = 國家/地區代碼
 *		- LOWORD = 包含語言標識符
 */
LCID DmListbox::GetLocale() const
{
	assert(this->IsWindow());
	// LB_GETLOCALE
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<DWORD>(::SendMessage(*this, LB_GETLOCALE, 0, 0));
}

/**
 *	@brief	取得指定項目的被選用狀態
 *	@param[in] nItem 指定項目的索引值
 *	@return	<b>型別: int</b> \n 若函數操作成功返回值為非 LB_ERR 以外的任意值，若返回值為非零值代表項目前被選中，若返回值為零則代表項目沒有被選中
 */
int DmListbox::GetSelectionState(int nItem) const
{
	assert(this->IsWindow());
	// LB_GETSEL
	// wParam = 指定項目的索引值
	// lParam = 未使用，必須為零
	return static_cast<int>(::SendMessage(*this, LB_GETSEL, static_cast<WPARAM>(nItem), 0));
}

/**
 *	@brief	取得被選取項目數量，若列表風格被設定為 single-selection，必定傳回 LB_ERR
 *	@return	@return	<b>型別: int</b> \n 若函數操作成功返回值為當前被選取項目的數量，若函數操作失敗則返回值為 LB_ERR
 **/
int DmListbox::GetSelectionCount() const
{
	assert(this->IsWindow());
	// LB_GETSELCOUNT
	// wParam = 未使用，必須為零
	// lParam = 未使用，必須為零
	return static_cast<int>(::SendMessage(*this, LB_GETSELCOUNT, 0, 0));
}

/**
 *	@brief	取得所有選擇的項目，所有的項目索引值將存放於指定的緩衝區中。
 *	@param[in]	nCount	指定接收最大的項目數量
 *	@param[in]	intPtr	指向 int 緩衝區位址的指標，用於接收所有項目索引值，緩衝區大小必須能夠容納 nCount 指定數量。
 *	@return	<b>型別: int</b> \n 若函數操作成功返回值為存放於緩衝區的項目數量，如果列表框是單選列表框，則返回值為 LB_ERR。
 */
int DmListbox::GetSlecetItems(int nCount, int* intPtr) const
{
	assert(this->IsWindow());
	// LB_GETSELITEMS
	// wParam = 指定接收最大的項目數量
	// lParam = 指向 int 緩衝區位址
	return static_cast<int>(::SendMessage(*this, LB_GETSELITEMS, static_cast<WPARAM>(nCount), reinterpret_cast<LPARAM>(intPtr)));
}

/**
 *	@brief	取得指定項目的文字內容
 *	@param[in]		nItem	指定項目的索引值
 *	@param[in,out]	textPtr	指向預存放字串的緩衝區位址的指標
 *	@return	<b>型別: int</b> \n 若函數操作成功返回值為指定項目文字長度，單位 TCHAR，不含結尾 null，若函數操作失敗返回值 LB_ERR
 */
int DmListbox::GetItemText(int nItem, TCHAR* textPtr) const
{
	assert(this->IsWindow());
	// LB_GETTEXT
	// wParam = 指定項目的索引值
	// lParam = 指向預存放字串的緩衝區位址
	return static_cast<int>(::SendMessage(*this, LB_GETTEXT, static_cast<WPARAM>(nItem), reinterpret_cast<LPARAM>(textPtr)));
}

/**
 *	@brief	取得指定項目的文字長度
 *	@param[in] nItem 指定欲取得項目的索引值
 *	@return	<b>型別: int</b> \n 若函數操作成功返回值為指定項目文字長度，單位 TCHAR，不含結尾 null，若函數操作失敗則返回值為 LB_ERR
 */
int DmListbox::GetItemTextLength(int nItem) const
{
	assert(this->IsWindow());
	// LB_GETTEXTLEN
	// wParam = 指定欲取得項目的索引值
	// lParam = 未使用，必須為零
	return static_cast<int>(::SendMessage(*this, LB_GETTEXTLEN, static_cast<WPARAM>(nItem), 0));
}

/**
 *	@brief	取得目前位於列表框最上面的項目索引值
 *	@return	<b>型別: int</b> \n 若函數操作成功返回值為列標框中最上層的項目索引值。
 */
int DmListbox::GetTopIndex() const
{
	assert(this->IsWindow());
	// LB_GETTOPINDEX
	// wParam = 未使用，必須為零
	// lParam = 未使用，必須為零
	return static_cast<int>(::SendMessage(*this, LB_GETTOPINDEX, 0, 0));
}

/**
 *	@brief	配置列表框(ListBox)項目數量所需的記憶體量
 *	@param[in] nItemNumber	要分配的項目數量
 *	@param[in] dwSize		要分配的記憶體空間，單位 byte
 *	@return	<b>型別: DWORD</b>
 *		\n 若函數操作成功，返回值是已預先分配內存的項目總數，即所有成功的 LB_INITSTORAGE 訊息建立的項目總數。
 *		\n 若函數操作失敗，返回值為 LB_ERRSPACE
 */
DWORD DmListbox::InitStorage(int nItemNumber, DWORD dwSize) const
{
	assert(this->IsWindow());
	// LB_INITSTORAGE
	// wParam = 要分配的項目數量
	// lParam = 要分配的記憶體空間
	return static_cast<DWORD>(::SendMessage(*this, LB_INITSTORAGE, static_cast<WPARAM>(nItemNumber), static_cast<LPARAM>(dwSize)));
}

/**
 *	@brief	新增一個項目至指定位置，若指定位置 -1, 表示從列表最底部新增項目。
 *	@param[in] nItem	指定項目的索引值，若此參數被設定為 -1, 則將指定項目插入到列表最後
 *	@param[in] textPtr	欲插入項目的字串緩衝區位址的指標，內容為要新增的文字內容。
 *	@return	<b>型別: int</b> \n 若函數操作成功返回值為新增項目索引值，若函數操作失敗則返回值為 LB_ERR
 */
int DmListbox::InsertString(int nItem, TCHAR* textPtr) const
{
	assert(this->IsWindow());
	// LB_INSERTSTRING
	// wParam = 指定項目的索引值
	// lParam = 欲插入項目的字串緩衝區位址
	return static_cast<int>(::SendMessage(*this, LB_INSERTSTRING, static_cast<WPARAM>(nItem), reinterpret_cast<LPARAM>(textPtr)));
}

/**
 *	@brief	依據座標取得項目索引值
 *	@param[in] x 左上角座標 X
 *	@param[in] y 左上角座標 Y
 *	@return	<b>型別: int</b> \n 若函數操作成功返回值為項目索引值。 \n 若函數操作失敗返回值為 LB_ERR (-1)
 */
int DmListbox::ItemFromPoint(int x, int y) const
{
	assert(this->IsWindow());
	// LB_ITEMFROMPOINT
	// wParam = 不使用，必須為零
	// lParam = 座標值 HIWORD = 座標 Y, LOWORD = 座標 X
	auto lResult = ::SendMessage(*this, LB_ITEMFROMPOINT, 0, static_cast<LPARAM>(MAKELPARAM(x, y)));

	if (HIWORD(lResult)) {
		return LB_ERR;
	}
	return static_cast<int>(LOWORD(lResult));
}

/**
 *	@brief	重置、清除列表框(ListBox)的所有項目
 *	@return	此函數沒有返回值
 */
void DmListbox::ResetContent() const
{
	assert(this->IsWindow());
	// LB_RESETCONTENT
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	::SendMessage(*this, LB_RESETCONTENT, 0, 0);
}

/**
 *	@brief	在列表框的項目中搜尋符合指定的字串，如果找到相符的項目，該項目會被選取。
 *	@param[in] nStartAfter	指定開始搜尋的項目索引值，從 0 算起。(不包含指定的項目，例如指定 -1, 會搜索整個列表)
 *	@param[in] szItemPtr	指向文字字串的緩衝區位址的指標，值為要進行搜尋的文字字串內容。
 *	@return	<b>型別: int</b> \n 若函數操作成功返回值為非 LB_ERR 以外的任意值，值為被選定的項目所引值，失敗則返回值為 LB_ERR
 */
int DmListbox::SelectString(int nStartAfter, const TCHAR* szItemPtr) const
{
	assert(this->IsWindow());
	// LB_SELECTSTRING
	// wParam = 指定開始搜尋的項目索引值
	// lParam = 指向文字字串的緩衝區位址
	return static_cast<int>(::SendMessage(*this, LB_SELECTSTRING, static_cast<WPARAM>(nStartAfter), reinterpret_cast<LPARAM>(szItemPtr)));
}

/**
 *	@brief	多重選擇模式下，在列表框中選取多個連續的項目。
 *	@param[in] bSelect		選定或取消選擇 (TRUE / FALSE)
 *	@param[in] nFirstItem	指定選擇開始項目的索引值。
 *	@param[in] nLastItem	指定選擇結束項目的索引值。
 *	@return	<b>型別: BOOL</b> \n 若函數操作成功，返回值為非零值。 \n 若函數操作失敗，返回值為零。
 */
BOOL DmListbox::SelItemRange(BOOL bSelect, int nFirstItem, int nLastItem) const
{
	assert(this->IsWindow());
	// LB_SELITEMRANGE
	// wParam = 選定或取消選擇
	// lParam = 選擇範圍，LOWORD(範圍開始項目索引值)，HIWORD(範圍最後項目的索引值)。
	return ::SendMessage(*this, LB_SELITEMRANGE, static_cast<WPARAM>(bSelect), static_cast<LPARAM>(MAKELPARAM(nFirstItem, nLastItem))) != LB_ERR;
}

/**
 *	@brief	多重選擇模式下，在列表框中選取多個連續的項目。
 *	@param[in] nFirstItem	指定選擇開始項目的索引值。
 *	@param[in] nLastItem	指定選擇結束項目的索引值。
 *	@return	<b>型別: BOOL</b> \n 若函數操作成功，返回值為非零值。 \n 若函數操作失敗，返回值為零。
 */
BOOL DmListbox::SelItemRangeEx(int nFirstItem, int nLastItem) const
{
	assert(this->IsWindow());
	// LB_SELITEMRANGEEX	- 若成功返回值為 LB_OKAY (0), 若失敗返回值為 LB_ERR (-1)
	// wParam = 開始項目的索引值
	// lParam = 結束項目的索引值
	return ::SendMessage(*this, LB_SELITEMRANGEEX, static_cast<WPARAM>(nFirstItem), static_cast<LPARAM>(nLastItem)) != LB_ERR;
}

/**
 *	@brief	設定多重選擇模式下，設定選擇開始點，由開始點以下全部選擇。
 *	@param[in] nItem	開始的項目索引值
 *	@return	<b>型別: BOOL</b> \n 若函數操作成功，返回值為非零值。 \n 若函數操作失敗，返回值為零。
 */
BOOL DmListbox::SetAnchorIndex(int nItem) const
{
	assert(this->IsWindow());
	// LB_SETANCHORINDEX	- 若成功返回值為 LB_OKAY (0), 若失敗返回值為 LB_ERR (-1)
	// wParam = 開始的項目索引值
	// lParam = 不使用，必須為零
	return ::SendMessage(*this, LB_SETANCHORINDEX, static_cast<WPARAM>(nItem), 0) == LB_OKAY;
}

/**
 *	@brief	設定多重選擇模式下，指定列表框的項目焦點範圍，若指定項目不在可視範圍，則滾動軸會自動滾動至指定項目至可視範圍內。
 *	@param[in] nItem	指定要顯示的項目索引值
 *	@param[in] bScroll	指定項目是否要完全呈現，若參數值為 TRUE 表示捲動至可視範圍內即停止，若參數值為 FALSE 則表示指定項目到完全顯露為止。
 *	@return	<b>型別: BOOL</b> \n 若函數操作成功，返回值為非零值。 \n 若函數操作失敗，返回值為零。
 */
BOOL DmListbox::SetCaretIndex(int nItem, BOOL bScroll) const
{
	assert(this->IsWindow());
	// LB_SETCARETINDEX		- 若成功返回值為 LB_OKAY (0), 若失敗返回值為 LB_ERR (-1)
	// wParam = 指定要顯示的項目索引值
	// lParam = 是否要完全呈現
	return ::SendMessage(*this, LB_SETCARETINDEX, static_cast<WPARAM>(nItem), static_cast<LPARAM>(bScroll)) == LB_OKAY;
}

/**
 *	@brief	多重選擇樣式(LBS_MULTICOLUMN)，以像素為單位設定列表框項目寬度。
 *	@param[in] cxWidth 指定像素為單位的寬度
 *	@return	此函數沒有返回值
 */
void DmListbox::SetColumnWidth(int cxWidth) const
{
	assert(this->IsWindow());
	// LB_SETCOLUMNWIDTH
	// wParam = 指定像素為單位的寬度
	// lParam = 不使用，必須為零
	::SendMessage(*this, LB_SETCOLUMNWIDTH, static_cast<WPARAM>(cxWidth), 0);
}

/*
 *	@brief	設定列表框項目的數量
 *	@param[in] nCount 新的項目數量
 *	@return	<b>型別: BOOL</b> \n 若函數操作成功返回值為非零值。 \n 若函數操作失敗返回值為零。
 */
BOOL DmListbox::SetCount(int nCount) const
{
	assert(this->IsWindow());
	// LB_SETCOUNT
	// wParam = 新的項目數量
	// lParam = 不使用，必須為零
	return ::SendMessage(*this, LB_SETCOUNT, static_cast<WPARAM>(nCount), 0) == 0;
}

/**
 *	@brief	設定選取反白光棒位置
 *	@param[in] nItem 指定項目的索引值，若此參數被設定為 -1，則取消選擇反白光棒。
 *	@return	<b>型別: int</b> \n 若函數操作成功返回值為目前列表光棒的索引值，當前被選取項目的索引值，若函數操作失敗返回 LB_ERR
 */
int DmListbox::SetCursel(int nItem) const
{
	assert(this->IsWindow());
	// LB_SETCURSEL
	// wParam = 指定項目的索引值
	// lParam = 未使用、必須為零
	return static_cast<int>(::SendMessage(*this, LB_SETCURSEL, static_cast<WPARAM>(nItem), 0));
}

/**
 *	@brief	設置列表框可以水平卷軸寬度，以像素(pixel)為單位。
 *		\n 如果列表框的寬度小於設定值，則水平卷軸將會捲動至能呈現至目前選取項目的位置。
 *		\n 如果列表框的寬度等於或大於設定值，則隱藏水平卷軸。
 *	@param[in] cxExtent	指定水平捲軸的寬度，以像素(pixel)為單位。
 *	@return	此函數沒有返回值
 *	@remark	此函數不適用於多重選擇模式下使用。
 */
void DmListbox::SetHorizontalExtent(int cxExtent)
{
	assert(this->IsWindow());
	// LB_SETHORIZONTALEXTENT
	// wParam = 指定水平捲軸的寬度
	// lParam = 不使用，必須為零
	::SendMessage(*this, LB_SETHORIZONTALEXTENT, static_cast<WPARAM>(cxExtent), 0);
}

/**
 *	@brief	設定指定項目關連值，若指定項目為 (-1)，表示列表框的所有項目皆進行與關連的數值進行關連。
 *	@param[in] nItem		指定項目的索引值，從 0 算起。
 *	@param[in] dwItemData	要關連的數值
 */
BOOL DmListbox::SetItemData(int nItem, DWORD_PTR dwItemData) const
{
	assert(this->IsWindow());
	// LB_SETITEMDATA		- 若失敗返回值為 LB_ERR
	// wParam = 指定項目的索引值
	// lParam = 要關連的數值
	return ::SendMessage(*this, LB_SETITEMDATA, static_cast<WPARAM>(nItem), static_cast<LPARAM>(dwItemData)) != LB_ERR;
}

/**
 *	@brief	設定列表框項目的高度。
 *	@param[in] nItem		指定列表框中的項目的索引值。使用這個參數只限列表框含有 LBS_OWNERDRAWVARIABLE 樣式; 否則應該設定為 0。
 *	@param[in] cyItemHeight	指定高度，單位為像素(pixel)。
 */
int DmListbox::SetItemHeight(int nItem, UINT cyItemHeight) const
{
	assert(this->IsWindow());
	// LB_SETITEMHEIGHT
	// wParam = 指定列表框中的項目的索引值
	// lParam = 指定高度
	return static_cast<int>(::SendMessage(*this, LB_SETITEMHEIGHT, static_cast<WPARAM>(nItem), static_cast<LPARAM>(cyItemHeight)));
}

/**
 *	@brief	設定列表框(ListBox)的語言區域設定。
 *	@param[in] dwLocale	區域設定
 *		- HIWORD = 國家/地區代碼
 *		- LOWORD = 包含語言標識符
 *	@return	<b>型別: LCID</b> \n 返回值為設定前的語言區域設定值
 */
LCID DmListbox::SetLocale(LCID dwLocale) const
{
	assert(this->IsWindow());
	// LB_SETLOCALE
	// wParam = 區域設定
	// lParam = 不使用、必須為零
	return static_cast<LCID>(::SendMessage(*this, LB_SETLOCALE, static_cast<WPARAM>(dwLocale), 0));
}

/**
 *	@brief	設定列表框(ListBox)的語言區域設定。
 *	@param[in] nCountry		國家/地區代碼
 *	@param[in] nLanguage	語言標識符
 *		- HIWORD = 國家/地區代碼
 *		- LOWORD = 包含語言標識符
 *	@return	<b>型別: LCID</b> \n 返回值為設定前的語言區域設定值
 */
LCID DmListbox::SetLocale(int nCountry, int nLanguage) const
{
	assert(this->IsWindow());
	// LB_SETLOCALE
	// wParam = 區域設定
	// lParam = 不使用、必須為零
	return static_cast<LCID>(::SendMessage(*this, LB_SETLOCALE, static_cast<WPARAM>(MAKELCID(nLanguage, nCountry)), 0));
}

/**
 *	@brief	設定被選取狀態
 *	@param[in] nItem	項目索引，從 0 算起。
 *	@param[in] bState	被選取狀態，
 *		- 參數為 TRUE  表示設定被選取 (多選狀態下、加入 multiple-selection 群)
 *		- 參數為 FALSE 表示取消被選取 (多選狀耐下、移出 multiple-selection 群)
 *	@return	<b>型別: int</b> \n 若函數操作成功返回值為 LB_ERR 以外的的任意值。 \n 若函數操作失敗責返回值為 LB_ERR。
 */
int DmListbox::SetSelectionState(int nItem, BOOL bState) const
{
	assert(this->IsWindow());
	// LB_SETSEL
	// wParam = 被選取的狀態
	// lParam = 指定項目的索引值
	return static_cast<int>(::SendMessage(*this, LB_SETSEL, static_cast<WPARAM>(bState), static_cast<LPARAM>(nItem)));
}

/**
 *	@brief	設定列表框(ListBox)定位停駐點位置。
 *	@param[in] nTabStops	指定列表框(ListBox)的定位停駐點的數目。
 *	@param[in] rgTabStops	包含以對話方塊單位的定位停駐點位置的整數的陣列的第一個成員指標。
 *	@return	<b>型別: BOOL</b> \n 若函數操作成功返回值為非零值。 \n 若函數操作失敗返回值為零。
 */
BOOL DmListbox::SetTabStops(int nTabStops, INT* rgTabStops) const
{
	assert(this->IsWindow());
	// LB_SETTABSTOPS
	// wParam = 指定列表框(ListBox)的定位停駐點的數目。
	// lParam = 包含以對話方塊單位的定位停駐點位置的整數的陣列的第一個成員指標。
	return ::SendMessage(*this, LB_SETTABSTOPS, static_cast<WPARAM>(nTabStops), reinterpret_cast<LPARAM>(rgTabStops)) != 0;
}

/**
 *	@brief	將指定的項目提升至列表框顯示(可見)的範圍內，通常用來配合搜索用。
 *	@param[in] nItem 指定項目的索引碼，從 0 算起。
 *	@return	<b>型別: int</b> \n 若函數操作成功，返回值為零。 \n 若函數操作失敗，返回值為 LB_ERR。
 */
int DmListbox::SetTopIndex(int nItem) const
{
	assert(this->IsWindow());
	// LB_SETTOPINDEX
	// wParam = 指定項目的索引碼
	// lParam = 不使用，必須為零
	return static_cast<int>(::SendMessage(*this, LB_SETTOPINDEX, static_cast<WPARAM>(nItem), 0));
}
