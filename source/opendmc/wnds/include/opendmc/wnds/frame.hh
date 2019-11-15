/**************************************************************************//**
 * @file	frame.hh
 * @brief	DmFrame 建立視窗框架類別宣告 Header
 * @date	2000-10-10
 * @date	2018-08-05
 * @author	Swang
 *****************************************************************************/
#ifndef	ODMC_WNDS_FRAME_HH
#define	ODMC_WNDS_FRAME_HH
#include "wnds.hh"

class DmFrame : public DmWnds
{
public:
	DmFrame();
	virtual ~DmFrame();

private:
	DmFrame(const DmFrame&) = delete;				//!< Disable copy construction
	DmFrame& operator=(const DmFrame&) = delete;	//!< Disable assignment operator
};

#endif // !ODMC_WNDS_FRAME_HH
