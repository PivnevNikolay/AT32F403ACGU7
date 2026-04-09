/**-------------------------------------------------------------------
 \date  
 *
 *   STM32F103C8T6    
 *   ------------ 
 *  |            |
 *  |            |
 *  |            |
 *  |        PB.2| ---->  LED
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
#include "stm32f10x.h"

#define Input_mode                   (0x0UL)
#define Output_mode_speed_10_MHz     (0x1UL)
#define Output_mode_max_speed_2_MHz  (0x2UL)
#define Output_mode_max_speed_50_MHz (0x3UL)

#define output_push_pull             (0x0UL)
#define output_Open_drain            (0x1UL)
#define Alternate_output_Push_pull   (0x2UL)
#define Alternate_output_Open_drain  (0x3UL)

#define GPIO_CRL_MODE2_Pos (8U)
#define GPIO_CRL_MODE2_Msk (0x3U << GPIO_CRL_MODE2_Pos)

#define GPIO_CRL_CNF2_Pos  (10U)
#define GPIO_CRL_CNF2_Msk  (0x3U << GPIO_CRL_CNF2_Pos)

volatile u32 i;

void PORTB_2_INIT(void) {
  // RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
  SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPBEN); // Запуск тактирования порта B

  // GPIOB->CRL &= ~GPIO_CRL_MODE2;
  // GPIOB->CRL |= GPIO_CRL_MODE2_1;
  // GPIOB->CRL &= ~GPIO_CRL_CNF2;
  MODIFY_REG(GPIOB->CRL, GPIO_CRL_MODE2, Output_mode_max_speed_2_MHz << GPIO_CRL_MODE2_Pos); // Настройка GPIOB пин 2 на выход со скоростью в 2 MHz
  MODIFY_REG(GPIOB->CRL, GPIO_CRL_CNF2, output_push_pull << GPIO_CRL_CNF2_Pos);              // Настройка GPIOB пин 2 на выход в режиме Push-Pull
}

int main(void) {
  PORTB_2_INIT();
  SEGGER_RTT_printf(0, "echo_print\r\n");
  SEGGER_RTT_WriteString(0, "echo_WriteString\r\n");

  while (1) {
    // GPIOB->BSRR |= GPIO_BSRR_BS2;
    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS2);
    for ( i = 0; i < 1000000; i++)
      asm("nop");
    SEGGER_RTT_printf(0, "Led_ON\r\n");

    // GPIOB->BSRR |= GPIO_BSRR_BR2;
    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR2);
    for ( i = 0; i < 1000000; i++)
      asm("nop");
    SEGGER_RTT_printf(0, "Led_OFF\r\n");
  }
}
/*************************** End of file ****************************/