/**
 * @author     Andreas Dirmeier
 * @copyright  Andreas Dirmeier (c) 2015
 * @par       Language: C++11
 */
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
#include <stm32f4xx_hal.h>
#include <STM32F407VDriver.h>

class STM32F407VNetworkPrivate
{
public:
  STM32F407VNetworkPrivate(STM32F407VNetwork* pParent)
    { m_pParent=pParent; s_Instance = this;}
  ~STM32F407VNetworkPrivate()
    { m_pParent=nullptr; }
  TIM_HandleTypeDef hTimer;
  static void tick()
  {
    //s_Instance->m_pParent->timeout();
  }
  ETH_HandleTypeDef oTypeDef;
  ETH_DMADescTypeDef oDMATxDscrTab;
  ETH_DMADescTypeDef oDMARxDscrTab;
  uint8 oTx_Buff[ETH_TXBUFNB][ETH_MAX_PACKET_SIZE];
  uint8 oRx_Buff[ETH_RXBUFNB][ETH_MAX_PACKET_SIZE];
  static STM32F407VNetworkPrivate* s_Instance;
private:
  STM32F407VNetwork* m_pParent;
};

STM32F407VNetworkPrivate* STM32F407VNetworkPrivate::s_Instance(nullptr);

STM32F407VNetwork::STM32F407VNetwork()
{
  m_pPrivate = new STM32F407VNetworkPrivate(this);
  CCMONITORNEW(m_pPrivate);

  uint8 macaddress[6]= {0x01, 0x23, 0x45, 0x67, 0x89, 0xab};

  m_pPrivate->oTypeDef.Instance = ETH;
  m_pPrivate->oTypeDef.Init.MACAddr = macaddress;
  m_pPrivate->oTypeDef.Init.AutoNegotiation = ETH_AUTONEGOTIATION_ENABLE;
  m_pPrivate->oTypeDef.Init.Speed = ETH_SPEED_100M;
  m_pPrivate->oTypeDef.Init.DuplexMode = ETH_MODE_FULLDUPLEX;
  m_pPrivate->oTypeDef.Init.MediaInterface = ETH_MEDIA_INTERFACE_MII;
  m_pPrivate->oTypeDef.Init.RxMode = ETH_RXINTERRUPT_MODE;
  m_pPrivate->oTypeDef.Init.ChecksumMode = ETH_CHECKSUM_BY_HARDWARE;
  m_pPrivate->oTypeDef.Init.PhyAddress = DP83848_PHY_ADDRESS;
  HAL_ETH_Init(&m_pPrivate->oTypeDef);

  //! @todo Setup GPIO and Interrupt

  /* Initialize Tx Descriptors list: Chain Mode */
  HAL_ETH_DMATxDescListInit(&m_pPrivate->oTypeDef, &m_pPrivate->oDMATxDscrTab, m_pPrivate->oTx_Buff[0], ETH_TXBUFNB);

  /* Initialize Rx Descriptors list: Chain Mode */
  HAL_ETH_DMARxDescListInit(&m_pPrivate->oTypeDef, &m_pPrivate->oDMARxDscrTab, m_pPrivate->oRx_Buff[0], ETH_RXBUFNB);

  __HAL_RCC_ETHMAC_CLK_ENABLE();
  __HAL_RCC_ETHMACTX_CLK_ENABLE();
  __HAL_RCC_ETHMACRX_CLK_ENABLE();

  /* Enable MAC and DMA transmission and reception */
  HAL_ETH_Start(&m_pPrivate->oTypeDef);


}

STM32F407VNetwork::~STM32F407VNetwork()
{
  CCDELETE(m_pPrivate);
}
