/*
 * This file is part of CcSshWorker.
 *
 * CcSshWorker is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcSshWorker is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcSshWorker.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Defining types for ssh protocol
 **/
#pragma once

#include "CcBase.h"
#include "CcString.h"
#include "CcByteArray.h"
#include "CcStatic.h"

namespace CcSsh
{
#pragma pack(push, 1)
  class buffer
  {
  public:
    buffer* create(uint32 uiLength)
    {
      CCNEWARRAYTYPE(pData, char, uiLength + sizeof(m_uiSize));
      buffer* pString = CCVOIDPTRCAST(buffer*, pData);
      pString->m_uiSize = uiLength;
      return pString;
    }

    void remove(buffer* pString)
    {
      char* pData = CCVOIDPTRCAST(char*, pString);
      CCDELETEARRAY(pData);
    }

    void swap()
    {
      m_uiSize = CcStatic::swapUint32(m_uiSize);
    }

  protected:
    buffer() = default;
    ~buffer() = default;
    uint32 m_uiSize = 0;
    byte   m_pData[1];
  };


  class packet : public buffer
  {
  public:
    byte* getPayload()
    {
      //                                  Size fiel        + padding field                                            
      return CCVOIDPTRCAST(byte*, this) + sizeof(m_uiSize) + sizeof(byte);
    }

    uint32 getPayloadSize()
    { 
      //    packetsize - (sizeof(padding) + padding)
      return m_uiSize  - (1               + getPaddingSize());
    }

    byte* getPadding()
    {
      //                                  Size fiel        + padding field                                            
      return getPayload() + getPayloadSize();
    }

    uint32 getPaddingSize()
    {
      //                  packetsize - (sizeof(padding) + padding)
      if (m_uiSize) return m_pData[0];
      else return 0;
    }

    byte* getMac()
    {
      return getPayload() + getPayloadSize() + getPaddingSize();
    }

    uint32 getMacSize()
    {
      return m_uiSize - (getPaddingSize() + getPayloadSize() + 1);
    }
  };

  class string :public  buffer
  {
  public:
    uint32 getLength()
    {
      return m_uiSize;
    }

    uint32 getSize()
    {
      return m_uiSize + sizeof(m_uiSize);
    }
  };

  class mpint : public buffer
  {
  public:
  };

  class namelist : public buffer
  {
  public:
    uint32 getLength()
    {
      return m_uiSize;
    }

    uint32 getSize()
    {
      return m_uiSize + sizeof(m_uiSize);
    }
  };

  #define SetNameList(name)                                                   \
        if (pBase->getPayloadSize() >= uiOffsetUsed + 4)                      \
        {                                                                     \
          name = CCVOIDPTRCAST(namelist*, pPayload + uiOffsetUsed); \
          name->swap();                                             \
          uiOffsetUsed += name->getSize();                                    \
        }

  class key_exchange
  {
  public:
    key_exchange(packet* pBase)
    {
      byte* pPayload = pBase->getPayload();
      uint32_t uiOffsetUsed = 1;
      if (pBase->getPayloadSize() >= uiOffsetUsed + 16)
      {
        CcStatic::memcpy(Cookie, pPayload + uiOffsetUsed, 16);
        uiOffsetUsed += 16;
      }
      SetNameList(kex_algorithms);
      SetNameList(server_host_key_algorithms);
      SetNameList(encryption_algorithms_client_to_server);
      SetNameList(encryption_algorithms_server_to_client);
      SetNameList(mac_algorithms_client_to_server);
      SetNameList(mac_algorithms_server_to_client);
      SetNameList(compression_algorithms_client_to_server);
      SetNameList(compression_algorithms_server_to_client);
      SetNameList(languages_client_to_server);
      SetNameList(languages_server_to_client);
      if (pBase->getPayloadSize() >= uiOffsetUsed + 1)
      {
        first_kex_packet_follows = pPayload[uiOffsetUsed + 1] != 0;
        uiOffsetUsed += 1;
      }
    }

    static CcByteArray generateSupported();

    byte Cookie[16];
    namelist*    kex_algorithms;
    namelist*    server_host_key_algorithms;
    namelist*    encryption_algorithms_client_to_server;
    namelist*    encryption_algorithms_server_to_client;
    namelist*    mac_algorithms_client_to_server;
    namelist*    mac_algorithms_server_to_client;
    namelist*    compression_algorithms_client_to_server;
    namelist*    compression_algorithms_server_to_client;
    namelist*    languages_client_to_server;
    namelist*    languages_server_to_client;
    bool         first_kex_packet_follows;
    uint32      reserved;
  };
  enum class EMessage
  {
    SSH_MSG_DISCONNECT                =   1,
    SSH_MSG_IGNORE                    =   2,
    SSH_MSG_UNIMPLEMENTED             =   3,
    SSH_MSG_DEBUG                     =   4,
    SSH_MSG_SERVICE_REQUEST           =   5,
    SSH_MSG_SERVICE_ACCEPT            =   6,
    SSH_MSG_KEXINIT                   =  20,
    SSH_MSG_NEWKEYS                   =  21,
    SSH_MSG_USERAUTH_REQUEST          =  50,
    SSH_MSG_USERAUTH_FAILURE          =  51,
    SSH_MSG_USERAUTH_SUCCESS          =  52,
    SSH_MSG_USERAUTH_BANNER           =  53,
    SSH_MSG_USERAUTH_INFO_REQUEST     =  60,
    SSH_MSG_USERAUTH_INFO_RESPONSE    =  61,
    SSH_MSG_GLOBAL_REQUEST            =  80,
    SSH_MSG_REQUEST_SUCCESS           =  81,
    SSH_MSG_REQUEST_FAILURE           =  82,
    SSH_MSG_CHANNEL_OPEN              =  90,
    SSH_MSG_CHANNEL_OPEN_CONFIRMATION =  91,
    SSH_MSG_CHANNEL_OPEN_FAILURE      =  92,
    SSH_MSG_CHANNEL_WINDOW_ADJUST     =  93,
    SSH_MSG_CHANNEL_DATA              =  94,
    SSH_MSG_CHANNEL_EXTENDED_DATA     =  95,
    SSH_MSG_CHANNEL_EOF               =  96,
    SSH_MSG_CHANNEL_CLOSE             =  97,
    SSH_MSG_CHANNEL_REQUEST           =  98,
    SSH_MSG_CHANNEL_SUCCESS           =  99,
    SSH_MSG_CHANNEL_FAILURE           = 100,
  };

  enum class EDisconnect
  {
    SSH_DISCONNECT_HOST_NOT_ALLOWED_TO_CONNECT    =  1,
    SSH_DISCONNECT_PROTOCOL_ERROR                 =  2,
    SSH_DISCONNECT_KEY_EXCHANGE_FAILED            =  3,
    SSH_DISCONNECT_RESERVED                       =  4,
    SSH_DISCONNECT_MAC_ERROR                      =  5,
    SSH_DISCONNECT_COMPRESSION_ERROR              =  6,
    SSH_DISCONNECT_SERVICE_NOT_AVAILABLE          =  7,
    SSH_DISCONNECT_PROTOCOL_VERSION_NOT_SUPPORTED =  8,
    SSH_DISCONNECT_HOST_KEY_NOT_VERIFIABLE        =  9,
    SSH_DISCONNECT_CONNECTION_LOST                = 10,
    SSH_DISCONNECT_BY_APPLICATION                 = 11,
    SSH_DISCONNECT_TOO_MANY_CONNECTIONS           = 12,
    SSH_DISCONNECT_AUTH_CANCELLED_BY_USER         = 13,
    SSH_DISCONNECT_NO_MORE_AUTH_METHODS_AVAILABLE = 14,
    SSH_DISCONNECT_ILLEGAL_USER_NAME              = 15,
  };

  static uint8 IS_OPTIONAL     = 0;
  static uint8 IS_RECOMMENDED  = 1;
  static uint8 IS_REQUIRED     = 2;

  typedef struct
  {
    const CcString Name;
    uint8          Requirement;
    bool           Supported;
  } SNameRequirement;

  extern SNameRequirement MacRequirements[];
  extern size_t MacRequirementsSize;

  extern SNameRequirement EncryptionRequirements[];
  extern size_t EncryptionRequirementsSize;

  extern SNameRequirement CompressionRequirements[];
  extern size_t CompressionRequirementsSize;

  extern SNameRequirement PublicKeyRequirements[];
  extern size_t PublicKeyRequirementsSize;

  extern SNameRequirement KeyExchangeRequirements[];
  extern size_t KeyExchangeRequirementsSize;

#pragma pack(pop)

}
