/**-------------------------------------------------------------------
 \date  
 *
 *   STM32G431CBU6
 *    BluePill +
 *   ------------
 *  |            |
 *  |            |
 *  |            |
 *  |        PB.2| ---->  LED
 *  |            |
 *  |        PA.3| <----  Button
 *  |            |
 *  |      +3.3V |
 *  |        GND |
 *
 * В качестве кнопки использую  touch button ttp223
 *
 *\ authors        ScuratovaAnna
 *\ сode debugging ScuratovaAnna
 */
#include "stdbool.h"
#include "stm32f10x.h"

#define Input_mode                     (0x0UL)
#define Output_mode_speed_10_MHz       (0x1UL)
#define Output_mode_max_speed_2_MHz    (0x2UL)
#define Output_mode_max_speed_50_MHz   (0x3UL)

#define Analog_mode                    (0x0UL)
#define Floating_input                 (0x1UL)
#define Input_with_pull_up_pull_down   (0x2UL)
#define Output_mode_max_speed_50_MHz   (0x3UL)

#define output_push_pull               (0x0UL)
#define output_Open_drain              (0x1UL)
#define Alternate_output_Push_pull     (0x2UL)
#define Alternate_output_Open_drain    (0x3UL)

#define GPIO_CRL_MODE0_Pos (0U)
#define GPIO_CRL_MODE0_Msk (0x3UL << GPIO_CRL_MODE0_Pos)

#define GPIO_CRL_MODE2_Pos (8U)
#define GPIO_CRL_MODE2_Msk (0x3U << GPIO_CRL_MODE2_Pos)

#define GPIO_CRL_MODE3_Pos (12U)
#define GPIO_CRL_MODE3_Msk (0x3UL << GPIO_CRL_MODE3_Pos)

#define GPIO_CRL_CNF0_Pos  (2U)
#define GPIO_CRL_CNF0_Msk  (0x3UL << GPIO_CRL_CNF0_Pos)

#define GPIO_CRL_CNF2_Pos  (10U)
#define GPIO_CRL_CNF2_Msk  (0x3U << GPIO_CRL_CNF2_Pos)

#define GPIO_CRL_CNF3_Pos  (14U)
#define GPIO_CRL_CNF3_Msk  (0x3UL << GPIO_CRL_CNF3_Pos)

#define Led_PB0_ON()  SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS2);
#define Led_PB0_OFF() SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR2);

bool Button_State = false;

void PORTB_2_INIT(void) {
  SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPBEN); // Запуск тактирования порта B

  MODIFY_REG(GPIOB->CRL, GPIO_CRL_MODE2, Output_mode_max_speed_2_MHz << GPIO_CRL_MODE2_Pos); // Настройка GPIOB пин 2 на выход со скоростью в 2 MHz
  MODIFY_REG(GPIOB->CRL, GPIO_CRL_CNF2, output_push_pull << GPIO_CRL_CNF2_Pos);              // Настройка GPIOB пин 2 на выход в режиме Push-Pull
}

void PORTA_3_INIT(void) {
  SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPAEN);                                  // Запуск тактирования порта A
  MODIFY_REG(GPIOA->CRL, GPIO_CRL_MODE3, Input_mode << GPIO_CRL_MODE3_Pos);   // Настройка GPIOA пин 3 на вход
  MODIFY_REG(GPIOA->CRL, GPIO_CRL_CNF3, Floating_input << GPIO_CRL_CNF3_Pos); // Настройка GPIOA пин 3 Floating input
}

int main(void) {
  PORTB_2_INIT();
  PORTA_3_INIT();

  while (1) {
    Button_State = READ_BIT(GPIOA->IDR, GPIO_IDR_IDR3);
    if (Button_State == 1) {
      Led_PB0_ON();
    } else {
      Led_PB0_OFF();
    }
  }
}
