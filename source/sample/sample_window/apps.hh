/**************************************************************************//**
 * @file	apps.hh
 * @brief	Sample Window CxApps header
 * @date	2000-10-10
 * @date	2018-09-01
 * @author	Swang
 *****************************************************************************/
#ifndef SWAPP_SAMPLEWINDOW_APPS_HH
#define SWAPP_SAMPLEWINDOW_APPS_HH
#include "frame.hh"

class CxApps : public DmWinapp
{
public:
	CxApps();
	virtual ~CxApps();
	virtual int Run() override;

protected:
	virtual BOOL InitComponent() override;

private:
	CxApps(const CxApps&) = delete;				//!< Disable copy construction
	CxApps& operator=(const CxApps&) = delete;	//!< Disable assignment operator
	CxFrame* m_cFrame;
};


#endif // !SWAPP_SAMPLEWINDOW_APPS_HH
