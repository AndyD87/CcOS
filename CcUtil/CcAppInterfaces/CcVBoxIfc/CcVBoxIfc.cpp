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
 * @brief     Implementation of Class CcVBoxIfc
 */
#include "CcVBoxIfc.h"
#include "CcString.h"
#include "CcStringUtil.h"
#include "Hash/CcSha256.h"

class CcVBoxIfc::CPrivate
{
public:
  static const CcString sCommonExecutable;
};

const CcString CcVBoxIfc::CPrivate::sCommonExecutable("vboxmanage");

CcVBoxIfc::CcVBoxIfc(const CcString& sPath) :
  m_sExePath(sPath)
{
  if(m_sExePath.length() == 0)
  {

  }
}

CcVBoxIfc::~CcVBoxIfc()
{
}
