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
 * @page      CcOSBuildConfig
 * @subpage   CcOSBuildConfigProject
 *
 * @page      CcOSBuildConfigProject
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcOSBuildConfigProject
 **/
#ifndef _CcOSBuildConfigProject_H_
#define _CcOSBuildConfigProject_H_

#include "CcBase.h"
#include "CcOSBuildConfigBase.h"
#include "CcOSBuildConfigProject.h"
#include "CcString.h"
#include "CcStringList.h"
#include "CcList.h"
#include "CcVersion.h"
#include "CcSharedPointer.h"
#include "CcGlobalStrings.h"

class CcXmlNode;
class CcOSBuildConfigDirectory;
class CcOSBuildConfigProject;

enum class EProjectActiveLevel
{
  NoBuild = 0,
  NiceToHave,
  Active,
  Manually,
  Mandatory,
};

#ifdef WINDOWS
template class CcOSBuildConfigSHARED CcSharedPointer<CcOSBuildConfigProject>;
template class CcOSBuildConfigSHARED CcList<CcSharedPointer<CcOSBuildConfigProject>>;
#endif

typedef CcList<CcSharedPointer<CcOSBuildConfigProject>>   CcOSBuildConfigProjectList;

/**
 * @brief Class impelmentation
 */
class CcOSBuildConfigSHARED CcOSBuildConfigProject
{
public:
  /**
   * @brief Constructor
   */
  CcOSBuildConfigProject() = default;
  
  /**
   * @brief CopyConstructor
   */
  CcOSBuildConfigProject(const CcOSBuildConfigProject& oToCopy);

  /**
   * @brief MoveConstructor
   */
  CcOSBuildConfigProject(CcOSBuildConfigProject&& oToMove);

  /**
   * @brief Constructor
   */
  CcOSBuildConfigProject(CcXmlNode& pNode, CcOSBuildConfigDirectory* pParentDir);

  /**
   * @brief Destructor
   */
  ~CcOSBuildConfigProject();

  CcOSBuildConfigProject& operator=(CcOSBuildConfigProject&& oToMove);
  CcOSBuildConfigProject& operator=(const CcOSBuildConfigProject& oToCopy);
  bool operator==(const CcOSBuildConfigProject& oToCompare) const;

  bool readConfig(CcXmlNode& pNode, CcOSBuildConfigDirectory* pParentDir);
  
  const CcString& getName() const
    { return m_sName; }

  const CcStringList& getPlatforms() const
  {
    return m_slPlatforms;
  }

  const CcStringList& getDependencies() const
  {
    return m_slDependency;
  }

  const CcStringList& getFeatures() const
  {
    return m_slFeatures;
  }

  void addIncommingDependency(CcSharedPointer<CcOSBuildConfigProject>& pProject);

  CcString getDefineString() const;
  CcString getVersionDefineString() const;
  CcString getActiveDefineString() const;
  CcString getPrebuildDefineString() const;
  CcString getStaticDefineString() const;
  CcString getStaticRuntimeDefineString() const;
  const CcList<CcSharedPointer<CcOSBuildConfigProject>>& getIncomeDependencies() const
    { return m_oIncomingDependencies; }

  const CcVersion& getVersion() const
    { return m_oVersion; }
  EProjectActiveLevel getActive() const
    { return m_iActive; }
  const CcString& getPrebuild() const
    { return m_sPrebuild; }
  const CcString& getStatic() const
    { return m_sStatic; }
  const CcString& getStaticRuntime() const
    { return m_sStaticRuntime; }

private:
  CcString   m_sName;
  CcVersion  m_oVersion;
  EProjectActiveLevel m_iActive;
  CcString m_sPrebuild      = CcGlobalStrings::False;
  CcString m_sStatic        = CcGlobalStrings::False;
  CcString m_sStaticRuntime = CcGlobalStrings::False;
  CcStringList m_slPlatforms;
  CcStringList m_slDependency;
  CcStringList m_slFeatures;
  CcXmlNode* m_pNode = nullptr;
  CcOSBuildConfigDirectory *m_pParentDir = nullptr;
  CcList<CcSharedPointer<CcOSBuildConfigProject>> m_oIncomingDependencies;
};

#endif /* _CcOSBuildConfigProject_H_ */
