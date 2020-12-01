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
 * @page      Types
 * @subpage   CcTree
 *
 * @page      CcTree
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcTree
 *
 * @todo     Make a Code review if it's realy a good way to implement it like that
 */
#ifndef H_CCTREE_H_
#define H_CCTREE_H_

#include "CcBase.h"
#include "CcBase.h"
#include "CcVector.h"

class CcTree;

#ifdef _MSC_VER
template class CcKernelSHARED CcVector<CcTree *>;
#endif

/**
 * @brief Button for GUI Applications
 */
class CcKernelSHARED  CcTree
{
public:
  /**
   * @brief Constructor
   */
  CcTree() = default;

  /**
   * @brief Destructor
   */
  ~CcTree();

  void addSubTree( CcTree *toAdd );

  CcTree *getNext();

  CcTree *getAt(uint32 pos);

  CcTree *begin();

  bool delSubTree(CcTree* toDel);

  void clear();

  size_t size();

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  virtual bool operator==(CcTree &toCompare) const;

private:
  CcVector<CcTree*> m_TreeList;
};

#endif // H_CcTREE_H_
