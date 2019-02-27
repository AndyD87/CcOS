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
 * @brief     Implementation of class CcTargetSystem
 **/
#include <STM32F407VSystemTimer.h>
#include "CcTargetSystem.h"
#include "CcKernel.h"
#include "CcGPIOTarget.h"
#include "CcDisplayTarget.h"
#include "CcTouchTarget.h"

CcTargetSystem::CcTargetSystem() {
  m_Display = 0;
  m_Timer = 0;
  m_Touch = 0;
  m_GPIO = 0;
  HAL_Init();
}

CcTargetSystem::~CcTargetSystem() {

}

void CcTargetSystem::init(void){
  initSystem();
  initTimer();
  initGPIO();
}

bool CcTargetSystem::start( void ){
  //emit ready state to Kernel
  Kernel.systemReady();
  //start the main loop
  while(1){
  }
}
bool CcTargetSystem::initGUI(void){
  initDisplay();
  initTouch();
  return true;
}

bool CcTargetSystem::initCLI(void){
  return false;
}

time_t CcTargetSystem::getTime( void ){
  return m_Timer->getTime();
}

bool CcTargetSystem::createThread(CcThread* object){
  object->start();
  object->run();
  object->stop();
  return true;
}

void CcTargetSystem::initSystem(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

  /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
  if (HAL_GetREVID() == 0x1001)
  {
    /* Enable the Flash prefetch */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
  }
}

void CcTargetSystem::initTimer( void ){
  STM32F407VSystemTimer *temp = new STM32F407VSystemTimer();
  m_Timer = temp;
}

void CcTargetSystem::initGPIO( void ){

}

void CcTargetSystem::initDisplay( void )
{
  // Initialize Display on STM32F4
  m_Display = new CcDisplayTarget();
  m_Display->open(OpenFlags::Open_RW);
  // Register Device to Kernel.
  Kernel.setDevice(m_Display, eDisplay);
}

void CcTargetSystem::initTouch( void )
{
  m_Touch = new CcTouchTarget();
  Kernel.setDevice(m_Touch, eTouchPanel);
  m_Touch->open(OpenFlags::Open_RW);
}
