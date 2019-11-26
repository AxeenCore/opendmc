/**************************************************************************//**
 * @file	winapp.hh
 * @brief	Windows 程序類別宣告 Header
 * @date	2000-10-10
 * @date	2019-01-11
 * @author	Swang
 *****************************************************************************/
#ifndef ODMC_WNDS_WINAPP_HH
#define	ODMC_WNDS_WINAPP_HH
#include "object.hh"

/**
 *	@class DmWinapp
 *	@brief DmWinapp, Windows 應用程式操作類別
 */
class DmWinapp : public DmWndsObject
{
	friend DmWinapp& GetWinapp();

public:
	DmWinapp();
	virtual ~DmWinapp();

	// Overridables
	virtual int Run();

	// Operations
	BOOL IsReady() const;
	HINSTANCE GetInstanceHandle() const;
	HINSTANCE GetResourceHandle() const;
	LONG_PTR GetMainframe() const;

protected:
	// Overridables
	virtual BOOL InitComponent();
	virtual int MessageLoopActive();
	virtual int MessageLoopStandard();
	virtual int MessageLoopStandardEx();
	void DefinitionMainframe(LONG_PTR lParam);


	BOOL		m_bInitialize;		//!< 初始化是否成功
	HINSTANCE	m_hInstance;		//!< Windows 程序實例操作代碼
	HINSTANCE	m_hResource;		//!< Windows 資源實例操作代碼
	LONG_PTR	m_lMainframe;		//!< 用來保存主視窗操作物件

private:
	DmWinapp(const DmWinapp&) = delete;				//!< Disable copy construction
	DmWinapp& operator=(const DmWinapp&) = delete;	//!< Disable assignment operator
	static DmWinapp* SetnGetThis(DmWinapp* thisPtr = NULL, BOOL bReset = FALSE);
};

///////////////////////////////////////////////////////////////////////////////
//
// Extern function
//
///////////////////////////////////////////////////////////////////////////////

extern DmWinapp& GetWinapp();

#endif // !ODMC_WNDS_WINAPP_HH
