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
 * @brief     Class CcAppDirectories
 */
#pragma once

#include "CcBase.h"
#include "CcString.h"

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
   * @param bUserContext: If true paths will be set to user locations like in in ~/.CcOS/etc/sSubDir, false will set system default paths like /etc.
   * @param bCreateIfNotExistst: If true, instantly create all directories. If false, it could be done later with createAllPaths
   */
  CcAppDirectories(const CcString& sSubDir, bool bUserContext, bool bCreateIfNotExistst = false);

  /**
   * @brief Destructor
   */
  virtual ~CcAppDirectories();

  //! @return Reference to configuruation directory path
  const CcString& getConfigDir()
  { return m_sConfigDir; }
  //! @return Reference to data directory path
  const CcString& getDataDir()
  { return m_sDataDir; }
  //! @return Reference to log directory path
  const CcString& getLogDir()
  { return m_sLogDir; }
  //! @return Reference to data subdirectory path set by application
  const CcString& getSubDir()
  { return m_sSubDir; }

  /**
   * @brief Create all paths if they are note existing recursively
   * @param bFailOnExists:  If this value is true, the return value will be false,
   *                        if at least on directory was already existing.
   * @return True if all paths are created.
   */
  bool createAllPaths(bool bFailOnExists = true);
private:
  void setupPaths(bool bUserContext);

private:
  CcString m_sSubDir;
  CcString m_sConfigDir;
  CcString m_sDataDir;
  CcString m_sLogDir;
};
