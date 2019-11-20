/**************************************************************************//**
 * @file	smw_apps.cc
 * @brief	SmwApp 類別成員函數定義
 * @date	2018-05-20
 * @date	2018-05-20
 * @author	Swang
 *****************************************************************************/
#include "smw_apps.hh"

/**
 *	@brief	CxSmwApp 建構式
 */
CxSmwApp::CxSmwApp() 
	: DmWinapp()
	, m_cMainWindow(NULL) {
}

/**
 *	@brief	CxSmwApp 解構式
 */
CxSmwApp::~CxSmwApp()
{
	SAFE_DELETE(m_cMainWindow);
}

#if SMW_OVERRIDE_RUN
/**
 *	@brief	執行視窗程式
 *	@return	<b>型別: int</b> \n 返回值為視窗程式結束碼。
 */
int CxSmwApp::Run()
{
	auto nExitCode = static_cast<int>(-1);
	if (this->IsReady()) {
		if (this->InitComponent()) {
			nExitCode = this->MessageLoopStandard();
		}
	}
	return nExitCode;
}
#endif

/**
 *	@brief	初始與建立視窗程序內容
 *	@return	<b>型別: BOOL</b>
 *		\n 若初始成功，返回值為非零值。
 *		\n 若初始失敗，返回值為零。
 */
BOOL CxSmwApp::InitComponent()
{
	auto cFrame = new (std::nothrow) CxSmwFrame();

	for (;;) {
		SAFE_DELETE(m_cMainWindow);

		if (cFrame == NULL) {
			break;
		}

		if (!cFrame->CreateSafe()) {
			break;
		}

		this->DefinitionMainframe(reinterpret_cast<LONG_PTR>(cFrame));
		m_cMainWindow = cFrame;
		return TRUE;
	}

	SAFE_DELETE(cFrame);
	this->DefinitionMainframe(0);
	return FALSE;
}
