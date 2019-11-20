/**************************************************************************//**
 * @file	tabview.hh
 * @brief	DmTabView 標籤類別宣告 Header
 * @date	2000-10-10
 * @date	2018-09-15
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_WNDS_TABVIEW_HH
#define ODMC_WNDS_TABVIEW_HH
#include "wnds.hh"

/**
 *	@struct TABPAGEINFO
 *	@brief 標籤頁面資訊
 */
struct TABPAGEINFO {
	DmWnds*	cObject;					//!< 頁面視窗或控制項操作類別物件
	UINT	uIDTabItem;					//!< 頁面控制項或是窗識別碼
	int		nIcon;						//!< 標籤卡 icon 索引
	TCHAR	szText[MAX_STRING_MENU];	//!< 標籤卡名稱
};
typedef TABPAGEINFO* LPTABPAGEINFO;		//!< 結構 TABPAGEINFO 指標型別

/**
 *	@class DmTabView
 *	@brief 標籤卡控制類別
 */
class DmTabView : public DmWnds
{
public:
	DmTabView();
	virtual ~DmTabView();

	virtual BOOL Create(const TCHAR* szCaptionPtr, int x, int y, int wd, int ht, HWND hWndParent, int nIDCItem) override;
	virtual BOOL CreateSample();

	virtual BOOL AddTabPage(DmWnds* dmPtr, const TCHAR* textPtr, HICON hIcon, UINT uIDTabItem);
	virtual BOOL AddTabPage(DmWnds* dmPtr, const TCHAR* textPtr, UINT uIDIcon, UINT uIDTabItem);

	// Wrappers of Tab message
	void		AdjustRect(RECT* rcPtr, BOOL bMode = FALSE) const;			// TCM_ADJUSTRECT
	BOOL		DeleteAllItem() const;										// TCM_DELETEALLITEMS
	BOOL		DeleteItem(int nCards) const;								// TCM_DELETEITEM
	void		DeSelectAll(BOOL bDeSel) const;								// TCM_DESELECTALL
	int			GetCurselFocus() const;										// TCM_GETCURFOCUS
	int			GetCursel() const;											// TCM_GETCURSEL
	DWORD		GetExtendedStyle() const;									// TCM_GETEXTENDEDSTYLE
	HIMAGELIST	GetImageList() const;										// TCM_GETIMAGELIST
	BOOL		GetItem(int nCards, TCITEM* tciPtr) const;					// TCM_GETITEM
	int			GetItemCount() const;										// TCM_GETITEMCOUNT
	BOOL		GetItemRect(int nCards, RECT* rcPtr) const;					// TCM_GETITEMRECT
	int			GetRowCount() const;										// TCM_GETROWCOUNT
	HWND		GetToolTips() const;										// TCM_GETTOOLTIPS
	BOOL		GetUnicodeFormat() const;									// TCM_GETUNICODEFORMAT
	BOOL		HighlightItem(int nTab, BOOL bHighlight = TRUE) const;		// TCM_HIGHLIGHTITEM
	int			HitTest(TCHITTESTINFO* hitPtr) const;						// TCM_HITTEST
	int			InsertItem(int nCards, TCITEM* tciPtr) const;				// TCM_INSERTITEM
	int			InsertItem(int nCards, const TCHAR* textPtr) const;			// TCM_INSERTITEM
	void		RemoveImage(int nImage) const;								// TCM_REMOVEIMAGE
	void		SetCurselFocus(int nCards) const;							// TCM_SETCURFOCUS
	int			SetCursel(int nCards) const;								// TCM_SETCURSEL
	DWORD		SetExtendedStyle(DWORD dwNewStyle, DWORD dwExMask = 0);		// TCM_SETEXTENDEDSTYLE
	HWND		SetImageList(HWND hImageList) const;						// TCM_SETIMAGELIST
	BOOL		SetItem(int nCards, TCITEM* tciPtr) const;					// TCM_SETITEM
	BOOL		SetItemExtra(int nBytes);									// TCM_SETITEMEXTRA
	LRESULT		SetItemSize(int wd, int ht) const;							// TCM_SETITEMSIZE
	INT			SetMinTabWidth(int wd) const;								// TCM_SETMINTABWIDTH
	void		SetPadding(int px, int py) const;							// TCM_SETPADDING
	void		SetToolTips(HWND hTips) const;								// TCM_SETTOOLTIPS
	void		SetUnicodeFormat(BOOL bFormat) const;						// TCM_SETUNICODEFORMAT

protected:
	std::vector<TABPAGEINFO> m_vTabView;

private:
	DmTabView(const DmTabView&) = delete;				//!< Disable copy construction
	DmTabView& operator=(const DmTabView&) = delete;	//!< Disable assignment operator
};

#endif // !ODMC_WNDS_TABVIEW_HH
