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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcHttpServerConfig
 *           Protocol: http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html
 */
#include "CcKernel.h"
#include "CcHttpServerConfig.h"
#include "Network/CcSocket.h"
#include "Network/CcCommonIps.h"

CcHttpServerConfig::CcHttpServerConfig(uint16 uiPort):
#ifdef GENERIC
  m_oDefaultEncoding(CcHttpTransferEncoding::Chunked),
  m_uiMaxWorker(2),
  m_uiMaxTransferPacketSize(1540)
#else
  m_oDefaultEncoding(CcHttpTransferEncoding::Normal),
  m_uiMaxWorker(10),
  m_uiMaxTransferPacketSize(10240)
#endif 
{
  m_oAddressInfo.init(ESocketType::TCP);
  //m_oAddressInfo.setIp(CcIp(127, 0, 0, 1));
  m_oAddressInfo.setIp(CcCommonIps::AnyAddress);
  m_oAddressInfo.setPort(uiPort);
  if (uiPort == CcCommonPorts::HTTPS)
  {
    m_bSslEnabled = true;
  }
}

void CcHttpServerConfig::parseJson(CcJsonNode& rJson)
{
  CCUNUSED(rJson);
}

void CcHttpServerConfig::writeJson(CcJsonNode& rNode)
{
  CCUNUSED(rNode);
}

void CcHttpServerConfig::parseBinary(const CcConfigBinary::CItem* pItem, size_t uiMaxSize)
{
  bool bAllOk = pItem->getInner(pItem, uiMaxSize);
  while (pItem->isEnd() == false && bAllOk)
  {
    switch (pItem->getType())
    {
      case CcConfigBinary::EType::WorkingDirectory:
        m_sWorkingDir = pItem->getString();
        break;
      case CcConfigBinary::EType::SslPrivateKey:
        m_sSslKey = pItem->getString();
        break;
      case CcConfigBinary::EType::SslCertificatePath:
        m_sSslCertificate = pItem->getString();
        break;
      case CcConfigBinary::EType::Timeout:
        m_oComTimeout = pItem->getDateTime();
        break;
      case CcConfigBinary::EType::SslEnable:
        m_bSslEnabled = pItem->getBool();
        break;
      case CcConfigBinary::EType::DefaultEncoding:
        m_oDefaultEncoding.parseValue(pItem->getString());
        break;
      case CcConfigBinary::EType::MaxThreads:
        m_uiMaxWorker = pItem->getUint32();
        break;
      case CcConfigBinary::EType::BufferSize:
        m_uiMaxTransferPacketSize = pItem->getUint64();
        break;
      default:
        // Ignore
        break;
    }
    if (bAllOk)
      bAllOk = pItem->getNext(pItem, uiMaxSize);
  }
}

size_t CcHttpServerConfig::writeBinary(CcConfigBinary::CItem* pItem, size_t& uiMaxSize)
{
  CcConfigBinary::CItem* pThisItem = pItem;
  size_t uiWritten = pItem->write(CcConfigBinary::EType::System);
  if(m_sWorkingDir.length() > 0 &&
     pItem->getInner(pItem, uiMaxSize))
  {
    uiWritten += pItem->write(CcConfigBinary::EType::WorkingDirectory, m_sWorkingDir, uiMaxSize);
  }
  //if(pItem->getNext(pItem, uiMaxSize))
  //{
  //  uiWritten += oWlanClient.writeBinary(pItem, uiMaxSize);
  //}
  //if(pItem->getNext(pItem, uiMaxSize))
  //{
  //  uiWritten += oWlanAccessPoint.writeBinary(pItem, uiMaxSize);
  //}
  //if(pItem->getNext(pItem, uiMaxSize))
  //{
  //  uiWritten += pItem->write(CcConfigBinary::EType::End);
  //}
  pThisItem->setSize(uiWritten);
  return uiWritten;
}
