/**************************************************************************//**
 * @file	frame.hh
 * @brief	Class CxFrame header file
 * @date	2000-10-10
 * @date	2018-09-01
 * @author	Swang
 *****************************************************************************/
#ifndef WINAPP_SAMPLEWINDOW_FRAME
#define WINAPP_SAMPLEWINDOW_FRAME
#include "define.hh"

class CxFrame : public DmFrame
{
public:
	CxFrame();
	virtual ~CxFrame();

protected:
	virtual LRESULT WndProc(UINT uMessage, WPARAM wParam, LPARAM lParam) override;
	virtual void	DoSafeRelease() override;

private:
	CxFrame(const CxFrame&) = delete;				//!< Disable copy construction
	CxFrame& operator=(const CxFrame&) = delete;	//!< Disable assignment operator
};

#endif // !WINAPP_SAMPLEWINDOW_FRAME
