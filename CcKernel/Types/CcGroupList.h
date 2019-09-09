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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcGroupList
 */
#ifndef H_CcGroupLIST_H_
#define H_CcGroupLIST_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcGroup.h"
#include "CcList.h"

#ifdef _MSC_VER
template class CcKernelSHARED CcList<CcGroup>;
#endif

/**
 * @brief Example Class implementation
 */
class CcKernelSHARED CcGroupList : public CcList<CcGroup>
{
public:
  /**
   * @brief Constructor
   */
  CcGroupList() = default;

  /**
   * @brief Constructor
   */
  CcGroupList(const CcGroupList& oToCopy);

  /**
   * @brief Destructor
   */
  ~CcGroupList() = default;

  CcGroup& findGroup(const CcString& Groupname);
  const CcGroup& findGroup(const CcString& Groupname) const;
  
  bool setCurrentUser(const CcString& Groupname);
private:
  CcGroupHandle m_CurrentUser;
};

#endif // H_CcGroupLIST_H_
