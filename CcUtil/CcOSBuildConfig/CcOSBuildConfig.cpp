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
 * @brief     Implemtation of class CcOSBuildConfig
 */
#include "CcOSBuildConfig.h"
#include "CcOSBuildConfigGlobals.h"
#include "CcOSBuildConfigCmake.h"
#include "CcFile.h"
#include "CcConsole.h"

CcOSBuildConfig::CcOSBuildConfig()
{
}

CcOSBuildConfig::~CcOSBuildConfig()
{
}

bool CcOSBuildConfig::loadConfigFile(const CcString& sPathToConfig)
{
  bool bSuccess = false;
  if (CcFile::exists(sPathToConfig))
  {
    m_oXmlFile.setFile(sPathToConfig);
    if (m_oXmlFile.readData())
    {
      CcXmlNode& rRootNode = m_oXmlFile.rootNode()[CcOSBuildConfigGlobals::Tags::Root];
      if (rRootNode.isNotNull())
      {
        bSuccess = readConfig(rRootNode, this);
      }
    }
  }
  return bSuccess;
}

void CcOSBuildConfig::writeAllProjects()
{
  for (CcOSBuildConfigPlatform& oOSBuildConfigPlatform : m_oPlatformlist)
  {
    // Last check if all dependencies are fullfilled

    while (rescanDependencies(oOSBuildConfigPlatform, true));
    CcConsole::writeLine("Platform: " + oOSBuildConfigPlatform.m_sName);
    CcConsole::writeLine("  Failed Projects:");
    for (CcSharedPointer<CcOSBuildConfigProject>& oProject : oOSBuildConfigPlatform.m_oQuarantaine)
    {
      CcConsole::writeLine("    " + oProject->getName() + ":\t"+ oProject->getDefineString());
    }
    CcConsole::writeLine("  Working Projects:");
    for (CcSharedPointer<CcOSBuildConfigProject>& oProject : oOSBuildConfigPlatform.m_oAllProjects)
    {
      CcConsole::writeLine("    " + oProject->getName() + ":\t" + oProject->getDefineString());
    }
  }
}

bool CcOSBuildConfig::writeCmakeDefines(const CcString& sPathToCmakeFile)
{
  CcOSBuildConfigCmake oCmakeFile;
  return oCmakeFile.writeProjects(m_oPlatformlist, sPathToCmakeFile);
}

void CcOSBuildConfig::insertToPlatformList(CcOSBuildConfigPlatform& oList, CcSharedPointer<CcOSBuildConfigProject> pProject)
{
  bool bDependencySolved = true;
  for (const CcString& sDependency : pProject->getDependencies())
  {
    bDependencySolved &= (locationDependencyInList(oList.m_oAllProjects, sDependency) != SIZE_MAX);
  }
  for (const CcString& sFeature : pProject->getFeatures())
  {
    bDependencySolved &= (locationDependencyInList(oList.m_oAllProjects, sFeature) != SIZE_MAX);
  }
  if (bDependencySolved)
  {
    // Add all strong dependencies to income list
    for (const CcString& sDependency : pProject->getDependencies())
    {
      size_t uiLocation = locationDependencyInList(oList.m_oAllProjects, sDependency);
      if (uiLocation != SIZE_MAX)
      {
        oList.m_oAllProjects[uiLocation]->addIncommingDependency(pProject);
      }
      else
      {
        CCERROR("This should not happen!!!");
      }
    }
    oList.m_oAllProjects.append(pProject);
    while (rescanDependencies(oList));
  }
  else
  {
    oList.m_oQuarantaine.append(pProject);
  }
}

size_t CcOSBuildConfig::locationDependencyInList(CcList<CcSharedPointer<CcOSBuildConfigProject>>& oList, const CcString& sProject)
{
  size_t uiLocation = 0;
  for (CcSharedPointer<CcOSBuildConfigProject> pCurrentProject : oList)
  {
    if (pCurrentProject->getName() == sProject)
    {
      return uiLocation;
    }
    uiLocation++;
  }
  if (uiLocation >= oList.size())
    uiLocation = SIZE_MAX;
  return uiLocation;
}

bool CcOSBuildConfig::rescanDependencies(CcOSBuildConfigPlatform& oList, bool bFinalRescan)
{
  bool bSuccess = false;
  for (CcSharedPointer<CcOSBuildConfigProject> pCurrentProject : oList.m_oQuarantaine)
  {
    bool bDependencySolved = true;
    for (const CcString& sDependency : pCurrentProject->getDependencies())
    {
      bDependencySolved &= (locationDependencyInList(oList.m_oAllProjects, sDependency) != SIZE_MAX);
    }
    CcStringList oDeletedFeatures;
    for (const CcString& sFeature : pCurrentProject->getFeatures())
    {
      if (!(locationDependencyInList(oList.m_oAllProjects, sFeature) != SIZE_MAX))
      {
        if (bFinalRescan)
        {
          oDeletedFeatures.append(sFeature);
        }
        else
        {
          bDependencySolved = false;
        }
      }
    }
    for (const CcString& sFeature : oDeletedFeatures)
    {
      pCurrentProject->removeFeature(sFeature);
    }
    if (bDependencySolved)
    {
      // Add all strong dependencies to income list
      for (const CcString& sDependency : pCurrentProject->getDependencies())
      {
        size_t uiLocation = locationDependencyInList(oList.m_oAllProjects, sDependency);
        if (uiLocation != SIZE_MAX)
        {
          oList.m_oAllProjects[uiLocation]->addIncommingDependency(pCurrentProject);
        }
        else
        {
          CCERROR("This should not happen!!!");
        }
      }
      oList.m_oAllProjects.append(pCurrentProject);
      oList.m_oQuarantaine.removeItem(pCurrentProject);
      bSuccess = true;
      break;
    }
  }
  return bSuccess;
}

void CcOSBuildConfig::addProject(CcSharedPointer<CcOSBuildConfigProject>& pProject)
{
  for (CcString& CcOSBuildConfigPlatformName : pProject->getPlatforms())
  {
    bool bInserted = false;
    for (size_t i = 0; i < m_oPlatformlist.size(); i++)
    {
      if (m_oPlatformlist[i].m_sName == CcOSBuildConfigPlatformName)
      {
        insertToPlatformList(m_oPlatformlist[i], pProject);
        bInserted = true;
      }
    }
    if (bInserted == false)
    {
      CcOSBuildConfigPlatform oNewPlatform;
      oNewPlatform.m_sName = CcOSBuildConfigPlatformName;
      m_oPlatformlist.append(oNewPlatform);
      insertToPlatformList(m_oPlatformlist.last(), pProject);
    }
  }
  for (size_t i = 0; i < m_oPlatformlist.size(); i++)
  {
    if (!pProject->getPlatforms().contains(m_oPlatformlist[i].m_sName))
    {
      m_oPlatformlist[i].m_oNoBuild.append(pProject);
    }
  }
}
