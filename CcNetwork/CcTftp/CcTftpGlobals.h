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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 */

#include "CcBase.h"
#include "CcTftp.h"
#include "ETftpServer.h"
#include "CcString.h"


namespace CcTftpGlobals
{
  namespace ErrorMsgs
  {
    extern const CcTftpSHARED CcString NoError;
    extern const CcTftpSHARED CcString FileNotFound;
    extern const CcTftpSHARED CcString AccessViolation;
    extern const CcTftpSHARED CcString DiskFull;
    extern const CcTftpSHARED CcString IllegalOperation;
    extern const CcTftpSHARED CcString UnknownTransfer;
    extern const CcTftpSHARED CcString FileAlreadyExists;
    extern const CcTftpSHARED CcString NoSuchUser;
    extern const CcTftpSHARED CcString Termination;
    extern const CcTftpSHARED CcString Unknown;
  }
};
