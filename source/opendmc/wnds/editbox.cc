/**************************************************************************//**
 * @file	editbox.cc
 * @brief	DmEditbox 編輯框類別成員函數定義
 * @date	2000-10-10
 * @date	2018-09-02
 * @author	Swang
 *****************************************************************************/
#include "opendmc/wnds/editbox.hh"

/**
 *	@brief DmEditbox 建構式
 */
DmEditbox::DmEditbox() : DmWnds(EmCtrls::Edit) { }

/**
 *	@brief DmEditbox 解構式
 *	@remark 解構時，進行釋放(銷毀)控制項
 */
DmEditbox::~DmEditbox() { this->SafeWndsDestroy(); }

/**
 *	@brief [重載] 建立一個編輯框
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
BOOL DmEditbox::Create(const TCHAR* szCaptionPtr, int x, int y, int wd, int ht, HWND hWndParent, int nIDCItem)
{
	const DWORD dwStyle = ES_AUTOHSCROLL;
	const DWORD dwExStyle = 0;
	WNDSCTRLS smCtrl;

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
 *	@brief 建立一個編輯框樣本 (由繼承者發展)
 *	@return	<b>型別: BOOL</b> 若視窗建立成功返回值為非零值，若視窗建立失敗責返回值為零。
 */
BOOL DmEditbox::CreateSample() { return FALSE; }

/**
 *	@brief 將指定字串內容新增至當前文件末端。
 *	@param[in] textPtr	指向文字字串緩衝區位址的指標，值為要增加的文字內容。
 *	@return 此函數沒有返回值
 */
void DmEditbox::AppendText(const TCHAR* textPtr) const
{
	DWORD posEnd = static_cast<DWORD>(::SendMessage(*this, WM_GETTEXTLENGTH, 0, 0));
	// 設定選取範圍 (選取編輯框尾端位置)
	this->SetSelect(posEnd, posEnd);
	// 進行文字替代
	this->ReplaceSel(textPtr);
}

/**
 *	@brief 在恢復的編輯的列隊(undo queue)中，是否還有可恢復的內容存在
 *	@return <b>型別: BOOL</b>
 *		\n 如果編輯框的恢復列隊中有內容存在，返回值為非零值。
 *		\n 如果編輯框的恢復列隊中無任何內容，返回值為零。
 */
BOOL DmEditbox::CanUndo() const
{
	assert(this->IsWindow());
	// EM_CANUNDO
	// wParam = 未使用，必須為零
	// lParam = 未使用，必須為零
	return ::SendMessage(*this, EM_CANUNDO, 0, 0) != 0;
}

/**
 *	@brief 取得編輯框指定文字位置的索引值
 *	@param[in, out] pointPtr 指向 POINT 結構緩衝區位址的指標，用來接收文字位置座標
 *	@return <b>型別: int</b> \n 返回值分高低兩組
 *		\n LOWORD = 指定文字水平位置的索引值(從 0 算起)，若單行編輯
 *		\n HIWORD = 單行編輯此值為零，若多行編輯此值為文字垂直位置索引。
 */
int DmEditbox::CharFromPos(POINT* pointPtr) const
{
	assert(this->IsWindow());
	// EM_CANUNDO
	// wParam = 未使用，必須為零
	// lParam = 指向 POINT 結構緩衝區位址
	return static_cast<int>(::SendMessage(*this, EM_CHARFROMPOS, 0, reinterpret_cast<LPARAM>(pointPtr)));
}

/**
 *	@brief 清除 Undo(撤銷) 緩衝區保存內容
 *	@return 此函數沒有返回值
 */
void DmEditbox::EmptyUndoBuffer() const
{
	assert(this->IsWindow());
	// EM_EMPTYUNDOBUFFER
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	::SendMessage(*this, EM_EMPTYUNDOBUFFER, 0, 0);
}

/**
 *	@brief 多行編輯中，開啟或關閉設定換行字元。(若開啟將會在進行換行時自動加入換行字元)
 *	@param[in] bAddEOL 開啟或關閉旗標
 *	@return <b>型別: BOOL</b> \n 返回值與 bAddEOL 相同。
 */
BOOL DmEditbox::FmtLines(BOOL bAddEOL) const
{
	assert(this->IsWindow());
	// EM_FMTLINES
	// wParam = 開啟或關閉換行指令
	// lParam = 不使用，必須為零
	return ::SendMessage(*this, EM_FMTLINES, static_cast<LPARAM>(bAddEOL), 0) != 0;
}

/**
 *	@brief 取得編輯框的提示文字
 *	@param[out]	textPtr 指向文字字串緩衝區位址的指標，用來接收提示文字的緩衝區。
 *	@param[in]	cchText	要取得的字串長度 (包含 NULL 結尾)，以 TCHAR 做為單位。
 *	@return <b>型別: BOOL</b> \n 若運作成功返回值為非零值，若運作失敗返回值為零。
 */
BOOL DmEditbox::GetCueBanner(TCHAR* textPtr, int cchText) const
{
	// EM_GETCUEBANNER
	// wParam = 要取得的字串長度
	// lParam = 指向文字字串緩衝區位址
	return ::SendMessage(*this, EM_GETCUEBANNER, static_cast<WPARAM>(cchText), reinterpret_cast<LPARAM>(textPtr)) != 0;
}

/**
 *	@brief 取得編輯框最上層可見的行列
 *	@return <b>型別: int</b> \n 返回值為最上層的可見行列的索引值。 \n 若僅為單行編輯框，則返回值值為 0。
 */
int DmEditbox::GetFirstVisibleLine() const
{
	assert(this->IsWindow());
	// EM_GETFIRSTVISIBLELINE
	// wParam = 未使用，必須為零
	// lParam = 未使用，必須為零
	return static_cast<int>(::SendMessage(*this, EM_GETFIRSTVISIBLELINE, 0, 0));
}

/*
 *	@brief 取得編輯控制項內容緩衝區操作代碼(handle)
 *	@return <b>型別: HANDLE</b>
 *		\n	返回值是一個記憶體的操作代碼(handle)，用來操作編輯框控制項內容的緩衝區。
 *		\n	如果發生錯誤, 那麼會發送一條單行訊息到編輯框，而返回值將為零。
 *	@remark	額外說明
 *		\n	若函數操作成功，應用程序可將返回值強制轉換為 HLOCAL 型別並將其傳遞給 LocalLock 來操作編輯框控制項緩衝區的內容
 *		\n	LocalLock 返回值為指向緩衝區的位址，該緩衝區是以 NULL 做為結尾的字串緩衝區，字串的字元採用 CHAR 或 WCHAR，取決於程式編譯時為 ANSI 或 Unicode。
 *	@see https://docs.microsoft.com/en-us/windows/desktop/controls/em-gethandle
 */
HANDLE DmEditbox::GetHandle() const
{
	assert(this->IsWindow());
	// EM_GETHANDLE
	// wParam = 未使用，必須為零
	// lParam = 未使用，必須為零
	return reinterpret_cast<HANDLE>(::SendMessage(*this, EM_GETHANDLE, 0, 0));
}

/**
 *	@brief 取得反白區域的位置資訊
 *	@param[out] pichStart	指向 int 資料緩衝區為指的指標，用來接收反白顯示區第一個字元位置索引值。
 *	@param[out] pichEnd		指向 int 資料緩衝區為指的指標，用來接收反白顯示區最後一個字元位置索引值。
 *	@return <b>型別: BOOL</b> \n 若函數操作成功返回值為非零值。 \n 若函數操作失敗返回值為零。
 */
BOOL DmEditbox::GetHighlight(int* pichStart, int* pichEnd) const
{
	// EM_GETHILITE - MSDN 尚未完成此一函數實做
	UNREFERENCED_PARAMETER(pichStart);
	UNREFERENCED_PARAMETER(pichEnd);
	return 0;
}

/**
 *	@brief 取得一個狀態旗標，表示 Edit 控制項如何與輸入法(IME)關聯。
 *	@return <b>型別: DWORD</b> \n Data specific to the type of status to retrieve. With the EMSIS_COMPOSITIONSTRING value for status.
 *	@see https://docs.microsoft.com/en-us/windows/desktop/controls/em-getimestatus
 */
DWORD DmEditbox::GetItemStatus() const
{
	assert(this->IsWindow());
	// EM_GETIMESTATUS 
	// wParam = The type of status to retrieve. This parameter can be the following value.
	// lParam = 未使用，最好為零
	return static_cast<DWORD>(::SendMessage(*this, EM_GETIMESTATUS, static_cast<WPARAM>(EMSIS_COMPOSITIONSTRING), 0));
}

/**
 *	@brief 取得編輯框字數限制
 *	@return <b>型別: int</b> \n 返回字數限制，單位 TCHAR
 */
int DmEditbox::GetLimitText() const
{
	assert(this->IsWindow());
	// EM_GETLIMITTEXT
	// wParam = 未使用，必須為零
	// lParam = 未使用，必須為零
	return static_cast<int>(::SendMessage(*this, EM_GETLIMITTEXT, 0, 0));
}

/**
 *	@brief 複製指定行的文字內容到指定的緩衝區
 *	@param[in]	nLine	指定要複製哪一行
 *	@param[out]	textPtr	存放複製的字串緩衝區位址
 *	@return <b>型別: int</b> 字元數量。\n 返回值為成功複製的文字字串長度
 */
int DmEditbox::GetLine(int nLine, TCHAR* textPtr) const
{
	assert(this->IsWindow());
	// EM_GETLINE
	// wParam = 指定要複製哪一行
	// lParam = 存放複製的字串緩衝區位址
	return static_cast<int>(::SendMessage(*this, EM_GETLINE, static_cast<WPARAM>(nLine), reinterpret_cast<LPARAM>(textPtr)));
}

/**
 *	@brief 複製指定行的文字內容到指定的緩衝區
 *	@param[in]		nLine		指定要複製哪一行
 *	@param[in,out]	textPtr		存放複製的字串緩衝區位址
 *	@param[in]		maxLength	指定緩衝區大小
 *	@return <b>型別: int</b> 字元數量。\n 返回值為成功複製的文字字串長度
 */
int DmEditbox::GetLine(int nLine, TCHAR* textPtr, int maxLength) const
{
	assert(this->IsWindow());
	// EM_GETLINE
	// wParam = 指定要複製哪一行
	// lParam = 存放複製的字串緩衝區位址
	*textPtr = static_cast<TCHAR>(maxLength);
	return static_cast<int>(::SendMessage(*this, EM_GETLINE, static_cast<WPARAM>(nLine), reinterpret_cast<LPARAM>(textPtr)));
}

/**
 *	@brief 取得多行編輯框的行數數量
 *	@return <b>型別: int</b> \n 返回值為多行模式的編輯框行號的總數。 \n 若編輯框內沒有任何文字，則傳回值為 1，返回值永遠傳回大於或等於 1
 */
int DmEditbox::GetLineCount() const
{
	assert(this->IsWindow());
	// EM_GETLINECOUNT
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<int>(::SendMessage(*this, EM_GETLINECOUNT, 0, 0));
}

/**
 *	@brief 取得編輯框的的左右邊界。
 *	@return <b>型別: DWORD</b> \n 返回值為編輯框的左右邊界
 *		- LOWORD 為左邊界，編輯框寬度的左側邊界值。
 *		- HIWORD 為右邊界，編輯框寬度的右側邊界值。
 */
DWORD DmEditbox::GetMargins() const
{
	assert(this->IsWindow());
	// EM_GETMARGINS
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<DWORD>(::SendMessage(*this, EM_GETMARGINS, 0, 0));
}

/**
 *	@brief 編輯框內容是否被修改
 *	@return <b>型別: BOOL</b> \n 若編輯框內容以被修改返回值為非零值。 \n 若編輯框內容沒有變動返回值為零。
 */
BOOL DmEditbox::GetModify() const
{
	assert(this->IsWindow());
	// EM_GETMODIFY
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return ::SendMessage(*this, EM_GETMODIFY, 0, 0) != 0;
}

/**
 *	@brief 取得密碼樣式編輯框的用於顯示的字元
 *	@return <b>型別: TCHAR</b> 返回值為顯示密碼用字元。
 */
TCHAR DmEditbox::GetPasswordChar() const
{
	assert(this->IsWindow());
	// EM_GETPASSWORDCHAR
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<TCHAR>(::SendMessage(*this, EM_GETPASSWORDCHAR, 0, 0));
}

/**
 *	@brief 取得編輯框矩形邊界尺寸
 *	@param[out] rcPtr 指向 RECT 結構緩衝區的位址
 *	@return 此函數沒有返回值。
 */
void DmEditbox::GetRect(LPRECT rcPtr) const
{
	assert(this->IsWindow());
	// EM_GETRECT
	// wParam = 未使用、必須為零。
	// lParam = 指向 RECT 結構緩衝區的位址
	::SendMessage(*this, EM_GETRECT, 0, reinterpret_cast<LPARAM>(rcPtr));
}

/**
 *	@brief 取得文字被選取的範圍資訊
 *	@param[out] firstPtr	指向 DWORD 型別緩衝區位址，被選取的第一個字元，可以傳入 NULL, 不擷取
 *	@param[out] lastPtr		指向 DWORD 型別緩衝區位址，被選取後的第一個字元，可以傳入 NULL, 不擷取
 *	@return 此函數沒有返回值
 *	@remark 額外說明 \n firstPtr 保存被選取的第一個字元位置 \n lastPtr 保存被選取範圍外的第一個字元位置
 */
void DmEditbox::GetSel(DWORD* firstPtr, DWORD* lastPtr) const
{
	assert(this->IsWindow());
	// EM_GETSEL
	// wParam = 存放被選取的第一個字元位置
	// lParam = 存放被選後的第一個字元位置
	::SendMessage(*this, EM_GETSEL, reinterpret_cast<WPARAM>(firstPtr), reinterpret_cast<LPARAM>(lastPtr));
}

/**
 *	@brief 取得文字被選取的範圍資訊
 *	@return <b>型別: DWORD</b>
 *		\n 返回值 HIWORD 部分為被選取的第一個字元位置
 *		\n 返回值 LOWORD 部分為被選取範圍後的第一個字元位置
 */
DWORD DmEditbox::GetSel() const
{
	assert(this->IsWindow());
	// EM_GETSEL
	// wParam = 存放被選取的第一個字元位置
	// lParam = 存放被選後的第一個字元位置
	return static_cast<DWORD>(::SendMessage(*this, EM_GETSEL, 0, 0));
}

/**
 *	@brief 取得多重編輯框(Rich Edit)垂直卷軸的滑塊位置
 *	@return <b>型別: int</b> \n 返回值為垂直卷軸滑塊位置
 */
int DmEditbox::GetThumb() const
{
	assert(this->IsWindow());
	// EM_GETTHUMB
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<int>(::SendMessage(*this, EM_GETTHUMB, 0, 0));
}

/**
 *	@brief 設定編輯框文字輸入限制
 *	@param[in] nChars 設定最大限制字數，以 TCHAR 為單位。指定長度不含 NULL 結尾。 \n 若此參數為 0 代表使用系統預設最大值。
 *	@return 此函數沒有返回值
 */
void DmEditbox::LimitText(int nChars) const
{
	assert(this->IsWindow());
	// EM_LIMITTEXT
	// wParam = 設定最大限制字數
	// lParam = 不使用，必須為零
	::SendMessage(*this, EM_LIMITTEXT, static_cast<WPARAM>(nChars), 0);
}

/**
 *	@brief 多重編輯框，取得指定字元所在行號
 *	@param[in] nCharIndex 字元的索引值(從 0 算起)，若此值為 (-1) 代表取得當前行號的字元位置 (當前行 = 目前游標的位置)。
 *	@return <b>型別: int</b> \n 返回值為 nCharIndex 指定的字元位於行數中的位置 (從 0 算起)。
 */
int DmEditbox::LineFromChar(int nCharIndex) const
{
	assert(this->IsWindow());
	// EM_LINEFROMCHAR
	// wParam = 字元的索引值
	// wParam = 不使用、必須為零
	return static_cast<int>(::SendMessage(*this, EM_LINEFROMCHAR, static_cast<WPARAM>(nCharIndex), 0));
}

/**
 *	@brief 多重編輯框，取得指定行號的第一個字元的行號索引值
 *	@param[in] nLine 指定行號的字元索引值，若此值為 (-1) 代表使用當前行數位置 (當前行號 = 游標所在位置)
 *	@return <b>型別: int</b> \n 返回值為字元所在行數的索引值(從 0 算起)。
 */
int DmEditbox::LineIndex(int nLine) const
{
	assert(this->IsWindow());
	// EM_LINEINDEX
	// wParam = 指定行號
	// lParam = 不使用，必須為零
	return static_cast<int>(::SendMessage(*this, EM_LINEINDEX, static_cast<WPARAM>(nLine), 0));
}

/**
 *	@brief 多重編輯框，取得指定行號的資料長度
 *	@param[in] nLine 指定行號的字元索引值，若此值為 (-1) 代表使用當前行數位置 (當前行號 = 游標所在位置)
 *	@return <b>型別: int</b>
 *		\n 多重編輯框，返回值為指定行號的資料長度，單位 TCHAR
 *		\n 單行編輯框，返回值為編輯框的資料長度，單位 TCHAR
 */
int DmEditbox::LineLength(int nLine) const
{
	assert(this->IsWindow());
	// EM_LINELENGTH
	// wParam = 指定行號的字元索引值
	// lParam = 不使用，必須為零
	return static_cast<int>(::SendMessage(*this, EM_LINELENGTH, static_cast<WPARAM>(nLine), 0));
}

/**
 *	@brief 多重編輯框，進行垂直卷軸與與水平卷軸捲動。單行編輯框進行水平捲動。
 *	@param[in] nLines 指定要垂直捲動行的數。
 *	@param[in] nChars 指定要水平捲動字元位置的數目。 (多重編輯框 模式下必須為零)
 *	@return 此函數沒有返回值
 */
void DmEditbox::LineScroll(int nLines, int nChars) const
{
	assert(this->IsWindow());
	// EM_LINESCROLL
	// wParam = 指定要水平捲動字元位置的數目，(RichEdit 模式下不使用，必須為零)
	// lParam = 指定要垂直捲動行的數
	::SendMessage(*this, EM_LINESCROLL, static_cast<WPARAM>(nChars), static_cast<LPARAM>(nLines));
}

/**
 *	@brief 多重編輯框，取得指定的字元在這個位置（左上角）
 *	@param[in] nChar 指定的字元索引值，從 0 算起)
 *	@return <b>型別: DWORD</b> \n 返回值為所指定之字元的左上角的座標
 *		- 返回值 LOWORD 為水平座標
 *		- 返回值 HIWORD 為垂直座標
 */
DWORD DmEditbox::PosFromChar(UINT nChar) const
{
	assert(this->IsWindow());
	// EM_POSFROMCHAR
	// wParam = 字元索引值
	// lParam = 不使用，必須為零
	return static_cast<DWORD>(::SendMessage(*this, EM_LINESCROLL, static_cast<WPARAM>(nChar), 0));
}

/**
 *	@brief 以新指定的文字取代目前編輯框中被選取的範圍。
 *	@param[in] szNewTextPtr	指向文字字串緩衝區位址的指標，值為要取代的新文字字串。
 *	@param[in] bCanUndo		指定是具備撤銷動作。
 *	@return 此函數沒有返回值。
 */
void DmEditbox::ReplaceSel(const TCHAR* szNewTextPtr, BOOL bCanUndo) const
{
	assert(this->IsWindow());
	// EM_REPLACESEL
	// wParam = 指定是具備撤銷動作
	// lParam = 指向文字字串緩衝區位址
	::SendMessage(*this, EM_REPLACESEL, static_cast<WPARAM>(bCanUndo), reinterpret_cast<LPARAM>(szNewTextPtr));
}

/**
 *	@brief 設定提示文字
 *	@param[in] textPtr	指向文字字串緩衝區位址的指標，以 NULL 做結尾的文字字串，內含提示用文字字串。
 *	@param[in] fDrawWhenFocused	自動停止顯示提示
 *		- 若參數為 TRUE，當使用者開始在編輯框中輸入時，就會消失提示。
 *		- 若參數為 FALSE，當編輯框成為輸入焦點時，不會立即消失提示。
 *	@return	<b>型別: BOOL</b> \n 若設定成功返回值為非零值，若定失敗責返回值為零。
 */
BOOL DmEditbox::SetCueBanner(const TCHAR* textPtr, BOOL fDrawWhenFocused) const
{
	assert(this->IsWindow());
	// EM_SETCUEBANNER
	// wParam = 自動停止顯示提示
	// lParam = 指向文字字串緩衝區位址
	return ::SendMessage(*this, EM_SETCUEBANNER, static_cast<WPARAM>(fDrawWhenFocused), reinterpret_cast<LPARAM>(textPtr)) != 0;
}

/**
 *	@brief 多重編輯，設定指定使用緩衝區的操作代碼
 *	@param[in] hBuffer 由 LocalAlloc 函數所建立的緩衝區操作代碼。
 *	@return 此函數沒有返回值
 */
void DmEditbox::SetHandle(HLOCAL hBuffer) const
{
	assert(this->IsWindow());
	// EM_SETHANDLE
	// wParam = 記憶體操作代碼
	// lParam = 不使用，必須為零
	::SendMessage(*this, EM_SETHANDLE, reinterpret_cast<WPARAM>(hBuffer), 0);
}

/**
 *	@brief 設定編輯框字數限制, 單位 TCHAR
 *	@param[in] ccMax 字數限制 單位 TCHAR
 *	@return 此函數沒有返回值
 *	@remark 在使用 EM_SETLIMITTEXT 之前，使用者可以在編輯控件中輸入的文本量的默認限制為32,767個字符(TCHAR)。
 */
void DmEditbox::SetLimitText(int ccMax) const
{
	assert(this->IsWindow());
	// EM_SETLIMITTEXT
	// wParam = 字數限制
	// lParam = 未使用、必須為零
	::SendMessage(*this, EM_SETLIMITTEXT, static_cast<WPARAM>(ccMax), 0);
}

/**
 *	@brief 設定此編輯控制項的左右邊界。
 *	@param[in] uLeft	左邊界
 *	@param[in] uRight	右邊界
 *	@return 此函數沒有返回值。
 */
void DmEditbox::SetMargins(UINT uLeft, UINT uRight) const
{
	assert(this->IsWindow());
	// EM_SETMARGINS
	// wParam = 指定設定方式
	// lParam = 左右邊界值 LOWORD = 左邊界，HIWORD = 右邊界。
	::SendMessage(*this, EM_SETMARGINS, static_cast<WPARAM>(EC_LEFTMARGIN | EC_RIGHTMARGIN), static_cast<LPARAM>(MAKELONG(uLeft, uRight)));
}

/**
 *	@brief 設定或清除編輯框的已修改旗標。
 *	@param[in] bModified 值為(TRUE)表示文字已修改，而值為(FALSE)指出它是未經修改。根據預設，會設定已修改的旗標。
 *	@return 此函數沒有返回值
 */
void DmEditbox::SetModify(BOOL bModified) const
{
	assert(this->IsWindow());
	// EM_SETMODIFY
	// wParam = 新的旗標值
	// lParam = 不使用，必須為零
	::SendMessage(*this, EM_SETMODIFY, static_cast<WPARAM>(bModified), 0);
}

/**
 *	@brief 設定密碼樣式編輯框要用於顯示的字元
 *	@param[in] ch 字元碼，要用於顯示密碼的代替字元。如果此參數為零，則編輯框會刪除當前密碼字元並顯示使用者輸入入的字元。
 *	@return 此函數沒有返回值
 */
void DmEditbox::SetPasswordChar(TCHAR ch)
{
	assert(this->IsWindow());
	// EM_SETPASSWORDCHAR
	// wParam = 字元碼
	// lParam = 不使用，必須為零
	::SendMessage(*this, EM_SETPASSWORDCHAR, static_cast<WPARAM>(ch), 0);
}

/**
 *	@brief 設定編輯框僅供讀取，不接受輸入，不影響程式內調用 WM_SETTEXT
 *	@param[in] bEnable 啟用或停用唯讀模式僅供讀取 (TRUE or FALSE)
 *	@return <b>型別: BOOL</b> \n	若函數操作成功返回值為非零值，\n 若函數操作失敗則返回值為零
 */
BOOL DmEditbox::SetReadonly(BOOL bEnable) const
{
	assert(this->IsWindow());
	// EM_SETREADONLY
	// wParam = 啟用或停用
	// lParam = 未使用、必須為零
	return ::SendMessage(*this, EM_SETREADONLY, static_cast<WPARAM>(bEnable), 0) != 0;
}

/**
 *	@brief 設定編輯框控制項多行格式的矩形邊界尺寸，此格式矩形僅限制編輯框的文字繪製範圍，與控制項的視窗尺寸大小無關。
 *	@param[in] rcPtr	指向 RECT 結構緩衝區的位址, 定義編輯框文字繪製區的尺寸。
 *	@param[in] nOffset	指定 lParam 是絕對做標還是相對座標，0 代表使用絕對做標，1 代表相對於當前格式矩形的偏移量 (偏移量可以是正數或負數)
 *	@return 此函數沒有返回值
 */
void DmEditbox::SetRect(RECT* rcPtr, int nOffset) const
{
	assert(this->IsWindow());
	// EM_SETRECT
	// wParam = 指定 lParam 是絕對做標還是相對座標
	// lParam = 定義編輯框文字繪製區的尺寸。
	::SendMessage(*this, EM_SETRECT, static_cast<WPARAM>(nOffset), reinterpret_cast<LPARAM>(rcPtr));
}

/**
 *	@brief 設定多行樣式的編輯框矩形格式。
 *	@param[in] rcPtr 指向 RECT 結構緩衝區位址的指標，內容為新的矩形格式維度資訊。
 *	@return 此函數沒有返回值
 */
void DmEditbox::SetRectNP(const RECT* rcPtr)
{
	assert(this->IsWindow());
	// EM_SETRECT
	// wParam = 指定使用絕對做標或者是相對座標，若為編輯框則必須設為零。
	// lParam = 定指向 RECT 結構緩衝區位址。
	::SendMessage(*this, EM_SETRECT, 0, reinterpret_cast<LPARAM>(rcPtr));
}

/**
 * @brief 設定文字選取範圍
 * @param[in] startPos	開始位置
 * @param[in] endPos	結束位置
 * @return 此函數沒有返回值
 */
void DmEditbox::SetSelect(DWORD startPos, DWORD endPos) const
{
	assert(this->IsWindow());
	// EM_SETSEL
	// wParam = 開始位置
	// lParam = 結束位置
	::SendMessage(*this, EM_SETSEL, static_cast<DWORD>(startPos), static_cast<DWORD>(endPos));
}

/**
 *	@brief 多重編輯框，設定 Tab 停駐點
 *	@return <b>型別: BOOL</b> \n 若帆樹操作成功返回值為非零值。 \n 若函數操作失敗則返回值為零。
 */
BOOL DmEditbox::SetTabStops() const
{
	assert(this->IsWindow());
	// EM_SETTABSTOPS
	// wParam = 0
	// lParam = 0
	return ::SendMessage(*this, EM_SETTABSTOPS, 0, 0) != 0;
}

/**
 *	@brief 多重編輯框，設定 Tab 停駐點
 *	@param[in] cxEachStop int 型別的參考，指定停駐點的資訊。
 *	@return <b>型別: BOOL</b> \n 若函數操作成功返回值為非零值。 \n 若函數操作失敗則返回值為零。
 */
BOOL DmEditbox::SetTabStops(const int& cxEachStop) const
{
	assert(this->IsWindow());
	// EM_SETTABSTOPS
	// wParam = 若為 0，表示忽略 lParam 指定的參數。
	// lParam = 指向 int 配置緩衝區位址
	return ::SendMessage(*this, EM_SETTABSTOPS, static_cast<WPARAM>(TRUE), reinterpret_cast<WPARAM>(&cxEachStop)) != 0;
}

/**
 *	@brief 多重編輯框，設定 Tab 停駐點
 *	@param[in] nTabStops			定位停駐點(nTabStopsPtr)的數目，這個數字必須大於 1。
 *	@param[in] nTabStopsPtrArray	對話方塊單位停止指向指定的索引標籤上的不帶正負號整數的陣列。
 *	@return <b>型別: BOOL</b> \n 若函數操作成功返回值為非零值。 \n 若函數操作失敗則返回值為零。
 *	@see https://docs.microsoft.com/en-us/windows/desktop/controls/em-settabstops
 */
BOOL DmEditbox::SetTabStops(int nTabStops, INT* nTabStopsPtrArray) const
{
	assert(this->IsWindow());
	// EM_SETTABSTOPS
	// wParam
	// lParam
	return ::SendMessage(*this, EM_SETTABSTOPS, static_cast<WPARAM>(nTabStops), reinterpret_cast<LPARAM>(nTabStopsPtrArray)) != 0;
}

/**
 *	@brief 清除、刪除編輯框(EditBox)內容
 *	@return 此函數沒有返回值
 */
void DmEditbox::Clear() const
{
	assert(this->IsWindow());
	// WM_CLEAR
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	::SendMessage(*this, WM_CLEAR, 0, 0);
}

/**
 *	@brief 複製編輯框(EditBox)內容，以 CF_TEXT 格式，將編輯框(EditBox)內容複製到剪貼板。
 *	@return 此函數沒有返回值
 */
void DmEditbox::Cpoy() const
{
	assert(this->IsWindow());
	// WM_COPY
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	::SendMessage(*this, WM_COPY, 0, 0);
}

/**
 *	@brief 剪下編輯框(EditBox)內容，並以 CF_TEXT 格式，將當前編輯框(EditBox)內容複製到剪貼板。
 *	@return 此函數沒有返回值
 */
void DmEditbox::Cut() const
{
	assert(this->IsWindow());
	// WM_CUT
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	::SendMessage(*this, WM_CUT, 0, 0);
}

/**
 *	@brief 將剪貼板的內容貼到編輯框(EditBox)裡，剪貼板要有 CF_TEXT 格式所保存的內容，才會進行貼上。
 *	return 此函數沒有返回值
 */
void DmEditbox::Paste() const
{
	assert(this->IsWindow());
	// WM_PASTE
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	::SendMessage(*this, WM_PASTE, 0, 0);
}

/**
 * @brief 恢復編輯框最後一個編輯操作
 * @return <b>型別: BOOL</b>
 *		\n 對於單行編輯編輯框，返回值始終為 TRUE。
 *		\n 對於多行編輯編輯框，如果撤回操作成功，則返回值為 TRUE, 如果取消操作失敗，則返回值為 FALSE。
 */
BOOL DmEditbox::Undo() const
{
	assert(this->IsWindow());
	// EM_UNDO
	// wParam = 未使用，必須為零
	// lParam = 未使用，必須為零
	return ::SendMessage(*this, EM_UNDO, 0, 0) != 0;
}
