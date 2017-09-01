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
 * @brief     Implemtation of class CcFileInfo
 */
#include "CcFileInfo.h"

CcFileInfo::CcFileInfo( void )
{
}

CcFileInfo::CcFileInfo( const CcFileInfo& oToCopy )
{
  operator=(oToCopy);
}

CcFileInfo::CcFileInfo( CcFileInfo&& oToMove )
{
  operator=(std::move(oToMove));
}

CcFileInfo::~CcFileInfo( void )
{
}

bool CcFileInfo::isDir() const
{
  bool bRet =false;
  bRet = IS_FLAG_SET(m_uiFlags, Directory);
  return bRet;
}

bool CcFileInfo::isFile() const
{
  bool bRet =false;
  bRet = IS_FLAG_NOT_SET(m_uiFlags, Directory);
  return bRet;
}

bool CcFileInfo::isExecutable() const
{
  bool bRet =false;
  return bRet;
}

bool CcFileInfo::isReadable() const
{
  bool bRet =false;
  return bRet;
}

bool CcFileInfo::isWritable() const
{
  bool bRet =false;
  return bRet;
}

void CcFileInfo::setFlags(uint16 uiFlags)
{
  m_uiFlags = uiFlags;
}

void CcFileInfo::addFlags(uint16 uiFlagsToAdd)
{
  m_uiFlags |= uiFlagsToAdd;
}

void CcFileInfo::removeFlags(uint16 uiFlagsToRome)
{
  m_uiFlags &= ~uiFlagsToRome;
}

void CcFileInfo::setName(const CcString& sFileName)
{
  m_sName = sFileName;
}

void CcFileInfo::setUserId(uint16 uiUserId)
{
  m_uiUserId = uiUserId;
}

void CcFileInfo::setGroupId(uint16 uiGroupId)
{
  m_uiGroupId = uiGroupId;
}

void CcFileInfo::setModified(CcDateTime oTime)
{
  m_oLastModified = oTime;
}

void CcFileInfo::setCreated(CcDateTime oTime)
{
  m_oCreated = oTime;
}

void CcFileInfo::setFileSize(uint64 uiFileSize)
{
  m_uiFileSize = uiFileSize;
}

CcString CcFileInfo::getFlagsString() const
{
  CcString sRet("----------");
  if (IS_FLAG_SET(m_uiFlags, Directory))
  {
    sRet[0] = 'd';
  }
  if (IS_FLAG_SET(m_uiFlags, UserExecute))
  {
    sRet[1] = 'x';
  }
  if (IS_FLAG_SET(m_uiFlags, UserRead))
  {
    sRet[2] = 'r';
  }
  if (IS_FLAG_SET(m_uiFlags, UserWrite))
  {
    sRet[3] = 'w';
  }
  if (IS_FLAG_SET(m_uiFlags, GroupExecute))
  {
    sRet[4] = 'x';
  }
  if (IS_FLAG_SET(m_uiFlags, GroupRead))
  {
    sRet[5] = 'r';
  }
  if (IS_FLAG_SET(m_uiFlags, GroupWrite))
  {
    sRet[6] = 'w';
  }
  if (IS_FLAG_SET(m_uiFlags, GlobalExecute))
  {
    sRet[7] = 'x';
  }
  if (IS_FLAG_SET(m_uiFlags, GlobalRead))
  {
    sRet[8] = 'r';
  }
  if (IS_FLAG_SET(m_uiFlags, GlobalWrite))
  {
    sRet[9] = 'w';
  }
  return sRet;
}

CcFileInfo& CcFileInfo::operator=(const CcFileInfo& oToCopy)
{
  m_sName         = oToCopy.m_sName        ;
  m_oCreated      = oToCopy.m_oCreated     ;
  m_oLastModified = oToCopy.m_oLastModified;
  m_uiFlags       = oToCopy.m_uiFlags      ;
  m_uiUserId      = oToCopy.m_uiUserId     ;
  m_uiGroupId     = oToCopy.m_uiGroupId    ;
  m_uiFileSize    = oToCopy.m_uiFileSize   ;
  return *this;
}

CcFileInfo& CcFileInfo::operator=(CcFileInfo&& oToMove)
{
  if(this != &oToMove)
  {
    m_sName = std::move(oToMove.m_sName);
    m_oCreated = oToMove.m_oCreated;
    m_oLastModified = oToMove.m_oLastModified;
    m_uiFlags = oToMove.m_uiFlags;
    m_uiUserId = oToMove.m_uiUserId;
    m_uiGroupId = oToMove.m_uiGroupId;
    m_uiFileSize = oToMove.m_uiFileSize;
  }
  return *this;
}

bool CcFileInfo::operator==(const CcFileInfo& oToCompare) const
{
  bool bRet = false;
  if (  m_sName == oToCompare.m_sName         &&
        m_uiFlags == oToCompare.m_uiFlags     &&
        m_uiUserId == oToCompare.m_uiUserId   &&
        m_uiGroupId == oToCompare.m_uiGroupId )
  {
    bRet = true;
  }
  return bRet;
}

bool CcFileInfo::operator!=(const CcFileInfo& oToCompare) const
{
  return !operator==(oToCompare);
}