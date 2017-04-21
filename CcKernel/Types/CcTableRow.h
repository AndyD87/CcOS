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
 * @subpage   CcTableRow
 *
 * @page      CcTableRow
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcTableRow
 */
#ifndef CcTableRow_H_
#define CcTableRow_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcListCommon.h"
#include "CcHandle.h"

class CcTable;

#ifdef WIN32
template class CcKernelSHARED CcHandle<CcTable>;
#endif

/**
 * @brief Storage Class for Tables
 *        it operates with a fixed size of Columns, and varios rows.
 */
class CcKernelSHARED  CcTableRow : public CcVariantList
{
public:
  CcTableRow(void);

  CcTableRow(CcTable& pParentTable);

  /**
   * @brief CopyConstructor
   */
  CcTableRow(const CcTableRow& oToCopy);

  /**
   * @brief CopyConstructor
   */
  CcTableRow(CcTableRow&& oToMove);


  /**
   * @brief Destructor
   */
  virtual ~CcTableRow( void );

  void setParentTable(CcTable* parentTable);

  CcTableRow& operator=(const CcTableRow& oToCopy);
  CcTableRow& operator=(CcTableRow&& oToMove);
  CcVariant& operator[](const CcString& sColumnName) const;
  inline CcVariant& operator[](size_t uiPosition) const
    { return CcVariantList::operator[](uiPosition); }

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwis false
   */
  bool operator==(const CcTableRow& oToCompare) const;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwis false
   */
  inline bool operator!=(const CcTableRow& oToCompare) const
    { return !operator==(oToCompare); }


private:
  CcTable* m_pPartOfTable;
};

#endif /* CcTableRow_H_ */
