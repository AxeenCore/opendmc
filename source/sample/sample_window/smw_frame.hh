/**************************************************************************//**
 * @file	smw_frame.hh
 * @brief	SmwFrame 類別宣告 Header
 * @date	2018-05-20
 * @date	2018-05-20
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_SAMPLEWINDOW_FRAME_HH
#define	ODMC_SAMPLEWINDOW_FRAME_HH
#include "smw_define.hh"

/**
 *	@class CxSmwFrame
 *	@brief CxSmwFrame Sample Window 主視窗類別
 */
class CxSmwFrame : public DmWnds
{
public:
	CxSmwFrame();
	virtual ~CxSmwFrame();
	BOOL CreateSafe();

protected:
	virtual void SafeUserRelease() override;
	virtual LRESULT WndProc(UINT uMessage, WPARAM wParam, LPARAM lParam) override;

private:
	CxSmwFrame(const CxSmwFrame&) = delete;				//!< Disable copy construction
	CxSmwFrame& operator=(const CxSmwFrame&) = delete;	//!< Disable assignment operator
};


#endif // !ODMC_SAMPLEWINDOW_FRAME_HH
