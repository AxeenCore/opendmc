/**************************************************************************//**
 * @file	frame.cc
 * @brief	Class CxFrame function member
 * @date	2000-10-10
 * @date	2018-09-01
 * @author	Swang
 *****************************************************************************/
#include "frame.hh"

/**
 *	@brief	CxFrame 建構式
 */
CxFrame::CxFrame() { }

/**
 *	@brief	CxFrame 解構式
 */
CxFrame::~CxFrame() { this->SafeWndsDestroy(); }


LRESULT CxFrame::WndProc(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = 0;

	switch (uMessage)
	{
	case WM_CREATE:
		this->WmCreate(wParam, lParam);
		break;
	case WM_CLOSE:
		this->WmClose(wParam, lParam);
		break;
	default:
		return this->DefaultWndProc(uMessage, wParam, lParam);
	}
	return lResult;
}

void CxFrame::SafeUserRelease()
{
	// TODO
}
