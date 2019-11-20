/**************************************************************************//**
 * @file	smw_apps.hh
 * @brief	SmwApp 類別宣告 Header
 * @date	2018-05-20
 * @date	2018-05-20
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_SAMPLEWINDOW_APPS_HH
#define	ODMC_SAMPLEWINDOW_APPS_HH
#include "smw_frame.hh"
#define	SMW_OVERRIDE_RUN	0 

/**
 *	@class CxSmwApp
 *	@brief CxSmwApp 視窗程序啟用類別
 */
class CxSmwApp : public DmWinapp
{
public:
	CxSmwApp();
	virtual ~CxSmwApp();

#if SMW_OVERRIDE_RUN
	int Run() override;
#endif

protected:
	BOOL InitComponent() override;

private:
	CxSmwApp(const CxSmwApp&) = delete;				//!< Disable copy construction
	CxSmwApp& operator=(const CxSmwApp&) = delete;	//!< Disable assignment operator

private:
	CxSmwFrame* m_cMainWindow;
};

#endif // !ODMC_SAMPLEWINDOW_APPS_HH
