/**-------------------------------------------------------------------
 \date  09.04.2026.
 *
 *   STM32F103C8T6    
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
 *\ сode debugging ScuratovaAnna 
 */
#include "SEGGER_RTT.h"
#include "stm32f103x6.h"
#include <stdio.h>
#include <stdint.h>

#define SET_BIT(REG, BIT) ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT) ((REG) &= ~(BIT))
#define READ_BIT(REG, BIT) ((REG) & (BIT))
#define CLEAR_REG(REG) ((REG) = (0x0))
#define WRITE_REG(REG, VAL) ((REG) = (VAL))
#define READ_REG(REG) ((REG))
#define MODIFY_REG(REG, CLEARMASK, SETMASK) WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

#define GPIO_PIN_0                    ((GPIO_BSRR_BS0  << 8U) | 0x00000001U)  // Select pin 0  
#define GPIO_PIN_1                    ((GPIO_BSRR_BS1  << 8U) | 0x00000002U)  // Select pin 1  
#define GPIO_PIN_2                    ((GPIO_BSRR_BS2  << 8U) | 0x00000004U)  // Select pin 2  
#define GPIO_PIN_3                    ((GPIO_BSRR_BS3  << 8U) | 0x00000008U)  // Select pin 3  
#define GPIO_PIN_4                    ((GPIO_BSRR_BS4  << 8U) | 0x00000010U)  // Select pin 4  
#define GPIO_PIN_5                    ((GPIO_BSRR_BS5  << 8U) | 0x00000020U)  // Select pin 5  
#define GPIO_PIN_6                    ((GPIO_BSRR_BS6  << 8U) | 0x00000040U)  // Select pin 6  
#define GPIO_PIN_7                    ((GPIO_BSRR_BS7  << 8U) | 0x00000080U)  // Select pin 7  
#define GPIO_PIN_8                    ((GPIO_BSRR_BS8  << 8U) | 0x04000001U)  // Select pin 8  
#define GPIO_PIN_9                    ((GPIO_BSRR_BS9  << 8U) | 0x04000002U)  // Select pin 9  
#define GPIO_PIN_10                   ((GPIO_BSRR_BS10 << 8U) | 0x04000004U)  // Select pin 10 
#define GPIO_PIN_11                   ((GPIO_BSRR_BS11 << 8U) | 0x04000008U)  // Select pin 11 
#define GPIO_PIN_12                   ((GPIO_BSRR_BS12 << 8U) | 0x04000010U)  // Select pin 12 
#define GPIO_PIN_13                   ((GPIO_BSRR_BS13 << 8U) | 0x04000020U)  // Select pin 13 
#define GPIO_PIN_14                   ((GPIO_BSRR_BS14 << 8U) | 0x04000040U)  // Select pin 14 
#define GPIO_PIN_15                   ((GPIO_BSRR_BS15 << 8U) | 0x04000080U)  // Select pin 15 

 //--------------------------------------------------------------------
//In input mode  MODE[1:0]= 00 -->
#define Analog_mode                          (0x0UL)
#define Floating_input                       (0x1UL)
#define Input_pull_up_pull_down              (0x2UL)
//In output mode MODE[1:0] > 00 -->
#define General_purpose_output_push_pull     (0x0UL)
#define General_purpose_output_Open_drain    (0x1UL)
#define Alternate_function_output_Push_pull  (0x2UL)
#define Alternate_function_output_Open_drain (0x3UL)
//MODEy[1:0]
#define Input_mode                           (0x0UL) //MODE[1:0] = 00
#define Output_mode_max_speed_10_MHz         (0x1UL) //MODE[1:0] > 00
#define Output_mode_max_speed_2_MHz          (0x2UL) //MODE[1:0] > 00
#define Output_mode_max_speed_50_MHz         (0x3UL) //MODE[1:0] > 00
 //--------------------------------------------------------------------

__STATIC_INLINE void GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint32_t PinMask);
__STATIC_INLINE void GPIO_SetOutputPin(GPIO_TypeDef *GPIOx, uint32_t PinMask);
__STATIC_INLINE void GPIO_ResetOutputPin(GPIO_TypeDef *GPIOx, uint32_t PinMask);
  void delay_simple(uint32_t ms);
__STATIC_INLINE void DWT_Init(void);
__STATIC_INLINE void DWT_Delay_ms(volatile uint32_t au32_milliseconds);
__STATIC_INLINE void DWT_Delay_us(volatile uint32_t au32_microseconds);

void PORTC_13_INIT(void) {

  SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPCEN);  

  MODIFY_REG(GPIOC->CRH, GPIO_CRH_MODE13, Output_mode_max_speed_50_MHz << GPIO_CRH_MODE13_Pos);   //Настройка GPIOC порта 13 на выход со максимальной скоростью в 50 MHz
  MODIFY_REG(GPIOC->CRH, GPIO_CRH_CNF13, General_purpose_output_push_pull << GPIO_CRH_CNF13_Pos); //Настройка GPIOC порта 13 на выход в режиме Push-Pull
  //GPIO_ResetOutputPin(GPIOC, GPIO_PIN_13);
  WRITE_REG(GPIOC->BRR, GPIO_PIN_13);
  delay_simple(500);
}

int main(void) {
  PORTC_13_INIT();
  DWT_Init();
  //SEGGER_RTT_printf(0, "echo\r\n");
  //SEGGER_RTT_WriteString(0, "echo\r\n");
  while (1) {
 //  GPIO_TogglePin(GPIOC, GPIO_PIN_13);
 //  DWT_Delay_ms(250);
 //--------------------------------------------------------------------
 GPIOC->BSRR |= GPIO_BSRR_BS13;
 DWT_Delay_ms(250);
 //SEGGER_RTT_printf(0, "Led_ON\r\n");
 GPIOC->BSRR |= GPIO_BSRR_BR13;
 DWT_Delay_ms(250);
 //SEGGER_RTT_printf(0, "Led_OFF\r\n");
  }
}
//--------------------------------------------------------------------
__STATIC_INLINE void GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
  uint32_t odr = READ_REG(GPIOx->ODR);
  uint32_t pinmask = ((PinMask >> 8U) & 0x0000FFFFU);
  WRITE_REG(GPIOx->BSRR, ((odr & pinmask) << 16u) | (~odr & pinmask));
}
//--------------------------------------------------------------------
__STATIC_INLINE void GPIO_SetOutputPin(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
  WRITE_REG(GPIOx->BSRR, PinMask);
}
//--------------------------------------------------------------------
__STATIC_INLINE void GPIO_ResetOutputPin(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
  WRITE_REG(GPIOx->BRR, PinMask);
}
//--------------------------------------------------------------------
void delay_simple(uint32_t ms) {
  for (volatile uint32_t i = 0; i < ms * 1000; i++) {// если не указать <volatile> будет переодически при старте зависать
    __NOP(); // No operation instruction
  }
}
//--------------------------------------------------------------------
__STATIC_INLINE void DWT_Init(void) {
  CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk; // 
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;  // 
  DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk;            // 
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;             //
  __ASM volatile("NOP");
  __ASM volatile("NOP");
  __ASM volatile("NOP");
}
//--------------------------------------------------------------------
__STATIC_INLINE void DWT_Delay_ms(volatile uint32_t au32_milliseconds) {
  uint32_t au32_initial_ticks = DWT->CYCCNT;
  uint32_t au32_ticks = (SystemCoreClock / 1000);
  au32_milliseconds *= au32_ticks;
  while ((DWT->CYCCNT - au32_initial_ticks) < au32_milliseconds);
}
//--------------------------------------------------------------------
__STATIC_INLINE void DWT_Delay_us(volatile uint32_t au32_microseconds) {
  uint32_t au32_initial_ticks = DWT->CYCCNT;
  uint32_t au32_ticks = (SystemCoreClock / 1000000);
  au32_microseconds *= au32_ticks;
  while ((DWT->CYCCNT - au32_initial_ticks) < au32_microseconds - au32_ticks);
}
//-------------------------  End of file  ----------------------------
