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
 * @page      CcUserList
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcUserList
 */
#ifndef CCUSERLIST_H_
#define CCUSERLIST_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcUser.h"
#include "CcList.h"

#ifdef WIN32
template class CcKernelSHARED CcList<CcUser>;
#endif

/**
 * @brief Example Class impelmentation
 */
class CcKernelSHARED CcUserList : public CcList<CcUser*>
{
public:
  /**
   * @brief Constructor
   */
  CcUserList( void );

  /**
  * @brief Constructor
  */
  CcUserList(const CcUserList& oToCopy);

  /**
   * @brief Destructor
   */
  virtual ~CcUserList( void );

  CcUser *findUser(const CcString& Username);
  CcUser *findUserPassword(const CcString& Username, const CcString& Password);
  inline CcUser *currentUser(void) const
    { return m_CurrentUser; }
  
  bool setCurrentUser(const CcString& Username);
private:
  CcUser *m_CurrentUser;
};

#endif /* CCUSERLIST_H_ */
