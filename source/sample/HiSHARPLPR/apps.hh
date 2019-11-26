/**************************************************************************//**
 * @file	apps.hh
 * @brief	LPRApps 類別宣告
 * @date	2019-11-20
 * @date	2019-11-20
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_HISHARPLPR_APPS_HH
#define	ODMC_HISHARPLPR_APPS_HH
#include "frame.hh"
#define	OLPR_OVERRIDE_RUN	1

 /**
  *	@class LPRApps
  *	@brief LPRApps 視窗程序啟用類別
  */
class LPRApps : public DmWinapp
{
public:
	LPRApps();
	virtual ~LPRApps();

#if OLPR_OVERRIDE_RUN
	int Run() override;
#endif

protected:
	BOOL InitComponent() override;

private:
	LPRApps(const LPRApps&) = delete;				//!< Disable copy construction
	LPRApps& operator=(const LPRApps&) = delete;	//!< Disable assignment operator

private:
	LPRFrame* m_cMainWindow;
};

#endif // !ODMC_HISHARPLPR_APPS_HH
