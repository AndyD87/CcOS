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
 * @subpage   CcOSBuildConfigDirectory
 *
 * @page      CcOSBuildConfigDirectory
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcOSBuildConfigDirectory
 **/
#ifndef CcOSBuildConfigDirectory_H_
#define CcOSBuildConfigDirectory_H_

#include "CcBase.h"
#include "CcList.h"
#include "CcString.h"
#include "CcOSBuildConfigProject.h"
#include "CcMacros.h"

class CcXmlNode;
class CcOSBuildConfig;
class CcOSBuildConfigDirectory;
class CcOSBuildConfigDirectoryPrivate;


#ifdef WIN32
template class CcOSBuildConfigSHARED CcSharedPointer<CcOSBuildConfigDirectory>;
template class CcOSBuildConfigSHARED CcList<CcSharedPointer<CcOSBuildConfigDirectory>>;
#endif

typedef CcList<CcSharedPointer<CcOSBuildConfigDirectory>>   CcOSBuildConfigDirectoryList;

/**
 * @brief Class impelmentation
 */
class CcOSBuildConfigSHARED CcOSBuildConfigDirectory
{
public:
  /**
   * @brief Constructor
   */
  CcOSBuildConfigDirectory(void);
  
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
  virtual ~CcOSBuildConfigDirectory( void );

  CcOSBuildConfigDirectory& operator=(const CcOSBuildConfigDirectory& oToCopy);
  CcOSBuildConfigDirectory& operator=(CcOSBuildConfigDirectory&& oToMove);
  bool operator==(const CcOSBuildConfigDirectory& oToCompare) const;

  bool readConfig(CcXmlNode& pNode, CcOSBuildConfigDirectory* pBuildConfig);

  const CcString& getName()const
    { return m_sName; }

  const CcOSBuildConfigDirectoryList& getDirectories();
  const CcOSBuildConfigProjectList& getProjects();
  CcString getDefineString();

  virtual void addProject(CcSharedPointer<CcOSBuildConfigProject>& pProject);

private: // Methods
  void deletePrivate();

private:
  CcString   m_sName;
  CcXmlNode* m_pNode = nullptr;
  CcOSBuildConfigDirectoryPrivate* m_pPrivateData = nullptr;
  CcOSBuildConfigDirectory* m_pParent = nullptr;
};

#endif /* CcOSBuildConfigDirectory_H_ */
