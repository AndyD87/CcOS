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
 * @brief     Implementation of class STM32F103I2C
 **/
#include <STM32F103I2C.h>
#include "CcKernel.h"
#include <STM32F103Driver.h>

class STM32F103I2CClient : public II2CClient
{
public:
  /**
   * @brief Constructor
   */
  STM32F103I2CClient(I2C_HandleTypeDef* hI2C1, uint16 uiAddress) :
    II2CClient(uiAddress<<1),
    m_hI2C1(hI2C1)
  {}

  /**
   * @brief Destructor
   */
  virtual ~STM32F103I2CClient()
  {}

  virtual size_t read(void* pBuffer, size_t uiSize) override
  {
    size_t uiRet = SIZE_MAX;
    if(HAL_OK == HAL_I2C_Master_Receive(m_hI2C1, m_uiAddress, static_cast<uint8*>(pBuffer), uiSize, 100))
    {
      uiRet = uiSize;
    }
    return uiRet;
  }

  virtual size_t write(const void* pBuffer, size_t uiSize) override 
  {
    size_t uiRet = SIZE_MAX;
    if(HAL_OK == HAL_I2C_Master_Transmit(m_hI2C1, m_uiAddress, const_cast<uint8*>(static_cast<const uint8*>(pBuffer)), uiSize, 100))
    {
      uiRet = uiSize;
    }
    return uiRet;
  }

  virtual size_t readRegister8(uint8 uiRegister, void* pBuffer, size_t uiSize) override
  {
    size_t uiRet = SIZE_MAX;
    if(HAL_OK == HAL_I2C_Mem_Read(m_hI2C1, m_uiAddress, uiRegister, I2C_MEMADD_SIZE_8BIT, const_cast<uint8*>(static_cast<const uint8*>(pBuffer)), uiSize, 100))
    {
      uiRet = uiSize;
    }
    return uiRet;
  }

  virtual size_t writeRegister8(uint8 uiRegister, void* pBuffer, size_t uiSize) override
  {
    size_t uiRet = SIZE_MAX;
    if(HAL_OK == HAL_I2C_Mem_Write(m_hI2C1, m_uiAddress, uiRegister, I2C_MEMADD_SIZE_8BIT, const_cast<uint8*>(static_cast<const uint8*>(pBuffer)), uiSize, 100))
    {
      uiRet = uiSize;
    }
    return uiRet;
  }
private:
  I2C_HandleTypeDef* m_hI2C1;
};

/**
* @brief I2C MSP Initialization
* This function configures the hardware resources used in this example
* @param hi2c: I2C handle pointer
* @retval None
*/
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
  //GPIO_InitTypeDef GPIO_InitStruct1 = {};
  GPIO_InitTypeDef GPIO_InitStruct2 = {};
  if(hi2c->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  __HAL_RCC_AFIO_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  /* USER CODE END I2C1_MspInit 0 */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
    //GPIO_InitStruct1.Pin = GPIO_PIN_6|GPIO_PIN_7;
    //GPIO_InitStruct1.Mode = GPIO_MODE_INPUT;
    //GPIO_InitStruct1.Speed = GPIO_SPEED_LOW;
    //HAL_GPIO_Init(GPIOB, &GPIO_InitStruct1);

    GPIO_InitStruct2.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct2.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct2.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct2);

    __HAL_AFIO_REMAP_I2C1_ENABLE();

    /* Peripheral clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
    /* USER CODE BEGIN I2C1_MspInit 1 */
    // work around which solves I2C initial busy problem.
    // I2C gets busy during setup, just after I2C clock enable.
    // force/release reset at this point helps to reset the busy
    // flag of the I2C and lets the I2C interface work as expected.   
    __HAL_RCC_I2C1_FORCE_RESET();
    __HAL_RCC_I2C1_RELEASE_RESET();
  
    /* USER CODE END I2C1_MspInit 1 */
  }

}


STM32F103I2C::STM32F103I2C()
{
  
}

STM32F103I2C::~STM32F103I2C()
{
}

CcStatus STM32F103I2C::onState(EState eState)
{
  CcStatus bSuccess = true;
  switch(eState)
  {
    case EState::Start:
      CcStatic_memsetZeroObject(m_hI2C1);
      m_hI2C1.Instance = I2C1;
      m_hI2C1.Init.ClockSpeed = 100000;
      m_hI2C1.Init.DutyCycle = I2C_DUTYCYCLE_2;
      m_hI2C1.Init.OwnAddress1 = 0;
      m_hI2C1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
      m_hI2C1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
      m_hI2C1.Init.OwnAddress2 = 0;
      m_hI2C1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
      m_hI2C1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
      if (HAL_I2C_Init(&m_hI2C1) != HAL_OK)
      {
        CCERROR("Failed to init I2C1");
      }
      else
      {
        bSuccess = true;
        /* GPIO Ports Clock Enable */
        __HAL_RCC_GPIOB_CLK_ENABLE();
      }
      break;
    case EState::Stop:
      if (HAL_I2C_DeInit(&m_hI2C1) == HAL_OK)
      {
        bSuccess = true;
      }
      break;
    default:
      break;
  }
  return bSuccess;
}

II2CClient* STM32F103I2C::createInterface(uint16 uiAddress)
{
  return CCNEW_INLINE(STM32F103I2CClient, &m_hI2C1, uiAddress);
}

void STM32F103I2C::removeInterface(II2CClient* pInterface)
{
  CCDELETE(pInterface);
}
