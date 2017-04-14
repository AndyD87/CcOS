/**
 * @copyright  Andreas Dirmeier (C) 2017
 *
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
 * @author    Andreas Dirmeier
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implementation of Class CcStdFile
 */

#include "CcStdFile.h"
#include "CcKernel.h"
#include "CcThreadObject.h"
#include "CcByteArray.h"
#include "stdio.h"

CcStdFile::CcStdFile(FILE * stdFile) :
m_File(stdFile)
{
}

CcStdFile::~CcStdFile() {

}
size_t CcStdFile::size(void)
{
  return SIZE_MAX;
}

size_t CcStdFile::read(char* buffer, size_t size)
{
  return fread(buffer, size, sizeof(char), m_File);
}

size_t CcStdFile::write(const char* buffer, size_t size)
{
  return fwrite(buffer, size, sizeof(char), m_File);
}

bool CcStdFile::open(EOpenFlags flags)
{
  CCUNUSED(flags);
  return false;
}

bool CcStdFile::close()
{
  if (fclose(m_File))
    return false;
  return true;
}

bool CcStdFile::setFilePointer(size_t pos)
{
  // @todo: insert FilePointer Handling for std files.
  CCUNUSED(pos);
  /*if (lseek(m_File, &pos))
    return false;*/
  return true;
}

