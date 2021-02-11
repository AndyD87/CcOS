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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcLinuxSharedMemory
 *            Abstract Class to define an interface to System own Shared Memory
 */
#ifndef H_CcLinuxSharedMemory_H_
#define H_CcLinuxSharedMemory_H_

#include "CcBase.h"
#include "CcBase.h"
#include "CcString.h"
#include "ISharedMemory.h"

class CcLinuxSharedMemoryPrivate;

/**
 * @brief Implementation of Shared Memory within Linux Systems
 */
class CcLinuxSharedMemory : public ISharedMemory
{
public:
  /**
   * @brief Create a Shared Memory by name and with specific size
   * @param sName: Name of new Memory
   * @param uiSize: size of new Memory
   */
  CcLinuxSharedMemory(const CcString& sName, size_t uiSize);

  /**
   * @brief Destructor
   */
  virtual ~CcLinuxSharedMemory();

  /**
   * @brief Create a shared memory and set the access right.
   * @param eOpenFlags: Required Accessrights as enum
   * @return CcStatus
   */
  virtual CcStatus open(EOpenFlags eOpenFlags) override;

  /**
   * @brief Close the currently accessed Shared Memory
   * @return CcStatus
   */
  virtual CcStatus close() override;

  /**
   * @brief Cancel an currently running operation like read and write
   * @return CcStatus
   */
  virtual CcStatus cancel() override;

  /**
   * @brief Claim will not create a new Shared memory, it will attach to one.
   * @param eOpenFlags: Required Accessrights as enum
   * @return CcStatus
   */
  virtual CcStatus claim(EOpenFlags eOpenFlags) override;

  /**
   * @brief Check if a Shared Memory is already available.
   * @return true if available
   */
  virtual bool exists() override;

  /**
   * @brief Read data from share memory to an allocated buffer.
   * @param pBuffer: Pointer to allocated Buffer
   * @param uiSize: Size of allocated Buffer
   * @return Number of Bytes read, or SIZE_MAX if an error occured
   */
  virtual size_t read(void* pBuffer, size_t uiSize) override;

  /**
   * @brief Write data to a share memory from buffer.
   * @param pBuffer: Pointer to Buffer
   * @param uSize:   Size of Buffer
   * @return Number of Bytes written, or SIZE_MAX if an error occured
   */
  virtual size_t write(const void* pBuffer, size_t uSize) override;

  /**
   * @brief Get current connected buffer
   * @return If Memory is connected successfully this method will return a non null pointer to buffer.
   */
  virtual void* getBuffer() override
  { return m_pBuffer; }

private:
  /**
   * @brief Open types on linux shared memory
   */
  enum class EOpenType
  {
    Closed,
    Created,
    Claimed,
  };

  CcString   m_sName;
  size_t     m_uiSize = 0;
  void*      m_pBuffer = nullptr;
  int        m_iId = -1;
  EOpenType  m_eOpenType = EOpenType::Closed;
};

#endif // H_CcLinuxSharedMemory_H_
