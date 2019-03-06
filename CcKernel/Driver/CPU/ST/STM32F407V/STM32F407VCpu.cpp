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
 * @brief     Implementation of class STM32F407VCpu
 **/
#include <STM32F407VCpu.h>
#include "CcKernel.h"
#include <stdlib.h>
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_pwr_ex.h>
#include <STM32F407VDriver.h>

#define CAST_REGISTERS(VAR) static_cast<STM32F407VCpu::STM32F407VCpuRegisters*>(VAR)

class STM32F407VCpu::STM32F407VCpuPrivate
{
public:
};

class STM32F407VCpu::STM32F407VCpuRegisters
{
  uint32 r0;
};

STM32F407VCpu::STM32F407VCpu()
{
  m_pPrivate = new STM32F407VCpuPrivate();
  CCMONITORNEW(m_pPrivate);
}

STM32F407VCpu::~STM32F407VCpu()
{
  CCDELETE(m_pPrivate);
}

size_t STM32F407VCpu::coreNumber()
{
  return 1;
}

void* STM32F407VCpu::captureRegisters(size_t uiCoreNr)
{
  void* pReturn = nullptr;
  if(uiCoreNr == 1)
  {
    STM32F407VCpuRegisters* pTemp = new STM32F407VCpuRegisters();
    pReturn = pTemp;
  }
  return pReturn;
}

void  STM32F407VCpu::restoreRegisters(size_t uiCoreNr, void* pState)
{
  if(uiCoreNr == 1)
  {
    CCUNUSED(pState);
    delteRegisters(pState);
  }
}

void  STM32F407VCpu::delteRegisters(void* pState)
{
  delete CAST_REGISTERS(pState);
}
