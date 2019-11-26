/**************************************************************************//**
 * @file	apps.cc
 * @brief	LPRApps 類別成員函數定義
 * @date	2019-11-20
 * @date	2019-11-20
 * @author	Swang
 *****************************************************************************/
#include "apps.hh"

/**
 *	@brief	LPRApps 建構式
 *	@return	此函數沒有返回值
 */
LPRApps::LPRApps()
	: DmWinapp()
	, m_cMainWindow(NULL) {
}

/**
 *	@brief	LPRApps 解構式
 *	@return	此函數沒有返回值
 */
LPRApps::~LPRApps()
{
	// 釋放成員
	SAFE_DELETE(m_cMainWindow);
}

#if OLPR_OVERRIDE_RUN
/**
 *	@brief	執行視窗程式
 *	@return	<b>型別: int</b> \n 返回值為視窗程式結束碼。
 */
int LPRApps::Run()
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
BOOL LPRApps::InitComponent()
{
	auto cFrame = new (std::nothrow) LPRFrame();

	for (;;) {
		SAFE_DELETE(m_cMainWindow);

		if (cFrame == NULL) {
			break;
		}

		if (!cFrame->CreateSafeWindow()) {
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
