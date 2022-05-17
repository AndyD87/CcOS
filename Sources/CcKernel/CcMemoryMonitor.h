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
 * @brief     Class CcMemoryMonitor
 **/
#pragma once

// Include only CcBase for common types.
#include "CcBase.h"

#ifdef USE_STD_MUTEX
#include <mutex>
#define CcMemoryMonitor_Type std::mutex       //!< Mutex type on common c++ frameworks
#elif defined(LINUX)
#include <pthread.h>
#define CcMemoryMonitor_Type pthread_mutex_t  //!< Mutex type on linux systems
#elif defined(WINDOWS)
#include <windows.h>
#define CcMemoryMonitor_Type CRITICAL_SECTION //!< Mutex type on windows systems
#else
#define CcMemoryMonitor_Type volatile bool    //!< Mutex type on generic systems
#endif

class IIo;
class IKernel;

/**
 * @brief Implementation of memory monitor managed by CcKernel and logging
 *        option for all new and delete methods.
 *        Helper methods CCNEW CCDELETE and their derivates should be used to track buffers.
 */
class CcKernelSHARED CcMemoryMonitor
{
public:
  /**
   * @brief Info storage for a tracked buffer.
   */
  class CItem
  {
  public:
    /**
     * @brief Initialize item with buffer
     * @param pBuffer: Taret buffer to track
     */
    CItem(const void* pBuffer) : pBuffer(pBuffer){}

    /**
     * @brief Copy buffer info from another item
     * @param rItem: Item to copy from
     */
    CItem(const CItem& rItem) : uiIndex(rItem.uiIndex), pBuffer(rItem.pBuffer), iLine(rItem.iLine), pFile(rItem.pFile)
    {}

    size_t      uiIndex = 0;        //!< Current index of raise only index for every buffer
    const void* pBuffer = nullptr;  //!< Butter value to track
    size_t      iLine = 0;          //!< Line number of tacking buffer, normaly generated with __LINE__
    const char* pFile = NULL;       //!< File path of tacking buffer, normaly generated with __FILE__
    static size_t uiCurrentIndex;   //!< Raise only index, it will be incremented on every use
  };

  /**
   * @brief Memory interface for overwrite list to write to.
   */
  typedef struct _SInterface
  {
    //! Pointer to delete memory form MemoryManager
    void (*opDelMemory)(const void*);
    //! Pointer to add memory to MemoryManager
    void (*opNewMemory)(
        const void* pBuffer,                 //!< Buffer of new memory for log
        const char* pFile,                   //!< File from __FILE__
        size_t iLine                         //!< Line from __LINE__
    );
  } SInterface; //!< Type of _SInterface

  /**
   * @brief Enable MemoryMonitor to accept inserts to list
   */
  static void enable();

  /**
   * @brief Disable MemoryMonitor. Inserts and deletes will be ignored
   */
  static void disable();

  /**
   * @brief Check if MemoryMonitor is enabled and ready for inputs.
   * @return True if enabled.
   */
  static bool isEnabled();

  /**
   * @brief Initialize MemoryMonitor. Create lists and reset counters.
   */
  static void init();

  /**
   * @brief Deinitialize MemoryMonitor. Clear lists.
   */
  static void deinit();

  /**
   * @brief Insert buffer to tracking list
   * @param pBuffer:  Buffer to add to list
   * @param pFile:    File where buffer was created by __FILE__
   * @param iLine:    Line in file where buffer was created by __LINE__
   */
  static void insert(const void* pBuffer, const char* pFile, size_t iLine);

  /**
   * @brief Remove tracked buffer from list
   * @param pBuffer:  Buffer to remove
   */
  static void remove(const void* pBuffer);

  /**
   * @brief Print stored buffers in memory list.
   * @param pStream: Stream to write informations to.
   */
  static void printLeft(IIo* pStream);

  /**
   * @brief Get number of buffers stored in list.
   * @return Size of buffer list
   */
  static size_t getAllocationCount();

  /**
   * @brief Clear all content from Memory monitor.
   */
  static void clear();

  /**
   * @brief Set interface to overwrite inserts and deletes.
   * @param pInterface: Target interface to write to.
   */
  static void setInterface(SInterface* pInterface);

  inline void* insert_inline(void* pNewObject, const char* pFile, size_t iLine)
  { insert(pNewObject, pFile, iLine); return pNewObject; }

private:
  static bool contains(const void* pBuffer);
  static void lock();
  static void unlock();
  static bool isLocked();
  /**
   * @brief Constructor
   */
  CcMemoryMonitor() = delete;

  /**
   * @brief Destructor
   */
  ~CcMemoryMonitor() = delete;
private:
  static CcMemoryMonitor_Type m_oContext;
};

/**
 * @brief Remove buffer from tracking list
 * @param pBuffer: Target buffer to remove from tracking list
 */
extern CcKernelSHARED void CcMemoryMonitor__remove(const void* pBuffer);

/**
 * @brief Append buffer to tracking list with additional compile informations
 * @param pBuffer:  Target buffer to append to tracking list
 * @param pFile:    File where this method will be executed
 * @param iLine:    Line of file where this method will be executed
 */
extern CcKernelSHARED void CcMemoryMonitor__insert(const void* pBuffer, const char* pFile, int iLine);

/**
 * @brief Append buffer to tracking list with additional compile informations
 * @param pBuffer:  Target buffer to append to tracking list
 * @param pFile:    File where this method will be executed
 * @param iLine:    Line of file where this method will be executed
 * @return pointer to inserted buffer
 */
extern CcKernelSHARED void* CcMemoryMonitor__insert_inline(void* pBuffer, const char* pFile, int iLine);

