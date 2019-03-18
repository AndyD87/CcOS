/*
 * This file is part of CcOS.
 *
 * CcOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcOS.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of class CcEthernetFrame
 */
#include "Network/CcEthernetFrame.h"
#include "CcStatic.h"
#include "CcStringList.h"

CcEthernetFrame::CcEthernetFrame()
{
}

CcEthernetFrame::~CcEthernetFrame()
{
}

CcEthernetFrame& CcEthernetFrame::operator=(CcEthernetFrame&& oToMove)
{
  if (&oToMove != this)
  {
    clear();
    m_pFrame = oToMove.m_pFrame;
    m_bOwner = oToMove.m_bOwner;
    m_uiSize = oToMove.m_uiSize;
    oToMove.m_pFrame = nullptr;
    oToMove.m_bOwner = false; 
    oToMove.m_uiSize = 0;
  }
  return *this;
}

CcEthernetFrame& CcEthernetFrame::operator=(const CcEthernetFrame& oToCopy)
{
  clear();
  m_bOwner = oToCopy.m_bOwner;
  m_uiSize = oToCopy.m_uiSize;
  if (oToCopy.m_bOwner)
  {
    m_pFrame = static_cast<SEthernetFrame*>(malloc(oToCopy.m_uiSize));
    CcStatic::memcpy(m_pFrame, oToCopy.m_pFrame, oToCopy.m_uiSize);
  }
  else
  {
    m_pFrame = oToCopy.m_pFrame;
  }
  return *this;
}

bool CcEthernetFrame::operator==(const CcEthernetFrame& oToCompare) const
{
  bool bRet = false;
  if (false)
  {
    if(oToCompare.m_pFrame == m_pFrame)
      bRet = true;
  }
  return bRet;
}

void* CcEthernetFrame::getData() const
{
  return m_pFrame->oEthernetFrame8023Raw.pPayload;
}

void CcEthernetFrame::clear()
{
  if (m_bOwner)
  {
    CCDELETEVOID(m_pFrame);
  }
}