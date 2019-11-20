/**************************************************************************//**
 * @file	editbox.hh
 * @brief	DmEditbox 編輯框類別宣告 Header
 * @date	2000-10-10
 * @date	2018-09-02
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_WNDS_EDITBOX_HH
#define ODMC_WNDS_EDITBOX_HH
#include "wnds.hh"

/**
 *	@class DmEditbox
 *	@brief 編輯框類別，文字編輯框。
 */
class DmEditbox : public DmWnds
{
public:
	DmEditbox();
	virtual ~DmEditbox();

	virtual BOOL Create(const TCHAR* szCaptionPtr, int x, int y, int wd, int ht, HWND hWndParent, int nIDCItem) override;
	virtual BOOL CreateSample();

	// Operations
	void	AppendText(const TCHAR* textPtr) const;									// ###
	BOOL	CanUndo() const;														// EM_CANUNDO
	int		CharFromPos(POINT* pointPtr) const;										// EM_CHARFROMPOS

	void	EmptyUndoBuffer() const;												// EM_EMPTYUNDOBUFFER
	BOOL	FmtLines(BOOL bAddEOL) const;											// EM_FMTLINES

	BOOL	GetCueBanner(TCHAR* textPtr, int cchText) const;						// EM_GETCUEBANNER
	int		GetFirstVisibleLine() const;											// EM_GETFIRSTVISIBLELINE
	HANDLE	GetHandle() const;														// EM_GETHANDLE
	BOOL	GetHighlight(int* pichStart, int* pichEnd) const;						// EM_GETHILITE
	DWORD	GetItemStatus() const;													// EM_GETIMESTATUS
	int		GetLimitText() const;													// EM_GETLIMITTEXT
	int		GetLine(int nLine, TCHAR* textPtr) const;								// EM_GETLINE
	int		GetLine(int nLine, TCHAR* textPtr, int maxLength) const;				// ## EM_GETLINE
	int		GetLineCount() const;													// EM_GETLINECOUNT
	DWORD	GetMargins() const;														// EM_GETMARGINS
	BOOL	GetModify() const;														// EM_GETMODIFY
	TCHAR	GetPasswordChar() const;												// EM_GETPASSWORDCHAR
	void	GetRect(LPRECT rcPtr) const;											// EM_GETRECT
	void	GetSel(DWORD* firstPtr, DWORD* lastPtr) const;							// EM_GETSEL
	DWORD	GetSel() const;
	int		GetThumb() const;														// EM_GETTHUMB

	void	LimitText(int nChars = 0) const;										// EM_LIMITTEXT
	int		LineFromChar(int nCharIndex = -1) const;								// EM_LINEFROMCHAR
	int		LineIndex(int nLine = -1) const;										// EM_LINEINDEX
	int		LineLength(int nLine = -1) const;										// EM_LINELENGTH
	void	LineScroll(int nLines, int nChars = 0) const;							// EM_LINESCROLL
	DWORD	PosFromChar(UINT nChar) const;											// EM_POSFROMCHAR

	void	ReplaceSel(const TCHAR* szNewTextPtr, BOOL bCanUndo = FALSE) const;		// EM_REPLACESEL
	BOOL	SetCueBanner(const TCHAR* textPtr, BOOL fDrawWhenFocused = FALSE) const;// EM_SETCUEBANNER
	void	SetHandle(HLOCAL hBuffer) const;										// EM_SETHANDLE
	void	SetLimitText(int ccMax) const;											// EM_SETLIMITTEXT
	void	SetMargins(UINT nLeft, UINT nRight) const;								// EM_SETMARGINS
	void	SetModify(BOOL bModified = TRUE) const;									// EM_SETMODIFY
	void	SetPasswordChar(TCHAR ch);												// EM_SETPASSWORDCHAR
	BOOL	SetReadonly(BOOL bEnable) const;										// EM_SETREADONLY
	void	SetRect(RECT* rcPtr, int nOffset = 0) const;							// EM_SETRECT
	void	SetRectNP(const RECT* rcPtr);											// EM_SETRECTNP
	void	SetSelect(DWORD startPos, DWORD endPos) const;							// EM_SETSEL
	BOOL	SetTabStops() const;													// EM_SETTABSTOPS
	BOOL	SetTabStops(const int& cxEachStop) const;								// ## EM_SETTABSTOPS
	BOOL	SetTabStops(int nTabStops, INT* nTabStopsPtrArray) const;				// ## EM_SETTABSTOPS

	// Clipboard Operations
	void	Clear() const;								// WM_CLEAR
	void	Cpoy() const;								// WM_COPY
	void	Cut() const;								// WM_CUT
	void	Paste() const;								// WM_PASTE
	BOOL	Undo() const;								// EM_UNDO, WM_UNDO

private:
	DmEditbox(const DmEditbox&) = delete;				//!< Disable copy construction
	DmEditbox& operator=(const DmEditbox&) = delete;	//!< Disable assignment operator
};

#endif // !ODMC_WNDS_EDITBOX_HH
