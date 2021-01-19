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
 * @brief     Class IIo
 */
#pragma once

#include "CcBase.h"

// forward declarations
class CcByteArray;
class CcString;
class CcBufferList;

/**
 * @brief Namespace for EOpenFlags for better readablity
 */
enum class EOpenFlags : uint16
{
  NoFlag    = 0x0000,   //!< Open device with no special settings.
  Read      = 0x0001,   //!< Open Read-Mode of Device.
  Write     = 0x0002,   //!< Open Write-Mode of Device.
  ReadWrite = 0x0003,   //!< Open Read-and-Write-Mode of Device.
  Append    = 0x0004,   //!< Open Appen-Mode of Device.
  Overwrite = 0x0012,   //!< Open Overwrite-Mode of Target Device.
  ShareRead = 0x0100,   //!< Open Read-Mode of Device.
  ShareWrite= 0x0200,   //!< Open Write-Mode of Device.
  Attributes= 0x8000
};

/**
 * @brief OR operatioin for EOpenFlags
 * @param leftOp:   First OR object
 * @param rightOp:  Second OR object
 * @return Result of OR operation
 */
inline EOpenFlags operator|(EOpenFlags leftOp, EOpenFlags rightOp)
{
  return static_cast<EOpenFlags>
    (static_cast<uint16>(leftOp) | static_cast<uint16>(rightOp));
}

/**
 * @brief AND operatioin for EOpenFlags
 * @param leftOp:   First AND object
 * @param rightOp:  Second AND object
 * @return Result of AND operation
 */
inline EOpenFlags operator&(EOpenFlags leftOp, EOpenFlags rightOp)
{
  return static_cast<EOpenFlags>
    (static_cast<uint16>(leftOp) & static_cast<uint16>(rightOp));
}

/**
 * @brief Abstract Class for inheriting to every IODevice
 */
class CcKernelSHARED IIo
{
public:
  /**
   * @brief Define common control requests as enum
   */
  enum class EControls
  {
    ENoBlocking,  //!< Set read / write request non blocking.
  };

  IIo() = default;
  virtual ~IIo() = default;

  /**
   * @brief Enumeration for Read, Write, none, or both
   */
  enum class ERwMode : uint8
  {
    Unknown   = 0,
    Read      = 1,
    Write     = 2,
    ReadWrite = 3,
  };

  /**
   * @brief Write string to output stream
   * @param toAdd: String to write
   * @return Handle to this to continue with next write request
   */
  IIo& operator<<(const CcString& toAdd);

  /**
   * @brief Write char array out to output stream until \\0 is reached
   * @param toAdd: String to write
   * @return Handle to this to continue with next write request
   */
  IIo& operator<<(const char *toAdd);

  /**
   * @brief Read an amount of Data from inheriting stream.
   * @param pBuffer: Buffer to load data to.
   * @param uSize:   Maximum Size of buffer to write.
   * @return Number of Bytes read from stream.
   */
  virtual size_t read(void* pBuffer, size_t uSize) = 0;

  /**
   * @brief Write an amount of Data to inheriting stream.
   * @param pBuffer: Buffer to load data from.
   * @param uSize:   Maximum size of buffer to read.
   * @return Number of Bytes written to stream.
   */
  virtual size_t write(const void* pBuffer, size_t uSize) = 0;

  /**
   * @brief Open stream in a specific mode.
   *        For more informations lock at: @ref EOpenFlags
   * @return true if stream was opened successfully.
   */
  virtual CcStatus open(EOpenFlags) = 0;

  /**
   * @brief Close the connection to stream.
   * @return true if Connection was successfully closed.
   */
  virtual CcStatus close() = 0;

  /**
   * @brief Cancel Current Operation.
   *        It can optionally be Implemented from inheriting stream.
   *        Look at stream definintion it it supports canceling.
   * @return true if Opperation was aborted successfully.
   */
  virtual CcStatus cancel() = 0;

  /**
   * @brief Communication to stream with it's IO API if supported.
   * @param cmd:        IO-Command as 32bit value.
   * @param pInArg:     Pointer to argument data to pass to the stream.
   * @param uiInSize:   Size of pInArg
   * @param pOutArg:    Pointer to result data from stream
   * @param uiOutSize:  Size of pOutArg
   * @param puiWritten: Number of bytes written to pOutArg
   * @return Status of operation
   */
  virtual CcStatus ioControl(uint32 cmd, const void *pInArg = nullptr, size_t uiInSize = 0, void *pOutArg = nullptr, size_t uiOutSize = 0, size_t* puiWritten = nullptr);

  /**
   * @brief Communication to stream with it's IO API if supported.
   * @param eControl: Known Command as enum
   * @param pInArg:     Pointer to argument data to pass to the stream.
   * @param uiInSize:   Size of pInArg
   * @param pOutArg:    Pointer to result data from stream
   * @param uiOutSize:  Size of pOutArg
   * @param puiWritten: Number of bytes written to pOutArg
   * @return Status of operation
   */
  virtual CcStatus ioControl(EControls eControl, const void *pInArg = nullptr, size_t uiInSize = 0, void *pOutArg = nullptr, size_t uiOutSize = 0, size_t* puiWritten = nullptr);

  /**
   * @brief For some features on supported Operating Systems it is usefull to get the system file handle
   *        if required. For example it is cated from FILE* to void*
   * @return System file handle or nullptr if not existing.
   */
  virtual void* getStdFile();
  
  /**
   * @brief Read data from IO direct to a ByteArray.
   * @param[in,out] oOutputArray: Target preallocated OutputBuffer to write the read data to.
   *                              At the end, buffer will be resized to received data size.
   * @param bDoResize             If true, oOutputArray will be resized to number of bytes read.
   * @return Number of bytes, read from IO.
   */
  size_t readArray(CcByteArray& oOutputArray, bool bDoResize = true);

  /**
   * @brief Read all available data until stream ends.
   *        The end is marked by read of 0 or SIZE_MAX bytes.
   * @param uiBufSize: Maximum buffer size to read at once.
   * @return Buffer read from stream
   */
  CcByteArray readAll(size_t uiBufSize = 1024);

  /**
   * @brief Write a byte array to stream
   * @param oArray: ByteArray to write out
   * @return Status of operation
   */
  CcStatus writeArray(const CcByteArray& oArray);

  /**
   * @brief Write a string to stream
   * @param oString: String to write out
   * @return Status of operation
   */
  CcStatus writeString(const CcString& oString);

  /**
   * @brief Write a string to stream and append new line signs
   * @param oString: String to write out
   * @return Status of operation
   */
  CcStatus writeLine(const CcString& oString);

  /**
   * @brief Write a buffer list out to stream
   * @param oBufferList: Buffer to write out
   * @return Status of operation
   */
  CcStatus writeBufferList(const CcBufferList& oBufferList);
};
