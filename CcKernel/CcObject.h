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
 * @page      CcKernel
 * @subpage   CcObject
 *
 * @page      CcObject
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcObject
 */
#ifndef H_CCOBJECT_H_
#define H_CCOBJECT_H_

#include "CcBase.h"

class CcEventHandler;
class CcEvent;

/**
 * @brief Basic Class for all Objects wich has receive callbacks
 */
class CcKernelSHARED  CcObject
{
public:
  /**
   * @brief Constructor
   */
  CcObject() = default;

  CCDEFINE_CONSTRUCTOR_TO_OPERATORS(CcObject);

  /**
   * @brief Destructor
   */
  virtual ~CcObject();

  /**
   * @brief Copy constructor
   */
  CcObject& operator=(const CcObject&)
  { m_pOnDeleteHandler = nullptr; return *this;}

  /**
   * @brief Move constructor
   */
  CcObject& operator=(CcObject&& oToCopy)
  { m_pOnDeleteHandler=oToCopy.m_pOnDeleteHandler; oToCopy.m_pOnDeleteHandler = nullptr; return *this;}

  void insertOnDelete(CcEvent pEventHandle);
  void removeOnDelete(CcObject* pObject);

protected:
  CcEventHandler& getOnDeleteHandler();

private:
  void clear();
private:
  CcEventHandler* m_pOnDeleteHandler = nullptr;
};

#endif // H_CcOBJECT_H_
