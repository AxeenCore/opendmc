/**************************************************************************//**
 * @file	lpr_frame.hh
 * @brief	CxLprFrame 類別宣告 Header
 * @date	2019-11-20
 * @date	2019-11-20
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_HSLPR_LPRFRAME_HH
#define	ODMC_HSLPR_LPRFRAME_HH
#include "lpr_define.hh"

 /**
  *	@class CxLprFrame
  *	@brief CxLprFrame 主視窗類別
  */
class CxLprFrame : public DmWnds
{
public:
	CxLprFrame();
	virtual ~CxLprFrame();
	BOOL CreateSafe();

protected:
	virtual void SafeUserRelease() override;
	virtual LRESULT WndProc(UINT uMessage, WPARAM wParam, LPARAM lParam) override;

private:
	CxLprFrame(const CxLprFrame&) = delete;				//!< Disable copy construction
	CxLprFrame& operator=(const CxLprFrame&) = delete;	//!< Disable assignment operator
};

#endif // !ODMC_HSLPR_LPRFRAME_HH
