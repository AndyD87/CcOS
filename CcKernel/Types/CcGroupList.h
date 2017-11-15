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
 * @subpage   CcGroupList
 *
 * @page      CcGroupList
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcGroupList
 */
#ifndef CcGroupLIST_H_
#define CcGroupLIST_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcGroup.h"
#include "CcList.h"

#ifdef WIN32
template class CcKernelSHARED CcList<CcGroup>;
#endif

/**
 * @brief Example Class impelmentation
 */
class CcKernelSHARED CcGroupList : public CcList<CcGroup>
{
public:
  /**
   * @brief Constructor
   */
  CcGroupList( void );

  /**
  * @brief Constructor
  */
  CcGroupList(const CcGroupList& oToCopy);

  /**
   * @brief Destructor
   */
  ~CcGroupList( void );

  CcGroup& findGroup(const CcString& Groupname);
  const CcGroup& findGroup(const CcString& Groupname) const;
  
  bool setCurrentUser(const CcString& Groupname);
private:
  CcGroupHandle m_CurrentUser;
};

#endif /* CcGroupLIST_H_ */
