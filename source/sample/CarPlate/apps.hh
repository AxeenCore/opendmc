/**************************************************************************//**
 * @file	apps.hh
 * @brief	AppsCarPlate 類別宣告 Header
 * @date	2019-12-05
 * @date	2019-12-05
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_CARPLATE_APPS_HH
#define	ODMC_CARPLATE_APPS_HH
#include "mainform.hh"

/**
 *	@class AppsCarPlate
 *	@brief CarPlate 程式啟動類別
 */
class AppsCarPlate : public DmWinapp
{
public:
	AppsCarPlate();
	virtual ~AppsCarPlate();

protected:
	BOOL InitComponent() override;

private:
	AppsCarPlate(const AppsCarPlate&) = delete;				//!< Disable copy construction
	AppsCarPlate& operator=(const AppsCarPlate&) = delete;	//!< Disable assignment operator

	CarMainform* m_cMainframe;
};


#endif // !ODMC_CARPLATE_APPS_HH

