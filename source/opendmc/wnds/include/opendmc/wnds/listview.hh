/**************************************************************************//**
 * @file	listview.hh
 * @brief	DmListView 圖示列表類別宣告 Header
 * @date	2000-10-10
 * @date	2018-09-08
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_WNDS_LISTVIEW_HH
#define	ODMC_WNDS_LISTVIEW_HH
#include "wnds.hh"

/**
 *	@class	DmListView
 *	@brief	DmListView 列表 ListView
 *	@remark <b>擴充樣式使用說明</b>
 *			\n 使用 CreateWindow or CreateWindowEx 建立 ListView 時，無法直接指定擴充(延伸)樣式，必須透過 LVM_SETEXTENDEDLISTVIEWSTYLE 訊息處理來設定。
 *			\n 若樣取得 ListView 擴充(延伸)樣式時，同樣必須透過 LVM_GETEXTENDEDLISTVIEWSTYLE 訊息處理取得。
 *
 *	@remark	<b>ListView 擴充(延伸)樣式</b>
 *			- LVS_EX_AUTOAUTOARRANGE	\n 如果未設置圖標位置，則自動排列圖標（與LVS_AUTOARRANGE類似）。
 *			- LVS_EX_AUTOCHECKSELECT	\n 單擊即自動選中復選框。
 *			- LVS_EX_AUTOSIZECOLUMNS	\n 自動調整listview列的大小。
 *			- LVS_EX_BORDERSELECT		\n 擇項目時更改邊框顏色，而不是突出顯示該項目。
 *			- LVS_EX_CHECKBOXES			\n 為 ListView 的項目啟用複選框。
 *										\n 選框在所有列表視圖模式下均可見且可用。單擊磁貼視圖模式中的複選框僅選擇項目。
 *										\n 您可以使用 DmListview::GetCheckState 獲取給定項的複選框的狀態。要設置檢查狀態，請使用 DmListview::SetCheckState。如果設置了此樣式，則當用戶單擊複選框或按空格鍵時，列表視圖控件會自動切換檢查狀態。
 *			- LVS_EX_COLUMNOVERFLOW		\n 指示如果沒有足夠的客戶端寬度來顯示完整的標題項集，則應在圖標/平鋪視圖中顯示溢出按鈕。ListView 在單擊溢出按鈕時發送 LVN_COLUMNOVERFLOWCLICK 通知。該標誌僅在指定了 LVS_EX_HEADERINALLVIEWS 時有效。
 *			- LVS_EX_COLUMNSNAPPOINTS	\n 用戶調整列大小時捕捉到最小列寬。
 *			- LVS_EX_DOUBLEBUFFER		\n 通過雙緩衝塗料，減少閃爍。這種擴展樣式還可以在支持它的系統上進行alpha混合選取框選擇。
 *			- LVS_EX_FLATSB				\n 在列表視圖中啟用平面滾動條。如果需要更多地控制列表視圖滾動條的外觀，則應使用Flat Scroll Bar API直接操作列表視圖的滾動條。如果系統指標發生變化，您負責使用FlatSB_SetScrollProp調整滾動條指標。有關詳細信息，請參閱平面滾動條。
 *			- LVS_EX_FULLROWSELECT		\n 選擇項目後，項目及其所有子項目將突出顯示。此樣式僅與LVS_REPORT樣式一起使用。
 *			- LVS_EX_GRIDLINES			\n 顯示項目和子項目周圍的網格線。此樣式僅與LVS_REPORT樣式一起使用。
 *			- LVS_EX_HEADERDRAGDROP		\n 在列表視圖控件中啟用列的拖放重新排序。此樣式僅適用於使用LVS_REPORT樣式的列表視圖控件。
 *			- LVS_EX_HEADERINALLVIEWS	\n 在所有視圖模式下顯示列標題。
 *			- LVS_EX_HIDELABELS			\n 隱藏圖標和小圖標視圖中的標籤。
 *			- LVS_EX_INFOTIP			\n 當列表視圖控件使用 LVS_EX_INFOTIP 樣式時，LVN_GETINFOTIP 通知代碼將在顯示項目的工具提示之前發送到父視窗。
 *			- LVS_EX_JUSTIFYCOLUMNS		\n 圖標排列在用完整個視圖的列中。
 *			- LVS_EX_LABELTIP			\n 如果任何列表視圖模式中的部分隱藏標籤缺少工具提示文本，則列表視圖控件將展開標籤。如果未設置此樣式，則列表視圖控件將僅為大圖標模式展開部分隱藏的標籤。
 *			- LVS_EX_MULTIWORKAREAS		\n 如果列表視圖控件具有 LVS_AUTOARRANGE樣式，則控件將不會自動排列其圖標，直到定義了一個或多個工作區（請參閱 LVM_SETWORKAREAS）。為了有效，必須在定義任何工作區域並將任何項目添加到控件之前設置此樣式。
 *			- LVS_EX_ONECLICKACTIVATE	\n 當用戶單擊項目時，列表視圖控件將 LVN_ITEMACTIVATE 通知代碼發送到父視窗。此樣式還可以在列表視圖控件中啟用熱跟踪。熱跟踪意味著當光標在項目上移動時，它會突出顯示但不會被選中。有關項目激活的討論，請參閱擴展列表 - 視圖樣式備註部分。
 *			- LVS_EX_REGIONAL			\n 將列表視圖視窗區域設置為僅使用SetWindowRgn包含項目圖標和文本。從視窗區域中排除任何不屬於項目的區域。此樣式僅適用於使用LVS_ICON樣式的列表視圖控件。
 *			- LVS_EX_SIMPLESELECT		\n 在圖標視圖中，將控件的狀態圖像移動到大圖標渲染的右上角。在圖標視圖以外的視圖中沒有變化。當用戶使用空格鍵更改狀態時，所有選定的項目都會循環，而不是具有焦點的項目。
 *			- LVS_EX_SINGLEROW			\n 此樣式不使用。
 *			- LVS_EX_SNAPTOGRID			\n 在圖標視圖中，圖標會自動捕捉到網格中。
 *			- LVS_EX_SUBITEMIMAGES		\n 允許顯示子項目的圖像。此樣式僅與 LVS_REPORT 樣式一起使用。
 *			- LVS_EX_TRACKSELECT		\n 在列表視圖控件中啟用熱軌選擇。熱曲目選擇意味著當光標在項目上保持一段時間時自動選擇項目。可以使用LVM_SETHOVERTIME消息從默認系統設置更改延遲。此樣式適用於所有樣式的列表視圖控件。您可以通過調用SystemParametersInfo來檢查是否啟用了熱軌選擇。
 *			- LVS_EX_TRANSPARENTBKGND	\n 背景由父母通過 WM_PRINTCLIENT 繪製。
 *			- LVS_EX_TRANSPARENTSHADOWTEXT	\n 僅在透明背景上啟用陰影文本。
 *			- LVS_EX_TWOCLICKACTIVATE	\n 當用戶雙擊項目時，列表視圖控件將 LVN_ITEMACTIVATE 通知代碼發送到父視窗。此樣式還可以在列表視圖控件中啟用熱跟踪。熱跟踪意味著當光標在項目上移動時，它會突出顯示但不會被選中。有關項目激活的討論，請參閱擴展列表 - 視圖樣式備註部分。
 *			- LVS_EX_UNDERLINECOLD		\n 使可能激活的非熱門項目顯示帶下劃線的文本。此樣式還要求設置 LVS_EX_TWOCLICKACTIVATE。有關項目激活的討論，請參閱擴展列表 - 視圖樣式備註部分。
 *			- LVS_EX_UNDERLINEHOT		\n 使可能激活的熱門項目顯示帶下劃線的文本。此樣式要求也設置 LVS_EX_ONECLICKACTIVATE 或 LVS_EX_TWOCLICKACTIVATE。有關項目激活的討論，請參閱擴展列表 - 視圖樣式備註部分。
 *
 *	@remark	<b>ListView 樣式</b>
 *			- LVS_ALIGNLEFT				\n 項目在圖標和小圖標視圖中左對齊。
 *			- LVS_ALIGNMASK				\n 控件的當前對齊方式。
 *			- LVS_ALIGNTOP				\n 項目與圖標和小圖標視圖中的列表視圖控件的頂部對齊。
 *			- LVS_AUTOARRANGE			\n 圖標自動排列在圖標和小圖標視圖中。
 *			- LVS_EDITLABELS			\n 可以就地編輯項目文本。父視窗必須處理 LVN_ENDLABELEDIT 通知代碼。
 *			- LVS_ICON					\n 此樣式指定圖標視圖。
 *			- LVS_LIST					\n 此樣式指定列表視圖。
 *			- LVS_NOCOLUMNHEADER		\n 列標題不會顯示在報表視圖中。默認情況下，列在報表視圖中具有標題。
 *			- LVS_NOLABELWRAP			\n 項目文本顯示在圖標視圖中的單行上。默認情況下，項目文本可以包裝在圖標視圖中。
 *			- LVS_NOSCROLL				\n 滾動已禁用。所有項目必須在客戶區域內。此樣式與LVS_LIST或LVS_REPORT樣式不兼容。有關進一步的討論，請參閱知識庫文章Q137520。
 *			- LVS_NOSORTHEADER			\n 列標題不像按鈕那樣工作。如果單擊報表視圖中的列標題不執行操作（例如排序），則可以使用此樣式。
 *			- LVS_OWNERDATA				\n 此樣式指定虛擬列表視圖控件。有關此列表控件樣式的詳細信息，請參閱關於列表視圖控件。
 *			- LVS_OWNERDRAWFIXED		\n 所有者視窗可以在報表視圖中繪製項目。列表視圖控件發送WM_DRAWITEM消息來繪製每個項目; 它不會為每個子項發送單獨的消息。DRAWITEMSTRUCT結構的iItemData成員包含指定列表視圖項的項數據。
 *			- LVS_REPORT				\n 此樣式指定報告視圖。將LVS_REPORT樣式與列表視圖控件一起使用時，第一列始終是左對齊的。您無法使用LVCFMT_RIGHT更改此對齊方式。有關列對齊的更多信息，請參閱LVCOLUMN。
 *			- LVS_SHAREIMAGELISTS		\n 控件被銷毀時，不會刪除圖像列表。此樣式允許使用具有多個列表視圖控件的相同圖像列表。
 *			- LVS_SHOWSELALWAYS			\n 即使控件沒有焦點，也始終顯示選擇（如果有）。
 *			- LVS_SINGLESEL				\n 一次只能選擇一個項目。默認情況下，可以選擇多個項目。
 *			- LVS_SMALLICON				\n 此樣式指定小圖標視圖。
 *			- LVS_SORTASCENDING			\n 項目索引按項目文本按升序排序。
 *			- LVS_SORTDESCENDING		\n 項目索引根據項目文本按降序排序。
 *			- LVS_TYPEMASK				\n 確定控件的當前視窗樣式。
 *			- LVS_TYPESTYLEMASK			\n 確定控制項目對齊和標題外觀和行為的視窗樣式。
 */
class DmListView : public DmWnds
{
public:
	DmListView();
	virtual ~DmListView();

	virtual BOOL Create(const TCHAR* szPtr, int x, int y, int wd, int ht, HWND hWndParent, int nIDCItem) override;
	virtual BOOL CreateSample();

	// Operations
	DWORD	ApproximateViewRec(int wd = -1, int ht = -1, int nCount = -1) const;					// LVM_APPROXIMATEVIEWRECT
	BOOL	Arrange(UINT uAlign) const;																// LVM_ARRANGE
	void	CancelEditLable() const;																// LVM_CANCELEDITLABEL
	HIMAGELIST CreateDragImage(int nItem, POINT* pointPtr) const;									// LVM_CREATEDRAGIMAGE
	BOOL	DeleteAllItem() const;																	// LVM_DELETEALLITEMS
	BOOL	DeleteColumn(int nColumn) const;														// LVM_DELETECOLUMN
	BOOL	DeleteAllColumn() const;																// ---
	BOOL	DeleteItem(int nItem) const;															// LVM_DELETEITEM
	void	EditLabel(int nIndex) const;															// LVM_EDITLABEL
	int		EnableGroupView(BOOL bView) const;														// LVM_ENABLEGROUPVIEW
	BOOL	EnsureVisible(int nItem, BOOL bPartial) const;											// LVM_ENSUREVISIBLE
	int		FindItem(int nStart, LVFINDINFO* findInfoPtr) const;									// LVM_FINDITEM
	COLORREF GetBkColor() const;																	// LVM_GETBKCOLOR
	BOOL	GetBkImage(LVBKIMAGE* lvbkiPtr) const;													// LVM_GETBKIMAGE
	UINT	GetCallbackMask() const;																// LVM_GETCALLBACKMASK
	BOOL	GetColumn(int nColumn, LPTSTR szColumnPtr, int ccMax) const;							// LVM_GETCOLUMN
	int		GetColumnCount() const;																	// ---
	BOOL	GetColumnOrderArray(int nCount, int* intPtr) const;										// LVM_GETCOLUMNORDERARRAY
	int		GetColumnWidth(int nColumn) const;														// LVM_GETCOLUMNWIDTH
	int		GetCountPerPage() const;																// LVM_GETCOUNTPERPAGE
	HWND	GetEditControl() const;																	// LVM_GETEDITCONTROL
	BOOL	GetEmptyText(size_t ccLen, TCHAR* textPre) const;										// LVM_GETEMPTYTEXT
	DWORD	GetExtendedListViewStyle() const;														// LVM_GETEXTENDEDLISTVIEWSTYLE
	int		GetFocusedGroup() const;																// LVM_GETFOCUSEDGROUP
	BOOL	GetFooterInfo(LVFOOTERINFO* lvfInfoPtr) const;											// LVM_GETFOOTERINFO
	BOOL	GetFooterItem(int nItem, LVFOOTERITEM* lvfItemPtr) const;								// LVM_GETFOOTERITEM
	BOOL	GetFooterItemRect(int nItem, RECT* rcPtr) const;										// LVM_GETFOOTERITEMRECT
	BOOL	GetFooterRect(RECT* rcPtr);																// LVM_GETFOOTERRECT
	int		GetGroupCount() const;																	// LVM_GETGROUPCOUNT
	int		GetGroupInfo(int nIDGroup, LVGROUP* lvgPtr) const;										// LVM_GETGROUPINFO
	BOOL	GetGroupInfoByIndex(int nGroupIndex, LVGROUP* lvgPtr) const;							// LVM_GETGROUPINFOBYINDEX
	void	GetGroupMetrics(LVGROUPMETRICS* lvgmPtr) const;											// LVM_GETGROUPMETRICS
	BOOL	GetGroupRect(int nIDGroup, LONG nType, RECT* rcPtr) const;								// LVM_GETGROUPRECT
	UINT	GetGroupState(int nIDGroup, DWORD dwMask) const;										// LVM_GETGROUPSTATE
	HWND	GetHeader() const;																		// LVM_GETHEADER
	HCURSOR	GetHotCursor() const;																	// LVM_GETHOTCURSOR
	int		GetHotItem() const;																		// LVM_GETHOTITEM
	DWORD	GetHoverTime() const;																	// LVM_GETHOVERTIME
	HIMAGELIST GetImageList(int iImageList) const;													// LVM_GETIMAGELIST
	BOOL	GetInsertMark(LVINSERTMARK* lvimPtr) const;												// LVM_GETINSERTMARK
	COLORREF GetInsertMarkColor() const; 															// LVM_GETINSERTMARKCOLOR
	int		GetInsertMarkRect(RECT* rectPtr) const;													// LVM_GETINSERTMARKRECT
	int		GetISearchString(TCHAR* textPtr) const;													// LVM_GETISEARCHSTRING
	BOOL	GetItem(LVITEM* lviPtr) const;															// LVM_GETITEM
	int		GetItemCount() const;																	// LVM_GETITEMCOUNT
	BOOL	GetItemIndexRect(LVITEMINDEX* lviiPtr, LONG nSubItem, LONG nCode, RECT* rcPtr) const;	// LVM_GETITEMINDEXRECT
	BOOL	GetItemPosition(int nItem, POINT* ptPtr) const;											// LVM_GETITEMPOSITION
	BOOL	GetItemRect(int nItem, RECT* rcPtr)  const;												// LVM_GETITEMRECT
	BOOL	GetItemRect(int nItem, int nMode, RECT* rcPtr) const;									// ---
	BOOL	GetItemSpacing(BOOL bSmall, int* nHorzSpacingPtr, int* nVertSpacingPtr) const;			// LVM_GETITEMSPACING
	int		GetItemState(int nItem, int nMode = LVIS_STATEIMAGEMASK) const;							// LVM_GETITEMSTATE
	BOOL	GetItemText(int nItem, int nSubItem, TCHAR* szTextPtr, int ccLen) const;				// LVM_GETITEMTEXT
	int		GetNextSelectItem();																	// LVM_GETNEXTITEM
	int		GetFirstSelectItem();																	// ---
	int		GetSelectItem() const;																	// ---
	BOOL	GetNextItemIndex(LVITEMINDEX* lviiPtr, int nFlags) const;								// LVM_GETNEXTITEMINDEX
	UINT	GetNumberOfWorkAreas() const;															// LVM_GETNUMBEROFWORKAREAS
	BOOL	GetOrigin(POINT* pointPtr) const;														// LVM_GETORIGIN
	COLORREF GetOutlineColor() const;																// LVM_GETOUTLINECOLOR
	UINT	GetSelectedColumn() const;																// LVM_GETSELECTEDCOLUMN
	int		GetSelectCount() const;																	// LVM_GETSELECTEDCOUNT
	int		GetSelectionMark() const;																// LVM_GETSELECTIONMARK
	int		GetStringWidth(const TCHAR* textPtr) const;												// LVM_GETSTRINGWIDTH
	BOOL	GetSubItemRect(int nItem, int nSubItem, int nArea, RECT* rcPtr) const;					// LVM_GETSUBITEMRECT
	COLORREF GetTextBkColor() const;																// LVM_GETTEXTBKCOLOR
	COLORREF GetTextColor() const;																	// LVM_GETTEXTCOLOR
	void	GetTileInfo(LVTILEINFO* lvtiPtr) const;													// LVM_GETTILEINFO
	void	GetTileViewInfo(LVTILEVIEWINFO* lvtviPtr) const;										// LVM_GETTILEVIEWINFO
	HWND	GetToolTips() const;																	// LVM_GETTOOLTIPS
	int		GetTopIndex() const;																	// LVM_GETTOPINDEX
	BOOL	GetUnicodeFormat() const;																// LVM_GETUNICODEFORMAT
	DWORD	GetView() const;																		// LVM_GETVIEW
	BOOL	GetViewRect(RECT* rcPtr) const;															// LVM_GETVIEWRECT
	void	GetWorkAreas(int nWorkAreas, RECT* rcPtr) const;										// LVM_GETWORKAREAS
	BOOL	HasGroup(int nIDGroup) const;															// LVM_HASGROUP
	int		HitTest(LVHITTESTINFO* lvhPtr) const;													// LVM_HITTEST
	int     HitTest(POINT* ptPtr, UINT* flagPtr = NULL) const;										// ### LVM_HITTEST
	int     InsertColumn(int nColumn, const LVCOLUMN* colPtr) const;								// LVM_INSERTCOLUMN
	int		InsertColumn(int nColumn, int wd, int nAlign, const TCHAR* textPtr) const;				// ### LVM_INSERTCOLUMN
	int		InsertGroup(int nGroup, LVGROUP* grpPtr);												// LVM_INSERTGROUP
	void	InsertGroupSorted(LVINSERTGROUPSORTED* lvigPtr) const;									// LVM_INSERTGROUPSORTED
	BOOL	InsertItem(const LVITEM* lviPtr) const;													// LVM_INSERTITEM
	BOOL	InsertItem(int nItem, const TCHAR* textPtr) const;										// LVM_INSERTITEM
	BOOL	InsertItem(int nItem, const TCHAR* textPtr, int nImage) const;							// LVM_INSERTITEM
	BOOL	InsertMarkHitTest(POINT* pointPtr, LVINSERTMARK* lvimPtr) const;						// LVM_INSERTMARKHITTEST
	BOOL	IsGroupViewEnabled() const;																// LVM_ISGROUPVIEWENABLED
	BOOL	IsItemVisible(int iItem) const;															// LVM_ISITEMVISIBLE
	int		MapIDToIndex(UINT uID) const;															// LVM_MAPIDTOINDEX
	UINT	MapIndexToID(int iItem) const;															// LVM_MAPINDEXTOID
	void	MoveGroup(int iGroupId, int toIndex) const;												// LVM_MOVEGROUP		-- MSDN : This message is not implemented.
	void	MoveItemToGroup(int idItemFrom, int idGroupTo) const;									// LVM_MOVEITEMTOGROUP	-- MSDN : This message is not implemented.
	BOOL	RedrawItems(int nFirst, int nLast) const;												// LVM_REDRAWITEMS
	void	RemoveAllGroups() const;																// LVM_REMOVEALLGROUPS
	int		RemoveGroup(int nGroupId) const;														// LVM_REMOVEGROUP
	BOOL	Scroll(int xScroll, int yScroll) const;													// LVM_SCROLL
	BOOL	Scroll(const SIZE* sizePtr) const;														// ##
	BOOL	SetBkColor(COLORREF dwColor) const;														// LVM_SETBKCOLOR
	BOOL	SetBkImage(const LVBKIMAGE* lvbiPtr) const;												// LVM_SETBKIMAGE
	BOOL	SetCallbackMask(UINT nMask) const;														// LVM_SETCALLBACKMASK
	BOOL	SetColumn(int nColumn, const TCHAR* textPtr) const;										// LVM_SETCOLUMN
	BOOL	SetColumnOrderArray(int nCount, INT* nArrayPtr) const;									// LVM_SETCOLUMNORDERARRAY
	BOOL	SetColumnWidth(int nColumn, int nPixel) const;											// LVM_SETCOLUMNWIDTH
	void	SetExtendedListViewStyleEx(DWORD dwExStyle) const;										// LVM_SETEXTENDEDLISTVIEWSTYLE
	int		SetGroupInfo(int iGroupId, LVGROUP* lvgPtr) const;										// LVM_SETGROUPINFO
	void	SetGroupMetrics(LVGROUPMETRICS* lvgmtPtr) const;										// LVM_SETGROUPMETRICS
	HCURSOR	SetHotCursor(HCURSOR hCursor) const;													// LVM_SETHOTCURSOR
	int		SetHotItem(int nItem) const;															// LVM_SETHOTITEM
	DWORD	SetHoverTime(DWORD  dwHoverTime = static_cast<DWORD>(-1)) const;						// LVM_SETHOVERTIME
	DWORD	SetIconSpacing(int cx, int cy) const;													// LVM_SETICONSPACING
	DWORD	SetIconSpacing(SIZE* sizePtr) const;													// *** LVM_SETICONSPACING
	HIMAGELIST SetImageList(HIMAGELIST hImageList, int nImageListType) const;						// LVM_SETIMAGELIST
	BOOL	SetInfoTip(LVSETINFOTIP* lvsitPtr) const;												// LVM_SETINFOTIP
	BOOL	SetInsertMark(LVINSERTMARK* lvimPtr) const;												// LVM_SETINSERTMARK
	COLORREF SetInsertMarkColor(COLORREF dwColor) const;											// LVM_SETINSERTMARKCOLOR
	BOOL	SetItem(const LVITEM* lviPtr) const;													// LVM_SETITEM
	BOOL	SetItemCount(int nItems, int nBehavior = LVSICF_NOINVALIDATEALL) const;					// LVM_SETITEMCOUNT
	BOOL	SetItemIndexState(LVITEMINDEX* lviiPtr, DWORD dwState, DWORD dwMask) const;				// LVM_SETITEMINDEXSTATE
	BOOL	SetItemPosition(int nItem, POINT* ptPtr) const;											// LVM_SETITEMPOSITION
	void	SetItemPosition32(int nItem, POINT* ptPtr) const;										// LVM_SETITEMPOSITION32
	BOOL	SetItemState(int nItem, const LVITEM* lviPtr) const;									// LVM_SETITEMSTATE
	BOOL	SetItemState(int nItem, UINT uState, UINT uMask) const;									// *** LVM_SETITEMSTATE
	BOOL	SetItemSelected(int nItem, BOOL bSelect) const;											// *** LVM_SETITEMSTATE
	BOOL	SetItemFocused(int nItem, BOOL bFocus) const;											// *** LVM_SETITEMSTATE
	BOOL	SetItemText(int nItem, int nSub, const TCHAR* textPtr) const;							// LVM_SETITEMTEXT
	COLORREF SetOutlineColor(COLORREF dwColor) const;												// LVM_SETOUTLINECOLOR
	void	SetSelectedColumn(int nCol) const;														// LVM_SETSELECTEDCOLUMN
	int		SetSelectionMark(int nItem) const;														// LVM_SETSELECTIONMARK
	BOOL	SetTextBkColor(COLORREF dwColor) const;													// LVM_SETTEXTBKCOLOR
	BOOL	SetTextColor(COLORREF dwColor) const;													// LVM_SETTEXTCOLOR
	BOOL	SetTileInfo(LVTILEINFO* lvtiPtr) const;													// LVM_SETTILEINFO
	BOOL	SetTileViewInfo(LVTILEVIEWINFO* lvtviPtr) const;										// LVM_SETTILEVIEWINFO
	HWND	SetToolTips(HWND hWndTips) const;														// LVM_SETTOOLTIPS
	BOOL	SetUnicodeFormat(BOOL bUnicode) const;													// LVM_SETUNICODEFORMAT
	int		SetView(DWORD dwShow) const;															// LVM_SETVIEW
	void	SetWorkAreas(int nWorkAreas, const RECT* rcPtrArray) const;								// LVM_SETWORKAREAS
	BOOL	SortGroups(PFNLVGROUPCOMPARE fnGroupComparePtr, LPVOID vPtr) const;						// LVM_SORTGROUPS
	BOOL	SortItems(PFNLVCOMPARE fnComparePtr, DWORD_PTR dwData) const;							// LVM_SORTITEMS
	BOOL	SortItemsEx(PFNLVCOMPARE fnComparePtr, DWORD_PTR dwData) const;							// LVM_SORTITEMSEX
	int		SubItemHitTest(LVHITTESTINFO* lvtiPtr) const;											// LVM_SUBITEMHITTEST
	BOOL	Update(int nItem) const;

protected:
	virtual void DefaultReportStyle() const;

	// 變數成員宣告
	int m_nSelectItemCount;		//!< 被選取項目總數
	int m_nSelectItemIndex;		//!< 當前選取的索引

private:
	DmListView(const DmListView&) = delete;				//!< Disable copy construction
	DmListView& operator=(const DmListView&) = delete;	//!< Disable assignment operator
};

#endif // !ODMC_WNDS_LISTVIEW_HH
