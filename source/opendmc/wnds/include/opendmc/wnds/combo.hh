/**************************************************************************//**
 * @file	combo.hh
 * @brief	DmCombo 組合框類別宣告 Header
 * @date	2000-10-10
 * @date	2018-09-08
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_WNDS_COMBO_HH
#define	ODMC_WNDS_COMBO_HH
#include "wnds.hh"

/**
 *	@class DmCombo
 *	@brief 組合式框類別
 */
class DmCombo : public DmWnds
{
public:
	DmCombo();
	virtual ~DmCombo();

	virtual BOOL Create(const TCHAR* szCaptionPtr, int x, int y, int wd, int ht, HWND hWndParent, int nIDCItem) override;
	virtual BOOL CreateSample() override;

	// Operations
	int		AddItem(LPCTSTR szPtr) const;									// CB_ADDSTRING
	int		DeleteItem(int nItem) const;									// CB_DELETESTRING
	int		Dir(UINT uAttr, const TCHAR* wildPtr) const;					// CB_DIR
	int		FindString(int nStart, const TCHAR*	textPtr) const;				// CB_FINDSTRING
	int		FindStringExact(int nStart, const TCHAR* textPtr) const;		// CB_FINDSTRINGEXACT
	BOOL	GetComboBoxInfo(COMBOBOXINFO* cbiPtr) const;					// CB_GETCOMBOBOXINFO
	int		GetCount() const;												// CB_GETCOUNT
	BOOL	GetCueBanner(TCHAR* textPtr, int cchMax) const;					// CB_GETCUEBANNER
	int		GetCurSel() const;												// CB_GETCURSEL
	BOOL	GetDroppedControlRect(RECT* rcPtr) const;						// CB_GETDROPPEDCONTROLRECT
	BOOL	GetDroppedState() const;										// CB_GETDROPPEDSTATE
	int		GetDroppedWidth() const;										// CB_GETDROPPEDWIDTH
	HWND	GetEditboxCtrl() const;
	DWORD	GetEditSel() const;												// CB_GETEDITSEL
	BOOL	GetExtendedUI() const;											// CB_GETEXTENDEDUI
	UINT	GetHorizontalExtent() const;									// CB_GETHORIZONTALEXTENT
	DWORD_PTR GetItemData(int nItem) const;									// CB_GETITEMDATA
	int		GetItemHeight(int nItem) const;									// CB_GETITEMHEIGHT
	int		GetItemText(int nItem, TCHAR* textPtr) const;					// CB_GETLBTEXT
	int		GetItemTextLength(int nItem) const;								// CB_GETLBTEXTLEN
	HWND	GetListboxCtrl() const;
	LCID	GetLocale() const;												// CB_GETLOCALE
	int		GetMinVisible() const;											// CB_GETMINVISIBLE
	int		GetTopIndex() const;											// CB_GETTOPINDEX
	int		InitStorage(int nNumber, UINT uBytes) const;					// CB_INITSTORAGE
	int		InsertItem(int nItem, LPCTSTR szPtr) const;						// CB_INSERTSTRING
	void	SetLimitText(int ccLimit) const;								// CB_LIMITTEXT
	void	ResetContent() const;											// CB_RESETCONTENT
	void	RemoveAllItem() const;
	int		SelectString(int nStartAfter, const TCHAR* strPtr) const;		// CB_SELECTSTRING
	BOOL	SetCueBanner(const TCHAR* textPtr) const;						// CB_SETCUEBANNER
	int		SetCurSel(int nItem) const;										// CB_SETCURSEL
	int		SetDroppedWidth(int nWidth) const;								// CB_SETDROPPEDWIDTH
	BOOL	SetEditSel(int nStartChar, int nEndChar) const;					// CB_SETEDITSEL
	BOOL	SetExtendedUI(BOOL bExtended = TRUE);							// CB_SETEXTENDEDUI
	void	SetHorizontalExtent(UINT nExtent);								// CB_SETHORIZONTALEXTENT
	BOOL	SetItemData(int nItem, DWORD_PTR dwItemData) const;				// CB_SETITEMDATA
	int		SetItemHeight(int nItem, int cyItemHeight) const;				// CB_SETITEMHEIGHT
	LCID	SetLocale(LCID dwLocale) const;									// CB_SETLOCALE
	LCID	SetLocale(int nCountry, int nLanguage) const;					// CB_SETLOCALE
	BOOL	SetMinVisibleItems(int iMinVisible);							// CB_SETMINVISIBLE
	int		SetTopIndex(int nItem) const;									// CB_SETTOPINDEX
	void	ShowDropdown(BOOL bEnable) const;								// CB_SHOWDROPDOWN

	// Clipboard Operations
	void Clear() const;							// WM_CLEAR
	void Cpoy() const;							// WM_COPY
	void Cut() const;							// WM_CUT
	void Paste() const;							// WM_PASTE
	BOOL UnDo() const;							// WM_UNDO

private:
	DmCombo(const DmCombo&) = delete;				//!< Disable copy construction
	DmCombo& operator=(const DmCombo&) = delete;	//!< Disable assignment operator
};

#endif // !ODMC_WNDS_COMBO_HH
