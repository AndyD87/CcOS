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
 * @brief     Class CcOSBuildConfigDirectory
 **/
#pragma once

#include "CcBase.h"
#include "CcList.h"
#include "CcString.h"
#include "CcOSBuildConfigProject.h"
#include "CcMacros.h"

class CcXmlNode;
class CcOSBuildConfig;
class CcOSBuildConfigDirectory;

#ifdef _MSC_VER
template class CcOSBuildConfigSHARED CcSharedPointer<CcOSBuildConfigDirectory>;
template class CcOSBuildConfigSHARED CcList<CcSharedPointer<CcOSBuildConfigDirectory>>;
#endif

//! List of build config dirs
typedef CcList<CcSharedPointer<CcOSBuildConfigDirectory>>   CcOSBuildConfigDirectoryList;

/**
 * @brief Class implementation
 */
class CcOSBuildConfigSHARED CcOSBuildConfigDirectory
{
public:
  /**
   * @brief Constructor
   */
  CcOSBuildConfigDirectory();
  
  /**
   * @brief CopyConstructor
   */
  CcOSBuildConfigDirectory(const CcOSBuildConfigDirectory& oToCopy);

  /**
   * @brief MoveConstructor
   */
  CcOSBuildConfigDirectory(CcOSBuildConfigDirectory&& oToMove);

  /**
   * @brief Constructor
   */
  CcOSBuildConfigDirectory(CcXmlNode& rNode, CcOSBuildConfigDirectory* pBuildConfig);

  /**
   * @brief Destructor
   *        virtual because it can be inherited from other
   */
  virtual ~CcOSBuildConfigDirectory();

  /**
   * @brief Move content of another directory to this
   * @param oToMove:  Object to move data from
   * @return Handle to this
   */
  CcOSBuildConfigDirectory& operator=(CcOSBuildConfigDirectory&& oToMove);

  /**
   * @brief Copy content of another directory to this
   * @param oToCopy:  Object to copy data from
   * @return Handle to this
   */
  CcOSBuildConfigDirectory& operator=(const CcOSBuildConfigDirectory& oToCopy);

  /**
   * @brief Compare if this content is same as content from another object
   * @param oToCompare: Object to compare with
   * @return True if both are same
   */
  bool operator==(const CcOSBuildConfigDirectory& oToCompare) const;

  /**
   * @brief Read config from xml node
   * @param pNode:        Node to parse for this config
   * @param pBuildConfig: Parent build config to pass
   * @return True if reading succeded
   */
  bool readConfig(CcXmlNode& pNode, CcOSBuildConfigDirectory* pBuildConfig);

  //! @return Name of this config item
  const CcString& getName()const
  { return m_sName; }
  //! @return Get child directories
  const CcOSBuildConfigDirectoryList& getDirectories();
  //! @return Get child projects
  const CcOSBuildConfigProjectList& getProjects();
  //! @return Get definstring for current directory
  CcString getDefineString();
  //! @return Get relative path current directory
  CcString getPath() const;

  /**
   * @brief Add project to this projects list.
   * @param pProject: Project to add
   */
  virtual void addProject(CcSharedPointer<CcOSBuildConfigProject>& pProject);

private: // Methods
  void deletePrivate();

private: // Types
  class CPrivate;
private: // Member
  CPrivate* m_pPrivate = nullptr;
  CcString   m_sName;
  CcXmlNode* m_pNode = nullptr;
  CcOSBuildConfigDirectory* m_pParent = nullptr;
};
