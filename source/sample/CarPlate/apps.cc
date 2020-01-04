/**************************************************************************//**
 * @file	apps.cc
 * @brief	AppsCarPlate 類別成員函數定義
 * @date	2019-12-05
 * @date	2019-12-05
 * @author	Swang
 *****************************************************************************/
#include "apps.hh"

/**
 *	@breif	AppsCarPlate 建構式
 *	@return	此函數沒有返回值
 */
AppsCarPlate::AppsCarPlate()
	: DmWinapp()
	, m_cMainframe(NULL) {
}

/**
 *	@brief	AppsCarPlate 解構式
 *	@return	此函數沒有返回值
 */
AppsCarPlate::~AppsCarPlate()
{
	SAFE_DELETE(m_cMainframe)}

/**
 *	@brief	建立程序元件
 *	@return	<b>型別: BOOL</b> \n 若函數運作成功，返回值為非零值。 \n 若函數運作失敗，則返回值為零。
 */
BOOL AppsCarPlate::InitComponent()
{
	auto cFrame = new (std::nothrow) CarMainform();

	for (;;) {
		SAFE_DELETE(m_cMainframe);

		if (cFrame == NULL) {
			break;
		}

		if (!cFrame->CreateMine()) {
			break;
		}

		this->DefinitionMainframe(reinterpret_cast<LONG_PTR>(cFrame));
		m_cMainframe = cFrame;
		return TRUE;
	}

	SAFE_DELETE(cFrame);
	this->DefinitionMainframe(0);
	return FALSE;
}
