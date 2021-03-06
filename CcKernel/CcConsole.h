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
 * @subpage   CcConsole
 *
 * @page      CcConsole
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcConsole
 */
#ifndef H_CcConsole_H_
#define H_CcConsole_H_

#include "CcBase.h"
#include "CcBase.h"

class CcString;
class CcStdIn;
class CcStdOut;
class CcByteArray;
class CcMutex;

/**
 * @brief Static class for Console in- and output.
 *        The default target for this class is stdin and stdout, but can be changed during runtime.
 */
class CcKernelSHARED CcConsole 
{
public:
  static void init();
  static void deinit();

  /**
   * @brief Redirect console input to an other IO-Device.
   * @param pInDev: Target device to get input data from
   * @return void
   */
  static void setInputDevice(CcStdIn* pInDev);

  /**
   * @brief Redirect console output to an other IO-Device.
   * @param pInDev: Target device to get input data from
   * @return void
   */
  static void setOutputDevice(CcStdOut* pOutDev);
  
  /**
   * @brief Read to a fixed buffer from commandline.
   * @param pBuffer: Target buffer to read to.
   * @param uiSize: Size of pBuffer.
   * @return Number of bytes read from console
   */
  static size_t read(void* pBuffer, size_t uiSize);

  /**
   * @brief Write a fixed buffer to commandline.
   * @param pBuffer: Target buffer to write from.
   * @param uiSize: Size of pBuffer.
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
   * @param oReturn: Data stored in CcByteArray container.
   * @param uiBufSize: Transfersize within this method.
   * @return number of read characters or SIZE_MAX if error occured
   */
  static size_t readAll(CcByteArray& oReturn, size_t uiBufSize);

  /**
   * @brief Read from input device until EOL is reached
   *        EOL will be excluded from return.
   * @param uiBufSize: Transfersize within this method.
   * @return number of read characters or SIZE_MAX if error occured
   */
  static size_t readLine(CcString& sReturn);
  
  /**
   * @brief Read from input device until EOL is reached but do not publish string to user.
   *        EOL will be excluded from return.
   * @param sReturn: String to write read data from console to
   * @return number of characters read.
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
   *        This method will start with a \r befor writing string and does not end with EOL.
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

  static void disableBuffering();

  static CcStdOut* getOutStream();

  static CcStdIn* getInStream();
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
  static CcStdIn*    s_pInput;
  static CcStdOut*   s_pOutput;
};

#endif // H_CcConsole_H_
