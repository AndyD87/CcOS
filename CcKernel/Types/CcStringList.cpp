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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implemtation of class CcStringList
 */

#include "CcGlobalStrings.h"
#include "CcStringList.h"

CcStringList::CcStringList(const CcString& sItem)
{
  append(sItem);
}

CcStringList::CcStringList(const CcStringList& toAssign):
  CcStringList_BaseType<CcString>(toAssign)
{
}

CcString CcStringList::parseArguments(const CcString& Line)
{
  CcString temp;
  bool bIsQuote = false;
  for (size_t i = 0; i < Line.length(); i++)
  {
    if (Line.at(i) == '\n')
    {

    }
    else if (Line.at(i) == '\r')
    {

    }
    else if (Line.at(i) == ' ' && bIsQuote == false)
    {
      append(temp);
      temp.clear();
    }
    else if (Line.at(i) == '"')
    {
      if(bIsQuote == true)
      {
        bIsQuote = false;
        append(temp);
        temp.clear();
      }
      else
      {
        bIsQuote = true;
      }
    }
    else if (Line.at(i) == '\\')
    {
      if(i+1 < Line.length())
      {
        i++;
        switch (Line.at(i))
        {
          case '\\':
            temp.append('\\');
            break;
          case 'n':
            temp.append('\n');
            break;
          case 'r':
            temp.append('\r');
            break;
          case '"':
            temp.append('"');
            break;
          case 't':
            temp.append('\t');
            break;
        }
      }
    }
    else
    {
      temp.append(Line.at(i));
    }
  }
  append(temp);
  temp = at(0);
  return temp;
}

CcString CcStringList::collapse(const CcString& seperator) const
{
  CcString sRet;
  bool bWriteSeperator = false;
  for (size_t i = 0; i < size(); i++)
  {
    if(bWriteSeperator)
      sRet.append(seperator);
    else
      bWriteSeperator = true;
    sRet.append(at(i));
  }
  return sRet;
}

CcString CcStringList::collapse(const CcString& seperator, const CcString& sPrepend, const CcString& sAppend) const
{
  CcString sRet;
  for (size_t i = 0; i < size(); i++)
  {
    sRet.append(sPrepend);
    sRet.append(at(i));
    sRet.append(sAppend);
    if(i+1 < size())
      sRet.append(seperator);
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

CcStringList& CcStringList::removeEmpty()
{
  CcStringList::iterator rIterator = this->begin();
  while (rIterator != this->end())
  {
    if (*rIterator == CcGlobalStrings::Empty)
    {
      rIterator = this->removeIterator(rIterator);
    }
    else
    {
      rIterator++;
    }
  }
  return *this;
}
