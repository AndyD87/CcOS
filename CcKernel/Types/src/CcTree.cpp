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
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implementation of Class CcTreeAv
 */
#include "CcTree.h"

CcTree::CcTree(void)
{}

CcTree::~CcTree(void)
{
  clear();
}

void CcTree::addSubTree(CcTree *toAdd)
{
  m_TreeList.append(toAdd);
}

CcTree* CcTree::getAt(uint32 pos)
{
  return m_TreeList.at(pos);
}

void CcTree::delSubTree(CcTree* toDel)
{
  m_TreeList.removeItem(toDel);
}

void CcTree::clear(void)
{
  m_TreeList.clear();
}

size_t CcTree::size(void)
{
  return m_TreeList.size();
}

bool CcTree::operator==(CcTree &toCompare)
{
  if (toCompare.m_TreeList.size() == toCompare.m_TreeList.size())
  {
    return true;
  }
  else return false;
}
