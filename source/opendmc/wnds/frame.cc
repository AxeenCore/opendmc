/**************************************************************************//**
 * @file	frame.cc
 * @brief	DmFrame 建立視窗框架類別成員函數定義
 * @date	2000-10-10
 * @date	2018-08-05
 * @author	Swang
 *****************************************************************************/
#include "opendmc/wnds/frame.hh"

//! DmFrame 建構式
DmFrame::DmFrame() : DmWnds() { }

//! DmFrame 解構式
DmFrame::~DmFrame() { this->DestroyMine(); }
