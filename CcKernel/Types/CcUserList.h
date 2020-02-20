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
#ifndef H_CCUSERLIST_H_
#define H_CCUSERLIST_H_

#include "CcBase.h"
#include "CcBase.h"
#include "CcUser.h"
#include "CcList.h"
#include "CcReferenceCount.h"

#ifdef _MSC_VER
template class CcKernelSHARED CcList<CcUser>;
#endif

/**
 * @brief Example Class implementation
 */
class CcKernelSHARED CcUserList : public CcList<CcUser*>
{
public:
  /**
   * @brief Constructor
   */
  CcUserList();
  ~CcUserList();

  /**
   * @brief Constructor
   */
  CcUserList(const CcUserList& oToCopy);

  CcUser* findUser(const CcString& Username);
  CcUser* findUser(const CcString& Username) const;
  CcUser* findUserPassword(const CcString& Username, const CcString& Password);
  inline CcUser* currentUser() const
    { return m_CurrentUser; }
  
  bool setCurrentUser(const CcString& Username);
private:
  CcReferenceCount* m_pReference = nullptr;
  CcUser*           m_CurrentUser = nullptr;
};

#endif // H_CcUSERLIST_H_
