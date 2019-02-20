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
 * @page      CcKernel
 * @subpage   CcAppDirectories
 *
 * @page      CcAppDirectories
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcAppDirectories
 */
#ifndef _CcAppDirectories_H_
#define _CcAppDirectories_H_

#include "CcBase.h"
#include "CcString.h"
#include "CcKernelBase.h"

/**
 * @brief Common Class for managing Application directores with config, data and temporary directories.
 */
class CcKernelSHARED CcAppDirectories
{
public:
  /**
   * @brief Setup directory paths with given sub directory.
   *        The sub direcotry will be appended to each default locations for system or data.
   * @param sSubDir:      Path to append to system paths
   * @param bUserContext: If true paths will be set to user locations, false will set default paths.
   */
  CcAppDirectories(const CcString& sSubDir, bool bUserContext);

  /**
   * @brief Destructor
   */
  virtual ~CcAppDirectories();

  const CcString& getConfigDir()
    {return m_sConfigDir;}
  const CcString& getDataDir()
    {return m_sConfigDir;}
  const CcString& getSubDir()
    {return m_sConfigDir;}
  const CcString& getLogDir()
    {return m_sConfigDir;}

private:
  void setupPaths(bool bUserContext);

private:
  CcString m_sSubDir;
  CcString m_sConfigDir;
  CcString m_sDataDir;
  CcString m_sLogDir;
};

#endif /* _CcAppDirectories_H_ */