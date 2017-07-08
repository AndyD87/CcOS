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
* @brief     Implemtation of class CcSyncExecutionConfig
*/
#include "CcSyncExecutionConfig.h"
#include "CcSyncGlobals.h"
#include "CcXml/CcXmlNode.h"

CcSyncExecutionConfig::CcSyncExecutionConfig(void)
{
}

CcSyncExecutionConfig::CcSyncExecutionConfig(const CcSyncExecutionConfig& oToCopy)
{
  operator=(oToCopy);
}

CcSyncExecutionConfig::CcSyncExecutionConfig(CcSyncExecutionConfig&& oToMove)
{
  operator=(std::move(oToMove));
}

CcSyncExecutionConfig::~CcSyncExecutionConfig(void)
{
}

CcSyncExecutionConfig::CcSyncExecutionConfig(CcXmlNode& pXmlNode)
{
  parseXmlNode(pXmlNode);
}

CcSyncExecutionConfig& CcSyncExecutionConfig::operator=(const CcSyncExecutionConfig& oToCopy)
{
  CCUNUSED(oToCopy);
  return *this;
}

CcSyncExecutionConfig& CcSyncExecutionConfig::operator=(CcSyncExecutionConfig&& oToMove)
{
  if (this != &oToMove)
  {
  }
  return *this;
}

void CcSyncExecutionConfig::parseXmlNode(CcXmlNode& pXmlNode)
{
  CcXmlNode& pTempNode = pXmlNode[CcSyncGlobals::Client::ConfigTags::Name];
  if (pTempNode.isNotNull())
    m_sName = pTempNode.getValue();

  pTempNode = pXmlNode[CcSyncGlobals::Client::ConfigTags::CommandExecutable];
  if (pTempNode.isNotNull())
    m_sExecutable = pTempNode.getValue();

  pTempNode = pXmlNode[CcSyncGlobals::Client::ConfigTags::CommandParameters];
  if (pTempNode.isNotNull())
    m_sParameters = pTempNode.getValue();

  pTempNode = pXmlNode[CcSyncGlobals::Client::ConfigTags::CommandWorkingDirectory];
  if (pTempNode.isNotNull())
    m_sWorkingDir = pTempNode.getValue();

  pTempNode = pXmlNode[CcSyncGlobals::Client::ConfigTags::CommandTimeMask];
  if (pTempNode.isNotNull())
    m_sTimeMask = pTempNode.getValue();
}

bool CcSyncExecutionConfig::operator==(const CcSyncExecutionConfig& oToCompare) const
{
  bool bRet = false;
  CCUNUSED(oToCompare);
  return bRet;
}

bool CcSyncExecutionConfig::operator!=(const CcSyncExecutionConfig& oToCompare) const
{
  return !operator==(oToCompare);
}