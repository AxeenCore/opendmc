/**************************************************************************//**
 * @file	listbox.hh
 * @brief	DmListbox 列表框類別宣告 Header
 * @date	2000-10-10
 * @date	2018-09-05
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_WNDS_LISTBOX_HH
#define	ODMC_WNDS_LISTBOX_HH
#include "wnds.hh"

/**
 *	@class DmListbox
 *	@brief DmListbox 視窗基底類別，建立視窗。
 */
class DmListbox : public DmWnds
{
public:
	DmListbox();
	virtual ~DmListbox();

	virtual BOOL Create(const TCHAR* szCaptionPtr, int x, int y, int wd, int ht, HWND hWndParent, int nIDCItem) override;
	virtual BOOL CreateSample() override;

	int		AddFile(const TCHAR* filePtr) const;								// LB_ADDFILE
	int		AddString(const TCHAR* strPtr) const;								// LB_ADDSTRING
	int		DeleteString(int nItem) const;										// LB_DELETESTRING
	int		Dir(int nAttrib, TCHAR* pathPtr) const;								// LB_DIR
	int		FindString(int nItem, TCHAR* strPtr) const;							// LB_FINDSTRING
	int		FindStringExact(int nItem, TCHAR* strPtr) const;					// LB_FINDSTRINGEXACT
	int		GetAnchorIndex() const;												// LB_GETANCHORINDEX
	int		GetCaretIndex() const;												// LB_GETCARETINDEX
	int		GetCount() const;													// LB_GETCOUNT
	int		GetCursel() const;													// LB_GETCURSEL
	int		GetHorizontalExtent() const;										// LB_GETHORIZONTALEXTENT
	DWORD_PTR GetItemData(int nItem) const;										// LB_GETITEMDATA
	void*	GetItemDataPtr(int nItem) const;									// ### LB_GETITEMDATA
	int		GetItemHeight(int nItem) const;										// LB_GETITEMHEIGHT
	int		GetItemRect(int nItem, RECT* rcPtr) const;							// LB_GETITEMRECT
	int		GetListBoxInfo() const;												// LB_GETLISTBOXINFO
	LCID	GetLocale() const;													// LB_GETLOCALE
	int		GetSelectionState(int nItem) const;									// LB_GETSEL
	int		GetSelectionCount() const;											// LB_GETSELCOUNT
	int		GetSlecetItems(int nCount, int* intPtr) const;						// LB_GETSELITEMS
	int		GetItemText(int nItem, TCHAR* textPtr) const;						// LB_GETTEXT
	int		GetItemTextLength(int nItem) const;									// LB_GETTEXTLEN
	int		GetTopIndex() const;												// LB_GETTOPINDEX
	DWORD	InitStorage(int nItemNumber, DWORD dwSize) const;					// LB_INITSTORAGE
	int		InsertString(int nItem, TCHAR* textPtr) const;						// LB_INSERTSTRING
	int		ItemFromPoint(int x, int y) const;									// LB_ITEMFROMPOINT
	void	ResetContent() const;												// LB_RESETCONTENT
	int		SelectString(int nStartAfter, const TCHAR* szItemPtr) const;		// LB_SELECTSTRING
	BOOL	SelItemRange(BOOL bSelect, int nFirstItem, int nLastItem) const;	// LB_SELITEMRANGE
	BOOL	SelItemRangeEx(int nFirstItem, int nLastItem) const;				// LB_SELITEMRANGEEX
	BOOL	SetAnchorIndex(int nItem) const;									// LB_SETANCHORINDEX
	BOOL	SetCaretIndex(int nItem, BOOL bScroll = TRUE) const;				// LB_SETCARETINDEX
	void	SetColumnWidth(int cxWidth) const;									// LB_SETCOLUMNWIDTH
	BOOL	SetCount(int nCount) const;											// LB_SETCOUNT
	int		SetCursel(int nItem) const;											// LB_SETCURSEL
	void	SetHorizontalExtent(int cxExtent);									// LB_SETHORIZONTALEXTENT
	BOOL	SetItemData(int nItem, DWORD_PTR dwItemData) const;					// LB_SETITEMDATA
	int		SetItemHeight(int nItem, UINT cyItemHeight) const;					// LB_SETITEMHEIGHT
	LCID	SetLocale(LCID dwLocale) const;										// LB_SETLOCALE
	LCID	SetLocale(int nCountry, int nLanguage) const;						// LB_SETLOCALE
	int		SetSelectionState(int nItem, BOOL bState) const;					// LB_SETSEL
	BOOL	SetTabStops(int nTabStops, INT* rgTabStops) const;					// LB_SETTABSTOPS
	int		SetTopIndex(int nItem) const;										// LB_SETTOPINDEX

private:
	DmListbox(const DmListbox&) = delete;				//!< Disable copy construction
	DmListbox& operator=(const DmListbox&) = delete;	//!< Disable assignment operator
};

#endif // !ODMC_WNDS_LISTBOX_HH
