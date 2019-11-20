/**************************************************************************//**
 * @file	wnds.hh
 * @brief	DmWnds 視窗基底類別宣告 Header
 * @date	2000-10-10
 * @date	2018-08-05
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_WNDS_DMWNDS
#define	ODMC_WNDS_DMWNDS
#include "winapp.hh"

/**
 *	@class DmWnds
 *	@brief DmWnds 視窗基底類別，建立視窗。
 */
class DmWnds : public DmWndsObject
{
public:
	DmWnds();
	DmWnds(EmCtrls eType);
	virtual ~DmWnds();

	virtual BOOL Attach(HWND hWnd);
	virtual BOOL AttachDlgItem(HWND hWndParent, int nIDCItem);
	virtual void Detach();

	virtual BOOL Create(const TCHAR* szPtr, int x, int y, int wd, int ht, HWND hWndParent, int nIDCItem);
	virtual BOOL CreateEx(DWORD dwExStyle, const TCHAR* szClassPtr, const TCHAR* szCaptionPtr, DWORD dwStyle, int x, int y, int wd, int ht, HWND hWndParent, HMENU hMenuOrIDDItem, void* vParamPtr);

	virtual EmCtrls		GetSafeType() const;
	virtual HWND		GetSafeHwnd() const;
	virtual HFONT		GetSafeFont() const;
	virtual LONG_PTR	GetSafeCallback() const;
	virtual LONG_PTR	GetSafePrevCallback() const;

	BOOL CreateFrame(const WNDSFRAME* smPtr);
	BOOL CreateControls(const WNDSCTRLS* smPtr);

	/* Operations function */
	operator HWND() const { return m_hWnd; };						//!< 取得此類別物件綁定的視窗操作代碼，同 DmWnds::GetSafeHwnd
	BOOL IsWindow() const { return ::IsWindow(m_hWnd); }			//!< 此類別物件視窗操作代碼是否存活中
	BOOL IsModal()	const { return m_bModal; }						//!< 此類別物件是否為 Dialog Modal 模式

	BOOL PostUserCreateMessage(WPARAM wParam = 0, LPARAM lParam = 0) const;
	BOOL CreateSafeFont(const TCHAR* szFacePtr, int nSize, BOOL bBlod, int nCharset = DEFAULT_CHARSET);
	void DeleteSafeFont();

	/* Win32 Windows controller API */
	HDC		BeginPaint(PAINTSTRUCT* psPtr) const;
	BOOL	BringWindowToTop() const;

	BOOL	CenterWindow() const;
	BOOL	CheckDlgButton(int nIDButton, UINT uCheck) const;
	BOOL	CheckRadioButton(int nIDFirstButton, int nIDLastButton, int nIDCheckButton) const;
	HWND	ChildWindowFromPoint(POINT Point) const;
	BOOL	ClientToScreen(POINT* ptPtr) const;
	BOOL	ClientToScreen(RECT* rcPtr) const;
	BOOL	CloseWindow() const;

	HFONT	CreateFont(const TCHAR* szFacePtr, int nSize, BOOL bBlod, int nCharset = DEFAULT_CHARSET);
	BOOL	DeleteFont(HFONT hFont);

	BOOL	DisableWindow() const;
	int		DlgDirList(TCHAR* szPathSpecPtr, int nIDListBox, int nIDStaticPath, UINT uFileType) const;
	int		DlgDirListComboBox(TCHAR* szPathSpecPtr, int nIDComboBox, int nIDStaticPath, UINT uFileType) const;
	BOOL	DlgDirSelectEx(TCHAR* strPtr, int chCount, int idListBox) const;
	BOOL	DlgDirSelectComboBoxEx(TCHAR* strPtr, int cchOut, int idComboBox) const;
	void	DragAcceptFiles(BOOL fAccept) const;
	BOOL	DrawAnimatedRects(int idAni, const RECT* rcFromPtr, const RECT* rcToPtr) const;
	BOOL	DrawCaption(HDC hDC, const RECT* rcPtr, UINT dwFlags) const;
	BOOL	DrawMenuBar() const;

	BOOL	EnableWindow(BOOL bEnable = TRUE) const;
	BOOL	EndPaint(const PAINTSTRUCT* psPtr) const;

	HWND	FindWindow(const TCHAR* szClassNamePtr, const TCHAR* szWindowNamePtr) const;
	HWND	FindWindowEx(HWND hWndParent, HWND hWndChildAfter, const TCHAR* szClassNamePtr, const TCHAR* szWindowNamePtr) const;

	HWND	GetActiveWindow() const;
	HWND	GetAncestor(UINT uFlag = GA_ROOT) const;
	int		GetBorderSize() const;
	HWND	GetCapture() const;
	BOOL	GetCaretPos(POINT* ptPtr) const;
	int		GetClassName(TCHAR* szClassNamePtr, int nMaxCount) const;
	ULONG_PTR GetClassLongPtr(int nIndex) const;
	BOOL	GetClientRect(RECT* rcPtr) const;
	HDC		GetDC() const;
	HDC		GetDCEx(HRGN  hrgnClip, DWORD dwFlags) const;
	HWND	GetDesktopWindow() const;
	int		GetDlgCtrlID() const;
	HWND	GetDlgItem(int nIDDlgItem) const;
	UINT	GetDlgItemInt(int nIDDlgItem, BOOL *bTranslatedPtr, BOOL bSigned) const;
	UINT	GetDlgItemInt(int nIDDlgItem, BOOL bSigned) const;
	UINT	GetDlgItemText(int nIDDlgItem, TCHAR* szStringPtr, int cchMax) const;
	DWORD	GetExStyle() const;
	HWND	GetFocus() const;
	HFONT	GetFont() const;
	HWND	GetForegroundWindow() const;
	HICON	GetIcon(BOOL bBigIcon) const;
	HWND	GetLastActivePopup() const;
	HMENU	GetMenu() const;
	HMODULE	GetModuleHandle() const;
	HWND	GetNextDlgGroupItem(HWND hCtrl, BOOL bPrevious) const;
	HWND	GetNextDlgTabItem(HWND hCtrl, BOOL bPrevious) const;
	HWND	GetParent() const;
	BOOL	GetScrollBarInfo(LONG idObject, SCROLLBARINFO* sbiPtr) const;
	BOOL	GetScrollInfo(int nBar, SCROLLINFO* siPtr) const;
	int		GetScrollPos(int nBar) const;
	BOOL	GetScrollRange(int nBar, int* nMinPosPtr, int* nMaxPosPtr) const;
	DWORD	GetStyle() const;
	HMENU	GetSystemMenu(BOOL bRevert) const;
	HWND	GetTopWindow() const;
	BOOL	GetUpdateRect(RECT* rcPtr, BOOL bErase) const;
	int		GetUpdateRgn(HRGN hRgn, BOOL bErase) const;
	HWND	GetWindow(UINT uCmd) const;
	HDC		GetWindowDC() const;
	LONG_PTR GetWindowLongPtr(int nIndex) const;
	BOOL	GetWindowInfo(WINDOWINFO* wiPtr) const;
	BOOL	GetWindowPlacement(WINDOWPLACEMENT *wpmPtr) const;
	BOOL	GetWindowRect(RECT* rcPtr) const;
	int		GetWindowRgn(HRGN hRgn) const;
	int		GetWindowText(TCHAR* textPtr, int nMaxCount) const;
	int		GetWindowTextLength() const;

	BOOL	HideCaret() const;
	BOOL	HideWindow() const;
	BOOL	HiliteMenuItem(HMENU hMenu, UINT uIDHiliteItem, UINT uHilite) const;

	void	Invalidate(BOOL bErase = TRUE) const;
	BOOL	InvalidateRect(const RECT* rectPtr, BOOL bErase = TRUE) const;
	BOOL	InvalidateRect(BOOL bErase = TRUE) const;
	BOOL	InvalidateRgn(HRGN hRgn, BOOL bErase) const;

	BOOL	IsChild() const;
	BOOL	IsDialogMessage(MSG* MsgPtr) const;
	UINT	IsDlgButtonChecked(int nIDButton) const;
	BOOL	IsIconic() const;
	BOOL	IsTouchWindow(ULONG* uFlagsPtr) const;
	BOOL	IsWindowEnabled() const;
	BOOL	IsWindowVisible() const;
	BOOL	IsZoomed() const;

	BOOL	KillTimer(UINT_PTR uIDEvent) const;
	BOOL	LockWindowUpdate() const;

	void	MapWindowPoints(HWND hWndTo, POINT* ptPtr, UINT uPoints) const;
	void	MapWindowPoints(HWND hWndTo, POINT* ptPtr) const;
	void	MapWindowPoints(HWND hWndTo, RECT* rcPtr) const;
	int		MessageBox(const TCHAR* textPtr, const TCHAR*	szCaptionPtr = NULL, UINT uType = MB_OK) const;
	BOOL	ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT uFlags = 0) const;
	BOOL	ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT uFlags = 0) const;
	BOOL	MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE) const;
	BOOL	MoveWindow(const RECT* rcPtr, BOOL bRepaint = TRUE) const;
	void	NotifyWinEvent(DWORD dwEvent, LONG idObject, LONG idChild) const;

	BOOL	PostMessage(UINT uMessage, WPARAM wParam = 0, LPARAM lParam = 0) const;
	void	Print(HDC hDC, DWORD dwFlags) const;
	BOOL	RedrawWindow(const RECT* rcUpdatePtr, UINT uFlags = RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN) const;
	BOOL	RedrawWindow(HRGN hRgn, UINT flags = RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN) const;
	BOOL	RedrawWindow(UINT flags = RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN) const;
	int		ReleaseDC(HDC hDC) const;

	BOOL	ScreenToClient(POINT* pointPtr) const;
	BOOL	ScreenToClient(RECT* rectPtr) const;
	BOOL	ScrollWindow(int xAmount, int yAmount, const RECT* rcPtr, const RECT* rcClipPtr) const;
	int		ScrollWindowEx(int dx, int dy, const RECT *rcScrollPtr, const RECT *rcClipPtr, HRGN hrgnUpdate, RECT* rcUpdatePtr, UINT flags) const;
	LRESULT	SendDlgItemMessage(int nIDDlgItem, UINT uMessage, WPARAM wParam = 0, LPARAM lParam = 0) const;
	LRESULT	SendMessage(UINT uMessage, WPARAM wParam = 0, LPARAM lParam = 0) const;
	BOOL	SendNotifyMessage(UINT uMessage, WPARAM wParam = 0, LPARAM lParam = 0) const;
	HWND	SetActiveWindow() const;
	HWND	SetCapture() const;
	BOOL	SetCaretPos(const POINT* pointPtr) const;
	BOOL	SetCaretPos(int nPosx, int nPosy) const;
	ULONG_PTR SetClassLongPtr(int nIndex, LONG_PTR dwNewLong) const;
	BOOL	SetClientSize(int nWidth, int nHeight) const;
	int		SetDlgCtrlID(int nID) const;
	BOOL	SetDlgItemInt(int nIDDlgItem, UINT uValue, BOOL bSigned = FALSE);
	BOOL	SetDlgItemText(int nIDDlgItem, const TCHAR* szTextPtr);
	DWORD	SetExStyle(DWORD dwExStyle) const;
	HWND	SetFocus(HWND hWnd = reinterpret_cast<HWND>(-1)) const;
	void	SetFont(HFONT hFont, BOOL bRedraw = TRUE) const;
	BOOL	SetForegroundWindow() const;
	HICON	SetIcon(HICON hIcon, BOOL bBigIcon) const;
	BOOL	SetMenu(HMENU hMenu) const;
	HWND	SetParent(HWND hWndNewParent) const;
	BOOL	SetRedraw(BOOL bRedraw = TRUE) const;
	int		SetScrollInfo(int nBar, const SCROLLINFO* siPtr, BOOL bRedraw) const;
	int		SetScrollPos(int nBar, int nPos, BOOL bRedraw) const;
	BOOL	SetScrollRange(int nBar, int nMinPos, int nMaxPos, BOOL bRedraw) const;
	DWORD	SetStyle(DWORD dwStyle) const;
	UINT_PTR SetTimer(UINT_PTR nIDEvent, UINT uElapse, TIMERPROC fnTimerFunc) const;
	LONG_PTR SetWindowLongPtr(int nIndex, LONG_PTR dwNewLong) const;
	BOOL	SetWindowPlacement(const WINDOWPLACEMENT* wndplPtr) const;
	BOOL	SetWindowPos(HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT uFlags) const;
	BOOL	SetWindowPosition(int x, int y) const;
	BOOL	SetWindowSize(int nWidth, int nHieght) const;
	int		SetWindowRgn(HRGN hRgn, BOOL bRedraw = TRUE) const;
	BOOL	SetWindowText(const TCHAR* textPtr) const;
	BOOL	ShowCaret() const;
	BOOL	ShowOwnedPopups(BOOL fShow) const;
	BOOL	ShowScrollBar(int wBar, BOOL bShow);
	BOOL	ShowWindow(int nCmdShow = SW_SHOWNORMAL) const;

	BOOL	UpdateWindow() const;
	BOOL	ValidateRect(const RECT* rcPtr) const;
	BOOL	ValidateRect() const;
	BOOL	ValidateRgn(HRGN hRgn) const;
	HWND	WindowFromPoint(POINT stPoint) const;

protected:
	/* Windows Callback 訊息處理 */
	LRESULT PassToNextWndProc(UINT uMessage, WPARAM wParam, LPARAM lParam);
	LRESULT DefaultWndProc(UINT uMessage, WPARAM wParam, LPARAM lParam);
	virtual void	SafeUserRelease() = 0;
	virtual LRESULT	WmNcCreate(WPARAM wParam, LPARAM lParam);
	virtual LRESULT	WmCreate(WPARAM wParam, LPARAM lParam);
	virtual void	WmDestroy(WPARAM wParam, LPARAM lParam);
	virtual void	WmNcDestroy(WPARAM wParam, LPARAM lParam);
	virtual	void	WmClose(WPARAM wParam, LPARAM lParam);
	virtual void	WmUserCreate(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WndProc(UINT uMessage, WPARAM wParam, LPARAM lParam);

	virtual void SetSafeType(EmCtrls eType);
	virtual void SetSafeFont(HFONT hFont);
	virtual void SetSafeHwnd(HWND hWnd);
	virtual void SetSafePrevCallback(LONG_PTR lCallback);
	virtual BOOL BindWindow(HWND hWnd);
	virtual void LooseWindow();

	/* Destory window and release */
	void SafeWndsDestroy(int nExitCode = 0);
	void SafeWndsRelease();
	void SafeWndsWaiting();

	BOOL SafeRegisterClass(const WNDSFRAME* smPtr);
	HWND SafeCreateWindows(const WNDSFRAME* smPtr);

	const TCHAR* GetControlsClassName(UINT uType) const;
	const TCHAR* GetControlsClassName() const;

protected:
	EmCtrls		m_eType;			//!< 控制項的種類
	HWND		m_hWnd;				//!< 保存綁定的視窗操作代碼
	LONG_PTR	m_lPrevCallback;	//!< 保存先前的訊息處理 Callback 位址
	HFONT		m_hFont;			//!< 保存使用者自定義字型操作代碼
	BOOL		m_bModal;			//!< 是否為 Dialog modal

private:
	DmWnds(const DmWnds&) = delete;				//!< Disable copy construction
	DmWnds& operator=(const DmWnds&) = delete;	//!< Disable assignment operator
	void InitCommControls() const;
	static LRESULT CALLBACK SafeWndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
};

#endif // !ODMC_WNDS_DMWNDS
