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
 * @brief     Implemtation of class CcOSBuildConfigCmake
 */
#include "CcOSBuildConfigCmake.h"
#include "CcOSBuildConfig.h"
#include "CcFile.h"
#include "CcGlobalStrings.h"
#include "CcStatic.h"

CcOSBuildConfigCmake::CcOSBuildConfigCmake(void )
{
}

CcOSBuildConfigCmake::~CcOSBuildConfigCmake(void )
{
}

bool CcOSBuildConfigCmake::writeProjects(CcList<CcOSBuildConfigPlatform>& oProjectList, const CcString& sOutputFile)
{
  m_oFile = CcFile(sOutputFile);
  bool bSuccess = true;
  if (m_oFile.exists())
  {
    bSuccess = m_oFile.open(EOpenFlags::Overwrite);
  }
  else
  {
    bSuccess = m_oFile.open(EOpenFlags::Write);
  }
  if (bSuccess)
  {
    for (CcOSBuildConfigPlatform& oPlatform : oProjectList)
    {
      writeLegend();
      bSuccess &= m_oFile.writeLine("if(" + oPlatform.m_sName.getUpper() + ")");
      for (CcSharedPointer<CcOSBuildConfigProject>& rpProject : oPlatform.m_oAllProjects)
      {
        bSuccess &= m_oFile.writeLine(  "  # Project: " + rpProject->getName());
        writeProjectSettings(rpProject);
        writeProjectActiveCheck(oPlatform.m_oAllProjects, rpProject);
        writeNoAllowedPlatform(oPlatform.m_oNoBuild, oPlatform.m_sName);
        m_oFile.writeLine(CcGlobalStrings::Empty);
      }
      bSuccess &= m_oFile.writeLine("endif(" + oPlatform.m_sName.getUpper() + ")");
    }
  }
  return bSuccess;
}

bool CcOSBuildConfigCmake::writeLegend()
{
  bool bSuccess = true;
  bSuccess &= m_oFile.writeLine("################################################################################");
  bSuccess &= m_oFile.writeLine("# Config Check for cmake builds");
  bSuccess &= m_oFile.writeLine("################################################################################");
  return bSuccess;
}

bool CcOSBuildConfigCmake::writeCmakeSet(const CcString& sVariable, const CcString sValue)
{
  bool bSuccess = true;
  bSuccess &= m_oFile.writeLine("  if(NOT DEFINED " + sVariable + ")");
  if(sValue.contains(" "))
    bSuccess &= m_oFile.writeLine("    set(" + sVariable + " \"" + sValue + "\")");
  else
    bSuccess &= m_oFile.writeLine("    set(" + sVariable + " " + sValue + ")");
  bSuccess &= m_oFile.writeLine("  endif(NOT DEFINED " + sVariable + ")");
  return bSuccess;
}

bool CcOSBuildConfigCmake::writeProjectSettings(CcSharedPointer<CcOSBuildConfigProject>& rProject)
{
  bool bSuccess = true;
  CcString sActive = CcString::fromNumber(static_cast<int32>(rProject->getActive()));
  CcString sActiveDefine = rProject->getActiveDefineString();
  bSuccess &= writeCmakeSet(sActiveDefine, sActive);

  const CcVersion& rVersion = rProject->getVersion();
  CcString sVersionDefine = rProject->getVersionDefineString();
  bSuccess &= writeCmakeSet(sVersionDefine, rVersion.getVersionString(4));

  CcString sPrebuild = rProject->getPrebuild();
  CcString sPrebuildDefine = rProject->getPrebuildDefineString();
  if (sPrebuild == CcGlobalStrings::True)
    bSuccess &= writeCmakeSet(sPrebuildDefine, CcGlobalStrings::True);
  else if (sPrebuild == CcGlobalStrings::False)
    bSuccess &= writeCmakeSet(sPrebuildDefine, CcGlobalStrings::False);
  else
    bSuccess &= writeCmakeSet(sPrebuildDefine, "${"+sPrebuild+"}");

  CcString sStatic = rProject->getStatic();
  CcString sStaticDefine = rProject->getStaticDefineString();
  if (sStatic == CcGlobalStrings::True)
    bSuccess &= writeCmakeSet(sStaticDefine, CcGlobalStrings::True);
  else if (sStatic == CcGlobalStrings::False)
    bSuccess &= writeCmakeSet(sStaticDefine, CcGlobalStrings::False);
  else
    bSuccess &= writeCmakeSet(sStaticDefine, "${" + sStatic + "}");

  CcString sStaticRuntime = rProject->getStaticRuntime();
  CcString sStaticRuntimeDefine = rProject->getStaticRuntimeDefineString();
  if (sStaticRuntime == CcGlobalStrings::True)
    bSuccess &= writeCmakeSet(sStaticRuntimeDefine, CcGlobalStrings::True);
  else if (sStaticRuntime == CcGlobalStrings::False)
    bSuccess &= writeCmakeSet(sStaticRuntimeDefine, CcGlobalStrings::False);
  else
    bSuccess &= writeCmakeSet(sStaticRuntimeDefine, "${" + sStaticRuntime + "}");

  return bSuccess;
}

void CcOSBuildConfigCmake::setupIncomeDependencies(CcSharedPointer<CcOSBuildConfigProject>& rProject)
{
  for (CcSharedPointer<CcOSBuildConfigProject>& pDependend : rProject->getIncomeDependencies())
  {
    CcString sDependendDefine = pDependend->getActiveDefineString();
    if (!m_slIncomeDependencies.contains(sDependendDefine))
    {
      m_slIncomeDependencies.append(sDependendDefine);
      setupIncomeDependencies(pDependend);
    }
  }
}

CcSharedPointer<CcOSBuildConfigProject>& CcOSBuildConfigCmake::getDependedProject(CcList<CcSharedPointer<CcOSBuildConfigProject>>& oProjectList, const CcString& sSearch)
{
  for (CcSharedPointer<CcOSBuildConfigProject>& rProject : oProjectList)
  {
    if (rProject->getName() == sSearch)
    {
      return rProject;
    }
  }
  CCERROR("This should not happen, it must be evolved before!");
  return CcStatic::getNullRef<CcSharedPointer<CcOSBuildConfigProject>>();
}

bool CcOSBuildConfigCmake::writeProjectActiveCheck(CcOSBuildConfigProjectList& oPlatform, CcSharedPointer<CcOSBuildConfigProject>& rProject)
{
  bool bSuccess = true;
  setupIncomeDependencies(rProject);
  bSuccess &= m_oFile.writeLine("  # Check if required because of income dependencies");
  bSuccess &= m_oFile.writeLine("  if(${" + rProject->getActiveDefineString() + "} EQUAL 1)");
  for (CcString& sIncomeDependency : m_slIncomeDependencies)
  {
    bSuccess &= m_oFile.writeLine("    if(DEFINED " + sIncomeDependency + ")");
    bSuccess &= m_oFile.writeLine("      if(${" + sIncomeDependency + "} GREATER 1)");
    bSuccess &= m_oFile.writeLine("        set(" + rProject->getActiveDefineString() + " 2)");
    bSuccess &= m_oFile.writeLine("      endif(${" + sIncomeDependency + "} GREATER 1)");
    bSuccess &= m_oFile.writeLine("    endif(DEFINED " + sIncomeDependency + ")");
  }
  bSuccess &= m_oFile.writeLine("  endif(${" + rProject->getActiveDefineString() + "} EQUAL 1)");
  m_slIncomeDependencies.clear();

  bSuccess &= m_oFile.writeLine("  # Check if required because of income dependencies");
  bSuccess &= m_oFile.writeLine("  if(${" + rProject->getActiveDefineString() + "} EQUAL 1)");
  for (CcString& sDependency : rProject->getDependencies())
  {
    CcSharedPointer<CcOSBuildConfigProject>& rDependency = getDependedProject(oPlatform, sDependency);
    CcString sCurrentDependency = rDependency->getActiveDefineString();
    bSuccess &= m_oFile.writeLine("    if(NOT DEFINED " + sCurrentDependency + ")");
    bSuccess &= m_oFile.writeLine("      message(\"Diable " + rProject->getName() + "\")");
    bSuccess &= m_oFile.writeLine("      message(\"Dependency for " + rProject->getName() + " not solved: " + sDependency + "\")");
    bSuccess &= m_oFile.writeLine("      set(" + rProject->getActiveDefineString() + " 0)");
    bSuccess &= m_oFile.writeLine("    elseif(${" + sCurrentDependency + "} EQUAL 0)");
    bSuccess &= m_oFile.writeLine("      message(\"Diable " + rProject->getName() + "\")");
    bSuccess &= m_oFile.writeLine("      message(\"Dependency for " + rProject->getName() + " not enabled but required: " + sDependency + "\")");
    bSuccess &= m_oFile.writeLine("     set(" + rProject->getActiveDefineString() + " 0)");
    bSuccess &= m_oFile.writeLine("    endif(NOT DEFINED " + sCurrentDependency + ")");
  }
  m_slIncomeDependencies.clear();

  bSuccess &= m_oFile.writeLine("  elseif(${" + rProject->getActiveDefineString() + "} GREATER 1)");
  for (CcString& sDependency : rProject->getDependencies())
  {
    CcSharedPointer<CcOSBuildConfigProject>& rDependency = getDependedProject(oPlatform, sDependency);
    CcString sCurrentDependency = rDependency->getActiveDefineString();
    bSuccess &= m_oFile.writeLine("    if(NOT DEFINED " + sCurrentDependency + ")");
    bSuccess &= m_oFile.writeLine("      message(FATAL_ERROR \"Dependency for " + rProject->getName() + " not solved: " + sDependency + "\")");
    bSuccess &= m_oFile.writeLine("    elseif(${" + sCurrentDependency + "} EQUAL 0)");
    bSuccess &= m_oFile.writeLine("      message(FATAL_ERROR \"Dependency for " + rProject->getName() + " not enabled but required: " + sDependency + "\")");
    bSuccess &= m_oFile.writeLine("    endif(NOT DEFINED " + sCurrentDependency + ")");
  }
  bSuccess &= m_oFile.writeLine("  endif(${" + rProject->getActiveDefineString() + "} EQUAL 1)");
  return bSuccess;
}

bool CcOSBuildConfigCmake::writeNoAllowedPlatform(CcOSBuildConfigProjectList& oPlatform, const CcString& sPlatformName)
{
  bool bSuccess = true;
  for (CcSharedPointer<CcOSBuildConfigProject>& sIncomeDependency : oPlatform)
  {
    CcString sDependency = sIncomeDependency->getActiveDefineString();
    bSuccess &= m_oFile.writeLine("  if(DEFINED " + sDependency + ")");
    bSuccess &= m_oFile.writeLine("    if(${" + sDependency + "} GREATER 0)");
    bSuccess &= m_oFile.writeLine("      message(FATAL_ERROR \"" + sIncomeDependency->getName() + " not allowed on " + sPlatformName + "\")");
    bSuccess &= m_oFile.writeLine("    endif(${" + sDependency + "} GREATER 0)");
    bSuccess &= m_oFile.writeLine("  endif(DEFINED " + sDependency + ")");
  }
  return bSuccess;
}
