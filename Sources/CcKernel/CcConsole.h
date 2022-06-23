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
 * @brief     Class CcConsole
 */
#pragma once

#include "CcBase.h"
#include "IIo.h"
#include "CcString.h"

class CcString;
class CcStdIn;
class CcStdOut;
class CcMutex;
class CcByteArray;

/**
 * @brief Static class for Console in- and output.
 *        The default target for this class is stdin and stdout, but can be changed during runtime.
 */
class CcKernelSHARED CcConsole
{
public:
  /**
   * @brief Initialize console operations
   */
  static void init();

  /**
   * @brief Deinitialize console operations an cleanup
   */
  static void deinit();

  /**
   * @brief Redirect console input to an other IO-Device.
   * @param pInDev: Target device to get input data from
   * @return void
   */
  static void setInputDevice(IIo* pInDev);

  /**
   * @brief Redirect console output to an other IO-Device.
   * @param pOutDev: Target device to write output to
   * @return void
   */
  static void setOutputDevice(IIo* pOutDev);

  /**
   * @brief Read to a fixed buffer from commandline.
   * @param pBuffer: Target buffer to read to.
   * @param uiSize: Size of @p pBuffer.
   * @return Number of bytes read from console
   */
  static size_t read(void* pBuffer, size_t uiSize);

  /**
   * @brief Write a fixed buffer to commandline.
   * @param pBuffer: Target buffer to write from.
   * @param uiSize: Size of @p pBuffer.
   * @return Number of bytes written to console
   */
  static size_t write(const void* pBuffer, size_t uiSize);

  /**
   * @brief Read data from commandline direct to a ByteArray.
   * @param[in,out] oOutputBuffer: Target preallocated OutputBuffer to write data to.
   *                               At the end, buffer will be resized to received data size.
   * @return number of bytes, read from console
   */
  static size_t readArray(CcByteArray& oOutputBuffer);

  /**
   * @brief Read all data from commandline direct to a ByteArray.
   *
   *        This Method will call readArray until no new data is received.
   *        To control size of data wich is read in one step of readArray, change uiBufSize.
   * @param[out] oReturn: Data stored in CcByteArray container.
   * @param uiBufSize: Transfersize within this method.
   * @return number of read characters or SIZE_MAX if error occured
   */
  static size_t readAll(CcByteArray& oReturn, size_t uiBufSize);

  /**
   * @brief Read from input device until EOL is reached
   *        EOL will be excluded from return.
   * @param[out] sReturn: Data received as string.
   * @return Number of read characters read or SIZE_MAX if error occured
   */
  static size_t readLine(CcString& sReturn);

  /**
   * @brief Read from input device until EOL is reached but do not publish string to user.
   *        EOL will be excluded from return.
   * @param sReturn: String to write read data from console to
   * @return Number of characters read.
   */
  static size_t readLineHidden(CcString& sReturn);

  /**
   * @brief Write a line to console.
   *        This method will additionally write a EOL at the end.
   * @param sOutput: Output to write to console
   */
  static void writeLine(const CcString& sOutput);

  /**
   * @brief Write a line to console.
   *        This method will start with a \\r befor writing string and does not end with EOL.
   *        Please make sure that after writeSameLine at least one writeLine for a common look and feel.
   * @param sOutput: Output to write to console
   */
  static void writeSameLine(const CcString& sOutput);

  /**
   * @brief Write a set of Data, stored in Array, to commandline.
   * @param oArray: Data container to write to commandline.
   */
  static void writeArray(const CcByteArray& oArray);

  /**
   * @brief Write a String to commandline
   * @param sOutput: String to write to console
   * @return
   */
  static void writeString(const CcString& sOutput);

  /**
   * @brief Disable buffering so all data gets flushed instantly
   */
  static void disableBuffering();

  /**
   * @brief Get pointer to default output stream
   * @return Default stdout instance
   */
  static IIo& getOutStream();

  /**
   * @brief Get pointer to default input stream
   * @return Default stdin instance
   */
  static IIo& getInStream();
private:
  /**
   * @brief Constructor
   */
  CcConsole() = delete;

  /**
   * @brief Destructor
   */
  ~CcConsole() = delete;

private:
  static CcMutex*    s_pLock;
  static IIo*        s_pInput;
  static IIo*        s_pOutput;
  static CcString    s_ReadLineBuffer;
};
