/**-------------------------------------------------------------------
 \date  05.03.2026
 *
 *   N32G455CCL7
 *   ------------
 *  |            |
 *  |            |
 *  |            |
 *  |       PC.13| ---->  LED
 *  |            |
 *  |            |
 *  |            |
 *  |      +3.3V |
 *  |        GND |
 *
 *\ authors        ScuratovaAnna
 *\ сode debugging ScuratovaAnna + PivnevNikolay
 */
 /********************************************************************/
#include "n32g45x.h"
#include <stdio.h>
#include <stdint.h>

__STATIC_INLINE void DWT_Init(void);
__STATIC_INLINE void DWT_Delay_ms(volatile uint32_t au32_milliseconds);
__STATIC_INLINE void DWT_Delay_us(volatile uint32_t au32_microseconds);

void delay_simple(volatile uint32_t count) {
  for (; count != 0; count--);
}

int main(void)
{
    
  RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOC, ENABLE);
  GPIO_InitType GPIO_InitStructure;

  GPIO_InitStructure.Pin = GPIO_PIN_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitPeripheral(GPIOC, &GPIO_InitStructure);

	GPIO_WriteBit(GPIOC,GPIO_PIN_13, Bit_SET);	
	delay_simple(10000000);//HSE+PLL
	//delay_simple(1000000);//HSE 8000000
	
	DWT_Init();

  while (1)
   {
	GPIO_WriteBit(GPIOC,GPIO_PIN_13, Bit_SET);
    //delay_simple(10000000);
	DWT_Delay_ms(100);
	GPIO_WriteBit(GPIOC,GPIO_PIN_13, Bit_RESET);
    //delay_simple(10000000);
	DWT_Delay_ms(100);
	}
}

/********************************************************************/
__STATIC_INLINE void DWT_Init(void) {
  CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk; 
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;  
  DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk;            
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;             
  __ASM volatile("NOP");
  __ASM volatile("NOP");
  __ASM volatile("NOP");
}
/********************************************************************/
__STATIC_INLINE void DWT_Delay_ms(volatile uint32_t au32_milliseconds) {
  uint32_t au32_initial_ticks = DWT->CYCCNT;
  uint32_t au32_ticks = (SystemCoreClock / 1000);
  au32_milliseconds *= au32_ticks;
  while ((DWT->CYCCNT - au32_initial_ticks) < au32_milliseconds);
}
/********************************************************************/
__STATIC_INLINE void DWT_Delay_us(volatile uint32_t au32_microseconds) {
  uint32_t au32_initial_ticks = DWT->CYCCNT;
  uint32_t au32_ticks = (SystemCoreClock / 1000000);
  au32_microseconds *= au32_ticks;
  while ((DWT->CYCCNT - au32_initial_ticks) < au32_microseconds - au32_ticks);
}
/*************************** End of file ****************************/
