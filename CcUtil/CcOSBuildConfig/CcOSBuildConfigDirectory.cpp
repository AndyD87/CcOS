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
 * @brief     Implemtation of class CcOSBuildConfigDirectory
 */
#include "CcOSBuildConfigDirectory.h"
#include "CcOSBuildConfigGlobals.h"
#include "CcXml/CcXmlNode.h"
#include "CcOSBuildConfig.h"

class CcOSBuildConfigDirectory::CPrivate
{
public:
  CcOSBuildConfigDirectoryList m_oDirectories;
  CcOSBuildConfigProjectList   m_oProjects;
};


CcOSBuildConfigDirectory::CcOSBuildConfigDirectory()
{
  m_pPrivateData = new CPrivate();
  CCMONITORNEW(m_pPrivateData);
}

CcOSBuildConfigDirectory::CcOSBuildConfigDirectory(const CcOSBuildConfigDirectory& oToCopy)
{
  m_pPrivateData = new CPrivate();
  CCMONITORNEW(m_pPrivateData);
  operator=(oToCopy);
}

CcOSBuildConfigDirectory::CcOSBuildConfigDirectory(CcOSBuildConfigDirectory&& oToMove)
{
  m_pPrivateData = new CPrivate();
  CCMONITORNEW(m_pPrivateData);
  operator=(std::move(oToMove));
}

CcOSBuildConfigDirectory::CcOSBuildConfigDirectory(CcXmlNode& rNode, CcOSBuildConfigDirectory* pParent) :
  m_pNode(&rNode)
{
  m_pPrivateData = new CPrivate();
  readConfig(rNode, pParent);
}

CcOSBuildConfigDirectory::~CcOSBuildConfigDirectory()
{
  CCDELETE(m_pPrivateData);
}

CcOSBuildConfigDirectory& CcOSBuildConfigDirectory::operator=(CcOSBuildConfigDirectory&& oToMove)
{
  if (this != &oToMove)
  {
    deletePrivate();
    m_pPrivateData = oToMove.m_pPrivateData;
    oToMove.m_pPrivateData = nullptr;
    m_pNode = oToMove.m_pNode;
    oToMove.m_pNode = nullptr;
    m_sName = std::move(oToMove.m_sName);
  }
  return *this;
}

CcOSBuildConfigDirectory& CcOSBuildConfigDirectory::operator=(const CcOSBuildConfigDirectory& oToCopy)
{
  *m_pPrivateData = *oToCopy.m_pPrivateData;
  m_pNode = oToCopy.m_pNode;
  m_sName = oToCopy.m_sName;
  return *this;
}

bool CcOSBuildConfigDirectory::operator==(const CcOSBuildConfigDirectory& oToCompare) const
{
  if (oToCompare.m_pNode == m_pNode)
    return true;
  else
    return false;
}

bool CcOSBuildConfigDirectory::readConfig(CcXmlNode& rParentNode, CcOSBuildConfigDirectory* pParent)
{
  m_pParent = pParent;
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
        else if (rNode.getName() == CcOSBuildConfigGlobals::Tags::Project)
        {
          CcOSBuildConfigProject* pNewProject = new CcOSBuildConfigProject(rNode, this);
          CCMONITORNEW(pNewProject);
          m_pPrivateData->m_oProjects.append(pNewProject);
          addProject(m_pPrivateData->m_oProjects.last());
        }
        else if (rNode.getName() == CcOSBuildConfigGlobals::Tags::Directory)
        {
          CcOSBuildConfigDirectory* oNewDirectory = new CcOSBuildConfigDirectory(rNode, this);
          CCMONITORNEW(oNewDirectory);
          m_pPrivateData->m_oDirectories.append(oNewDirectory);
        }
      }
    }
  }
  return true;
}

const CcOSBuildConfigDirectoryList& CcOSBuildConfigDirectory::getDirectories()
{
  return m_pPrivateData->m_oDirectories;
}

const CcOSBuildConfigProjectList& CcOSBuildConfigDirectory::getProjects()
{
  return m_pPrivateData->m_oProjects;
}

CcString CcOSBuildConfigDirectory::getDefineString()
{
  if (m_pParent != nullptr  && m_pParent != this)
  {
    CcString sConcat = m_pParent->getDefineString();
    if (sConcat.length() > 0)
      sConcat << "_";
    sConcat << m_sName.getUpper();
    return sConcat;
  }
  else
  {
    return CcString(getName()).toUpper();
  }
}

void CcOSBuildConfigDirectory::addProject(CcSharedPointer<CcOSBuildConfigProject>& pProject)
{
  if (m_pParent != nullptr && m_pParent != this)
  {
    m_pParent->addProject(pProject);
  }
}

void CcOSBuildConfigDirectory::deletePrivate()
{
  CCDELETE(m_pPrivateData);
}
