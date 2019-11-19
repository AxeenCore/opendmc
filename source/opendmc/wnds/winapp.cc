/**************************************************************************//**
 * @file	winapp.cc
 * @brief	Windows 程序類別成員函數定義
 * @date	2000-10-10
 * @date	2019-01-11
 * @author	Swang
 *****************************************************************************/
#include "opendmc/wnds/winapp.hh"

///////////////////////////////////////////////////////////////////////////////
//
// class DmWinapp
//
///////////////////////////////////////////////////////////////////////////////

/**
 *	@brief	DmWinapp 建構式
 *	@return	此函數沒有返回值
 */
DmWinapp::DmWinapp()
	: DmWndsObject()
	, m_bInitialize(FALSE)
	, m_hInstance(NULL)
	, m_hResource(NULL)
	, m_lMainframe(0) {

	static LONG_PTR llAddress = 0;
	for (;;) {

		if (DmWinapp::SetnGetThis() != NULL) {
			break;
		}

		MEMORY_BASIC_INFORMATION mbi;
		::memset(&mbi, 0, sizeof(mbi));
		::VirtualQuery(&llAddress, &mbi, sizeof(mbi));
		if (mbi.AllocationBase == NULL) {
			break;
		}

		m_hInstance = reinterpret_cast<HINSTANCE>(mbi.AllocationBase);
		m_hResource = reinterpret_cast<HINSTANCE>(mbi.AllocationBase);
		m_bInitialize = DmWinapp::SetnGetThis(this) != NULL;
		break;
	}
}

/**
 *	@brief	DmWinapp 解構式
 *	@return	此函數沒有返回值
 */
DmWinapp::~DmWinapp() {  }

/**
 *	@brief	建立程序內容 & 執行訊息迴圈
 *	@return	<b>型別: int</b> \n 返回值為 Windows 應用程式結束碼
 */
int DmWinapp::Run()
{
	auto nExitCode = (-1);

	if (this->IsReady()) {
		if (this->InitComponent()) {
			nExitCode = this->MessageLoopStandardEx();
		}
	}
	return nExitCode;
}

/**
 *	@brief	應用程式環境是否準備就去
 *	@return	<b>型別: BOOL</b> \n 若類別物件已就緒返回值為非零值，若未就緒則返回值為零。
 */
BOOL DmWinapp::IsReady() const { return m_bInitialize; }

/**
 *	@brief	取得應用程序實例代碼
 *	@return	<b>型別: HINSTANCE</b>
 *		\n 若程序 instance 已存在返回值為非零值。
 *		\n 若程序 instance 不存在則返回值為零。
 */
HINSTANCE DmWinapp::GetInstanceHandle() const
{
	if (this->IsReady()) {
		return m_hInstance;
	}
	return NULL;
}

/**
 *	@brief	取得應用程序資源實例代碼
 *	@return	<b>型別: HINSTANCE</b>
 *		\n 若程序 instance 已存在返回值為非零值。
 *		\n 若程序 instance 不存在則返回值為零。
 */
HINSTANCE DmWinapp::GetResourceHandle() const
{
	if (this->IsReady()) {
		return m_hResource;
	}
	return NULL;
}

/**
 *	@brief	取得標示的主視窗物件
 *	@return	<b>型別: LONG_PTR</b> \n 若主事窗物件已存在反為值為非零值，否則返回值為零。
 */
LONG_PTR DmWinapp::GetMainframe() const
{
	if (this->IsReady()) {
		return m_lMainframe;
	}
	return static_cast<LONG_PTR>(NULL);
}

/**
 *	@breif	初始化使用者自定義應用程式內容
 *	@return	<b>型別: BOOL</b> \n 此函數在此時永遠返回 FALSE，實際作用由繼承者撰寫。
 */
BOOL DmWinapp::InitComponent()
{
	const TCHAR* szCaption = TEXT("opendmc_wnds");
	const TCHAR* szText = TEXT("Oops! this virtual member function must inheritance,  and programming in derived class.");
	::MessageBox(NULL, szText, szCaption, MB_ICONHAND | MB_OK);
	return FALSE;
}

/**
 *	@brief	Windows 訊息迴圈 (活躍，不等候訊息發生)
 *	@return	<b>型別: int</b> \n 返回值為 Window 程序結束返回碼。
 */
int DmWinapp::MessageLoopActive()
{
	MSG message;
	::memset(&message, 0, sizeof(message));

	for (;;) {
		// PeekMessage 特性。
		// 如果消息可用，則返回值為非零值。
		// 如果沒有可用消息，則返回值為零。
		while (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}

		// 結束訊息迴圈 ?
		if (message.message == WM_QUIT) {
			break;
		}

		// TODO: 在此運作要持續處理動作項目 (如動態 UI，計時或遊戲)
	}
	return static_cast<int>(LOWORD(message.wParam));
}

/**
 *	@brief	Windows 訊息迴圈 (標準式，等候著、直到訊息發生)
 *	@return	<b>型別: int</b> \n 返回值為 Window 程序結束返回碼。
 */
int DmWinapp::MessageLoopStandard()
{
	BOOL bRet;
	MSG message;

	::memset(&message, 0, sizeof(message));
	for (;;) {
		bRet = ::GetMessage(&message, NULL, 0, 0);

		// bRet > 0 表示有訊息必須處理
		if (bRet > 0) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		// bRet < 0 表示有錯誤發生
		else if (bRet < 0) {
			// Handle or log the error; possibly exit
			// ...
		}
		// bRet = 0 表示要結束程式
		else break;
	}
	return static_cast<int>(LOWORD(message.wParam));
}

/**
 *	@brief	Windows 訊息迴圈 (標準式，等候著、直到訊息發生)
 *	@return	<b>型別: int</b> \n 返回值為 Window 程序結束返回碼。
 */
int DmWinapp::MessageLoopStandardEx()
{
	MSG message;
	::memset(&message, 0, sizeof(message));
	while (::GetMessage(&message, NULL, 0, 0)) {
		::TranslateMessage(&message);
		::DispatchMessage(&message);
	}
	return static_cast<int>(LOWORD(message.wParam));
}

/**
 *	@brief	指定主視窗
 *	@param[in]	lParam	主視窗物件
 *	@return	此函數沒有返回值
 */
void DmWinapp::DefinitionMainframe(LONG_PTR lParam)
{
	m_lMainframe = lParam;
}

/**
 *	@brief	設定或取得 DmWinapp 物件
 *	@param[in] thisPtr	物件 DmWinapp 指標
 *	@param[in] bReset	是否進行重設，預設值 FALSE
 */
DmWinapp* DmWinapp::SetnGetThis(DmWinapp* thisPtr, BOOL bReset)
{
	static DmWinapp* myApps = NULL;

	if (myApps == NULL) {
		myApps = thisPtr;
	}
	else assert(thisPtr == NULL);

	if (bReset) { myApps = NULL; }

	return myApps;
}



///////////////////////////////////////////////////////////////////////////////
//
// extren function
//
///////////////////////////////////////////////////////////////////////////////

/**
 *	@biref	取得 DmWinapp 物件
 *	@return	<b>型別: DmWinapp</b> \n 返回值為物件 DmWinapp 的參考。
 */
DmWinapp& GetWinapp()
{
	DmWinapp* dmPtr = DmWinapp::SetnGetThis();
	assert(dmPtr);
	return *dmPtr;
}
