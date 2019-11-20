/**************************************************************************//**
 * @file	smw_apps.hh
 * @brief	SmwApp 類別宣告 Header
 * @date	2018-05-20
 * @date	2018-05-20
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_HSLPR_LPRAPPS_HH
#define	ODMC_HSLPR_LPRAPPS_HH
#include "lpr_frame.hh"
#define	LPR_OVERRIDE_RUN	0 

 /**
  *	@class CxLprApps
  *	@brief CxLprApps 視窗程序啟用類別
  */
class CxLprApps : public DmWinapp
{
public:
	CxLprApps();
	virtual ~CxLprApps();

#if LPR_OVERRIDE_RUN
	int Run() override;
#endif

protected:
	BOOL InitComponent() override;

private:
	CxLprApps(const CxLprApps&) = delete;				//!< Disable copy construction
	CxLprApps& operator=(const CxLprApps&) = delete;	//!< Disable assignment operator

private:
	CxLprFrame* m_cMainWindow;
};

#endif // !ODMC_HSLPR_LPRAPPS_HH
