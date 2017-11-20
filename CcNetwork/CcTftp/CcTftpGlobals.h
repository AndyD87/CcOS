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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 */

#include "CcBase.h"
#include "ETftpServer.h"
#include "CcString.h"


namespace CcTftpGlobals
{
namespace ErrorMsgs
{
static const CcString NoError("NoError");
static const CcString FileNotFound("FileNotFound");
static const CcString AccessViolation("AccessViolation");
static const CcString DiskFull("DiskFull");
static const CcString IllegalOperation("IllegalOperation");
static const CcString UnknownTransfer("UnknownTransfer");
static const CcString FileAlreadyExists("FileAlreadyExists");
static const CcString NoSuchUser("NoSuchUser");
static const CcString Termination("Termination");
static const CcString Unknown("Unknown Error");
}
};


namespace CcTftpGlobals
{
}