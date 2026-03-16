/**-------------------------------------------------------------------
 \date  08.03.2026
 *
 *   STM32G431CBU6
 *   ------------
 *  |            |
 *  |            |
 *  |            |
 *  |        PC.6| ---->  LED
 *  |            |
 *  |       PC.13| <----  Button
 *  |            |
 *  |      +3.3V |
 *  |        GND |
 *
 * 
 *
 *
 *\ authors        ScuratovaAnna
 *\ сode debugging ScuratovaAnna
 */
 
#include "stm32g4xx.h"
#include "stdbool.h"

__IO bool led_state = 0;

#define GPIO_Pin_0 ((uint16_t)0x0001)
#define GPIO_Pin_1 ((uint16_t)0x0002)
#define GPIO_Pin_2 ((uint16_t)0x0004)
#define GPIO_Pin_3 ((uint16_t)0x0008)
#define GPIO_Pin_4 ((uint16_t)0x0010)
#define GPIO_Pin_5 ((uint16_t)0x0020)
#define GPIO_Pin_6 ((uint16_t)0x0040) //Led
#define GPIO_Pin_7 ((uint16_t)0x0080)
#define GPIO_Pin_8 ((uint16_t)0x0100)
#define GPIO_Pin_9 ((uint16_t)0x0200)
#define GPIO_Pin_10 ((uint16_t)0x0400)
#define GPIO_Pin_11 ((uint16_t)0x0800)
#define GPIO_Pin_12 ((uint16_t)0x1000)
#define GPIO_Pin_13 ((uint16_t)0x2000)//Button
#define GPIO_Pin_14 ((uint16_t)0x4000)
#define GPIO_Pin_15 ((uint16_t)0x8000)

#define Input_mode (0x0UL)
#define General_purpose_output_mode (0x1UL)
#define Alternate_function_mode (0x2UL)
#define Analog_mode (0x3UL)

#define Output_push_pull (0x0UL)
#define Output_open_drain (0x1UL)

#define Low_speed (0x0UL)
#define Medium_speed (0x1UL)
#define High_speed (0x2UL)
#define Very_high_speed (0x3UL)

#define No_pull_up_No_pull_down (0x0UL)
#define Pull_up (0x1UL)
#define Pull_down (0x2UL)

void delay_simple(volatile uint32_t ms) {
  for (uint32_t i = 0; i < ms* 100; i++) {
    __NOP(); // No operation instruction
  }
}

void PORTC_6_INIT_Led(void) {
  SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOCEN);                                                      // Запуск тактирования порта C
  MODIFY_REG(GPIOC->MODER, GPIO_MODER_MODE6, General_purpose_output_mode << GPIO_MODER_MODE6_Pos); // Настройка GPIOC пин 6 на выход (output mode)
  MODIFY_REG(GPIOC->OTYPER, GPIO_OTYPER_OT6, Output_push_pull << GPIO_OTYPER_OT6_Pos);             // Настройка GPIOC пин 6 в режим Push-Pull
  MODIFY_REG(GPIOC->OSPEEDR, GPIO_OSPEEDR_OSPEED6, High_speed << GPIO_OSPEEDR_OSPEED6_Pos);        // Настройка GPIOC пин 6 в режим High_speed
  MODIFY_REG(GPIOC->PUPDR, GPIO_PUPDR_PUPD6, No_pull_up_No_pull_down << GPIO_PUPDR_PUPD6_Pos);     // Настройка GPIOC пин 6 в режим No_pull_up_No_pull_down
}

void PORTC_13_INIT_Button(void) {
  MODIFY_REG(GPIOC->MODER, GPIO_MODER_MODE13, Input_mode << GPIO_MODER_MODE13_Pos);// Настройка GPIOC пин 13 на выход (Input_mode)
  MODIFY_REG(GPIOC->PUPDR, GPIO_PUPDR_PUPD13, Pull_down << GPIO_PUPDR_PUPD13_Pos); // Настройка GPIOC пин 13 в режим Pull_down
}

uint8_t button_debounce(void) {
  if (((READ_BIT(GPIOC->IDR, GPIO_Pin_13) == (GPIO_Pin_13)) ? 1UL : 0UL) == SET)   // Кнопка нажата?
  {
    delay_simple(100);                                                             // Задержка 10-20 мс
    if (((READ_BIT(GPIOC->IDR, GPIO_Pin_13) == (GPIO_Pin_13)) ? 1UL : 0UL) == SET) // Проверяем еще раз
    {
      // Ждем отпускания кнопки
      while (((READ_BIT(GPIOC->IDR, GPIO_Pin_13) == (GPIO_Pin_13)) ? 1UL : 0UL) == SET);
      delay_simple(10); // Задержка после отпускания
      return 1;           // Кнопка была нажата
    }
  }
  return 0; // Кнопка не нажата
}

int main(void) {
  PORTC_6_INIT_Led();
  PORTC_13_INIT_Button();
  while (1) {
    // Опрашиваем кнопку с антидребезгом
    if (button_debounce()) {
      // Меняем состояние светодиода
      led_state = !led_state;
      if (led_state)
        SET_BIT(GPIOC->BSRR, GPIO_BSRR_BS6); // Включить
      else
        SET_BIT(GPIOC->BSRR, GPIO_BSRR_BR6); // Выключить
    }
  }
}

/*************************** End of file ****************************/
