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
 * @subpage   CcUserList
 *
 * @page      CcUserList
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcUserList
 */
#ifndef _CCUSERLIST_H_
#define _CCUSERLIST_H_

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
class CcKernelSHARED CcUserList : public CcList<CcUserHandle>
{
public:
  /**
   * @brief Constructor
   */
  CcUserList(void );

  /**
   * @brief Constructor
   */
  CcUserList(const CcUserList& oToCopy);

  /**
   * @brief Destructor
   */
  ~CcUserList(void );

  CcUserHandle findUser(const CcString& Username);
  CcUserHandle findUser(const CcString& Username) const;
  CcUserHandle findUserPassword(const CcString& Username, const CcString& Password);
  inline CcUserHandle currentUser(void) const
    { return m_CurrentUser; }
  
  bool setCurrentUser(const CcString& Username);
private:
  CcUserHandle m_CurrentUser;
};

#endif /* _CCUSERLIST_H_ */
