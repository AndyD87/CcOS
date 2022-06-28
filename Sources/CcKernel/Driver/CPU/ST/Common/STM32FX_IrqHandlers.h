#pragma once

#ifdef STM32F407xx

  #define TIM2_IRQHandler_Handled
  #define ETH_IRQHandler_Handled
  #define USART3_IRQHandler_Handled
  #define OTG_HS_IRQHandler_Handled
  #define OTG_FS_IRQHandler_Handled

#elif defined(STM32F103xx)

  #define USART2_IRQHandler_Handled

#elif defined(STM32F207xx)

  #define USART3_IRQHandler_Handled
  #define OTG_HS_IRQHandler_Handled
  #define OTG_FS_IRQHandler_Handled

#elif defined(STM32F303xC)

  #define USART3_IRQHandler_Handled

#endif
