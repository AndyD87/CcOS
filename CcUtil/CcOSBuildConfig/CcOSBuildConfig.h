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
 * @page      CcUtil
 * @subpage   CcOSBuildConfig
 *
 * @page      CcOSBuildConfig
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcOSBuildConfig
 **/
#ifndef CcOSBuildConfig_H_
#define CcOSBuildConfig_H_

#include "CcBase.h"
#include "CcXml/CcXmlFile.h"
#include "CcOSBuildConfigBase.h"
#include "CcOSBuildConfigDirectory.h"
#include "CcOSBuildConfigProject.h"
#include "CcOSBuildConfigPlatform.h"

/**
 * @brief Class impelmentation
 */
class CcOSBuildConfigSHARED CcOSBuildConfig : private CcOSBuildConfigDirectory
{
public:
  /**
   * @brief Constructor
   */
  CcOSBuildConfig( void );

  /**
   * @brief Destructor
   */
  ~CcOSBuildConfig( void );

  bool loadConfigFile(const CcString& sPathToConfig);
  void writeAllProjects();
  bool writeCmakeDefines(const CcString& sPathToCmakeFile);
  
private: // methods
  void insertToPlatformList(CcOSBuildConfigPlatform& oList, CcSharedPointer<CcOSBuildConfigProject> pProject);
  size_t locationDependencyInList(CcList<CcSharedPointer<CcOSBuildConfigProject>>& oList, const CcString& pProject);
  bool rescanDependencies(CcOSBuildConfigPlatform& oList);
  virtual void addProject(CcSharedPointer<CcOSBuildConfigProject>& pProject) override;

private:
  CcXmlFile m_oXmlFile;
  CcList<CcOSBuildConfigPlatform> m_oPlatformlist;
};

#endif /* CcOSBuildConfig_H_ */
