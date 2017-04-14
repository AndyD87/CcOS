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
 * @page      CcTree
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcTree
 *
 * @todo     Make a Code review if it's realy a good way to implement it like that
 */
#ifndef CCTREE_H_
#define CCTREE_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcVector.h"

class CcTree;

#ifdef WIN32
template class CcKernelSHARED CcVector<CcTree *>;
#endif

/**
* @brief Button for GUI Applications
*/
class CcKernelSHARED  CcTree {
public:
  /**
  * @brief Constructor
  */
  CcTree(void);

  /**
  * @brief Destructor
  */
  virtual ~CcTree(void);

  void addSubTree( CcTree *toAdd );

  CcTree *getNext(void);

  CcTree *getAt(uint32 pos);

  CcTree *begin();

  void delSubTree(CcTree* toDel);

  void clear(void);

  size_t size(void);

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwis false
   */
  virtual bool operator==(CcTree &toCompare);

private:
  CcVector<CcTree*> m_TreeList;
};

#endif /* CCTREE_H_ */
