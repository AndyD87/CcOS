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
 */

#include "CcBase.h"
#include "CcScpiGlobals.h"

namespace CcScpiGlobals
{
  namespace ErrorMsgs
  {
    const CcString NoError("NoError");                      //!< No error report
    const CcString FileNotFound("FileNotFound");            //!< File not found error string
    const CcString AccessViolation("AccessViolation");      //!< Acesss violation error string
    const CcString DiskFull("DiskFull");                    //!< Disk full error string
    const CcString IllegalOperation("IllegalOperation");    //!< Illegal operation error string
    const CcString UnknownTransfer("UnknownTransfer");      //!< Unknown transfer operation error string
    const CcString FileAlreadyExists("FileAlreadyExists");  //!< File already exists error string
    const CcString NoSuchUser("NoSuchUser");                //!< User authentication failed error string
    const CcString Termination("Termination");              //!< Terminating error string
    const CcString Unknown("Unknown Error");                //!< Unknown error string
  }
};
