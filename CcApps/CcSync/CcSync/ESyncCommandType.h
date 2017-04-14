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
 * @page      ESyncCommandType
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class ESyncCommandType
 **/
#ifndef ESyncCommandType_H_
#define ESyncCommandType_H_

#include "CcBase.h"

/**
 * @brief Class impelmentation
 */
enum class ESyncCommandType : uint16
{
  Unkown              =      0   ,
  AllOk                          ,
  Crc                            ,
  Close                          ,
  ServerGetInfo          = 0x0100,
  ServerAccountCreate            ,
  AccountCreate          = 0x0200,
  AccountLogin                   ,
  AccountGetData                 ,
  AccountSetData                 ,
  AccountGetDirectoryList        ,
  AccountGetCommandList          ,
  DirectoryGetFileList   = 0x0300,
  DirectoryGetFileInfo           ,
  DirectoryGetDirectoryInfo      ,
  DirectoryCreateDirectory       ,
  DirectoryRemoveDirectory       ,
  DirectoryUploadFile            ,
  DirectoryDownloadFile          ,
  DirectoryRemoveFile            ,
  DirectoryUpdateFileInfo        ,
};

#endif /* ESyncCommandType_H_ */
