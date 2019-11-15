/**************************************************************************//**
 * @file	treeview.hh
 * @brief	DmTreeView 圖示樹狀類別宣告 Header
 * @date	2000-10-10
 * @date	2018-09-12
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_WNDS_TREEVIEW_HH
#define	ODMC_WNDS_TREEVIEW_HH
#include "wnds.hh"

/**
 *	@class DmTreeView
 *	@brief 樹狀列表
 */
class DmTreeView : public DmWnds
{
public:
	DmTreeView();
	virtual ~DmTreeView();

	virtual BOOL Create(const TCHAR* szCaptionPtr, int x, int y, int wd, int ht, HWND hWndParent, int nIDCItem) override;
	virtual BOOL CreateSample() override;

	HWND		CreateDragImage(HTREEITEM hItem) const;								// TVM_CREATEDRAGIMAGE
	BOOL		DeleteAllItem() const;												// TVM_DELETEITEM
	BOOL		DeleteItem(HTREEITEM hItem) const;									// TVM_DELETEITEM
	HWND		EditLabel(HTREEITEM hItem) const;									// TVM_EDITLABEL
	BOOL		EndEditLabelNow(BOOL fCancelWithoutSave) const;						// TVM_ENDEDITLABELNOW
	BOOL		EnsureVisible(HTREEITEM hItem) const;								// TVM_ENSUREVISIBLE
	BOOL		Expand(HTREEITEM hItem, UINT nCode) const;							// TVM_EXPAND
	COLORREF	GetBkColor() const;													// TVM_GETBKCOLOR
	HTREEITEM	GetChildItem(HTREEITEM hItem) const;
	UINT		GetCount() const;													// TVM_GETCOUNT
	HTREEITEM	GetDropHiLightItem() const;
	HWND		GetEditControl() const;  											// TVM_GETEDITCONTROL
	DWORD		GetExtendedStyle() const;  											// TVM_GETEXTENDEDSTYLE
	HTREEITEM	GetFirstVisibleItem() const;
	HIMAGELIST	GetImageList(UINT nImageList) const;								// TVM_GETIMAGELIST
	UINT		GetIndent() const;													// TVM_GETINDENT
	COLORREF	GetInsertMarkColor() const;											// TVM_GETINSERTMARKCOLOR
	UINT		GetISearchString(const TCHAR* strPtr) const;						// TVM_GETISEARCHSTRING

	BOOL		GetItem(TVITEM* tiPtr) const;											// TVM_GETITEM
	DWORD_PTR	GetItemData(HTREEITEM hItem) const;										// TVM_GETITEM
	int			GetItemHeight() const;													// TVM_GETITEMHEIGHT
	BOOL		GetItemImage(HTREEITEM hItem, int& nImage, int& nSelectedImage) const;	// TVM_GETITEM
	BOOL		GetItemPartRect(HTREEITEM hItem, int nPart, RECT* rcPtr) const;			// TVM_GETITEMPARTRECT
	BOOL		GetItemRect(HTREEITEM hItem, RECT* rcPtr, BOOL bTextOnly) const;		// TVM_GETITEMRECT
	UINT		GetItemState(HTREEITEM hItem, UINT nStateMask) const;					// TVM_GETITEMSTATE
	COLORREF	GetLineColor() const;													// TVM_GETLINECOLOR
	BOOL		GetItemText(HTREEITEM hItem, TCHAR* textPtr, int cchMax) const;			// TVM_GETITEM
	HTREEITEM	GetLastVisibleItem() const;
	HTREEITEM	GetNextItem(HTREEITEM hItem, UINT nCode) const;							// TVM_GETNEXTITEM
	HTREEITEM	GetNextSiblingItem(HTREEITEM hItem) const;
	HTREEITEM	GetNextSelectedItem(HTREEITEM hItem) const;
	HTREEITEM	GetNextVisibleItem(HTREEITEM hItem) const;
	HTREEITEM	GetParentItem(HTREEITEM hItem) const;
	HTREEITEM	GetPrevSiblingItem(HTREEITEM hItem) const;
	HTREEITEM	GetPrevVisibleItem(HTREEITEM hItem) const;
	HTREEITEM	GetRootItem() const;
	UINT		GetScrollTime() const;												// TVM_GETSCROLLTIME
	HTREEITEM	GetSelectionItem() const;
	UINT		GetSelectedCount() const;											// TVM_GETSELECTEDCOUNT
	COLORREF	GetTextColor() const;												// TVM_GETTEXTCOLOR
	HWND		GetToolTips() const;												// TVM_GETTOOLTIPS
	BOOL		GetUnicodeFormat() const;											// TVM_GETUNICODEFORMAT
	UINT		GetVisibleCount() const;											// TVM_GETVISIBLECOUNT

	HTREEITEM	HitTest(POINT* ptPtr, UINT* flagPtr = NULL) const;					// TVM_HITTEST
	HTREEITEM	HitTest(TVHITTESTINFO* hitinfoPtr) const;							// TVM_HITTEST
	HTREEITEM	InsertItem(UINT nMask, const TCHAR* szItemPtr, int nImage, int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam, HTREEITEM hParent, HTREEITEM hInsertAfter) const;		// TVM_INSERTITEM
	HTREEITEM	InsertItem(const TCHAR* textPtr, HTREEITEM hParent, HTREEITEM hInsertAfter) const;									// TVM_INSERTITEM
	HTREEITEM	InsertItem(const TCHAR* textPtr, int nImage, int nSelectedImage, HTREEITEM hParent, HTREEITEM hInsertAfter)	const;	// TVM_INSERTITEM
	BOOL		ItemHasChildren(HTREEITEM hItem) const;

	HTREEITEM	MapAccIdToItem(UINT uAccId) const;									// TVM_MAPACCIDTOHTREEITEM
	UINT		MapItemToAccID(HTREEITEM hItem) const;								// TVM_MAPHTREEITEMTOACCID
	BOOL		SelectItem(HTREEITEM hItem, UINT nCode) const;						// TVM_SELECTITEM
	BOOL		SetAutoscrollInfo(UINT uPixelsPerSec, UINT uUpdateTime) const;		// TVM_SETAUTOSCROLLINFO
	COLORREF	SetBkColor(COLORREF clr);											// TVM_SETBKCOLOR
	DWORD		SetBorder(DWORD dwFlags, int xBorder, int yBorder) const;			// TVM_SETBORDER
	DWORD		SetExtendedStyle(DWORD dwExMask, DWORD dwExStyles) const;			// TVM_SETEXTENDEDSTYLE
	BOOL		SetHot(HTREEITEM hHotItem) const;									// TVM_SETHOT
	HIMAGELIST	SetImageList(HIMAGELIST hImageList, int nImageListType) const;		// TVM_SETIMAGELIST
	void		SetIndent(UINT nIndent) const;										// TVM_SETINDENT
	BOOL		SetInsertMark(HTREEITEM hItem, BOOL fAfter = TRUE) const;			// TVM_SETINSERTMARK
	COLORREF	SetInsertMarkColor(COLORREF clrNew) const;							// TVM_SETINSERTMARKCOLOR

	BOOL		SetItem(TVITEM* tviPtr) const;										// TVM_SETITEM
	BOOL		SetItem(HTREEITEM hItem, UINT uMask, const TCHAR* szItemPtr, int nImage, int nSelectedImage, UINT uState, UINT uStateMask, LPARAM lParam) const;
	BOOL		SetItemData(HTREEITEM hItem, DWORD_PTR dwData) const;				// TVM_SETITEM
	int			SetItemHeight(int nHeight) const;									// TVM_SETITEMHEIGHT
	BOOL		SetItemImage(HTREEITEM hItem, int nImage, int nSelectedImage) const;// TVM_SETITEM
	BOOL		SetItemText(HTREEITEM hItem, const TCHAR* textPtr) const;			// TVM_SETITEM

	COLORREF	SetLineColor(COLORREF clrNew = CLR_DEFAULT) const;					// TVM_SETLINECOLOR
	UINT		SetScrollTime(UINT uScrollTime) const;								// TVM_SETSCROLLTIME
	COLORREF	SetTextColor(COLORREF clr) const;									// TVM_SETTEXTCOLOR
	HWND		SetToolTips(HWND hWndTip) const;									// TVM_SETTOOLTIPS
	BOOL		SetUnicodeFormat(BOOL bUnicode = TRUE) const;						// TVM_SETUNICODEFORMAT
	void		ShowInfoTip(HTREEITEM hItem) const;									// TVM_SHOWINFOTIP
	BOOL		SortChildren(HTREEITEM hItem, BOOL bRecursive = FALSE) const;		// TVM_SORTCHILDREN
	BOOL		SortChildrenCB(TVSORTCB* tvsPtr);									// TVM_SORTCHILDRENCB

private:
	DmTreeView(const DmTreeView&) = delete;				//!< Disable copy construction
	DmTreeView& operator=(const DmTreeView&) = delete;	//!< Disable assignment operator
};


#endif // !ODMC_WNDS_TREEVIEW_HH
