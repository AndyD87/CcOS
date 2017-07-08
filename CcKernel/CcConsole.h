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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcConsole
 */
#ifndef CcConsole_H_
#define CcConsole_H_

#include "CcBase.h"
#include "CcKernelBase.h"

class CcString;
class CcIODevice;
class CcByteArray;

/**
 * @brief Static class for Console in- and output.
 *        The default target for this class is stdin and stdout, but can be changed during runtime.
 */
class CcKernelSHARED CcConsole 
{
public:
  /**
   * @brief Redirect console input to an other IO-Device.
   * @param pInDev: Target device to get input data from
   * @return void
   */
  static void setInputDevice(CcIODevice *pInDev)
    { s_Input = pInDev; }

  /**
   * @brief Redirect console output to an other IO-Device.
   * @param pInDev: Target device to get input data from
   * @return void
   */
  static void setOutputDevice(CcIODevice *pOutDev)
    { s_Output = pOutDev; }
  
  /**
   * @brief Read to a fixed buffer from commandline.
   * @param pBuffer: Target buffer to read to.
   * @param uiSize: Size of pBuffer.
   * @return Number of bytes read from console
   */
  static size_t read(char* pBuffer, size_t uiSize);

  /**
   * @brief Write a fixed buffer to commandline.
   * @param pBuffer: Target buffer to write from.
   * @param uiSize: Size of pBuffer.
   * @return Number of bytes written to console
   */
  static size_t write(const char* pBuffer, size_t uiSize);

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
   * @param uiBufSize: Transfersize within this method.
   * @return Data stored in CcByteArray container.
   */
  static CcByteArray readAll(size_t uiBufSize);

  /**
   * @brief Read from input device until EOL is reached
   *        EOL will be excluded from return.
   * @return Whole line as String.
   */
  static CcString readLine();

  /**
   * @brief Write a line to console.
   *        This method will additionally write a EOL at the end.
   * @param sOutput: Output to write to console
   */
  static void writeLine(const CcString& sOutput);

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

private:
  /**
   * @brief Constructor
   */
  CcConsole(void);

  /**
   * @brief Destructor
   */
  ~CcConsole(void);

  static CcIODevice *s_Output; //!< Output device
  static CcIODevice *s_Input;  //!< Input device
};

#endif /* CcConsole_H_ */
