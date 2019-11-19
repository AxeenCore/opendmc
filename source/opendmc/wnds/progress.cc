/**************************************************************************//**
 * @file	progress.cc
 * @brief	DmProgress 進度框類別成員函數
 * @date	2019-09-19
 * @date	2018-09-19
 * @author	Swang
 *****************************************************************************/
#include "opendmc/wnds/progress.hh"

/**
 *	@brief	DmProgressBar 建構式
 *	@return	此函數沒有返回值
 */
DmProgressBar::DmProgressBar() : DmWnds() { }

/**
 *	@brief	DmProgressBar 解構式
 *	@return	此函數沒有返回值
 */
DmProgressBar::~DmProgressBar() { this->SafeWndsDestroy(); }

/**
 *	@brief	OffsetPos
 *	@param[in] nInc 參照當前進度條位置後的增量值
 *	@return	<b>型別: int</b> \n 增量前的進度條位置。
 */
int DmProgressBar::OffsetPos(int nInc) const
{
	assert(this->IsWindow());
	// PBM_DELTAPOS
	// wParam = 要增量的數值
	// lParam = 不使用，必須為零
	return static_cast<int>(::SendMessage(*this, PBM_DELTAPOS, static_cast<WPARAM>(nInc), static_cast<LPARAM>(0)));
}

/**
 *	@brief	取得進度條顏色
 *	@return	<b>型別: COLORREF</b> \n 返回值為進度條顏色值，若值為 CLR_DEFAULT 代表使用系統預設值。
 */
COLORREF DmProgressBar::GetBarColor() const
{
	assert(this->IsWindow());
	// PBM_GETBARCOLOR
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<COLORREF>(::SendMessage(*this, PBM_GETBARCOLOR, static_cast<WPARAM>(0), static_cast<LPARAM>(0)));
}

/**
 *	@brief	取得進度條背景顏色
 *	@return	<b>型別: COLORREF</b> \n 返回值為進度條背景顏色，若值為 CLR_DEFAULT 代表使用系統預設值。
 */
COLORREF DmProgressBar::GetBkColor() const
{
	assert(this->IsWindow());
	// PBM_GETBKCOLOR
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<COLORREF>(::SendMessage(*this, PBM_GETBKCOLOR, static_cast<WPARAM>(0), static_cast<LPARAM>(0)));
}

/**
 *	@brief	取得進度條當前位置
 *	@return	<b>型別: int</b> \n 返回值為進度條當前位置。
 */
int DmProgressBar::GetPos() const
{
	assert(this->IsWindow());
	// PBM_GETPOS
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<int>(::SendMessage(*this, PBM_GETPOS, static_cast<WPARAM>(0), static_cast<LPARAM>(0)));
}

/**
 *	@brief	取得進度條範圍上限、範圍下限
 *	@param[in]	lowLimit 指定取得上限值(FALSE)，下限值(TRUE)
 *	@return	<b>型別: int</b> \n 返回值為依據 lowLimit 指定種類返回對應資料，範圍上限值或範圍下限值。
 */
int DmProgressBar::GetRange(BOOL lowLimit) const
{
	assert(this->IsWindow());
	// PBM_GETRANGE
	// wParam = 指定要取得的種類 (範圍上限值? 範圍下限值?)
	// lParam = 指向 PBRANGE
	return static_cast<int>(::SendMessage(*this, PBM_GETRANGE, static_cast<WPARAM>(lowLimit), static_cast<LPARAM>(NULL)));
}

/**
 *	@brief	取得進度條範圍上限、範圍下限。
 *	@param[in]	lowLimit 指定取得範圍上限值(FALSE)，範圍下限值(TRUE)。
 *	@param[out]	rangePtr 指向結構 PBRANGE 緩衝區位址，接收進度條的範圍值。
 *	@return	<b>型別: int</b>
 *		\n 返回值為依據 lowLimit 指定種類返回對應資料，範圍上限或範圍下限。
 *		\n 若 rangePtr 不為 NULL，則會將範圍資料，輸出到指定的 PBRANGE 緩衝區裡。
 *	@remark <b>額外說明</b> \n 不論 reagePtr 是否為 NULL, 返回值會依據 lowLimit 的指定返回定應資料。
 */
int DmProgressBar::GetRange(BOOL lowLimit, PBRANGE* rangePtr) const
{
	assert(this->IsWindow());
	// PBM_GETRANGE
	// wParam = 指定要取得的種類 (上限? 下限?)
	// lParam = 指向 PBRANGE
	return static_cast<int>(::SendMessage(*this, PBM_GETRANGE, static_cast<WPARAM>(lowLimit), reinterpret_cast<LPARAM>(rangePtr)));
}

/**
 *	@brief	取得進度條狀態
 *	@return	<b>型態: int</b> \n 返回值為目前進度條的狀態
 *	@remark	<b>狀態值額外說明</b>
 *		- PBST_NORMAL	\n In progress.
 *		- PBST_ERROR	\n Error.
 *		- PBST_PAUSED	\n Paused.
 */
int DmProgressBar::GetState() const
{
	assert(this->IsWindow());
	// PBM_GETSTATE
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<int>(::SendMessage(*this, PBM_GETSTATE, static_cast<WPARAM>(0), static_cast<LPARAM>(0)));
}

/**
 *	@brief	取得目前進度條的步進增量值
 *	@return	<b>型別: int</b> \n 返回值目前步驟的為進度條的增量值
 */
int DmProgressBar::GetStep() const
{
	assert(this->IsWindow());
	// PBM_GETSTEP
	// wParam = 不使用，必須為零
	// lParam = 不使用，必須為零
	return static_cast<UINT>(::SendMessage(*this, PBM_GETSTEP, static_cast<WPARAM>(0), static_cast<LPARAM>(0)));
}

/**
 *	@brief	設定進度條顏色
 *	@param[in] colBar 要設定的顏色值
 *	@return	<b>型別: COLORREF</b> \n 返回設定前的進度條顏色，如果進度條顏色為系統預設，則返回 CLR_DEFAULT。
 */
COLORREF DmProgressBar::SetBarColor(COLORREF colBar) const
{
	assert(this->IsWindow());
	// PBM_SETBARCOLOR
	// wParam = 不使用，必須為零
	// lParam = 要設定的顏色值
	return static_cast<COLORREF>(::SendMessage(*this, PBM_SETBARCOLOR, static_cast<WPARAM>(0), static_cast<LPARAM>(colBar)));
}

/**
 *	@brief	設定進度條背景顏色
 *	@param[in] colBackground 要設定的顏色值
 *	@return	<b>型別: COLORREF</b> \n 返回設定前的進度條顏色，如果進度條顏色為系統預設，則返回 CLR_DEFAULT。
 */
COLORREF DmProgressBar::SetBkColor(COLORREF colBackground) const
{
	assert(this->IsWindow());
	// PBM_SETBKCOLOR
	// wParam = 不使用，必須為零
	// lParam = 要設定的顏色值
	return static_cast<COLORREF>(::SendMessage(*this, PBM_SETBKCOLOR, static_cast<WPARAM>(0), static_cast<LPARAM>(colBackground)));
}

/**
 *	@brief	Sets the progress bar to marquee mode. This causes the progress bar to move like a marquee.
 *	@param[in]	bSelectMode		Indicates whether to turn the marquee mode on or off.
 *	@param[in]	dwMilliseconds	Time, in milliseconds, between marquee animation updates.
 *		\n If this parameter is zero, the marquee animation is updated every 30 milliseconds.
 *	@return	此函數沒有回值
 */
void DmProgressBar::SetMarQuee(BOOL bSelectMode, DWORD dwMilliseconds) const
{
	assert(this->IsWindow());
	// PBM_SETMARQUEE
	// wParam = Indicates whether to turn the marquee mode on or off.
	// lParam = Time, in milliseconds
	::SendMessage(*this, PBM_SETMARQUEE, static_cast<WPARAM>(bSelectMode), static_cast<LPARAM>(dwMilliseconds));
}

/**
 *	@brief	設定進度條新位置
 *	@param[in] nPos	指定新位置數值。
 *	@return	<b>型別: int</b> \n 返回設定前的位置值。
 */
int DmProgressBar::SetPos(int nPos) const
{
	assert(this->IsWindow());
	// PBM_SETPOS
	// wParam = 新的位置數值，需用有號數。
	// lParam = 不使用，必須為零
	return static_cast<COLORREF>(::SendMessage(*this, PBM_SETPOS, static_cast<WPARAM>(nPos), static_cast<LPARAM>(0)));
}

/**
 *	@brief	設定進度條範圍
 *	@param[in] dwRange	LOWORD 為範圍下限值、不得為負數，HIWORD 為範圍上限值、範圍上限值必須大於範圍下限值。
 *	@return	<b>型別: DWORD</b> \n LOWORD 為設定前的範圍下限值，HIWORD 為設定前的範圍上限值。
 */
DWORD DmProgressBar::SetRange(DWORD dwRange) const
{
	assert(this->IsWindow());
	// PBM_SETRANGE
	// wParam = 不使用，必須為零
	// lParam = 指定新的範圍值
	return static_cast<DWORD>(::SendMessage(*this, PBM_SETRANGE, static_cast<WPARAM>(0), static_cast<LPARAM>(dwRange)));
}

/**
 *	@brief	設定進度條範圍，(32-bits)
 *	@param[in] lowLimit	指定範圍的下限值
 *	@param[in] hiLimit	指定範圍的上限值
 *	@return	<b>型別: DWORD</b> \n LOWORD 為設定前的範圍下限值，HIWORD 為設定前的範圍上限值。
 */
UINT32 DmProgressBar::SetRange32(UINT32 lowLimit, UINT32 hiLimit) const
{
	assert(this->IsWindow());
	// PBM_SETRANGE32
	// wParam = 指定範圍的下限值。
	// lParam = 指定範圍的上限值
	return static_cast<UINT32>(::SendMessage(*this, PBM_SETRANGE32, static_cast<WPARAM>(lowLimit), static_cast<LPARAM>(hiLimit)));
}

/**
 *	@brief	設定進度條狀態
 *	@param[in] nStatus 狀態值
 *	@return	<b>型態: int</b> \n 返回值為目前進度條的狀態
 *	@remark	<b>狀態值額外說明</b>
 *		- PBST_NORMAL	\n In progress.
 *		- PBST_ERROR	\n Error.
 *		- PBST_PAUSED	\n Paused.
 */
int DmProgressBar::SetState(int nState) const
{
	assert(this->IsWindow());
	// PBM_SETSTATE
	// wParam = 狀態值
	// lParam = 不使用，必須為零
	return static_cast<int>(::SendMessage(*this, PBM_SETSTATE, static_cast<WPARAM>(nState), static_cast<LPARAM>(0)));
}

/**
 *	@brief	指定進度條的步進增量值
 *	@param[in] nInc	要步進增量的值
 *	@return	<b>型別: int</b> \n 返回設定前的步進增量值
 */
int DmProgressBar::SetStep(int nInc) const
{
	assert(this->IsWindow());
	// PBM_SETSTEP
	// wParam = 新的步進增量值
	// lParam = 不使用，必須為零
	return static_cast<int>(::SendMessage(*this, PBM_SETSTEP, static_cast<WPARAM>(nInc), static_cast<LPARAM>(0)));
}

/**
 *	@brief	重新繪製步進後的新位置
 *	@return	<b>型別: int</b> \n 返回值為舊的位置
 */
int DmProgressBar::Stepit() const
{
	assert(this->IsWindow());
	// PBM_STEPIT
	// wParam = 新的步進增量值
	// lParam = 不使用，必須為零
	return static_cast<int>(::SendMessage(*this, PBM_STEPIT, static_cast<WPARAM>(0), static_cast<LPARAM>(0)));
}

