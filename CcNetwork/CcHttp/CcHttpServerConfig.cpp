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
#include "CcJson/CcJsonNode.h"
#include "CcJson/CcJsonObject.h"
#include "NDocumentsGlobals.h"

const CcString CcHttpServerConfig::s_sDefaultSslKey("Key.crt");
const CcString CcHttpServerConfig::s_sDefaultSslCertificate("Certificate.crt");
const CcDateTime CcHttpServerConfig::s_oDefaultTimeout(CcDateTimeFromSeconds(5));
const bool CcHttpServerConfig::s_bDefaultSslEnabled(false);
const uint32 CcHttpServerConfig::s_uiDefaultMaxWorker = 5;
const uint32 CcHttpServerConfig::s_uiDefaultMaxTransferPacketSize = 9000;

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
  if (rJson.isObject())
  {
    for (CcJsonNode& rNode : rJson.object())
    {
      if (rNode.isObject())
      {
      }
      else if (rNode.isValue())
      {
        if (rNode.getName() == NDocumentsGlobals::NConfig::WorkingDirectory &&
            rNode.value().isString())
        {
          m_sWorkingDir = rNode.value().getString();
        }
        else if (rNode.getName() == NDocumentsGlobals::NConfig::SslPrivateKey &&
                 rNode.value().isString())
        {
          m_sSslKey = rNode.value().getString();
        }
        else if (rNode.getName() == NDocumentsGlobals::NConfig::SslCertificatePath &&
          rNode.value().isString())
        {
          m_sSslCertificate = rNode.value().getString();
        }
        else if (rNode.getName() == NDocumentsGlobals::NConfig::Timeout &&
                 rNode.value().isInt())
        {
          m_oComTimeout = CcDateTimeFromMSeconds(rNode.getValue().getInt());
        }
        else if (rNode.getName() == NDocumentsGlobals::NConfig::SslEnable &&
                 rNode.value().isBool())
        {
          m_bSslEnabled = rNode.getValue().getBool();
        }
        else if ( rNode.getName() == NDocumentsGlobals::NConfig::DefaultEncoding &&
                  rNode.value().isString())
        {
          m_oDefaultEncoding.parseValue(rNode.value().getString());
        }
        else if ( rNode.getName() == NDocumentsGlobals::NConfig::MaxThreads &&
                  rNode.value().isInt())
        {
          m_uiMaxWorker = rNode.getValue().getUint32();
        }
        else if (rNode.getName() == NDocumentsGlobals::NConfig::BufferSize &&
              rNode.value().isInt())
        {
          m_uiMaxTransferPacketSize = rNode.getValue().getUint32();
        }
        else if (rNode.getName() == NDocumentsGlobals::NConfig::Port &&
          rNode.value().isInt())
        {
          m_oAddressInfo.setPort(rNode.getValue().getUint16());
        }
      }
    }
  }
}

void CcHttpServerConfig::writeJson(CcJsonNode& rNode)
{
  if (rNode.isObject())
  {
    rNode.object().append(CcJsonNode(NDocumentsGlobals::NConfig::WorkingDirectory, CcVariant(m_sWorkingDir)));
    rNode.object().append(CcJsonNode(NDocumentsGlobals::NConfig::SslPrivateKey, CcVariant(m_sSslKey)));
    rNode.object().append(CcJsonNode(NDocumentsGlobals::NConfig::SslCertificatePath, CcVariant(m_sSslCertificate)));
    rNode.object().append(CcJsonNode(NDocumentsGlobals::NConfig::Timeout, CcVariant(m_oComTimeout.getTimestampMs())));
    rNode.object().append(CcJsonNode(NDocumentsGlobals::NConfig::SslEnable, CcVariant(m_bSslEnabled)));
    rNode.object().append(CcJsonNode(NDocumentsGlobals::NConfig::DefaultEncoding, CcVariant(m_oDefaultEncoding.getValue())));
    rNode.object().append(CcJsonNode(NDocumentsGlobals::NConfig::MaxThreads, CcVariant(m_uiMaxWorker)));
    rNode.object().append(CcJsonNode(NDocumentsGlobals::NConfig::BufferSize, CcVariant(m_uiMaxTransferPacketSize)));
    rNode.object().append(CcJsonNode(NDocumentsGlobals::NConfig::Port, CcVariant(m_oAddressInfo.getPort())));
  }
}

const CcConfigBinary::CItem* CcHttpServerConfig::parseBinary(const CcConfigBinary::CItem* pItem, size_t uiMaxSize)
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
        m_uiMaxTransferPacketSize = static_cast<uint32>(pItem->getUint32());
        break;
      case CcConfigBinary::EType::Port:
        m_oAddressInfo.setPort(pItem->getUint16());
        break;
      default:
        // Ignore
        break;
    }
    if (bAllOk)
      bAllOk = pItem->getNext(pItem, uiMaxSize);
  }
  return pItem;
}

size_t CcHttpServerConfig::writeBinary(IIo& pStream)
{
  size_t uiWritten = CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::HttpServer);
  if(m_sWorkingDir.length())
  {
    uiWritten = CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::WorkingDirectory, m_sWorkingDir);
  }
  if(uiWritten != SIZE_MAX && m_sSslKey != s_sDefaultSslKey)
  {
    uiWritten = CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::SslPrivateKeyPath, m_sSslKey);
  }
  if(uiWritten != SIZE_MAX && m_sSslCertificate != s_sDefaultSslCertificate)
  {
    uiWritten = CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::SslCertificatePath, m_sSslCertificate);
  }
  if(uiWritten != SIZE_MAX && m_oComTimeout != s_oDefaultTimeout)
  {
    uiWritten = CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::Timeout, m_oComTimeout);
  }
  if(uiWritten != SIZE_MAX && m_bSslEnabled != s_bDefaultSslEnabled)
  {
    uiWritten = CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::SslEnable, m_bSslEnabled);
  }
  if(uiWritten != SIZE_MAX && m_oDefaultEncoding.getFlags() != CcHttpTransferEncoding::Normal)
  {
    uiWritten = CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::DefaultEncoding, m_oDefaultEncoding.getValue());
  }
  if(uiWritten != SIZE_MAX && m_uiMaxWorker != s_uiDefaultMaxWorker)
  {
    uiWritten = CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::MaxThreads, static_cast<uint32>(m_uiMaxWorker));
  }
  if(uiWritten != SIZE_MAX && m_uiMaxTransferPacketSize != s_uiDefaultMaxTransferPacketSize)
  {
    uiWritten = CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::BufferSize, static_cast<uint64>(m_uiMaxTransferPacketSize));
  }
  if (uiWritten != SIZE_MAX)
  {
    uiWritten = CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::Port, m_oAddressInfo.getPort());
  }
  if(uiWritten != SIZE_MAX)
  {
    CcConfigBinary::CItem::write(pStream, CcConfigBinary::EType::End);
  }
  return uiWritten;
}
