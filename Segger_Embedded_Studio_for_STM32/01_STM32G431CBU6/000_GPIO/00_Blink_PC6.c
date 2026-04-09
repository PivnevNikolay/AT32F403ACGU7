/**-------------------------------------------------------------------
 \date  10.03.2026
 *
 *   STM32G431CBU6
 *   ------------
 *  |            |
 *  |            |
 *  |            |
 *  |        PC.6| ---->  LED
 *  |            |
 *  |            |
 *  |            |
 *  |      +3.3V |
 *  |        GND |
 *
 *\ authors        ScuratovaAnna
 *\ сode debugging ScuratovaAnna
 */
//*************************** Пример первый **************************
#include "stm32g4xx.h"

void delay_ms(uint32_t ms) {
  for (uint32_t i = 0; i < ms * 1000; i++) {
    __NOP();    // No operation instruction
  }
}

void PORTC_6_INIT(void) {
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

  GPIOC->MODER &= ~GPIO_MODER_MODE6;         // Clear mode bits
  GPIOC->MODER |= GPIO_MODER_MODE6_0;        // Output mode (01)
  GPIOC->OTYPER &= ~GPIO_OTYPER_OT6;         // Push-pull
  GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEED6;    // High speed
  GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD6;         // No pull-up/pull-down
}
int main(void) {
  PORTC_6_INIT();
  while (1) {
    GPIOC->BSRR |= GPIO_BSRR_BS6;
    delay_ms(500);
    GPIOC->BSRR |= GPIO_BSRR_BR6;
    delay_ms(500);
  }
}

/*************************** End of file ****************************/
//*************************** Пример второй **************************
#include "stm32g4xx.h"

#define Input_mode                  (0x0UL)
#define General_purpose_output_mode (0x1UL)
#define Alternate_function_mode     (0x2UL)
#define Analog_mode                 (0x3UL)

#define Output_push_pull            (0x0UL)
#define Output_open_drain           (0x1UL)

#define Low_speed                   (0x0UL)
#define Medium_speed                (0x1UL)
#define High_speed                  (0x2UL)
#define Very_high_speed             (0x3UL)

#define No_pull_up_No_pull_down     (0x0UL)
#define Pull_up                     (0x1UL)
#define Pull_down                   (0x2UL)

void delay_ms(uint32_t ms) {
  for (uint32_t i = 0; i < ms * 1000; i++) {
    __NOP(); // No operation instruction
  }
}

void PORTC_6_INIT(void) {
  SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOCEN);                                                      // Запуск тактирования порта C
  MODIFY_REG(GPIOC->MODER, GPIO_MODER_MODE6, General_purpose_output_mode << GPIO_MODER_MODE6_Pos); // Настройка GPIOC пин 6 на выход (output mode)
  MODIFY_REG(GPIOC->OTYPER, GPIO_OTYPER_OT6, Output_push_pull << GPIO_OTYPER_OT6_Pos);             // Настройка GPIOC пин 6 в режим Push-Pull
  MODIFY_REG(GPIOC->OSPEEDR, GPIO_OSPEEDR_OSPEED6, High_speed << GPIO_OSPEEDR_OSPEED6_Pos);        // Настройка GPIOC пин 6 в режим High_speed
  MODIFY_REG(GPIOC->PUPDR, GPIO_PUPDR_PUPD6, No_pull_up_No_pull_down << GPIO_PUPDR_PUPD6_Pos);     // Настройка GPIOC пин 6 в режим No_pull_up_No_pull_down
}

int main(void) {
  PORTC_6_INIT();
  while (1) {
    SET_BIT(GPIOC->BSRR, GPIO_BSRR_BS6);
    delay_ms(200);
    SET_BIT(GPIOC->BSRR, GPIO_BSRR_BR6);
    delay_ms(200);
  }
}
/*************************** End of file ****************************/
//************************** Пример третий ***************************
#include "stm32g4xx.h"
#include "stdbool.h"

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

#define GPIO_Pin_0  ((uint16_t)0x0001)
#define GPIO_Pin_1  ((uint16_t)0x0002)
#define GPIO_Pin_2  ((uint16_t)0x0004)
#define GPIO_Pin_3  ((uint16_t)0x0008)
#define GPIO_Pin_4  ((uint16_t)0x0010)
#define GPIO_Pin_5  ((uint16_t)0x0020)
#define GPIO_Pin_6  ((uint16_t)0x0040)//Led
#define GPIO_Pin_7  ((uint16_t)0x0080)
#define GPIO_Pin_8  ((uint16_t)0x0100)
#define GPIO_Pin_9  ((uint16_t)0x0200)
#define GPIO_Pin_10 ((uint16_t)0x0400)
#define GPIO_Pin_11 ((uint16_t)0x0800)
#define GPIO_Pin_12 ((uint16_t)0x1000)
#define GPIO_Pin_13 ((uint16_t)0x2000)
#define GPIO_Pin_14 ((uint16_t)0x4000)
#define GPIO_Pin_15 ((uint16_t)0x8000)

#define Input_mode                  (0x0UL)
#define General_purpose_output_mode (0x1UL)
#define Alternate_function_mode     (0x2UL)
#define Analog_mode                 (0x3UL)

#define Output_push_pull            (0x0UL)
#define Output_open_drain           (0x1UL)

#define Low_speed                   (0x0UL)
#define Medium_speed                (0x1UL)
#define High_speed                  (0x2UL)
#define Very_high_speed             (0x3UL)

#define No_pull_up_No_pull_down     (0x0UL)
#define Pull_up                     (0x1UL)
#define Pull_down                   (0x2UL)

// Структура для представления светодиода
typedef struct {
  GPIO_TypeDef *port;   // Порт GPIO
  u16 pin;         // Номер пина
  bool active_state; // Логика управления (0 - активный низкий, 1 - активный высокий)
} LED_HandleTypeDef;

// Конфигурация для светодиода на отладочной плате STM32G431CBU6 (PC6)
LED_HandleTypeDef STM32G431_Led = {
    .port = GPIOC,
    .pin = GPIO_Pin_6,
    .active_state = 1 // Активный высокий уровень
};

void LED_On(LED_HandleTypeDef *led);
void LED_Off(LED_HandleTypeDef *led);
void LED_Toggle(LED_HandleTypeDef *led);
void delay_ms(u32 ms);

void PORTC_6_INIT(void) {
  SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOCEN);                                                      // Запуск тактирования порта C
  MODIFY_REG(GPIOC->MODER, GPIO_MODER_MODE6, General_purpose_output_mode << GPIO_MODER_MODE6_Pos); // Настройка GPIOC пин 6 на выход (output mode)
  MODIFY_REG(GPIOC->OTYPER, GPIO_OTYPER_OT6, Output_push_pull << GPIO_OTYPER_OT6_Pos);             // Настройка GPIOC пин 6 в режим Push-Pull
  MODIFY_REG(GPIOC->OSPEEDR, GPIO_OSPEEDR_OSPEED6, High_speed << GPIO_OSPEEDR_OSPEED6_Pos);        // Настройка GPIOC пин 6 в режим High_speed
  MODIFY_REG(GPIOC->PUPDR, GPIO_PUPDR_PUPD6, No_pull_up_No_pull_down << GPIO_PUPDR_PUPD6_Pos);     // Настройка GPIOC пин 6 в режим High_speed
}

int main(void) {
  PORTC_6_INIT();
  while (1) {
    LED_Toggle(&STM32G431_Led);// Переключение состояния светодиода
    delay_ms(100);// Задержка
  }
}

// Включение светодиода
void LED_On(LED_HandleTypeDef *led) {
  if (led->active_state) {
    led->port->BSRR = led->pin; // Установить пин (SET)
  } else {
    led->port->BSRR = (u32)led->pin << 16; // Сбросить пин (RESET)
  }
}

// Выключение светодиода
void LED_Off(LED_HandleTypeDef *led) {
  if (led->active_state) {
    led->port->BSRR = (u32)led->pin << 16; // Сбросить пин (RESET)
  } else {
    led->port->BSRR = led->pin; // Установить пин (SET)
  }
}

// Переключение состояния светодиода
void LED_Toggle(LED_HandleTypeDef *led) {
  led->port->ODR ^= led->pin; // Инвертировать состояние через регистр ODR
}

// Простая функция задержки
void delay_ms(u32 ms) {
  for (__IO u32 q = 0; q < ms * 2000; q++) {
    __NOP();
  }
}
/*************************** End of file ****************************/