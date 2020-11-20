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
 * @brief     Implementation of class STM32F407Network
 **/
#include <STM32F407Network.h>
#include "CcKernel.h"
#include "Devices/IGpioPort.h"
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_eth.h>
#include <STM32F407Driver.h>

class STM32F407NetworkPrivate
{
public:
  STM32F407NetworkPrivate(STM32F407Network* pParent) :
    m_pParent(pParent)
  {s_Instance = this;}
  ~STM32F407NetworkPrivate()
    {}
  TIM_HandleTypeDef hTimer;
  ETH_HandleTypeDef oTypeDef;
  ETH_DMADescTypeDef pDMATxDscrTab[ETH_TXBUFNB];
  ETH_DMADescTypeDef pDMARxDscrTab[ETH_RXBUFNB];
  uint8 oTx_Buff[ETH_TXBUFNB][ETH_MAX_PACKET_SIZE];
  uint8 oRx_Buff[ETH_RXBUFNB][ETH_MAX_PACKET_SIZE];
  static STM32F407NetworkPrivate* s_Instance;
  uint32 uiRegValue = 0;
  CcMacAddress oMacAddress = CcMacAddress(0x00, 0x80, 0xff, 0x34, 0x45, 0x56);
  STM32F407Network* m_pParent;
};

CCEXTERNC void ETH_IRQHandler()
{
  HAL_ETH_IRQHandler(&STM32F407NetworkPrivate::s_Instance->oTypeDef);
  STM32F407NetworkPrivate::s_Instance->m_pParent->readFrame();
}

STM32F407NetworkPrivate* STM32F407NetworkPrivate::s_Instance(nullptr);

void STM32F407Network_defaultInitMac(ETH_MACInitTypeDef* pMacDef)
{
    pMacDef->Watchdog = ETH_WATCHDOG_ENABLE;
    pMacDef->Jabber = ETH_JABBER_ENABLE;
    pMacDef->InterFrameGap = ETH_INTERFRAMEGAP_96BIT;
    pMacDef->CarrierSense = ETH_CARRIERSENCE_ENABLE;
    pMacDef->ReceiveOwn = ETH_RECEIVEOWN_DISABLE;
    pMacDef->LoopbackMode = ETH_LOOPBACKMODE_DISABLE;
    pMacDef->ChecksumOffload = ETH_CHECKSUMOFFLAOD_DISABLE;
    pMacDef->RetryTransmission = ETH_RETRYTRANSMISSION_DISABLE;
    pMacDef->AutomaticPadCRCStrip = ETH_AUTOMATICPADCRCSTRIP_DISABLE;
    pMacDef->BackOffLimit = ETH_BACKOFFLIMIT_10;
    pMacDef->DeferralCheck = ETH_DEFFERRALCHECK_DISABLE;
    pMacDef->ReceiveAll = ETH_RECEIVEALL_ENABLE;
    pMacDef->SourceAddrFilter = ETH_SOURCEADDRFILTER_DISABLE;
    pMacDef->PassControlFrames = ETH_PASSCONTROLFRAMES_FORWARDALL;
    pMacDef->BroadcastFramesReception = ETH_BROADCASTFRAMESRECEPTION_ENABLE;
    pMacDef->DestinationAddrFilter = ETH_DESTINATIONADDRFILTER_NORMAL;
    pMacDef->PromiscuousMode = ETH_PROMISCIOUSMODE_DISABLE;
    pMacDef->MulticastFramesFilter = ETH_MULTICASTFRAMESFILTER_PERFECT;
    pMacDef->UnicastFramesFilter = ETH_UNICASTFRAMESFILTER_PERFECT;
    pMacDef->HashTableHigh = 0x0;
    pMacDef->HashTableLow = 0x0;
    pMacDef->PauseTime = 0x0;
    pMacDef->ZeroQuantaPause = ETH_ZEROQUANTAPAUSE_DISABLE;
    pMacDef->PauseLowThreshold = ETH_PAUSELOWTHRESHOLD_MINUS4;
    pMacDef->UnicastPauseFrameDetect = ETH_UNICASTPAUSEFRAMEDETECT_DISABLE;
    pMacDef->ReceiveFlowControl = ETH_RECEIVEFLOWCONTROL_DISABLE;
    pMacDef->TransmitFlowControl = ETH_TRANSMITFLOWCONTROL_DISABLE;
    pMacDef->VLANTagComparison = ETH_VLANTAGCOMPARISON_16BIT;
    pMacDef->VLANTagIdentifier = 0x0;
}

STM32F407Network::STM32F407Network()
{
  CCNEW(m_pPrivate, STM32F407NetworkPrivate, this);

  CcHandle<IGpioPort> pPortA = CcKernel::getDevice(EDeviceType::GpioPort, 0).getDevice<IGpioPort>();
  CcHandle<IGpioPort> pPortB = CcKernel::getDevice(EDeviceType::GpioPort, 1).getDevice<IGpioPort>();
  CcHandle<IGpioPort> pPortC = CcKernel::getDevice(EDeviceType::GpioPort, 2).getDevice<IGpioPort>();
  if( pPortA.isValid() &&
      pPortB.isValid() &&
      pPortC.isValid())
  {
    // Enable alternate functions for ethernet RMII
    pPortA->setPinsDirection((1<<1)|(1<<2)|(1<<7), IGpioPin::EDirection::Alternate, GPIO_AF11_ETH);
    pPortB->setPinsDirection((1<<11)|(1<<12)|(1<<13), IGpioPin::EDirection::Alternate, GPIO_AF11_ETH);
    pPortC->setPinsDirection((1<<1)|(1<<4)|(1<<5), IGpioPin::EDirection::Alternate, GPIO_AF11_ETH);

    __HAL_RCC_ETH_CLK_ENABLE();

    CcStatic_memsetZeroObject(m_pPrivate->oTypeDef);
    m_pPrivate->oTypeDef.Instance = ETH;
    m_pPrivate->oTypeDef.Init.MACAddr = const_cast<uint8*>(getMacAddress().getMac());
    m_pPrivate->oTypeDef.Init.AutoNegotiation = ETH_AUTONEGOTIATION_ENABLE;
    m_pPrivate->oTypeDef.Init.Speed = ETH_SPEED_100M;
    m_pPrivate->oTypeDef.Init.DuplexMode = ETH_MODE_FULLDUPLEX;
    m_pPrivate->oTypeDef.Init.MediaInterface = ETH_MEDIA_INTERFACE_RMII;
    m_pPrivate->oTypeDef.Init.RxMode = ETH_RXINTERRUPT_MODE;
    m_pPrivate->oTypeDef.Init.ChecksumMode = ETH_CHECKSUM_BY_HARDWARE;
    m_pPrivate->oTypeDef.Init.PhyAddress = DP83848_PHY_ADDRESS;
    HAL_ETH_Init(&m_pPrivate->oTypeDef);

    /* Initialize Tx Descriptors list: Chain Mode */
    HAL_ETH_DMATxDescListInit(&m_pPrivate->oTypeDef, m_pPrivate->pDMATxDscrTab, m_pPrivate->oTx_Buff[0], ETH_TXBUFNB);

    /* Initialize Rx Descriptors list: Chain Mode */
    HAL_ETH_DMARxDescListInit(&m_pPrivate->oTypeDef, m_pPrivate->pDMARxDscrTab, m_pPrivate->oRx_Buff[0], ETH_RXBUFNB);

    ETH_MACInitTypeDef oMacDef;
    STM32F407Network_defaultInitMac(&oMacDef);
    if(HAL_ETH_ConfigMAC(&m_pPrivate->oTypeDef, &oMacDef) == HAL_OK)
    {
      /* Enable MAC and DMA transmission and reception */
      if( HAL_ETH_Start(&m_pPrivate->oTypeDef) == HAL_OK)
      {
        uint32_t uiRegValue = 0;
        /**** Configure PHY to generate an interrupt when Eth Link state changes ****/
        /* Read Register Configuration */
        if(HAL_ETH_ReadPHYRegister(&m_pPrivate->oTypeDef, PHY_SR, &uiRegValue) == HAL_OK)
        {
          m_pPrivate->uiRegValue = uiRegValue;
        }
        /**** Configure PHY to generate an interrupt when Eth Link state changes ****/
        /* Read Register Configuration */
        if(HAL_ETH_ReadPHYRegister(&m_pPrivate->oTypeDef, PHY_MICR, &uiRegValue) == HAL_OK)
        {
          uiRegValue |= (PHY_MICR_INT_EN | PHY_MICR_INT_OE);

          /* Enable Interrupts */
          if(HAL_OK == HAL_ETH_WritePHYRegister(&m_pPrivate->oTypeDef, PHY_MICR, uiRegValue ))
          {
            uiRegValue = 0;
            if(HAL_OK == HAL_ETH_ReadPHYRegister(&m_pPrivate->oTypeDef, PHY_MICR, &uiRegValue ))
            {
              if(IS_FLAG_SET(uiRegValue,(PHY_MICR_INT_EN | PHY_MICR_INT_OE)))
              {
                /* Read Register Configuration */
                if(HAL_OK == HAL_ETH_ReadPHYRegister(&m_pPrivate->oTypeDef, PHY_MISR, &uiRegValue))
                {
                  uiRegValue |= PHY_MISR_LINK_INT_EN;
                  /* Enable Interrupt on change of link status */
                  if(HAL_OK == HAL_ETH_WritePHYRegister(&m_pPrivate->oTypeDef, PHY_MISR, uiRegValue))
                  {
                    /* enable interrupts */
                    HAL_NVIC_SetPriority(ETH_IRQn, 5, 0);
                    HAL_NVIC_EnableIRQ(ETH_IRQn);
                    m_oState = true;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

STM32F407Network::~STM32F407Network()
{
  CCDELETE(m_pPrivate);
}

const CcMacAddress& STM32F407Network::getMacAddress()
{
  return m_pPrivate->oMacAddress;
}

bool STM32F407Network::isConnected()
{
  bool bRet = false;
  uint32 uiRegValue;
  if(HAL_ETH_ReadPHYRegister(&m_pPrivate->oTypeDef, PHY_SR, &uiRegValue) == HAL_OK)
  {
    bRet = IS_FLAG_SET(uiRegValue, PHY_LINK_STATUS);
  }
  return bRet;
}

uint32 STM32F407Network::getChecksumCapabilities()
{
  if(m_pPrivate->oTypeDef.Init.ChecksumMode == ETH_CHECKSUM_BY_HARDWARE)
  {
    return INetwork::CChecksumCapabilities::uUDP |
        INetwork::CChecksumCapabilities::uTCP |
#ifdef ETH
  #undef ETH
#endif
        INetwork::CChecksumCapabilities::uETH |
        INetwork::CChecksumCapabilities::uICMP;
  }
  else
  {
    return 0;
  }
}

void STM32F407Network::readFrame()
{
  HAL_StatusTypeDef iStatus = HAL_ETH_GetReceivedFrame_IT(&m_pPrivate->oTypeDef);
  CcNetworkPacket* pData;
  while(iStatus == HAL_StatusTypeDef::HAL_OK)
  {
    CCNEW(pData, CcNetworkPacket);
    if(pData != nullptr)
    {
      pData->pInterface = this;
      m_uiReceivedFrames++;
      /* Obtain the size of the packet and put it into the "len" variable. */
      uint32 len = m_pPrivate->oTypeDef.RxFrameInfos.length;
      char* buffer = (char *)m_pPrivate->oTypeDef.RxFrameInfos.buffer;
      CcByteArray oByteArray(buffer, len);
      pData->append(oByteArray);

      ETH_DMADescTypeDef* dmarxdesc = m_pPrivate->oTypeDef.RxFrameInfos.FSRxDesc;

      /* Set Own bit in Rx descriptors: gives the buffers back to DMA */
      for (size_t i=0; i< (m_pPrivate->oTypeDef.RxFrameInfos).SegCount; i++)
      {
        dmarxdesc->Status = ETH_DMARXDESC_OWN;
        dmarxdesc = (ETH_DMADescTypeDef *)(dmarxdesc->Buffer2NextDescAddr);
      }

      /* Clear Segment_Count */
      (m_pPrivate->oTypeDef.RxFrameInfos).SegCount = 0;

      /* When Rx Buffer unavailable flag is set: clear it and resume reception */
      if (((m_pPrivate->oTypeDef.Instance)->DMASR & ETH_DMASR_RBUS) != (uint32_t)RESET)
      {
        /* Clear RBUS ETHERNET DMA flag */
        (m_pPrivate->oTypeDef.Instance)->DMASR = ETH_DMASR_RBUS;
        /* Resume DMA reception */
        (m_pPrivate->oTypeDef.Instance)->DMARPDR = 0;
      }
      if( pData->size() &&
          m_oReceiver.isValid())
      {
        CPacket oPacket;
        oPacket.pPacket = pData;
        pData = nullptr;
        m_oReceiver.call(&oPacket);
        CCDELETE(oPacket.pPacket);
      }
      if(pData != nullptr)
      {
        CCDELETE( pData );
      }
      iStatus = HAL_ETH_GetReceivedFrame_IT(&m_pPrivate->oTypeDef);
    }
  }
}

bool STM32F407Network::writeFrame(CcNetworkPacketRef oFrame)
{
  uint8_t* pBuffer = (uint8_t*)(m_pPrivate->oTypeDef.TxDesc->Buffer1Addr);
  uint32 uiFrameSize = oFrame->size();
  if( pBuffer != nullptr &&
      uiFrameSize <= ETH_TX_BUF_SIZE)
  {
    oFrame->readAll(pBuffer, uiFrameSize);
    if(HAL_ETH_TransmitFrame(&m_pPrivate->oTypeDef, uiFrameSize) == HAL_OK)
    {
      m_uiSendFrames++;
      CCDELETE(oFrame);
      oFrame = nullptr;
      return true;
    }
  }
  return false;
}
