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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcOSBuildConfigProject
 **/
#ifndef H_CcOSBuildConfigProject_H_
#define H_CcOSBuildConfigProject_H_

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

//! Active level for cmake project
enum class EProjectActiveLevel
{
  NoBuild = 0,
  NiceToHave,
  Active,
  Manually,
  Mandatory,
};

#ifdef _MSC_VER
template class CcOSBuildConfigSHARED CcSharedPointer<CcOSBuildConfigProject>;
template class CcOSBuildConfigSHARED CcList<CcSharedPointer<CcOSBuildConfigProject>>;
#endif

//! List of projects
typedef CcList<CcSharedPointer<CcOSBuildConfigProject>>   CcOSBuildConfigProjectList;

/**
 * @brief Class implementation
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

  /**
   * @brief Move content of another project to this
   * @param oToMove:  Object to move data from
   * @return Handle to this
   */
  CcOSBuildConfigProject& operator=(CcOSBuildConfigProject&& oToMove);

  /**
   * @brief Copy content of another project to this
   * @param oToCopy:  Object to copy data from
   * @return Handle to this
   */
  CcOSBuildConfigProject& operator=(const CcOSBuildConfigProject& oToCopy);

  /**
   * @brief Compare if this content is same as content from another object
   * @param oToCompare: Object to compare with
   * @return True if both are same
   */
  bool operator==(const CcOSBuildConfigProject& oToCompare) const;

  /**
   * @brief Read config from xml node
   * @param pNode:        Node to parse for this config
   * @param pBuildConfig: Parent build config to pass
   * @return True if reading succeded
   */
  bool readConfig(CcXmlNode& pNode, CcOSBuildConfigDirectory* pParentDir);

  //! @return Name of this config item
  const CcString& getName() const
  { return m_sName; }
  //! @return Get list of supported platforms for this project
  const CcStringList& getPlatforms() const
  { return m_slPlatforms; }
  //! @return Get list of required dependencies for this project
  const CcStringList& getDependencies() const
  { return m_slDependency; }
  //! @return Get list of supported features for this project
  const CcStringList& getFeatures() const
  { return m_slFeatures;}
  //! @param pProject: Add dependency for pProject to this
  void addIncommingDependency(CcSharedPointer<CcOSBuildConfigProject>& pProject);
  //! @return Gete define string for this project
  CcString getDefineString() const;
  //! @return Get version define string
  CcString getVersionDefineString() const;
  //! @return Get active define string
  CcString getActiveDefineString() const;
  //! @return Get version define string
  CcString getPrebuildDefineString() const;
  //! @return Get static define string
  CcString getStaticDefineString() const;
  //! @return Get static runtime define string
  CcString getStaticRuntimeDefineString() const;
  //! @return Get list of incomming dependencies from other projects
  const CcList<CcSharedPointer<CcOSBuildConfigProject>>& getIncomeDependencies() const
  { return m_oIncomingDependencies; }

  //! @return Get version
  const CcVersion& getVersion() const
  { return m_oVersion; }
  //! @return Get active level
  EProjectActiveLevel getActive() const
  { return m_iActive; }
  //! @return Get Prebuild value
  const CcString& getPrebuild() const
  { return m_sPrebuild; }
  //! @return Get static value
  const CcString& getStatic() const
  { return m_sStatic; }
  //! @return Get Prebuild value
  const CcString& getStaticRuntime() const
  { return m_sStaticRuntime; }
  //! @return Relativ path for this project
  CcString getPath() const;
  //! @param sFeatureName: Remove feature from list
  void removeFeature(const CcString& sFeatureName);

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

#endif // H_CcOSBuildConfigProject_H_
