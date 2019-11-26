/**************************************************************************//**
 * @file	object.cc
 * @brief	class DmWndsObject function member
 * @date	2000-10-10
 * @date	2018-08-10
 * @author	Swang
 *****************************************************************************/
#include "opendmc/wnds/object.hh"

/**
 *	@brief	DmWndsObject 建構式
 */
DmWndsObject::DmWndsObject() { }

/**
 *	@brief	DmWndsObject 解構式
 */
DmWndsObject::~DmWndsObject() { }

/**
 *	@brief	取得 opendmc_wnds 程式庫版本
 *	@return	<b>型別: UINT32</b> 版本碼
 *		- HIWORD	\n 版本碼 (HIBYTE 主版號，LOBYTE 次版號)
 *		- LOWORD	\n 建置碼
 */
UINT32 DmWndsObject::GetVersionInfo()
{
	auto verinfo = static_cast<UINT32>(WNDS_VER_MAJOR);
	verinfo = (verinfo << 8) + static_cast<UINT32>(WNDS_VER_MINOR);
	verinfo = (verinfo << 8) + static_cast<UINT32>(WNDS_VER_BUILD);
	return verinfo;
}

/**
 *	@brief	取得 opendmc_wnds 程式庫版本
 *	@param	smPtr 指向 WNDVERINFO 結構指標，用來存放 opendmc_wnds 版本資訊
 *	@return	此函數沒有返回值
 */
void DmWndsObject::GetVersionOnfo(WNDSVERINFO* smPtr)
{
	if (smPtr != NULL) {
		smPtr->uMajor = static_cast<unsigned int>(WNDS_VER_MAJOR);
		smPtr->uMinor = static_cast<unsigned int>(WNDS_VER_MINOR);
		smPtr->uBuild = static_cast<unsigned int>(WNDS_VER_BUILD);
	}
}
