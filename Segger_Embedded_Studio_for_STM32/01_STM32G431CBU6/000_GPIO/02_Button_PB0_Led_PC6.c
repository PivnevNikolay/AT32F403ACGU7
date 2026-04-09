/**-------------------------------------------------------------------
 \date  15.03.2026
 *
 *   STM32G431CBU6
 *   ------------
 *  |            |
 *  |            |
 *  |            |
 *  |        PC.6| ---->  LED
 *  |            |
 *  |        PB.0| <----  Button
 *  |            |
 *  |      +3.3V |
 *  |        GND |
 *
 * В качестве кнопки использую  touch button ttp223
 *
 * Подробные примеры тут:
 * https://deepbluembedded.com/stm32-gpio-pin-read-lab-digital-input/
 *
 *\ authors        ScuratovaAnna
 *\ сode debugging ScuratovaAnna
 */
#include "stdbool.h"
#include "stm32g4xx.h"

volatile bool condition = false;

#define GPIO_Pin_0  ((uint16_t)0x0001)
#define GPIO_Pin_1  ((uint16_t)0x0002)
#define GPIO_Pin_2  ((uint16_t)0x0004)
#define GPIO_Pin_3  ((uint16_t)0x0008)
#define GPIO_Pin_4  ((uint16_t)0x0010)
#define GPIO_Pin_5  ((uint16_t)0x0020)
#define GPIO_Pin_6  ((uint16_t)0x0040)
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

void PORTC_6_INIT_Led(void) {
  SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOCEN);                                                      // Запуск тактирования порта C
  MODIFY_REG(GPIOC->MODER, GPIO_MODER_MODE6, General_purpose_output_mode << GPIO_MODER_MODE6_Pos); // Настройка GPIOC пин 6 на выход (output mode)
  MODIFY_REG(GPIOC->OTYPER, GPIO_OTYPER_OT6, Output_push_pull << GPIO_OTYPER_OT6_Pos);             // Настройка GPIOC пин 6 в режим Push-Pull
  MODIFY_REG(GPIOC->OSPEEDR, GPIO_OSPEEDR_OSPEED6, High_speed << GPIO_OSPEEDR_OSPEED6_Pos);        // Настройка GPIOC пин 6 в режим High_speed
  MODIFY_REG(GPIOC->PUPDR, GPIO_PUPDR_PUPD6, No_pull_up_No_pull_down << GPIO_PUPDR_PUPD6_Pos);     // Настройка GPIOC пин 6 в режим High_speed
}

void PORTB_0_INIT_Button(void) {
  SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOBEN);                                                      // Запуск тактирования порта B
  MODIFY_REG(GPIOB->MODER, GPIO_MODER_MODE0, Input_mode << GPIO_MODER_MODE0_Pos);                  // Настройка GPIOB пин 0 на выход (Input_mode)
  MODIFY_REG(GPIOB->PUPDR, GPIO_PUPDR_PUPD0, No_pull_up_No_pull_down << GPIO_PUPDR_PUPD0_Pos);     // Настройка GPIOB пин 0 в режим No_pull_up_No_pull_down
}

int main(void) {
  PORTC_6_INIT_Led();
  PORTB_0_INIT_Button();
  while (1) {
    //--------------------------------------------------------------------
    //condition = READ_BIT(GPIOB->IDR, GPIO_IDR_IDR_0);
    //(condition == true) ? (SET_BIT(GPIOC->BSRR, GPIO_BSRR_BS6)) : (SET_BIT(GPIOC->BSRR, GPIO_BSRR_BR6));
    //--------------------------------------------------------------------
    //((GPIOB->IDR & 0x1Ul)==0x1UL)? (SET_BIT(GPIOC->BSRR, GPIO_BSRR_BS6)) : (SET_BIT(GPIOC->BSRR, GPIO_BSRR_BR6));//PB.0
    //--------------------------------------------------------------------
    //Если в проекте был настроен на вход вывод PB.1, то
    //((GPIOB->IDR & 0x2Ul)==0x1UL)? (SET_BIT(GPIOC->BSRR, GPIO_BSRR_BS6)) : (SET_BIT(GPIOC->BSRR, GPIO_BSRR_BR6));
    //--------------------------------------------------------------------
    //Если в проекте был настроен на вход вывод PB.2, то
    //((GPIOB->IDR & 0x4Ul)==0x1UL)? (SET_BIT(GPIOC->BSRR, GPIO_BSRR_BS6)) : (SET_BIT(GPIOC->BSRR, GPIO_BSRR_BR6));
    //--------------------------------------------------------------------
    //Если в проекте был настроен на вход вывод PB.3, то
    //((GPIOB->IDR & 0x8Ul)==0x1UL)? (SET_BIT(GPIOC->BSRR, GPIO_BSRR_BS6)) : (SET_BIT(GPIOC->BSRR, GPIO_BSRR_BR6));
    //--------------------------------------------------------------------
    //#define GPIO_Pin_0  ((uint16_t)0x0001)//Px.0(PA.0; PB.0; PC.0 ... Px.0)
    //#define GPIO_Pin_1  ((uint16_t)0x0002)//Px.1
    //#define GPIO_Pin_2  ((uint16_t)0x0004)//Px.2
    //#define GPIO_Pin_3  ((uint16_t)0x0008)//Px.3 и т.д.
    //#define GPIO_Pin_4  ((uint16_t)0x0010)
    //#define GPIO_Pin_5  ((uint16_t)0x0020)
    //#define GPIO_Pin_6  ((uint16_t)0x0040)
    //#define GPIO_Pin_7  ((uint16_t)0x0080)
    //#define GPIO_Pin_8  ((uint16_t)0x0100)
    //#define GPIO_Pin_9  ((uint16_t)0x0200)
    //#define GPIO_Pin_10 ((uint16_t)0x0400)
    //#define GPIO_Pin_11 ((uint16_t)0x0800)
    //#define GPIO_Pin_12 ((uint16_t)0x1000)
    //#define GPIO_Pin_13 ((uint16_t)0x2000)
    //#define GPIO_Pin_14 ((uint16_t)0x4000)
    //#define GPIO_Pin_15 ((uint16_t)0x8000)//Px.15
    //согласно выше приведённому можно использовать макросы, для того чтобы код было удобно читать и сам код был более понятным:
    ((GPIOB->IDR & GPIO_Pin_0)==0x1UL)? (SET_BIT(GPIOC->BSRR, GPIO_BSRR_BS6)) : (SET_BIT(GPIOC->BSRR, GPIO_BSRR_BR6));
  }
}
/*************************** End of file ****************************/


// authors        PivnevNikolay 
// сode debugging ScuratovaAnna + PivnevNikolay 
#include "stdbool.h"
#include "stm32g4xx.h"

__IO bool led_state = 0; // 0 - выключен, 1 - включен

#define GPIO_Pin_0  ((uint16_t)0x0001)
#define GPIO_Pin_1  ((uint16_t)0x0002)
#define GPIO_Pin_2  ((uint16_t)0x0004)
#define GPIO_Pin_3  ((uint16_t)0x0008)
#define GPIO_Pin_4  ((uint16_t)0x0010)
#define GPIO_Pin_5  ((uint16_t)0x0020)
#define GPIO_Pin_6  ((uint16_t)0x0040)
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

// Структура для светодиода
typedef struct {
  GPIO_TypeDef *port;
  uint16_t pin;
  bool active_state;
} LED_HandleTypeDef;

// Структура для кнопки
typedef struct {
  GPIO_TypeDef *port;
  uint16_t pin;
  bool active_state; // 0 - активный низкий, 1 - активный высокий
} Button_HandleTypeDef;

// Конфигурация для светодиода PC.6
LED_HandleTypeDef STM32G431_led = {
    .port = GPIOC,
    .pin = GPIO_Pin_6,
    .active_state = 1 // Активный высокий
};

// Конфигурация для кнопки PB.0
Button_HandleTypeDef STM32G431_button = {
    .port = GPIOB,
    .pin = GPIO_Pin_0,
    .active_state = 0 // ???
};

void PORTC_6_INIT_Led(void) {
  SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOCEN);                                                      // Запуск тактирования порта C
  MODIFY_REG(GPIOC->MODER, GPIO_MODER_MODE6, General_purpose_output_mode << GPIO_MODER_MODE6_Pos); // Настройка GPIOC пин 6 на выход (output mode)
  MODIFY_REG(GPIOC->OTYPER, GPIO_OTYPER_OT6, Output_push_pull << GPIO_OTYPER_OT6_Pos);             // Настройка GPIOC пин 6 в режим Push-Pull
  MODIFY_REG(GPIOC->OSPEEDR, GPIO_OSPEEDR_OSPEED6, High_speed << GPIO_OSPEEDR_OSPEED6_Pos);        // Настройка GPIOC пин 6 в режим High_speed
  MODIFY_REG(GPIOC->PUPDR, GPIO_PUPDR_PUPD6, No_pull_up_No_pull_down << GPIO_PUPDR_PUPD6_Pos);     // Настройка GPIOC пин 6 в режим High_speed
}

void PORTB_0_INIT_Button(void) {
  SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOBEN);                                                      // Запуск тактирования порта B
  MODIFY_REG(GPIOB->MODER, GPIO_MODER_MODE0, Input_mode << GPIO_MODER_MODE0_Pos);                  // Настройка GPIOB пин 0 на выход (Input_mode)
  MODIFY_REG(GPIOB->PUPDR, GPIO_PUPDR_PUPD0, No_pull_up_No_pull_down << GPIO_PUPDR_PUPD0_Pos);     // Настройка GPIOB пин 0 в режим No_pull_up_No_pull_down
}

void LED_On(LED_HandleTypeDef *led);
void LED_Off(LED_HandleTypeDef *led);
bool Button_Read(Button_HandleTypeDef *btn);

int main(void) {
  PORTC_6_INIT_Led();
  PORTB_0_INIT_Button();
  while (1) {
    led_state = Button_Read(&STM32G431_button);
    led_state = !led_state; // инверсия сигнала, данную строку можно закомментировать.
    if (led_state) {
      LED_On(&STM32G431_led);
    } else {
      LED_Off(&STM32G431_led);
    }
  }
}

// Включение светодиода
void LED_On(LED_HandleTypeDef *led) {
  if (led->active_state) {
    led->port->BSRR = led->pin;
  } else {
    led->port->BSRR = (uint32_t)led->pin << 16;
  }
}

// Выключение светодиода
void LED_Off(LED_HandleTypeDef *led) {
  if (led->active_state) {
    led->port->BSRR = (uint32_t)led->pin << 16;
  } else {
    led->port->BSRR = led->pin;
  }
}

// Чтение состояния кнопки
bool Button_Read(Button_HandleTypeDef *btn) {
  // Читаем состояние пина и сравниваем с активным состоянием
  if (btn->active_state) {
    return (btn->port->IDR & btn->pin) != 0;
  } else {
    return (btn->port->IDR & btn->pin) == 0;
  }
}
/*************************** End of file ****************************/