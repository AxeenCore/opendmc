/**************************************************************************//**
 * @file	struct.hh
 * @brief	dmwins library 結構、巨集、列舉宣告 Header
 * @date	2000-10-10
 * @date	2018-05-20
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_WNDS_STRUCT_HH
#define	ODMC_WNDS_STRUCT_HH

// 視窗訊息，使用者自定義訊息
#ifdef WM_USERCREATE
#undef WM_USERCREATE
#endif
#define WM_USERCREATE						(WM_APP + 0x3F01)	//!< 建立視窗訊息

// 視窗樣式
#define WNDS_COMMON_STYLE					WS_VISIBLE | WS_CLIPSIBLINGS
#define	WNDS_COMMON_CHILD_STYLE				WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_TABSTOP
#define	WNDS_COMMON_CHILD_EXSTYLE			WS_EX_CLIENTEDGE


// 常用數值定義
#define WNDS_DESTORY_OVERTIME	1000		//!< 等待視窗結束最大等待回合數
#define WNDS_DESTROY_WAITING	50			//!< 等待視窗結束，回合間時間差 (ms)

#define MAX_NULL_STRING			2			//!< 預留 NULL 字串結尾字元數
#define MAX_TAB_STRING			80			//!< 標籤最大字元數 (字串長度)，單位 TCHAR
#define NON_WINC_IDITEM			0			//!< 控制項識別碼無效值
#define MIN_WINC_IDITEM			1			//!< 控制項識別碼最小值
#define MAX_WINC_IDITEM			65535		//!< 控制項識別碼最大值

#define LV_INSERT_TOP			0	//!< 由 ListBox or ListView 最開頭新增項目
#define LV_INSERT_BOTTOM		-1	//!< 由 ListBox or ListView 最底部新增項目

/**
 *	@enum	EmLVColumn
 *	@brief	列舉 ListView 顯示位置設定值
 */
enum class EmLVColumn
{
	AlignLife = 0,				//!< ListView Column 顯示位置靠左
	AlignRight,					//!< ListView Column 顯示位置靠右
	AlignCenter,				//!< ListView Column 顯示位置居中
	AlignPassend,				//!< ListView Column 結尾識別
};

/**
 *	@enum	EmCtrls
 *	@brief	列舉 Microsoft Windows 控制項索引
 */
enum class EmCtrls : UINT {
	Empty = 0,					//!< 空值
	Button,						//!< 控制項 : Button
	Combo,						//!< 控制項 : Combo Box
	Edit,						//!< 控制項 : Edit control.
	List,						//!< 控制項 : List box.
	MDIClient,					//!< 控制項 : MDI client window.
	ScrollBar,					//!< 控制項 : Scroll bar.
	Static,						//!< 控制項 : Static control.

	ComboLBox,					//!< 控制項 : List box, list box contained in a combo box.
	DDEMLEvent,					//!< Dynamic Data Exchange Management Library (DDEML) events.
	Message,					//!< Message-only window.
	Du32768,					//!< 控制項 : Menu.
	Du32769,					//!< 控制項 : Desktop window.
	Dialog,						//!< 控制項 : Dialog box. (32770)
	Du32771,					//!< 控制項 : Task switch window.
	Du32772,					//!< 控制項 : Icon titles.

	Toolbar,					//!< 控制項 : Toolbar
	Tooltips,					//!< 控制項 : Tooltip
	Statusbar,					//!< 控制項 : Status bar
	TreeView,					//!< 控制項 : Tree view
	ListView,					//!< 控制項 : List view
	Animate,					//!< 控制項 : Animation
	Header,						//!< 控制項 : Header
	Hotkey,						//!< 控制項 : Hot-key
	Progress,					//!< 控制項 : Progress bar
	RichEdit,					//!< 控制項 : Rich edit
	UpDown,						//!< 控制項 : Up-Down
	TabControl,					//!< 控制項 : Tab
	TheEnd,						//!< 結束識別符號
};

/**
 *	@struct	WNDVERINFO
 *	@brief	opendmc_wnds 程式庫版本訊息
 */
struct WNDSVERINFO {
	unsigned int	uMajor;		//!< 主版號
	unsigned int	uMinor;		//!< 次版號
	unsigned int	uBuild;		//!< 建置碼
};
typedef WNDSVERINFO* LPWNDSVERINFO;	//!< 結構 WNDSVERINFO 指標型別

/**
 *	@struct	WNDSFRAME
 *	@brief	建立標準視窗框架結構，依據結構內容，使用 DmWnds 建立對應視窗框架
 */
struct WNDSFRAME {
	HINSTANCE		hInstance;		//!< set handle of instance (handle of process module)
	HWND			hWndParent;		//!< set handle of parent window
	WNDPROC			fnWndProc;		//!< set a window call-back function pointer, must be NULL
	const TCHAR* pszClass;		//!< pointer of class name
	const TCHAR* pszCaption;		//!< pointer of window's name
	int				nPosx;			//!< set display window x-coordinate
	int				nPosy;			//!< set display window y-coordinate
	int				nWidth;			//!< set window client-area width
	int				nHeight;		//!< set window client-area height
	UINT			uClassStyle;	//!< set register window class style
	HBRUSH			hBackground;	//!< set wndow background color
	HICON			hIcon;			//!< set handle of icon
	HICON			hIconSm;		//!< set handle of small icon
	HCURSOR			hCursor;		//!< set handle of cursor
	DWORD			dwStyle;		//!< set window style
	DWORD			dwExStyle;		//!< set window extend style
	HMENU			hMenuOrItemID;	//!< set menu headle or item id
	void* vParam;			//!< create a window parameter
};
typedef WNDSFRAME* LPWNDSFRAME;	//!< 結構 WNDSFRAME 指標型態
#define WNDS_DEFAULT_CLASSNAME		"OPENDMC_FRAME_CLASS"	//!< 預設視窗註冊名稱
#define WNDS_DEFAULT_CAPTION		"OpenDMC Sample Window"	//!< 預設視窗標題名稱
#define WNDS_DEFAULT_PROFILE		"profile.bin"			//!< 視窗版面保存檔案名稱
#define WNDS_DEFAULT_WIDTH			1280					//!< 預設視窗寬度
#define WNDS_DEFAULT_HEIGHT			720						//!< 預設視窗高度
#define WNDS_DEFAULT_STYLE			WS_OVERLAPPEDWINDOW | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX	//!< 預設視窗樣式	
#define WNDS_DEFAULT_EXSTYLE		0						//!< 預設視窗延伸樣式

/**
 *	@struct	WNDSCTRLS
 *	@brief	建立控制項之結構 constrols，依照結構定義內容建立控制項
 */
struct WNDSCTRLS {
	HINSTANCE		hInstance;		//!< the handle of module
	HWND			hParent;		//!< handle of parent window
	const TCHAR* pszClass;		//!< conctrller item common controls class name
	const TCHAR* pszCaption;		//!< conctrller item window text
	DWORD			dwStyle;		//!< conctrller item style
	DWORD			dwExStyle;		//!< conctrller item extern style
	int				nPosx;			//!< start draw position (x-coordination)
	int				nPosy;			//!< start draw position (y-coordination)
	int				nWidth;			//!< the window width
	int				nHeight;		//!< the window height
	int				nIDCItem;		//!< conctrller item id
};
typedef WNDSCTRLS* LPWNDSCTRLS;	//!< WINSCTRLS 結構指標型別

/**
 *	@struct WNDSPARAM
 *	@brief 額外參數結構，建立視窗、控制項或對話框時適用。
 */
struct WNDSPARAM {
	WPARAM	wParam;					//!< 參數 wParam
	LPARAM	lParam;					//!< 參數 lParam;
	void* dmParam1;				//!< 物件指標 (保留)
	void* dmParam2;				//!< 物件指標 (保留)
};
typedef	WNDSPARAM* LPWNDSPARAM;		//!< 結構 WINSPARAM 指標型態

#endif // !ODMC_WNDS_STRUCT_HH
