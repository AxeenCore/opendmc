/**************************************************************************//**
 * @file	combo.cc
 * @brief	DmCombo 組合框類別成員函數定義
 * @date	2000-10-10
 * @date	2018-09-08
 * @author	Swang
 *****************************************************************************/
#include "opendmc/wnds/combo.hh"

/**
 *	@brief	DmCombo 建構式
 */
DmCombo::DmCombo() : DmWnds(EmCtrls::Combo) { }

/**
 *	@brief	DmCombo 解構式
 *	@remark 解構時，進行釋放(銷毀)控制項
 */
DmCombo::~DmCombo() { this->DestroyMine(); }

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
BOOL DmCombo::Create(const TCHAR* szCaptionPtr, int x, int y, int wd, int ht, HWND hWndParent, int nIDCItem)
{
	const DWORD dwStyle = CBS_DROPDOWN;
	const DWORD dwExStyle = 0;
	WNDSCTRLS smCtrl;

	UNREFERENCED_PARAMETER(szCaptionPtr);
	::memset(&smCtrl, 0, sizeof(smCtrl));
	smCtrl.hInstance	= ::GetWinapp().GetInstanceHandle();
	smCtrl.hParent		= hWndParent;
	smCtrl.pszCaption	= nullptr;	// Combo Box 沒有標題文字
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
BOOL DmCombo::CreateSample() { return FALSE; }

/**
 *	@brief 新增文字項目，此方式會受 CBS_SORT 樣式的影響自動進行項目排序
 *	@param[in] szPtr 指向文字字串緩衝區位址的指標
 *	@return <b>型別: int</b> \n 若函數操作成功返回值為新增項目的索引值，若函數操作失敗則返回值為 CB_ERR
 */
int DmCombo::AddItem(LPCTSTR szPtr) const
{
	assert(this->IsWindow());
	// CB_ADDSTRING
	// wParam = 不使用，必須為零
	// lParam = 指向文字字串緩衝區位址
	return static_cast<int>(::SendMessage(*this, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(szPtr)));
}

/**
 *	@brief 新增指定文字字串或與指定符合匹配的目錄和文件的名稱
 *	@param[in] uAttr	要增加到下拉式組合框的文件或目錄的屬性
 *	@param[in] wildPtr	指向以 NULL 結尾的文字字串緩衝區位址的指標，可以指定絕對路徑，相對路徑或文件名稱
 *	@return <b>型別: int</b> \n 若函數操作成功返回值為 CB_ERR 以外的任意數值。 \n 若函數操作失敗則返回值為零。
 *	@remark <b>uAttr 屬性列表</b>
 *		- DDL_ARCHIVE		\n 包括存檔文件。
 *		- DDL_DIRECTORY		\n 包含子目錄，用方括號（[]）括起來。
 *		- DDL_DRIVES		\n 所有映射的磁碟將增加到列表中。驅動器以[ - x - ] 的形式列出，其中x是磁碟編號。
 *		- DDL_EXCLUSIVE		\n 僅包含具有指定屬性的文件。默認情況下，即使未指定 DDL_READWRITE，也會列出讀/寫文件。
 *		- DDL_HIDDEN		\n 包含隱藏文件。
 *		- DDL_READONLY		\n 包括只讀文件。
 *		- DDL_READWRITE		\n 包括沒有其他屬性的讀/寫文件。這是默認值。
 *		- DDL_SYSTEM		\n 包括系統文件。
 */
int DmCombo::Dir(UINT uAttr, const TCHAR* wildPtr) const
{
	assert(this->IsWindow());
	// CB_DIR
	// wParam = 屬性
	// lParam = 指向以 null 結尾的文字字串緩衝區位址
	return static_cast<int>(::SendMessage(*this, CB_DIR, static_cast<WPARAM>(uAttr), reinterpret_cast<LPARAM>(wildPtr)));
}

/**
 *	@brief 在下拉式組合框(ComboBox)的列表框(ListBox)，找尋第一個與指定字串相符的項目索引值 (從 0 起算)
 *	@param[in] nStart	指定要開始搜尋的第一個項目索引值，從已指定的上一個項目開始搜尋，若此值為 1 那麼就是從頭開始搜尋
 *	@param[in] textPtr	指向一個以 null 結尾的文字字串緩衝區位址的指標，用來作為搜尋的參照文字。
 *	@return <b>型別: int</b> \n 若函數操作成功返回值為第一個搜到的項目索引值。\n 若函數操作失敗則返回 CB_ERR
 */
int DmCombo::FindString(int nStart, const TCHAR* textPtr) const
{
	assert(this->IsWindow());
	// CB_FINDSTRING
	// wParam = 指定要開始搜尋的第一個項目索引值
	// lParam = 指向一個以 null 結尾的文字字串緩衝區位址
	return static_cast<int>(::SendMessage(*this, CB_FINDSTRING, static_cast<WPARAM>(nStart), reinterpret_cast<LPARAM>(textPtr)));
}

/**
 *	@brief 在下拉式組合框(ComboBox) 尋找與 textPtr 指定的文字第一個相符、匹配的項目索引值 (從 0 起算)
 *	@param[in] nStart	指定要開始搜尋的第一個項目索引值，從已指定的上一個項目開始搜尋，若此值為 1 那麼就是從頭開始搜尋
 *	@param[in] textPtr	textPtr	指向一個以 null 結尾的文字字串緩衝區位址的指標，用來作為搜尋的參照文字。
 *	@return <b>型別: int</b> \n 返回值是匹配的項目從零開始的索引值。如果搜索不成功，則返回值為 CB_ERR。
 */
int DmCombo::FindStringExact(int nStart, const TCHAR * textPtr) const
{
	assert(this->IsWindow());
	// CB_FINDSTRINGEXACT
	// wParam = 指定要開始搜尋的第一個項目索引值
	// lParam = 指向一個以 null 結尾的文字字串緩衝區位址
	return static_cast<int>(::SendMessage(*this, CB_FINDSTRINGEXACT, static_cast<WPARAM>(nStart), reinterpret_cast<LPARAM>(textPtr)));
}

/**
 *	@brief 取得指定下拉式組合框(ComboBox)的訊息。
 *	@param[out]	cbiPtr	指向 COMBOBOXINFO 結構緩衝區位址的指標，用來接收下拉框的訊息
 *	@return <b>型別: BOOL</b>
 *		\n 若函數操作成功返回值為非零值。
 *		\n 若函數操作失敗返回值為零。要取得錯誤訊息，請調用 GetLastError
 */
BOOL DmCombo::GetComboBoxInfo(COMBOBOXINFO* cbiPtr) const
{
	assert(this->IsWindow());
	// CB_GETCOMBOBOXINFO
	// wParam = 不使用，必須為零
	// lParam = 指向 COMBOBOXINFO 結構緩衝區位址
	return ::SendMessage(*this, CB_GETCOMBOBOXINFO, 0, reinterpret_cast<LPARAM>(cbiPtr)) != 0;
}

/**
 *	@brief 刪除一個指定的項目
 *	@param[in] nItem 要刪除的項目索引值 (從 0 算起)
 *	@return <b>型別: int</b> \n 若函數操作成功返回值為目前項目的數量，若函數操作失敗則返回值為 CB_ERR
 */
int DmCombo::DeleteItem(int nItem) const
{
	assert(this->IsWindow());
	// CB_DELETESTRING
	// wParam = 要刪除項目的索引值
	// lParam = 不使用，必須為零
	return static_cast<int>(::SendMessage(*this, CB_DELETESTRING, static_cast<WPARAM>(nItem), 0));
}

/**
 *	@brief 取得下拉式組合框(ComboBox)所有項目的數量
 *	@return <b>型別: int</b> \n 若函數操作成功返回值為項目數量，若函數操作失敗則返回值為 CB_ERR
 */
int DmCombo::GetCount() const
{
	assert(this->IsWindow());
	// CB_GETCOUNT
	// wParam 不使用，必須為零
	// lParam 不使用，必須為零
	return static_cast<int>(::SendMessage(*this, CB_GETCOUNT, 0, 0));
}

/**
 *	@brief 取得下拉框(ComboBox)項目的文字內容
 *	@param[out]	textPtr	指向文字字串緩衝區位址的指標，接收下拉式組合框提示文字。
 *	@param[in]	cchMax	要取得字數最大值(單位: TCHAR)
 *	@return <b>型別: BOOL</b> \n 若函數操作成功返回值為非零值 \n 若函數操作失敗返回值為零
 */
BOOL DmCombo::GetCueBanner(TCHAR* textPtr, int cchMax) const
{
	assert(this->IsWindow());
	// CB_GETCUEBANNER
	// wParam = 要取得字數最大值(單位: TCHAR)
	// lParam = 指向文字字串緩衝區位址的指標
	return ::SendMessage(*this, CB_GETCUEBANNER, static_cast<WPARAM>(cchMax), reinterpret_cast<LPARAM>(textPtr)) == 1;
}

/**
 *	@brief 取得光標所在位置
 *	@return <b>型別: int</b> \n 若函數操作成功返回值為光標所在位置的索引值，若函數操作失敗則返回值為 CB_ERR
 */
int DmCombo::GetCurSel() const
{
	assert(this->IsWindow());
	// CB_GETCURSEL
	// wParam 不使用，必須為零
	// lParam 不使用，必須為零
	return static_cast<int>(::SendMessage(*this, CB_GETCURSEL, 0, 0));
}

/**
 *	@brief 取得下拉式組合框的列表框座標
 *	@param[in] rcPtr 指向 RECT 結構緩衝區位址的指標，用來接收下拉式組合框的列表框的座標。
 *	@return <b>型別: BOOL</b> \n 若函數操作成功返回值為非零值。 \n 若函數操作失敗返回值為零。
 */
BOOL DmCombo::GetDroppedControlRect(RECT* rcPtr) const
{
	assert(this->IsWindow());
	// CB_GETDROPPEDCONTROLRECT
	// wParam = 不使用，必須為零
	// lParam = 指向 RECT 結構緩衝區位址
	return ::SendMessage(*this, CB_GETDROPPEDCONTROLRECT, 0, reinterpret_cast<LPARAM>(rcPtr)) != 0;
}

/**
 *	@brief 取得下拉式組合框的列表框(ListBox)顯示狀態
 *	@return <b>型別: BOOL</b> \n 若列表框為可視狀態，返回值為非零值。 \n 若列表框為不可視或隱藏狀態，返回值為零。
 */
BOOL DmCombo::GetDroppedState() const
{
	assert(this->IsWindow());
	// CB_GETDROPPEDSTATE
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return ::SendMessage(*this, CB_GETDROPPEDSTATE, 0, 0) != 0;
}

/**
 *	@brief 取得下拉式組合框(ComboBox)的列表框(ListBox)最小可允許的寬度
 *	@return <b>型別: int</b>
 *		\n 若函數操作成功返回值為下拉式組合框(ComboBox)的列表框(ListBox)的寬度
 *		\n 若函數操作失敗凡回值為 CB_ERR
 */
int DmCombo::GetDroppedWidth() const
{
	assert(this->IsWindow());
	// CB_GETDROPPEDWIDTH
	// wParam 不使用，必須為零
	// lParam 不使用，必須為零
	return static_cast<int>(::SendMessage(*this, CB_GETDROPPEDWIDTH, 0, 0));
}

/**
 *	@brief 取得下拉式組合框(ComboBox)的編輯框(EditBox)操作代碼
 *	@return <b>型別: HWND</b> \n 若函數操作成功返回值為非零值，值為編輯框的操作代碼。 \n 若函數操作失敗返回值為零。
 */
HWND DmCombo::GetEditboxCtrl() const
{
	HWND hEdit = nullptr;
	COMBOBOXINFO cbi;
	::memset(&cbi, 0, sizeof(COMBOBOXINFO));
	cbi.cbSize = sizeof(COMBOBOXINFO);
	if (::GetComboBoxInfo(*this, &cbi)) {
		hEdit = cbi.hwndItem;
	}
	return hEdit;
}

/**
 *	@brief 取得下拉式組合框(ComboBox)的編輯框(EditBox)中文字被選取的範圍
 *	@return <b>型別: DWORD</b>
 *		\n 編輯框中文字被選取的範圍
 *		\n LOWORD = 選取範圍的起始文字，由左算起第幾個字元 (從零開始起算)
 *		\n HIWORD = 選取範圍的結束文字，由左算起第幾個字元 (從零開始起算)
 */
DWORD DmCombo::GetEditSel() const
{
	assert(this->IsWindow());
	// CB_GETEDITSEL
	// wParam 不使用，必須為零
	// lParam 不使用，必須為零
	return static_cast<DWORD>(::SendMessage(*this, CB_GETEDITSEL, 0, 0));
}

/**
 *	@brief 取得下拉式組合框是否有使用擴展樣式
 *	@return <b>型別: BOOL</b> 若下拉式組合框有使用擴展樣式則返回值為非零值，否則返回值為零。
 */
BOOL DmCombo::GetExtendedUI() const
{
	assert(this->IsWindow());
	// CB_GETEXTENDEDUI
	// wParam 不使用，必須為零
	// lParam 不使用，必須為零
	return static_cast<DWORD>(::SendMessage(*this, CB_GETEXTENDEDUI, 0, 0));
}

/**
 *	@brief 取得下拉式組合框的列表框水平捲軸寬度，單位像素(pixel)
 *	@return <b>型別: UINT</b> \n 返回值為水平寬度，單位像素(pixel)
 */
UINT DmCombo::GetHorizontalExtent() const
{
	assert(this->IsWindow());
	// CB_GETHORIZONTALEXTENT
	// wParam 不使用，必須為零
	// lParam 不使用，必須為零
	return static_cast<UINT>(::SendMessage(*this, CB_GETHORIZONTALEXTENT, 0, 0));
}

/**
 *	@brief 取得指定項目的關連值
 *	@param[in] nItem 指定項目的索引值
 *	@return <b>型別: DWORD_PTR</b>
 *		\n 若函數操作成功，返回值為 CB_ERR 以外的任意數值，值為與項關聯的值。
 *		\n 若含數操作失敗或發生錯誤，則返回值為 CB_ERR。
 */
DWORD_PTR DmCombo::GetItemData(int nItem) const
{
	assert(this->IsWindow());
	// CB_GETITEMDATA
	// wParam = 指定項目索引值
	// lParam = 不使用，必須為零
	return static_cast<DWORD_PTR>(::SendMessage(*this, CB_GETITEMDATA, static_cast<WPARAM>(nItem), 0));
}

/**
 *	@brief 取得下拉式組合框(ComboBox)的列表(ListBox)項目的高度
 *	@param[in] nItem 指定下拉式組合框的高度，若下拉式組合框定義了 CBS_OWNERDRAWVARIABLE 樣式，那麼此值為項目索引值
 *	@return <b>型別: int</b> \n 若函數操作成功返回值為高度(單位為像素下拉式組合框指定的項目)。 \n 若函數操作失敗或發生錯誤，傳回值為 CB_ERR。
 */
int DmCombo::GetItemHeight(int nItem) const
{
	assert(this->IsWindow());
	// CB_GETITEMHEIGHT
	// wParam = 指定下拉式組合框的高度
	// lParam = 不使用，必須為零
	return static_cast<int>(::SendMessage(*this, CB_GETITEMHEIGHT, static_cast<WPARAM>(nItem), 0));
}

/**
 *	@brief 取得指定項目的文字內容
 *	@param[in]	nItem	指定項目的索引值
 *	@param[out]	textPtr	指向保存文字字串緩衝區位址的指標
 *	@return <b>型別: int</b>
 *		\n 若函數操作成功返回值為指定項目的文字內容長度，單位 TCHAR (不含 NULL 結尾)。
 *		\n 若函數操作失敗則返回值為 CB_ERR
 */
int DmCombo::GetItemText(int nItem, TCHAR* textPtr) const
{
	assert(this->IsWindow());
	// CB_GETLBTEXT
	// wParam = 指定項目的索引值
	// lParam = 指向保存文字字串的緩衝區位址
	return static_cast<int>(::SendMessage(*this, CB_GETLBTEXT, static_cast<WPARAM>(nItem), reinterpret_cast<LPARAM>(textPtr)));
}

/**
 *	@brief 取得指定項目文字字串長度
 *	@param[in] nItem 要取得字串的索引值
 *	@return <b>型別: int</b>
 *		\n 若函數操作成功返回值為指定項目的字串長度(單位 TCHAR，不含 NULL 結尾)。
 *		\n 若函數操作失敗則返回值 CB_ERR
 */
int DmCombo::GetItemTextLength(int nItem) const
{
	assert(this->IsWindow());
	// CB_GETLBTEXTLEN
	// wParam = 指定項目的索引值
	// lParam = 不使用，必須為零
	return static_cast<int>(::SendMessage(*this, CB_GETLBTEXTLEN, static_cast<WPARAM>(nItem), 0));
}

/**
 *	@brief 取得下拉式組合框(ComboBox)的列表框(ListBox)操作代碼
 *	@return	<b>型別: HWND</b>
 *		\n 若函數操作成功返回值為非零值，值為下拉式組合框的列表框操作代碼。
 *		\n 若函數操作失敗返回值為零。
 */
HWND DmCombo::GetListboxCtrl() const
{
	HWND hList = nullptr;
	COMBOBOXINFO cbi;
	::memset(&cbi, 0, sizeof(COMBOBOXINFO));
	cbi.cbSize = sizeof(COMBOBOXINFO);
	if (::GetComboBoxInfo(*this, &cbi)) {
		hList = cbi.hwndList;
	}
	return hList;
}

/**
 *	@brief 取得下拉式組合框(ComboBox)當前地區語言設置，可以使用區域設置來確定顯示文字的正確排序順序。
 *	@return	<b>型別: LCID</b> \n 返回值為當前下拉式組合框(ComboBox)的地區語言設置。
 *		- HIWORD = 國家/地區代碼
 *		- LOWORD = 包含語言標識符
 */
LCID DmCombo::GetLocale() const
{
	assert(this->IsWindow());
	// CB_GETLOCALE
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<LCID>(::SendMessage(*this, CB_GETLOCALE, 0, 0));
}

/**
 *	@brief 取得可見項目範圍最小值
 *	@return	<b>型別: int</b>
 *		\n 若函數操作成功返回值為 CB_ERR 以外的任意值，值為下拉式組合框的列表框最小可見的項目數量。
 *		\n 若函數操作失敗或發生錯誤，返回值為 CB_ERR。
 */
int DmCombo::GetMinVisible() const
{
	assert(this->IsWindow());
	// CB_GETMINVISIBLE
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<int>(::SendMessage(*this, CB_GETMINVISIBLE, 0, 0));
}

/**
 *	@brief 取得下拉式組合框(ComboBox)第一個可見項目的索引值
 *	@return	<b>型別: int</b>
 *		\n 若函數操作成功返回值為 CB_ERR 已外的任意值，值為第一個可見的項目所引值。
 *		\n 若函數操作失敗返回值為零。
 */
int DmCombo::GetTopIndex() const
{
	assert(this->IsWindow());
	// CB_GETTOPINDEX
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<int>(::SendMessage(*this, CB_GETTOPINDEX, 0, 0));
}

/**
 *	@brief 配置 ComboBox 記憶體
 *	@param[in] nNumber	指定要增加的項目數量
 *	@param[in] uBytes	指定記憶體數量 (單位: byte)
 *	@return	<b>型別: int</b>
 *		\n 若運作成功返回值為已分配的記憶體總合。(即所有成功使用 CB_INITSTORAGE 增加的記憶體總數)
 *		\n 若運作失敗返回值為 CB_ERRSPACE
 */
int DmCombo::InitStorage(int nNumber, UINT uBytes) const
{
	// CB_INITSTORAGE
	// wParam = 指定要鄒積的項目數量
	// lParam = 指定記憶體數量
	return static_cast<int>(::SendMessage(*this, CB_INITSTORAGE, static_cast<WPARAM>(nNumber), static_cast<LPARAM>(uBytes)));
}

/**
 *	@brief 新增項目、插入一個新的項目，新增的項目會依據指定位置，其指定的位置的項目含以後項目會依序往後遞增。此方式新增不會受 CBS_SORT 樣式影響，不會自動排序。
 *	@param[in] nItem	指定要插入的索引值，若指定值為 -1 則表示將在列表最底部新增項目
 *	@param[in] szPtr	指向字串緩衝區位址的指標
 *	@return	<b>型別: int</b> \n 若函數操作成功返回值為新增索引值，若函數操作失敗則返回值為 CB_ERR
 */
int DmCombo::InsertItem(int nItem, LPCTSTR szPtr) const
{
	assert(this->IsWindow());
	// CB_INSERTSTRING
	// wParam = 指定要插入的索引值
	// lParam = 指向字串緩衝區位址
	return static_cast<int>(::SendMessage(*this, CB_INSERTSTRING, static_cast<WPARAM>(nItem), reinterpret_cast<LPARAM>(szPtr)));
}

/**
 *	@brief 設定 ComboBox 的 EditBox 文本輸入字數限制 (in TCHAR)
 *	@param[in] ccLimit 限制輸入的自串長度 (單位 TCHAR), 最大長度限制為 0x7FFFFFFE 個 TCHAR 字元。
 *	@return 此函數沒有返回值，CBS_AUTOHSCROLL 訊息處理始終返回 TRUE
 *	@remark 如果 ComboBox 沒有具備 CBS_AUTOHSCROLL 樣式，則將文字字串字元數限制設置大於 EditBox 控制項的大小則無效。
 */
void DmCombo::SetLimitText(int ccLimit) const
{
	assert(this->IsWindow());
	// CB_LIMITTEXT
	// wParam = 限制輸入的自串長度 (單位 TCHAR)
	// lParam = 不使用，必須為零
	::SendMessage(*this, CB_LIMITTEXT, static_cast<WPARAM>(ccLimit), 0);
}

/**
 *	@brief 重置 ComboBox，清除 ComboBox 列表上的所有項目
 *	@return 此函數沒有返回值
 *	@see DmCombo::RemoveAllItem
 */
void DmCombo::ResetContent() const
{
	assert(this->IsWindow());
	// CB_RESETCONTENT
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	::SendMessage(*this, CB_RESETCONTENT, 0, 0);
}

/**
 *	@brief 移除 ComboBox 列表上的所有項目
 *	@return 此函數沒有返回值
 */
void DmCombo::RemoveAllItem() const
{
	assert(this->IsWindow());
	// CB_RESETCONTENT
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	::SendMessage(*this, CB_RESETCONTENT, 0, 0);
}

/**
 *	@brief 搜尋下拉式組合框(ComboBox)的列表框(ListBox)清單的字串，並找到字串，如果在列表框中找到符合的字串，將其文字複製下拉式組合框的編輯框。
 *	@param[in] nStartAfter	指定要進行搜索的起始索引值 (從 0 算起), 搜尋不含指定的起始值值。
 *	@param[in] strPtr		指向文字字緩衝區位址的指標，存放用來進行搜尋的比對參照字串。
 *	@return	<b>型別: int</b>
 *		\n 若函數操作成功返回值為 CB_ERR 已外的任意值，值為找到字串的索引值。
 *		\n 若函數操作失敗返回值為 CB_ERR 且不會變更當前的選擇。
 */
int DmCombo::SelectString(int nStartAfter, const TCHAR* strPtr) const
{
	assert(this->IsWindow());
	// CB_SELECTSTRING
	// wParam = 指定要進行搜索的起始索引值
	// lParam = 指向文字字緩衝區位址
	return static_cast<int>(::SendMessage(*this, CB_SELECTSTRING, static_cast<WPARAM>(nStartAfter), reinterpret_cast<LPARAM>(strPtr)));
}

/**
 *	@brief 設定下拉式控制框(ComboBox)所顯示的提式文字。
 *	@param[in] textPtr 指向文字字串緩衝區位址的指標，要設定的文字內容存放區
 *	@return <b>型別: BOOL</b> \n 若函數操作成功返回值為非零值 \n 若雸術操作失敗返回值為零
 */
BOOL DmCombo::SetCueBanner(const TCHAR* textPtr) const
{
	assert(this->IsWindow());
	// CB_SETCUEBANNER
	// wParam = 不使用，必須為零
	// lParam = 指向文字字串緩衝區位址
	return ::SendMessage(*this, CB_SETCUEBANNER, 0, reinterpret_cast<LPARAM>(textPtr)) == 1;
}

/**
 *	@brief 設定目前清單上光標位置 (選擇項目)
 *	@param[in] nItem 欲設定 cursel 位置的索引值
 *	@return <b>型別: int</b> \n 若函數操作成功返回值為當前光標的位置，若函數操作失敗則返回值為 CB_ERR
 */
int DmCombo::SetCurSel(int nItem) const
{
	assert(this->IsWindow());
	// CB_SETCURSEL
	// wParam = 指定列表光標(Cursel)未置的索引值
	// lParam = 不使用、必須為零
	return static_cast<int>(::SendMessage(*this, CB_SETCURSEL, static_cast<WPARAM>(nItem), 0));
}

/**
 *	@brief 設定下拉式組合框(ComboBox)的列表框(ListBox)最小可允許的寬度
 *	@param[in] nWidth 指定寬度，單位: 像素(pixel)
 *	@return	<b>型別: int</b>
 *		\n 若函數操作成功返回值為下拉式組合框(ComboBox)的列表框(ListBox)的新寬度
 *		\n 若函數操作失敗凡回值為 CB_ERR
 */
int DmCombo::SetDroppedWidth(int nWidth) const
{
	// CB_SETDROPPEDWIDTH
	// wParam = 指定寬度，單位: 像素(pixel)
	// lParam = 不使用，必須為零
	return static_cast<int>(::SendMessage(*this, CB_SETDROPPEDWIDTH, static_cast<WPARAM>(nWidth), 0));
}

/**
 *	@brief 取得下拉式組合框(ComboBox)的編輯框(EditBox) 字元
 *	@param[in] nStartChar	指定開始位置 (從 0 算起的字元位置)
 *	@param[in] nEndChar		指定結束位置 (從 0 算起的自原位置)
 *	return <b>型別: BOOL</b> \n 若函數操作成功返回值為非零值。 \n 若函數操作失敗返回值為零。
 */
BOOL DmCombo::SetEditSel(int nStartChar, int nEndChar) const
{
	assert(this->IsWindow());
	// CB_SETEDITSEL
	// wParam = 不使用、必須為零
	// lParam = 指定範圍
	return ::SendMessage(*this, CB_SETEDITSEL, 0, static_cast<LPARAM>(MAKELONG(nStartChar, nEndChar))) != 0;
}

/**
 *	@brief 設定下拉式組合框使用擴展樣式
 *	@param[in] bExtended 指定使用擴展樣式
 *		- TRUE	\n 使用，使用擴展樣式
 *		- FALSE	\n 不使用，使用標準樣式
 *	@return	<b>型別: BOOL</b> \n 若函數操作成功返回值為非零值。 \n 若函數操作失敗則返回值為零
 */
BOOL DmCombo::SetExtendedUI(BOOL bExtended)
{
	assert(this->IsWindow());
	// CB_SETEXTENDEDUI		- 若成功返回 CB_OKAY，若失敗返回 CB_ERR
	// wParam = 設定值
	// lParam = 不使用、必須為零
	return static_cast<int>(::SendMessage(*this, CB_SETEXTENDEDUI, static_cast<WPARAM>(bExtended), 0));
}

/**
 *	@brief 設定下拉式組合框(ComboBox)的列表框(ListBox)水平捲軸的寬度。
 *	@param[in] nExtent 水平捲軸的寬度，單位像素(pixel)
 *	@return 此函數沒有返回值。
 */
void DmCombo::SetHorizontalExtent(UINT nExtent)
{
	assert(this->IsWindow());
	// CB_SETHORIZONTALEXTENT
	// wParam = 水平捲軸的寬度
	// lParam = 不使用、必須為零
	::SendMessage(*this, CB_SETHORIZONTALEXTENT, static_cast<WPARAM>(nExtent), 0);
}

/**
 *	@brief 設定一個 32-bit 的值到指定的項目中
 *	@param[in] nItem		指定要進行關連的項目索引值
 *	@param[in] dwItemData	要進行關連的指定值
 *	return <b>型別: BOOL</b> \n 若函數操作成功返回值為非零值。 \n 若函數操作失敗返回值為零。
 */
BOOL DmCombo::SetItemData(int nItem, DWORD_PTR dwItemData) const
{
	assert(this->IsWindow());
	// CB_SETITEMDATA		- 若發生錯誤返回值為 LB_ERR
	// wParam = 項目索引值
	// lParam = 關連值
	return ::SendMessage(*this, CB_SETITEMDATA, static_cast<WPARAM>(nItem), static_cast<LPARAM>(dwItemData)) != LB_ERR;
}

/**
 *	@brief 設定下拉式組合框(ComboBox)的列表(ListBox)項目的高度
 *	@param[in] nItem		指定下拉式組合框的高度，若下拉式組合框定義了 CBS_OWNERDRAWVARIABLE 樣式，那麼此值為項目索引值。
 *	@param[in] cyItemHeight	指定由 nItem 標示的下拉式組合框高度，以像素(pixel)為單位。
 *	@return	<b>型別: int</b> \n 若函數操作成功返回值為項目所引值或高度 \n 若函數操作失敗或發生錯誤返回值為 CB_ERR
 */
int DmCombo::SetItemHeight(int nItem, int cyItemHeight) const
{
	assert(this->IsWindow());
	// CB_SETITEMHEIGHT
	// wParam = 指定下拉式組合框的高度
	// lParam = 指定由 nItem 標示的下拉式組合框高度
	return static_cast<int>(::SendMessage(*this, CB_SETITEMHEIGHT, static_cast<WPARAM>(nItem), static_cast<LPARAM>(cyItemHeight)));
}

/**
 *	@brief 設定下拉式組合框(ComboBox)的區域設定。
 *	@param[in] dwLocale	區域設定
 *		- HIWORD = 國家/地區代碼
 *		- LOWORD = 包含語言標識符
 *	@return <b>型別: LCID</b> \n 返回值為設定前的區域設定值
 */
LCID DmCombo::SetLocale(LCID dwLocale) const
{
	assert(this->IsWindow());
	// CB_SETLOCALE
	// wParam = 區域設定
	// lParam = 不使用、必須為零
	return static_cast<LCID>(::SendMessage(*this, CB_SETLOCALE, static_cast<WPARAM>(dwLocale), 0));
}

/**
 *	@brief 設定下拉式組合框(ComboBox)的區域設定。
 *	@param[in] nCountry		國家/地區代碼
 *	@param[in] nLanguage	語言標識符
 *	@return <b>型別: LCID</b> \n 返回值為設定前的區域設定值
 *	@remark <b>訊息參數 LPARAM 資料表示</b>
 *		- HIWORD = 國家/地區代碼
 *		- LOWORD = 包含語言標識符
 */
LCID DmCombo::SetLocale(int nCountry, int nLanguage) const
{
	assert(this->IsWindow());
	// CB_SETLOCALE
	// wParam = 區域設定
	// lParam = 不使用、必須為零
	return static_cast<LCID>(::SendMessage(*this, CB_SETLOCALE, static_cast<WPARAM>(MAKELCID(nLanguage, nCountry)), 0));
}

/**
 *	@brief 設定可見範圍內的項目數量，下拉式組合框的列表框的列表框項目數量。
 *	@param[in] iMinVisible	項目數量
 *	@return <b>型別: BOOL</b> \n 若函數操作成功返回值為非零值。 \n 若含數操作不當則返回值為零。
 */
BOOL DmCombo::SetMinVisibleItems(int iMinVisible)
{
	assert(this->IsWindow());
	// CB_SETMINVISIBLE
	// wParam = 項目數量
	// lParam = 不使用，必須為零
	return ::SendMessage(*this, CB_SETMINVISIBLE, static_cast<WPARAM>(iMinVisible), 0) != 0;
}

/**
 *	@brief 設定指定項目
 *	@param[in] nItem 指定項目索引值
 *	@return <b>型別: int</b> \n 若函數操作成功返回值為零 \n 若函數操作失敗返回 CB_ERR
 */
int DmCombo::SetTopIndex(int nItem) const
{
	assert(this->IsWindow());
	// CB_SETTOPINDEX
	// wParam = 指定項目索引值
	// lParam = 不使用，必須為零
	return static_cast<int>(::SendMessage(*this, CB_SETTOPINDEX, static_cast<WPARAM>(nItem), 0));
}

/**
 *	@brief 顯示或隱藏下拉框
 *	@param[in] bEnable 若值為 TRUE 則表示顯示下拉框，若值為 FALSE 表表示收起下拉框。
 *	@return 此函數沒有返回值，CB_SHOWDROPDOWN 訊息操作後始終返回 TRUE
 */
void DmCombo::ShowDropdown(BOOL bEnable) const
{
	assert(this->IsWindow());
	// CB_SHOWDROPDOWN
	// wParam = 指定顯示或隱藏 (TRUE / FALSE)
	// lParam = 不使用、必須為零
	::SendMessage(*this, CB_SHOWDROPDOWN, static_cast<WPARAM>(bEnable), 0);
}

/**
 *	@brief 清除、刪除下拉式組合框(ComboBox)當前選擇的的編輯框(EditBox)內容
 *	@return 此函數沒有返回值
 */
void DmCombo::Clear() const
{
	assert(this->IsWindow());
	// WM_CLEAR
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	::SendMessage(*this, WM_CLEAR, 0, 0);
}

/**
 *	@brief 複製下拉式組合框(ComboBox)當前選擇的編輯框(EditBox)內容，以 CF_TEXT 格式將當前選擇編輯框(EditBox)內容複製到剪貼板。
 *	@return 此函數沒有返回值
 */
void DmCombo::Cpoy() const
{
	assert(this->IsWindow());
	// WM_COPY
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	::SendMessage(*this, WM_COPY, 0, 0);
}

/**
 *	@brief 剪下下拉式組合框(ComboBox)的當前選擇編輯框(EditBox)內容，並以 CF_TEXT 格式將當前選擇編輯框(EditBox)內容複製到剪貼板。
 *	@return 此函數沒有返回值
 */
void DmCombo::Cut() const
{
	assert(this->IsWindow());
	// WM_CUT
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	::SendMessage(*this, WM_CUT, 0, 0);
}

/**
 *	@brief 將剪貼板的內容貼到下拉式組合框(ComboBox)當前被選取的編輯框(EditBox)裡，剪貼板要有 CF_TEXT 格式所保存的內容，才會進行貼上。
 *	return 此函數沒有返回值
 */
void DmCombo::Paste() const
{
	assert(this->IsWindow());
	// WM_PASTE
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	::SendMessage(*this, WM_PASTE, 0, 0);
}

/**
 *	@brief 復原最後的改變
 *	@return	<b>型別: BOOL</b> \n 若復原作業成功返回值為非零值。 \n 若復原失敗返回值為零。
 */
BOOL DmCombo::UnDo() const
{
	assert(this->IsWindow());
	// EM_UNDO
	// wParam = 未使用，必須為零
	// lParam = 未使用，必須為零
	return ::SendMessage(*this, WM_UNDO, 0, 0) != 0;
}
