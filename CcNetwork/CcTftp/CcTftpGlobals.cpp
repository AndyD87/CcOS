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
 * @page      CcNetwork
 * @subpage   CcTftpGlobals
 *
 * @page      CcTftpGlobals
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 */

#include "CcBase.h"
#include "CcTftpGlobals.h"

namespace CcTftpGlobals
{
  namespace ErrorMsgs
  {
    const CcString NoError("NoError");
    const CcString FileNotFound("FileNotFound");
    const CcString AccessViolation("AccessViolation");
    const CcString DiskFull("DiskFull");
    const CcString IllegalOperation("IllegalOperation");
    const CcString UnknownTransfer("UnknownTransfer");
    const CcString FileAlreadyExists("FileAlreadyExists");
    const CcString NoSuchUser("NoSuchUser");
    const CcString Termination("Termination");
    const CcString Unknown("Unknown Error");
  }
};
