/**************************************************************************//**
 * @file	object.hh
 * @brief	opendmc Header
 * @date	2000-10-10
 * @date	2018-08-10
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_WNDS_OBJECT_HH
#define ODMC_WNDS_OBJECT_HH
#include "opendmc/conf/configure.hh"
#include "struct.hh"

#define	ODMC_WNDS_MAJOR		1
#define ODMC_WNDS_MINOR		18
#define ODMC_WNDS_BUILD		1112

/**
 *	@class DmWndsObject
 *	@brief DmWndsObject OpenDmc 基底類別
 */
class DmWndsObject
{
public:
	DmWndsObject();
	virtual ~DmWndsObject();
	virtual UINT32 GetVersionInfo();
	virtual void GetVersionOnfo(WNDSVERINFO* smPtr);

private:
	DmWndsObject(const DmWndsObject&) = delete;				//!< Disable copy construction
	DmWndsObject& operator=(const DmWndsObject&) = delete;	//!< Disable assignment operator
};




#endif // !ODMC_WNDS_OBJECT_HH
