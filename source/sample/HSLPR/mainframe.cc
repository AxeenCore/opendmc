/**************************************************************************//**
 * @file	mainframe.cc
 * @brief	CxMainframe 成員函數
 * @date	2019-11-18
 * @date	2019-11-18
 * @author	Swang
 *****************************************************************************/
#include "mainframe.hh"

/**
 *	@brief	CxMainframe 建構式
 */
CxMainframe::CxMainframe() : DmWnds() { }

/**
 *	@brief	CxMainframe 解構式
 */
CxMainframe::~CxMainframe() { this->SafeWndsDestroy(); }

/**
 *	@brief	[重載] 釋放類別中使用資源或物件
 *	@return	此函數沒有返回值
 */
void CxMainframe::SafeUserRelease()
{
}

/**
 *	@brief [重載] 視窗訊息處理 Callback
 *	@param[in] uMessage	視窗訊息
 *	@param[in] wParam	參數 1
 *	@param[in] lParam	參數 2
 *	@return <b>型別: LRESULT</b> \n 訊息處理後，返回處理結果
 */
LRESULT CxMainframe::WndProc(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	auto lResult = static_cast<LRESULT>(0);

	switch (uMessage)
	{
	case WM_CREATE:
		this->WmCreate(wParam, lParam);
		break;
	case WM_NCDESTROY:
		this->WmNcDestroy(wParam, lParam);
		break;
	case WM_CLOSE:
		this->WmClose(wParam, lParam);
		break;
	default:
		return this->DefaultWndProc(uMessage, wParam, lParam);
	}
	return lResult;
}
