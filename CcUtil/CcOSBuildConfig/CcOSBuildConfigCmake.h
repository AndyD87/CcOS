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
 * @subpage   CcOSBuildConfigCmake
 *
 * @page      CcOSBuildConfigCmake
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcOSBuildConfigCmake
 **/
#ifndef CcOSBuildConfigCmake_H_
#define CcOSBuildConfigCmake_H_

#include "CcBase.h"
#include "CcList.h"
#include "CcOSBuildConfigPlatform.h"
#include "CcFile.h"

/**
 * @brief Class impelmentation
 */
class CcOSBuildConfigCmake {
public:
  /**
   * @brief Constructor
   */
  CcOSBuildConfigCmake( void );

  /**
   * @brief Destructor
   */
  ~CcOSBuildConfigCmake( void );

  bool writeProjects(CcList<CcOSBuildConfigPlatform>& oProjectList, const CcString& sOutputFile);

private:
  bool writeLegend();
  bool writeCmakeSet(const CcString& sVariable, const CcString sValue);
  bool writeProjectSettings(CcSharedPointer<CcOSBuildConfigProject>& rProject);
  bool writeProjectActiveCheck(CcOSBuildConfigProjectList& oPlatform, CcSharedPointer<CcOSBuildConfigProject>& rProject);
  void setupIncomeDependencies(CcSharedPointer<CcOSBuildConfigProject>& rProject);
  CcSharedPointer<CcOSBuildConfigProject>& getDependedProject(CcList<CcSharedPointer<CcOSBuildConfigProject>>& oProjectList, const CcString& sSearch);
  bool writeNoAllowedPlatform(CcOSBuildConfigProjectList& oPlatform, const CcString& sPlatformName);

private:
  CcFile m_oFile;
  CcStringList m_slIncomeDependencies;
};

#endif /* CcOSBuildConfigCmake_H_ */
