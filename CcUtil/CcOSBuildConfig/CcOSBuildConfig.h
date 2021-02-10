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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcOSBuildConfig
 **/
#pragma once

#include "CcBase.h"
#include "CcXml/CcXmlFile.h"
#include "CcOSBuildConfigBase.h"
#include "CcOSBuildConfigDirectory.h"
#include "CcOSBuildConfigProject.h"
#include "CcOSBuildConfigPlatform.h"

/**
 * @brief Class implementation
 */
class CcOSBuildConfigSHARED CcOSBuildConfig : private CcOSBuildConfigDirectory
{
public:
  CcOSBuildConfig();
  virtual ~CcOSBuildConfig();

  /**
   * @brief Load configuration from file
   * @param sPathToConfig: Path to file to load config from
   * @return True if config was read and was valid
   */
  bool loadConfigFile(const CcString& sPathToConfig);

  /**
   * @brief Write all projects to std output
   */
  void writeAllProjects();

  /**
   * @brief Write cmake definitions to filee
   * @param sPathToCmakeFile: Path to write generated file to
   * @return True if all definitions are generated well.
   */
  bool writeCmakeDefines(const CcString& sPathToCmakeFile);
  
private: // methods
  void insertToPlatformList(CcOSBuildConfigPlatform& oList, CcSharedPointer<CcOSBuildConfigProject> pProject);
  size_t locationDependencyInList(CcList<CcSharedPointer<CcOSBuildConfigProject>>& oList, const CcString& pProject);
  bool rescanDependencies(CcOSBuildConfigPlatform& oList, bool bFinalRescan = false);
  virtual void addProject(CcSharedPointer<CcOSBuildConfigProject>& pProject) override;

private:
  CcXmlFile m_oXmlFile;
  CcList<CcOSBuildConfigPlatform> m_oPlatformlist;
};
