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
* @par       Web: http://coolcow.de
* @version   0.01
* @date      2016-04
* @par       Language   C++ ANSI V3
* @brief     Implemtation of class CcSyncLocationConfigClientCommand
*/
#include "CcSyncGlobals.h"
#include "CcSyncLocationConfig.h"
#include "CcXml/CcXmlNode.h"
#include "CcXml/CcXmlUtil.h"
#include "CcStringUtil.h"

CcSyncLocationConfig::CcSyncLocationConfig(void)
{

}

CcSyncLocationConfig::CcSyncLocationConfig(const CcSyncLocationConfig& oToCopy)
{
  operator=(oToCopy);
}

CcSyncLocationConfig::CcSyncLocationConfig(CcSyncLocationConfig&& oToMove)
{
  operator=(std::move(oToMove));
}

bool CcSyncLocationConfig::readConfig(CcXmlNode& pLocationNode)
{
  bool bRet = false;
  CcXmlNode& pTempNode = pLocationNode.getNode(CcSyncGlobals::Server::ConfigTags::LocationPath);
  if (pTempNode.isNotNull())
  {
    m_sPath = pTempNode.getValue();
    bRet = true;
  }
  return bRet;
}

CcSyncLocationConfig& CcSyncLocationConfig::operator=(const CcSyncLocationConfig& oToCopy)
{
  m_sPath = oToCopy.m_sPath;
  return *this;
}

CcSyncLocationConfig& CcSyncLocationConfig::operator=(CcSyncLocationConfig&& oToMove)
{
  if (this != &oToMove)
  {
    m_sPath = std::move(oToMove.m_sPath);
  }
  return *this;
}
