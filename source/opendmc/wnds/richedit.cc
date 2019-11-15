/**************************************************************************//**
 * @file	richedit.cc
 * @brief	DmRichEdit 多功能文字編輯類別成員函數
 * @date	2019-09-17
 * @date	2019-09-17
 * @author	Swang
 *****************************************************************************/
#include "opendmc/wnds/richedit.hh"

/**
 *	@brief	DmRichEdit 建構式
 *	@return	此函數沒有返回值
 *	@remark	建構式向父類別指定要建立的控制項種類。
 */
DmRichEdit::DmRichEdit() : DmWnds(EmCtrls::RichEdit) { }

/**
 *	@brief	DmRichEdit 解構式
 *	@return	此函數沒有返回值
 *	@remark	解構時釋放(銷毀)控制項資源。
 */
DmRichEdit::~DmRichEdit()
{
	if (this->IsWindow()) {
		this->RequestToDestroy(0);
	}
}

/**
 *	@brief	Enables or disables automatic detection of URLs by a rich edit control.
 *		\n 啟用或停用超連結(URL)自動偵測
 *	@param[in]	bType	啟用或停用種類代碼
 *	@param[in]	szPtr	指定 scheme 名稱, 若不指定 scheme 值為 NULL, 會自動使用預設 scheme
 *	@return	<b>型別: BOOL</b>
 *		\n 若設定成功，返回值為非零值 (TRUE)
 *		\n 若設定失敗，返回值為零 (FALSE)，使用 GetLastError 取得錯誤資訊。
 *	@remark	<b>bType 參數說明</b>
 * 		- AURL_DISABLEMIXEDLGC		\n Windows 8: Disable recognition of domain names that contain labels with characters belonging to more than one of the following scripts: Latin, Greek, and Cyrillic.
 *		- AURL_ENABLEDRIVELETTERS	\n Windows 8: Recognize file names that have a leading drive specification, such as c:\temp.
 *		- AURL_ENABLEEA				\n This value is deprecated; use AURL_ENABLEEAURLS instead.
 *		- AURL_ENABLEEAURLS			\n Recognize URLs that contain East Asian characters.
 *		- AURL_ENABLEEMAILADDR		\n Windows 8: Recognize email addresses.
 *		- AURL_ENABLETELNO			\n Windows 8: Recognize telephone numbers.
 *		- AURL_ENABLEURL			\n Windows 8: Recognize URLs that include the path.
 */
BOOL DmRichEdit::AutoUrlDetect(int nType, TCHAR* szPtr) const
{
	assert(this->IsWindow());
	// EM_AUTOURLDETECT
	// wParam = 未使用，必須為零
	// lParam = 未使用，必須為零

	auto wParam = static_cast<WPARAM>(nType);
	auto lParam = reinterpret_cast<LPARAM>(szPtr);
	auto lResult = ::SendMessage(*this, EM_AUTOURLDETECT, wParam, lParam);

	// 設定錯誤資訊
	if (lResult != 0) {
		::SetLastError(static_cast<DWORD>(lResult));
	}
	return lResult == 0;
}

/**
 *	@brief	Determines whether a rich edit control can paste a specified clipboard format.
 *	@param[in]	nFormat	指定要使用剪貼的格式, 若使用當前模式填入 0
 *	@return	<b>型別: BOOL</b>
 *		\n If the clipboard format can be pasted, the return value is a nonzero value.
 *		\n If the clipboard format cannot be pasted, the return value is zero.
 *	@remark	<b>nFormat 指定格式說明</b>
 *		<pre>
 *		Clipboard Format	Conversion Format
 *		CF_BITMAP			CF_DIB
 *		CF_BITMAP			CF_DIBV5
 *		CF_DIB				CF_BITMAP
 *		CF_DIB				CF_PALETTE
 *		CF_DIB				CF_DIBV5
 *		CF_DIBV5			CF_BITMAP
 *		CF_DIBV5			CF_DIB
 *		CF_DIBV5			CF_PALETTE
 *		CF_ENHMETAFILE		CF_METAFILEPICT
 *		CF_METAFILEPICT		CF_ENHMETAFILE
 *		CF_OEMTEXT			CF_TEXT
 *		CF_OEMTEXT			CF_UNICODETEXT
 *		CF_TEXT				CF_OEMTEXT
 *		CF_TEXT				CF_UNICODETEXT
 *		CF_UNICODETEXT		CF_OEMTEXT
 *		CF_UNICODETEXT		CF_TEXT
 *		</pre>
 *	@see https://docs.microsoft.com/zh-tw/windows/win32/dataxchg/clipboard-formats#standard-clipboard-formats
 */
BOOL DmRichEdit::CanPasre(int nFormat) const
{
	assert(this->IsWindow());
	// EM_CANPASTE
	// wParam = 指定要使用的 Clipboard 種類
	// lParam = 未使用，必須為零
	return static_cast<BOOL>(::SendMessage(*this, EM_CANPASTE, static_cast<WPARAM>(nFormat), static_cast<LPARAM>(0)));
}

/**
 *	@brief	Determines whether there are any actions in the control redo queue.
 *	@return	<b>型別: BOOL</b>
 *		\n If there are actions in the control redo queue, the return value is a nonzero value.
 *		\n If the redo queue is empty, the return value is zero.
 *	@remark	\n To redo the most recent undo operation, send the EM_REDO message.
 */
BOOL DmRichEdit::CanRedo() const
{
	assert(this->IsWindow());
	// EM_CANREDO
	// wParam = 未使用，必須為零
	// lParam = 未使用，必須為零
	return static_cast<BOOL>(::SendMessage(*this, EM_CANREDO, static_cast<WPARAM>(0), static_cast<LPARAM>(0)));
}

/**
 *	@brief	This message is not implemented.
 *	@return	此函數沒有返回值
 */
void DmRichEdit::ConvPosition() const
{
	return;
}

/**
 *	@brief	Displays a portion of the contents of a rich edit control, as previously formatted for a device using the EM_FORMATRANGE message.
 *	@param[in]	rcPtr	A RECT structure specifying the display area of the device.
 *	@return	<b>型別: BOOL</b>
 *		\n If the operation succeeds, the return value is TRUE.
 *		\n If the operation fails, the return value is FALSE.
 *	@remark	<b>額外說明</b>
 *		\n 此矩形結構是要顯示區域和通常是 FORMATRANGE 結構 EM_FORMATRANGE 訊息中使用的 RECT 的內容可以不一樣。
 *		\n 例如矩形不相同，如果採用頁面的某些部分或正在使用內置的邊距。應先使用 EM_FORMATRANGE 訊息設定後再使用此訊息。
 */
BOOL DmRichEdit::DisplayBand(RECT* rcPtr) const
{
	assert(this->IsWindow());
	// EM_DISPLAYBAND
	// wParam = 未使用，必須為零
	// lParam = 未使用，必須為零
	return static_cast<BOOL>(::SendMessage(*this, EM_DISPLAYBAND, static_cast<WPARAM>(0), reinterpret_cast<LPARAM>(rcPtr)));
}

/**
 *	@brief	Retrieves the starting and ending character positions of the selection in a rich edit control.
 *		\n 取得選擇區塊，開始字元位置與結束字元位置。
 *	@param[out]	crPtr 指定存放(選擇區資訊) CHARRANGE 結構緩衝區位址。
 *	@return	<b>此函數沒有返回值</b>
 */
void DmRichEdit::ExGetSelection(CHARRANGE* crPtr) const
{
	assert(this->IsWindow());
	// EM_EXGETSEL
	// wParam = 未使用，必須為零
	// lParam = 指定存放(選擇區資訊) CHARRANGE 結構緩衝區位址。
	::SendMessage(*this, EM_EXGETSEL, static_cast<WPARAM>(0), reinterpret_cast<LPARAM>(crPtr));
}

/**
 *	@brief	Sets an upper limit to the amount of text the user can type or paste into a rich edit control.
 *	@param[in] nMaximum	設定可容納自元的上限值 (若填入 0, 則使用系統預設值 64K 個字元)
 *	@return	<b>此函數沒有返回值</b>
 */
void DmRichEdit::ExLimitText(int nMaximum) const
{
	assert(this->IsWindow());
	// EM_EXLIMITTEXT
	// wParam = 未使用，必須為零
	// lParam = 指定存放(選擇區資訊) CHARRANGE 結構緩衝區位址。
	::SendMessage(*this, EM_EXLIMITTEXT, static_cast<WPARAM>(0), static_cast<LPARAM>(nMaximum));
}

/**
 *	@brief	Determines which line contains the specified character in a rich edit control.
 *	@param[in] nCharIndex 字元的位置索引碼 (zero-base)
 *	@return	<b>型別: int</b> \n zero-based index of the line.
 */
int DmRichEdit::ExLineFromChar(int nCharIndex) const
{
	assert(this->IsWindow());
	// EM_EXLINEFROMCHAR
	// wParam = 未使用，必須為零
	// lParam = 指定存放(選擇區資訊) CHARRANGE 結構緩衝區位址。
	return static_cast<int>(::SendMessage(*this, EM_EXLINEFROMCHAR, static_cast<WPARAM>(0), static_cast<LPARAM>(nCharIndex)));
}

/**
 *	@brief	Selects a range of characters or Component Object Model (COM) objects in a Microsoft Rich Edit control.
 *	@param[int] crPtr 指向 CHARRANGE 結構緩衝區，內涵指定範圍數據。
 *	@return	<b>型別: int</b> \n 返回值為實際選擇字元數。
 */
int DmRichEdit::ExSetSelection(CHARRANGE* crPtr) const
{
	assert(this->IsWindow());
	// EM_EXSETSEL
	// wParam = 未使用，必須為零
	// lParam = 指向 CHARRANGE 結構緩衝區，內涵指定範圍數據。
	return static_cast<int>(::SendMessage(*this, EM_EXSETSEL, static_cast<WPARAM>(0), reinterpret_cast<LPARAM>(crPtr)));
}

/**
 *	@brief	Finds text within a rich edit control.
 *	@param[in]	nMode	Specify the parameters of the search operation. This parameter can be one or more of the following values.
 *	@param[in]	textPtr	指向 FINDTEXT 結構緩衝區位址，填寫要找尋的文字內容。
 *	@return	<b>型別: int</b>
 *		\n 若搜尋成功返回值為 (-1) 以外的值，返回值為搜尋到文字第一個字元索引碼(zero-base)。
 *		\n 若搜尋失敗返回值為 (-1)
 *	@remark	<b>nMode 參數額外說明</b>
 *		- FR_DOWN			\n Microsoft Rich Edit 2.0 and later: If set, the search is from the end of the current selection to the end of the document. If not set, the search is from the end of the current selection to the beginning of the document.
 *							\n Microsoft Rich Edit 1.0: The FR_DOWN flag is ignored. The search is always from the end of the current selection to the end of the document.
 *		- FR_MATCHALEFHAMZA	\n Microsoft Rich Edit 3.0 and later: If set, the search differentiates between Arabic alefs with different accents. If not set, all alefs are matched by the alef character alone.
 *		- FR_MATCHDIAC		\n Microsoft Rich Edit 3.0 and later: If set, the search operation considers Arabic and Hebrew diacritical marks. If not set, diacritical marks are ignored.
 *		- FR_MATCHKASHIDA	\n Microsoft Rich Edit 3.0 and later: If set, the search operation considers Arabic kashidas. If not set, kashidas are ignored.
 *		- FR_MATCHWIDTH		\n Windows 8: If set, single-byte and double-byte versions of the same character are considered to be not equal.
 *		- FR_WHOLEWORD		\n If set, the operation searches only for whole words that match the search string. If not set, the operation also searches for word fragments that match the search string.
 */
int DmRichEdit::FindText(int nMode, FINDTEXT* textPtr) const
{
	assert(this->IsWindow());
	// EM_FINDTEXT
	// wParam = 未使用，必須為零
	// lParam = 指向 FINDTEXT 結構緩衝區，填寫要找尋的文字內容。
	return static_cast<int>(::SendMessage(*this, EM_FINDTEXT, static_cast<WPARAM>(nMode), reinterpret_cast<LPARAM>(textPtr)));
}

/**
 *	@brief	Finds text within a rich edit control.
 *	@param[in]	nMode	Specify the parameters of the search operation. This parameter can be one or more of the following values.
 *	@param[in]	textPtr	指向 FINDTEXTEX 結構緩衝區位址，填寫要找尋的文字內容。
 *	@return	<b>型別: int</b>
 *		\n 若搜尋成功返回值為 (-1) 以外的值，返回值為搜尋到文字第一個字元索引碼(zero-base)。
 *		\n 若搜尋失敗返回值為 (-1)
 *	@see DmRichEdit::FindText()
 */
int DmRichEdit::FindTextEx(int nMode, FINDTEXTEX* textPtr) const
{
	assert(this->IsWindow());
	// EM_FINDTEXTEX
	// wParam = 未使用，必須為零
	// lParam = 指向 FINDTEXT 結構緩衝區，填寫要找尋的文字內容。
	return static_cast<int>(::SendMessage(*this, EM_FINDTEXTEX, static_cast<WPARAM>(nMode), reinterpret_cast<LPARAM>(textPtr)));
}

/**
 *	@brief	Finds Unicode text within a rich edit control.
 *	@param[in]	nMode	Specify the parameters of the search operation. This parameter can be one or more of the following values.
 *	@param[in]	textPtr	指向 FINDTEXTEXW 結構緩衝區位址，填寫要找尋的文字內容。
 *	@return	<b>型別: int</b>
 *		\n 若搜尋成功返回值為 (-1) 以外的值，返回值為搜尋到文字第一個字元索引碼(zero-base)。
 *		\n 若搜尋失敗返回值為 (-1)
 *	@see DmRichEdit::FindText()
 */
int DmRichEdit::FindTextExW(int nMode, FINDTEXTEXW* textPtr) const
{
	assert(this->IsWindow());
	// EM_FINDTEXTEXW
	// wParam = 未使用，必須為零
	// lParam = 指向 FINDTEXTEXW 結構緩衝區，填寫要找尋的文字內容。
	return static_cast<int>(::SendMessage(*this, EM_FINDTEXTEXW, static_cast<WPARAM>(nMode), reinterpret_cast<LPARAM>(textPtr)));
}

/**
 *	@brief	Finds Unicode text within a rich edit control.
 *	@param[in]	nMode	Specify the parameters of the search operation. This parameter can be one or more of the following values.
 *	@param[in]	textPtr	指向 FINDTEXTW 結構緩衝區位址，填寫要找尋的文字內容。
 *	@return	<b>型別: int</b>
 *		\n 若搜尋成功返回值為 (-1) 以外的值，返回值為搜尋到文字第一個字元索引碼(zero-base)。
 *		\n 若搜尋失敗返回值為 (-1)
 *	@see DmRichEdit::FindText()
 */
int DmRichEdit::FindTextW(int nMode, FINDTEXTW* textPtr) const
{
	assert(this->IsWindow());
	// EM_FINDTEXTW
	// wParam = 未使用，必須為零
	// lParam = 指向 FINDTEXTW 結構緩衝區，填寫要找尋的文字內容。
	return static_cast<int>(::SendMessage(*this, EM_FINDTEXTW, static_cast<WPARAM>(nMode), reinterpret_cast<LPARAM>(textPtr)));
}
