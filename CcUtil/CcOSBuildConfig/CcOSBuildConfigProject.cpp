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
 * @brief     Implemtation of class CcOSBuildConfigProject
 */
#include "CcOSBuildConfigProject.h"
#include "CcOSBuildConfigGlobals.h"
#include "CcXml/CcXmlNode.h"
#include "CcXml/CcXmlUtil.h"
#include "CcSharedPointer.h"
#include "CcOSBuildConfigDirectory.h"
#include "CcGlobalStrings.h"

CcOSBuildConfigProject::CcOSBuildConfigProject(const CcOSBuildConfigProject& oToCopy)
{
  operator=(oToCopy);
}

CcOSBuildConfigProject::CcOSBuildConfigProject(CcOSBuildConfigProject&& oToMove)
{
  operator=(std::move(oToMove));
}

CcOSBuildConfigProject::CcOSBuildConfigProject(CcXmlNode& rNode, CcOSBuildConfigDirectory* pParentDir) :
  m_pNode(&rNode)
{
  readConfig(rNode, pParentDir);
}

CcOSBuildConfigProject::~CcOSBuildConfigProject()
{
}

CcOSBuildConfigProject& CcOSBuildConfigProject::operator=(CcOSBuildConfigProject&& oToMove)
{
  if (this != &oToMove)
  {
    m_sName = std::move(oToMove.m_sName);
    m_oVersion = std::move(oToMove.m_oVersion);
    m_iActive = oToMove.m_iActive;
    m_sPrebuild = std::move(oToMove.m_sPrebuild);
    m_sStatic = std::move(oToMove.m_sStatic);
    m_sStaticRuntime = std::move(oToMove.m_sStaticRuntime);
    m_slPlatforms = std::move(oToMove.m_slPlatforms);
    m_slDependency = std::move(oToMove.m_slDependency);
    m_slFeatures = std::move(oToMove.m_slFeatures);
    m_pNode = oToMove.m_pNode;
    oToMove.m_pNode = nullptr;
    m_pParentDir = oToMove.m_pParentDir;
    oToMove.m_pParentDir = nullptr;
  }
  return *this;
}

CcOSBuildConfigProject& CcOSBuildConfigProject::operator=(const CcOSBuildConfigProject& oToCopy)
{
  m_sName = oToCopy.m_sName;
  m_oVersion = oToCopy.m_oVersion;
  m_iActive = oToCopy.m_iActive;
  m_sPrebuild = oToCopy.m_sPrebuild;
  m_sStatic = oToCopy.m_sStatic;
  m_sStaticRuntime = oToCopy.m_sStaticRuntime;
  m_slPlatforms = oToCopy.m_slPlatforms;
  m_slDependency = oToCopy.m_slDependency;
  m_slFeatures = oToCopy.m_slFeatures;
  m_pParentDir = oToCopy.m_pParentDir;
  m_pNode = oToCopy.m_pNode;
  return *this;
}

bool CcOSBuildConfigProject::operator==(const CcOSBuildConfigProject& oToCompare) const
{
  if (oToCompare.m_pNode == m_pNode)
    return true;
  else
    return false;
}

bool CcOSBuildConfigProject::readConfig(CcXmlNode& rParentNode, CcOSBuildConfigDirectory* pParentDir)
{
  m_pParentDir = pParentDir;
  if (rParentNode.isNotNull())
  {
    m_pNode = &rParentNode;
    for (CcXmlNode& rNode : rParentNode.getNodeList())
    {
      if (rNode.getType() == EXmlNodeType::Node)
      {
        if (rNode.getName() == CcOSBuildConfigGlobals::Tags::Name)
        {
          m_sName = rNode.innerText();
        }
        else if (rNode.getName() == CcOSBuildConfigGlobals::Tags::Version)
        {
          m_oVersion = rNode.innerText();
        }
        else if (rNode.getName() == CcOSBuildConfigGlobals::Tags::Active)
        {
          m_iActive = static_cast<EProjectActiveLevel>(rNode.innerText().toInt32());
        }
        else if (rNode.getName() == CcOSBuildConfigGlobals::Tags::Prebuild)
        {
          bool bOk;
          m_sPrebuild = rNode.innerText();
          bool bRestult = CcXmlUtil::getBoolFromNodeValue(rNode, false, &bOk);
          if (bOk)
          {
            if (bRestult)
              m_sPrebuild = CcGlobalStrings::True;
            else
              m_sPrebuild = CcGlobalStrings::False;
          }
          else
          {
            if (m_sPrebuild.length() == 0)
            {
              m_sPrebuild = CcGlobalStrings::False;
            }
          }
        }
        else if (rNode.getName() == CcOSBuildConfigGlobals::Tags::Static)
        {
          bool bOk;
          m_sStatic = rNode.innerText();
          bool bRestult = CcXmlUtil::getBoolFromNodeValue(rNode, false, &bOk);
          if (bOk)
          {
            if (bRestult)
              m_sStatic = CcGlobalStrings::True;
            else
              m_sStatic = CcGlobalStrings::False;
          }
          else
          {
            if (m_sStatic.length() == 0)
            {
              m_sStatic = CcGlobalStrings::False;
            }
          }
        }
        else if (rNode.getName() == CcOSBuildConfigGlobals::Tags::StaticRuntime)
        {
          bool bOk;
          m_sStaticRuntime = rNode.innerText();
          bool bRestult = CcXmlUtil::getBoolFromNodeValue(rNode, false, &bOk);
          if (bOk)
          {
            if (bRestult)
              m_sStaticRuntime = CcGlobalStrings::True;
            else
              m_sStaticRuntime = CcGlobalStrings::False;
          }
          else
          {
            if (m_sStaticRuntime.length() == 0)
            {
              m_sStaticRuntime = CcGlobalStrings::False;
            }
          }
        }
        else if (rNode.getName() == CcOSBuildConfigGlobals::Tags::Platforms)
        {
          for (CcXmlNode& oPlatformNode : rNode)
          {
            if (oPlatformNode.getName() == CcOSBuildConfigGlobals::Tags::Platform)
            {
              m_slPlatforms.append(oPlatformNode.innerText());
            }
          }
        }
        else if (rNode.getName() == CcOSBuildConfigGlobals::Tags::Features)
        {
          for (CcXmlNode& oFeatureNode : rNode)
          {
            if (oFeatureNode.getName() == CcOSBuildConfigGlobals::Tags::Feature)
            {
              m_slFeatures.append(oFeatureNode.innerText());
            }
          }
        }
        else if (rNode.getName() == CcOSBuildConfigGlobals::Tags::Dependencies)
        {
          for (CcXmlNode& oDependencyNode : rNode)
          {
            if (oDependencyNode.getName() == CcOSBuildConfigGlobals::Tags::Dependency)
            {
              m_slDependency.append(oDependencyNode.innerText());
            }
          }
        }
      }
    }
  }
  return true;
}

void CcOSBuildConfigProject::addIncommingDependency(CcSharedPointer<CcOSBuildConfigProject>& pProject)
{
  m_oIncomingDependencies.append(pProject);
}

CcString CcOSBuildConfigProject::getDefineString() const
{
  if (m_pParentDir != nullptr)
  {
    return m_pParentDir->getDefineString() + "_" + m_sName.getUpper();
  }
  else
  {
    return m_sName;
  }
}

CcString CcOSBuildConfigProject::getVersionDefineString() const
{
  return getDefineString() + "_VERSION";
}

CcString CcOSBuildConfigProject::getActiveDefineString() const
{
  return getDefineString() + "_ACTIVE";
}

CcString CcOSBuildConfigProject::getPrebuildDefineString() const
{
  return getDefineString() + "_PREBUILD";
}

CcString CcOSBuildConfigProject::getStaticDefineString() const
{
  return getDefineString() + "_STATIC";
}

CcString CcOSBuildConfigProject::getStaticRuntimeDefineString() const
{
  return getDefineString() + "_STATICRUNTIME";
}

void CcOSBuildConfigProject::removeFeature(const CcString& sFeatureName)
{
  m_slFeatures.removeItem(sFeatureName);
}
