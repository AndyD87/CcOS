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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcStringList
 */

#include "CcStringList.h"

CcStringList::CcStringList()
{
  // todo Auto-generated constructor stub
}

CcStringList::CcStringList(const CcStringList& toAssign):
  CcList<CcString>(toAssign)
{
}

CcStringList::~CcStringList()
{
  // todo Auto-generated destructor stub
}

CcString CcStringList::parseArguments(const CcString& Line)
{
  CcString temp;
  for (size_t i = 0; i < Line.length(); i++)
  {
    if (Line.at(i) == '\n')
    {

    }
    else if (Line.at(i) == '\r')
      ;
    else if (Line.at(i) == ' ')
    {
      append(temp);
      temp.clear();
    }
    else{
      temp.append(Line.at(i));
    }
  }
  append(temp);
  temp = at(0);
  remove(0);
  return temp;
}

CcString CcStringList::collapseList(const CcString& seperator) const
{
  CcString sRet;
  bool first = true;
  for (size_t i = 0; i < size(); i++)
  {
    if (!first)
      first = false;
    else
      sRet.append(seperator);
    sRet.append(at(i));
  }
  return sRet;
}

size_t CcStringList::findString(const CcString& toFind, size_t offset) const
{
  for (size_t i = offset; i < size(); i++)
  {
    if (at(i) == toFind)
      return i;
  }
  return SIZE_MAX;
}
