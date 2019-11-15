/**************************************************************************//**
 * @file	apps.cc
 * @brief	Sample Window CxApps function member
 * @date	2000-10-10
 * @date	2018-09-01
 * @author	Swang
 *****************************************************************************/
#include "apps.hh"

CxApps::CxApps() : DmWinapp(), m_cFrame(NULL) { }

CxApps::~CxApps()
{
	SAFE_DELETE(m_cFrame);
}

int CxApps::Run()
{
	auto nExitCode = static_cast<int>(-1);
	if (this->IsReady()) {
		if (this->InitComponent()) {
			nExitCode = this->MessageLoopStandard();
		}
	}
	return nExitCode;
}

BOOL CxApps::InitComponent()
{
	auto cFrame = new (std::nothrow) CxFrame();

	for (;;) {
		SAFE_DELETE(m_cFrame);

		if (cFrame == NULL) {
			break;
		}

		if (!cFrame->CreateSample()) {
			break;
		}

		this->DefinitionMainframe(reinterpret_cast<LONG_PTR>(cFrame));
		m_cFrame = cFrame;
		return TRUE;
	}

	SAFE_DELETE(cFrame);
	this->DefinitionMainframe(0);
	return FALSE;
}
