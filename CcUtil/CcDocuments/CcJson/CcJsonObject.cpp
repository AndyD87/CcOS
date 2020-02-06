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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implemtation of class CcJsonObject
 */
#include "CcJson/CcJsonObject.h"

CcJsonObject::CcJsonObject()
{
}

CcJsonObject::~CcJsonObject()
{
}

bool CcJsonObject::contains(const CcString& sName) const
{
  for (const CcJsonNode& rValue : *this)
  {
    if (rValue.getName() == sName)
      return true;
  }
  return false;
}

CcJsonNode& CcJsonObject::operator[](const CcString& sName)
{
  for (CcJsonNode& rValue : *this)
  {
    if (rValue.getName() == sName)
      return rValue;
  }
  return c_CcJsonNullNode;
}

const CcJsonNode& CcJsonObject::operator[](const CcString& sName) const
{
  for (const CcJsonNode& rValue : *this)
  {
    if (rValue.getName() == sName)
      return rValue;
  }
  return c_CcJsonNullNode;
}

CcJsonObject& CcJsonObject::operator=(CcJsonObject&& oToMove)
{
  if (this != &oToMove)
  {
    CcList<CcJsonNode>::operator=(CCMOVE(oToMove));
  }
  return *this;
}

CcJsonObject& CcJsonObject::operator=(const CcJsonObject& oToCopy)
{
    CcList<CcJsonNode>::operator=(oToCopy);
  return *this;
}

bool CcJsonObject::operator==(const CcJsonObject& oToCompare) const
{
  bool bSuccess = false;
  if (size() == oToCompare.size())
  {
    bSuccess = true;
    CcList<CcJsonNode>::const_iterator rIterator = oToCompare.begin();
    for (const CcJsonNode& rNode : *this)
    {
      if (*rIterator != rNode)
      {
        bSuccess = false;
        break;
      }
      rIterator++;
    }
  }
  return bSuccess;
}
