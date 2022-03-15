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
 * @brief     Implemtation of class CcOSBuildConfigDirectory
 */
#include "CcOSBuildConfigDirectory.h"
#include "CcOSBuildConfigGlobals.h"
#include "CcXml/CcXmlNode.h"
#include "CcOSBuildConfig.h"
#include "CcGlobalStrings.h"

class CcOSBuildConfigDirectory::CPrivate
{
public:
  CcOSBuildConfigDirectoryList m_oDirectories;
  CcOSBuildConfigProjectList   m_oProjects;
};


CcOSBuildConfigDirectory::CcOSBuildConfigDirectory()
{
  CCNEW(m_pPrivate,CPrivate);
}

CcOSBuildConfigDirectory::CcOSBuildConfigDirectory(const CcOSBuildConfigDirectory& oToCopy)
{
  CCNEW(m_pPrivate,CPrivate);
  operator=(oToCopy);
}

CcOSBuildConfigDirectory::CcOSBuildConfigDirectory(CcOSBuildConfigDirectory&& oToMove)
{
  CCNEW(m_pPrivate,CPrivate);
  operator=(CCMOVE(oToMove));
}

CcOSBuildConfigDirectory::CcOSBuildConfigDirectory(CcXmlNode& rNode, CcOSBuildConfigDirectory* pParent) :
  m_pNode(&rNode)
{
  CCNEW(m_pPrivate,CPrivate);
  readConfig(rNode, pParent);
}

CcOSBuildConfigDirectory::~CcOSBuildConfigDirectory()
{
  CCDELETE(m_pPrivate);
}

CcOSBuildConfigDirectory& CcOSBuildConfigDirectory::operator=(CcOSBuildConfigDirectory&& oToMove)
{
  if (this != &oToMove)
  {
    deletePrivate();
    m_pPrivate = oToMove.m_pPrivate;
    oToMove.m_pPrivate = nullptr;
    m_pNode = oToMove.m_pNode;
    oToMove.m_pNode = nullptr;
    m_sName = CCMOVE(oToMove.m_sName);
  }
  return *this;
}

CcOSBuildConfigDirectory& CcOSBuildConfigDirectory::operator=(const CcOSBuildConfigDirectory& oToCopy)
{
  *m_pPrivate = *oToCopy.m_pPrivate;
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
      if (rNode.getType() == CcXmlNode::EType::Node)
      {
        if (rNode.getName() == CcOSBuildConfigGlobals::Tags::Name)
        {
          m_sName = rNode.innerText();
        }
        else if (rNode.getName() == CcOSBuildConfigGlobals::Tags::Project)
        {
          CCNEWTYPE(pNewProject,CcOSBuildConfigProject,rNode,this);
          m_pPrivate->m_oProjects.append(pNewProject);
          addProject(m_pPrivate->m_oProjects.last());
        }
        else if (rNode.getName() == CcOSBuildConfigGlobals::Tags::Directory)
        {
          CCNEWTYPE(pNewDirectory,CcOSBuildConfigDirectory,rNode,this);
          m_pPrivate->m_oDirectories.append(pNewDirectory);
        }
      }
    }
  }
  return true;
}

const CcOSBuildConfigDirectoryList& CcOSBuildConfigDirectory::getDirectories()
{
  return m_pPrivate->m_oDirectories;
}

const CcOSBuildConfigProjectList& CcOSBuildConfigDirectory::getProjects()
{
  return m_pPrivate->m_oProjects;
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

CcString CcOSBuildConfigDirectory::getPath() const
{
  CcString sPath;
  const CcOSBuildConfigDirectory* pTemp = this;
  while ( pTemp != nullptr &&
          pTemp->m_pParent != nullptr &&
          pTemp->getName() != "")
  {
    sPath.prepend(pTemp->getName() + CcGlobalStrings::Seperators::Slash);
    pTemp = pTemp->m_pParent;
  }
  return sPath;
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
  CCDELETE(m_pPrivate);
}
