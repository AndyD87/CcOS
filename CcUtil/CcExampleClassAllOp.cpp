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
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcExampleClassAllOp
 */
#include "CcExampleClassAllOp.h"

CcExampleClassAllOp::CcExampleClassAllOp( void )
{
}

CcExampleClassAllOp::CcExampleClassAllOp( const CcExampleClassAllOp& oToCopy )
{
  operator=(oToCopy);
}

CcExampleClassAllOp::CcExampleClassAllOp( CcExampleClassAllOp&& oToMove )
{
  operator=(std::move(oToMove));
}

CcExampleClassAllOp::~CcExampleClassAllOp( void )
{
}

CcExampleClassAllOp& CcExampleClassAllOp::operator=(const CcExampleClassAllOp& oToCopy)
{
  CCUNUSED(oToCopy);
  return *this;
}

CcExampleClassAllOp& CcExampleClassAllOp::operator=(CcExampleClassAllOp&& oToMove)
{
  if(this != &oToMove)
  {
  }
  return *this;
}

bool CcExampleClassAllOp::operator==(const CcExampleClassAllOp& oToCompare) const
{
  bool bRet = false;
  CCUNUSED(oToCompare);
  return bRet;
}

bool CcExampleClassAllOp::operator!=(const CcExampleClassAllOp& oToCompare) const
{
  return !operator==(oToCompare);
}