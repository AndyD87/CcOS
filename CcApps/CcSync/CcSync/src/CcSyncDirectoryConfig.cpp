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
* @brief     Implemtation of class CcSyncDirectoryConfig
*/
#include "CcSyncDirectoryConfig.h"
#include "CcStringUtil.h"
#include "CcSyncGlobals.h"
#include "CcXml/CcXmlNode.h"
#include "CcJson/CcJsonObject.h"
#include "CcDirectory.h"

CcSyncDirectoryConfig::CcSyncDirectoryConfig(void)
{
}

CcSyncDirectoryConfig::CcSyncDirectoryConfig(CcXmlNode& pXmlNode)
{
  parseXmlNode(pXmlNode);
}

CcSyncDirectoryConfig::CcSyncDirectoryConfig(const CcJsonObject& pJsonNode)
{
  parseJsonNode(pJsonNode);
}

CcSyncDirectoryConfig::CcSyncDirectoryConfig(const CcSyncDirectoryConfig& oToCopy)
{
  operator=(oToCopy);
}

CcSyncDirectoryConfig::CcSyncDirectoryConfig(CcSyncDirectoryConfig&& oToMove)
{
  operator=(std::move(oToMove));
}

CcSyncDirectoryConfig::~CcSyncDirectoryConfig(void)
{
}

CcSyncDirectoryConfig& CcSyncDirectoryConfig::operator=(const CcSyncDirectoryConfig& oToCopy)
{
  m_sLocation = oToCopy.m_sLocation;
  m_sName = oToCopy.m_sName;
  return *this;
}

CcSyncDirectoryConfig& CcSyncDirectoryConfig::operator=(CcSyncDirectoryConfig&& oToMove)
{
  if (this != &oToMove)
  {
    m_sLocation = std::move(oToMove.m_sLocation);
    m_sName = std::move(oToMove.m_sName);
  }
  return *this;
}

bool CcSyncDirectoryConfig::operator==(const CcSyncDirectoryConfig& oToCompare) const
{
  bool bRet = false;
  CCUNUSED(oToCompare);
  return bRet;
}

bool CcSyncDirectoryConfig::operator!=(const CcSyncDirectoryConfig& oToCompare) const
{
  return !operator==(oToCompare);
}

void CcSyncDirectoryConfig::parseXmlNode(CcXmlNode& pXmlNode)
{
  CcXmlNode& pTempNode = pXmlNode[CcSyncGlobals::Client::ConfigTags::Name];
  if (pTempNode.isNotNull())
    m_sName = pTempNode.getValue();
  pTempNode = pXmlNode[CcSyncGlobals::Client::ConfigTags::DirectoryLocation];
  if (pTempNode.isNotNull())
  {
    m_sLocation = pTempNode.getValue();
  }
}

void CcSyncDirectoryConfig::parseJsonNode(const CcJsonObject& rJsonNode)
{
  const CcJsonData& pTempNode = rJsonNode[CcSyncGlobals::Client::ConfigTags::Name];
  if (pTempNode.isValue())
      m_sName = pTempNode.getValue().getString();
  const CcJsonData& pLocationNode = rJsonNode[CcSyncGlobals::Client::ConfigTags::DirectoryLocation];
  if (pLocationNode.isValue())
  {
    m_sLocation = pLocationNode.getValue().getString();
  }
}

CcXmlNode CcSyncDirectoryConfig::getXmlNode()
{
  CcXmlNode oDirectoryNode(EXmlNodeType::Node);
  oDirectoryNode.setName(CcSyncGlobals::Client::ConfigTags::Directory);
  {
    CcXmlNode oDirectoryName(EXmlNodeType::Node);
    oDirectoryName.setName(CcSyncGlobals::Client::ConfigTags::DirectoryName);
    oDirectoryName.setValue(m_sName);
    oDirectoryNode.append(std::move(oDirectoryName));

    CcXmlNode oDirectoryLocation(EXmlNodeType::Node);
    oDirectoryLocation.setName(CcSyncGlobals::Client::ConfigTags::DirectoryLocation);
    oDirectoryLocation.setValue(m_sName);
    oDirectoryNode.append(std::move(oDirectoryLocation));
  }
  return oDirectoryNode;
}