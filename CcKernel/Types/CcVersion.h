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
 * @page      Types
 * @subpage   CcVersion
 *
 * @page      CcVersion
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcVersion
 */
#ifndef CcVersion_H
#define CcVersion_H

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcString.h"

class CcKernelSHARED CcVersion
{
public:
  CcVersion();
  CcVersion(const CcString& sVersion);
  CcVersion(uint8 uiMajor, uint8 uiMinor);
  CcVersion(uint8 uiMajor, uint8 uiMinor, uint16 uiBuild, uint16 uiRevision);
  virtual ~CcVersion();

  bool setVersionString(const CcString& sVersion);
  inline void setMajor(uint8 uiMajor) { m_uiMajor = uiMajor; }
  inline void setMinor(uint8 uiMinor) { m_uiMinor = uiMinor; }
  inline void setBuild(uint16 uiBuild) { m_uiBuild = uiBuild; }
  inline void setRevision(uint16 uiRevision) { m_uiRevision = uiRevision; }

  inline uint8 getMajor() { return m_uiMajor; }
  inline uint8 getMinor() { return m_uiMinor; }
  inline uint16 getBuild() { return m_uiBuild; }
  inline uint16 getRevision() { return m_uiRevision; }

private:
  void setVersionByPosition(uint8 uiPos, uint16 uiVersionNr);
private:
  uint8 m_uiMajor;
  uint8 m_uiMinor;
  uint16 m_uiBuild;
  uint16 m_uiRevision;
};

#endif // CcVersion_H
