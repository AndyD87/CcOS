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
 * @brief     Implementation of class STM32F407VNetwork
 **/
#include <STM32F407VNetwork.h>
#include "CcKernel.h"
#include "Devices/IGpioPort.h"
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_eth.h>
#include <STM32F407VDriver.h>

class STM32F407VNetworkPrivate
{
public:
  STM32F407VNetworkPrivate(STM32F407VNetwork* pParent) :
    m_pParent(pParent)
  {s_Instance = this;}
  ~STM32F407VNetworkPrivate()
    {}
  TIM_HandleTypeDef hTimer;
  ETH_HandleTypeDef oTypeDef;
  ETH_DMADescTypeDef pDMATxDscrTab[ETH_TXBUFNB];
  ETH_DMADescTypeDef pDMARxDscrTab[ETH_RXBUFNB];
  uint8 oTx_Buff[ETH_TXBUFNB][ETH_MAX_PACKET_SIZE];
  uint8 oRx_Buff[ETH_RXBUFNB][ETH_MAX_PACKET_SIZE];
  static STM32F407VNetworkPrivate* s_Instance;
  uint32 uiRegValue = 0;
  CcMacAddress oMacAddress = CcMacAddress(0x01, 0x12, 0x23, 0x34, 0x45, 0x56);
  STM32F407VNetwork* m_pParent;
};

CCEXTERNC void ETH_IRQHandler()
{
  HAL_ETH_IRQHandler(&STM32F407VNetworkPrivate::s_Instance->oTypeDef);
  STM32F407VNetworkPrivate::s_Instance->m_pParent->readFrame();
}

STM32F407VNetworkPrivate* STM32F407VNetworkPrivate::s_Instance(nullptr);

void STM32F407VNetwork_defaultInitMac(ETH_MACInitTypeDef* pMacDef)
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

STM32F407VNetwork::STM32F407VNetwork()
{
  m_pPrivate = new STM32F407VNetworkPrivate(this);
  CCMONITORNEW(m_pPrivate);

  CcHandle<IGpioPort> pPortA = CcKernel::getDevice(EDeviceType::GPIOPort, 0).cast<IGpioPort>();
  CcHandle<IGpioPort> pPortB = CcKernel::getDevice(EDeviceType::GPIOPort, 1).cast<IGpioPort>();
  CcHandle<IGpioPort> pPortC = CcKernel::getDevice(EDeviceType::GPIOPort, 2).cast<IGpioPort>();
  if( pPortA.isValid() &&
      pPortB.isValid() &&
      pPortC.isValid())
  {
    pPortA->getPin(7)->setDirection(IGpioPin::EDirection::Alternate);
    pPortA->getPin(1)->setDirection(IGpioPin::EDirection::Alternate);
    pPortA->getPin(2)->setDirection(IGpioPin::EDirection::Alternate);
    pPortB->getPin(11)->setDirection(IGpioPin::EDirection::Alternate);
    pPortB->getPin(12)->setDirection(IGpioPin::EDirection::Alternate);
    pPortB->getPin(13)->setDirection(IGpioPin::EDirection::Alternate);
    pPortC->getPin(1)->setDirection(IGpioPin::EDirection::Alternate);
    pPortC->getPin(4)->setDirection(IGpioPin::EDirection::Alternate);
    pPortC->getPin(5)->setDirection(IGpioPin::EDirection::Alternate);

    pPortA->getPin(7)->setAlternateValue(GPIO_AF11_ETH);
    pPortA->getPin(1)->setAlternateValue(GPIO_AF11_ETH);
    pPortA->getPin(2)->setAlternateValue(GPIO_AF11_ETH);
    pPortB->getPin(11)->setAlternateValue(GPIO_AF11_ETH);
    pPortB->getPin(12)->setAlternateValue(GPIO_AF11_ETH);
    pPortB->getPin(13)->setAlternateValue(GPIO_AF11_ETH);
    pPortC->getPin(1)->setAlternateValue(GPIO_AF11_ETH);
    pPortC->getPin(4)->setAlternateValue(GPIO_AF11_ETH);
    pPortC->getPin(5)->setAlternateValue(GPIO_AF11_ETH);

    pPortA->getPin(7)->reconfigure();
    pPortA->getPin(1)->reconfigure();
    pPortA->getPin(2)->reconfigure();
    pPortB->getPin(11)->reconfigure();
    pPortB->getPin(12)->reconfigure();
    pPortB->getPin(13)->reconfigure();
    pPortC->getPin(1)->reconfigure();
    pPortC->getPin(4)->reconfigure();
    pPortC->getPin(5)->reconfigure();

    __HAL_RCC_ETH_CLK_ENABLE();

    CcStatic_memsetZeroObject(m_pPrivate->oTypeDef);
    m_pPrivate->oTypeDef.Instance = ETH;
    m_pPrivate->oTypeDef.Init.MACAddr = const_cast<uint8*>(getMacAddress().getMac());
    m_pPrivate->oTypeDef.Init.AutoNegotiation = ETH_AUTONEGOTIATION_DISABLE;
    m_pPrivate->oTypeDef.Init.Speed = ETH_SPEED_100M;
    m_pPrivate->oTypeDef.Init.DuplexMode = ETH_MODE_FULLDUPLEX;
    m_pPrivate->oTypeDef.Init.MediaInterface = ETH_MEDIA_INTERFACE_RMII;
    m_pPrivate->oTypeDef.Init.RxMode = ETH_RXINTERRUPT_MODE;
    m_pPrivate->oTypeDef.Init.ChecksumMode = ETH_CHECKSUM_BY_HARDWARE;
    m_pPrivate->oTypeDef.Init.PhyAddress = DP83848_PHY_ADDRESS;
    HAL_ETH_Init(&m_pPrivate->oTypeDef);

    //! @todo Setup GPIO and Interrupt

    /* Initialize Tx Descriptors list: Chain Mode */
    HAL_ETH_DMATxDescListInit(&m_pPrivate->oTypeDef, m_pPrivate->pDMATxDscrTab, m_pPrivate->oTx_Buff[0], ETH_TXBUFNB);

    /* Initialize Rx Descriptors list: Chain Mode */
    HAL_ETH_DMARxDescListInit(&m_pPrivate->oTypeDef, m_pPrivate->pDMARxDscrTab, m_pPrivate->oRx_Buff[0], ETH_RXBUFNB);

    ETH_MACInitTypeDef oMacDef;
    STM32F407VNetwork_defaultInitMac(&oMacDef);
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

STM32F407VNetwork::~STM32F407VNetwork()
{
  CCDELETE(m_pReceiver);
  CCDELETE(m_pPrivate);
}

const CcMacAddress& STM32F407VNetwork::getMacAddress()
{
  return m_pPrivate->oMacAddress;
}

void STM32F407VNetwork::readFrame()
{
  HAL_StatusTypeDef iStatus = HAL_ETH_GetReceivedFrame_IT(&m_pPrivate->oTypeDef);
  CcNetworkPacket* pData;
  while(iStatus == HAL_StatusTypeDef::HAL_OK)
  {
    pData = new CcNetworkPacket();
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
        m_pReceiver != nullptr)
    {
      m_pReceiver->call(pData);
      pData = nullptr;
    }
    else
    {
      CCDELETE( pData );
    }
    iStatus = HAL_ETH_GetReceivedFrame_IT(&m_pPrivate->oTypeDef);
  }
}

bool STM32F407VNetwork::writeFrame(const CcNetworkPacket& oFrame)
{
  uint8_t* pBuffer = (uint8_t*)(m_pPrivate->oTypeDef.TxDesc->Buffer1Addr);
  uint32 uiFrameSize = oFrame.size();
  if( pBuffer != nullptr &&
      uiFrameSize <= ETH_TX_BUF_SIZE)
  {
    oFrame.readAll(pBuffer, oFrame.size());
    if(HAL_ETH_TransmitFrame(&m_pPrivate->oTypeDef, oFrame.size()))
    {
      m_uiSendFrames++;
      return true;
    }
  }
  return false;
}

bool STM32F407VNetwork::isConnected()
{
  bool bRet = false;
  uint32 uiRegValue;
  if(HAL_ETH_ReadPHYRegister(&m_pPrivate->oTypeDef, PHY_SR, &uiRegValue) == HAL_OK)
  {
    bRet = IS_FLAG_SET(uiRegValue, PHY_LINK_STATUS);
  }
  return bRet;
}
