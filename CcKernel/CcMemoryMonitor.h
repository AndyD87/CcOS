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
 * @subpage   CcMemoryMonitor
 *
 * @page      CcMemoryMonitor
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcMemoryMonitor
 **/
#ifndef H_CcMemoryMonitor_H_
#define H_CcMemoryMonitor_H_

#include "CcKernelBase.h"
#include <list>

class IIo;

/**
 * @brief Class implementation
 */
class CcKernelSHARED CcMemoryMonitor 
{
public:
  class CItem
  {
  public:
    CItem(const void* pBuffer) : pBuffer(pBuffer){}
    CItem(const CItem& rItem) : uiIndex(rItem.uiIndex), pBuffer(rItem.pBuffer), iLine(rItem.iLine), pFile(rItem.pFile)
    {}
    size_t      uiIndex = 0;
    const void* pBuffer = nullptr;
    size_t      iLine = 0;
    const char* pFile = NULL;
    static size_t uiCurrentIndex;
  };

  static void enable();
  static void disable();
  static bool isEnabled();
  static void init();
  static void deinit();
  static void lock();
  static void unlock();
  static void insert(const void* pBuffer, const char* pFile, size_t iLine);
  static void remove(const void* pBuffer);
  static void printLeft(IIo* pStream);
  static size_t getAllocationCount();
  static void clear();

private:
  static bool contains(const void* pBuffer);
  /**
   * @brief Constructor
   */
  CcMemoryMonitor() = delete;

  /**
   * @brief Destructor
   */
  ~CcMemoryMonitor() = delete;
};

#endif // H_CcMemoryMonitor_H_
