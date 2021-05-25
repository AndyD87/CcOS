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
 * @brief     Class CcTestSocket
 **/
#pragma once

#include "CcBase.h"
#include "CcByteArray.h"
#include "Network/ISocket.h"

class IKernel;

/**
 * @brief CcTestSocket is used to used to test if loading libraries is working.
 */
class CcTestSocket : public ISocket
{
public:
  CcTestSocket();
  virtual ~CcTestSocket();
  virtual CcStatus setAddressInfo(const CcSocketAddressInfo& oAddrInfo) override
  { m_oAddressInfo = oAddrInfo; return true; }
  virtual CcSocketAddressInfo& getAddressInfo() override
  { return m_oAddressInfo; }
  virtual CcStatus bind() override
  { return true; }
  virtual CcStatus connect() override
  { return true; }
  virtual CcStatus listen() override
  { return true; }
  virtual ISocket* accept() override;
  virtual CcStatus setTimeout(const CcDateTime& CCUNUSED_PARAM(uiTimeValue), ERwMode CCUNUSED_PARAM(eMode)) override
  { return true; }
  virtual CcSocketAddressInfo& getPeerInfo() override
  { return m_oPeerInfo; }
  virtual void setPeerInfo(const CcSocketAddressInfo& oPeerInfo) override
  { m_oPeerInfo = oPeerInfo; }
  virtual CcSocketAddressInfo getHostByName(const CcString& sHostname) override;
  virtual CcStatus setOption(ESocketOption eOption, void* pData, size_t uiDataLen) override;
  virtual CcStatus setOptionRaw(int iLevel, int iOptName, void* pData, size_t uiDataLen) override;

  virtual size_t read(void* pBuffer, size_t uSize) override;
  virtual size_t write(const void* pBuffer, size_t uSize) override;
  virtual CcStatus open(EOpenFlags CCUNUSED_PARAM(flags)) override { return true; }
  virtual CcStatus close() override { return true; }
  virtual CcStatus cancel() override { return true; }

  CcByteArray& getReadData()
  { return m_oReadData; }
  CcByteArray& getWriteData()
  { return m_oWriteData; }

private:
  CcSocketAddressInfo m_oAddressInfo; 
  CcSocketAddressInfo m_oPeerInfo;
  CcByteArray         m_oReadData;
  CcByteArray         m_oWriteData;
};
