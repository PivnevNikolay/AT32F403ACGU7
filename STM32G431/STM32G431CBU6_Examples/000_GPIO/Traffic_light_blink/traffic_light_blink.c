/**-------------------------------------------------------------------
 \date  06.03.2026
 *
 *  traffic light blink
 *
 *   STM32G431CBU6
 *   ------------
 *  |            |
 *  |            |
 *  |            |
 *  |        PA.0| ---->  LED R
 *  |        PA.1| ---->  LED Y
 *  |        PA.2| ---->  LED G
 *  |            |
 *  |            |
 *  |      +3.3V |
 *  |        GND |
 *
 * ссылка на модуль который использую в примере(LED светофорный модуль): 
 * https://aliexpress.ru/item/32881664301.html?spm=a2g2w.orderdetail.0.0.3ea24aa6tVe7MA&sku_id=65632741499&_ga=2.38745450.837217396.1752098094-1084817420.1750673554
 *\ authors        ScuratovaAnna
 *\ сode debugging ScuratovaAnna
 */
//**************************  Пример первый **************************
#include "stm32g4xx.h"

typedef uint32_t  u32;
typedef uint16_t  u16;
typedef uint8_t   u8;

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

volatile u8 CW[3]={0x1,0x2,0x4};

void delay_ms(uint32_t ms) {
  for (uint32_t i = 0; i < ms * 1000; i++) {
    __NOP(); 
  }
}

void GPIO_INIT(void) {
  SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOAEN);                                                      // Запуск тактирования порта A

  MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODE0, General_purpose_output_mode << GPIO_MODER_MODE0_Pos); // Настройка GPIOA пин 0 на выход (output mode)
  MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODE1, General_purpose_output_mode << GPIO_MODER_MODE1_Pos); // Настройка GPIOA пин 1 на выход (output mode)
  MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODE2, General_purpose_output_mode << GPIO_MODER_MODE2_Pos); // Настройка GPIOA пин 2 на выход (output mode)

  MODIFY_REG(GPIOA->OTYPER, GPIO_OTYPER_OT0, Output_push_pull << GPIO_OTYPER_OT0_Pos);             // Настройка GPIOA пин 0 в режим Push-Pull
  MODIFY_REG(GPIOA->OTYPER, GPIO_OTYPER_OT1, Output_push_pull << GPIO_OTYPER_OT1_Pos);             // Настройка GPIOA пин 1 в режим Push-Pull
  MODIFY_REG(GPIOA->OTYPER, GPIO_OTYPER_OT2, Output_push_pull << GPIO_OTYPER_OT2_Pos);             // Настройка GPIOA пин 2 в режим Push-Pull

  MODIFY_REG(GPIOA->OSPEEDR, GPIO_OSPEEDR_OSPEED0, High_speed << GPIO_OSPEEDR_OSPEED0_Pos);        // Настройка GPIOA пин 0 в режим High_speed
  MODIFY_REG(GPIOA->OSPEEDR, GPIO_OSPEEDR_OSPEED1, High_speed << GPIO_OSPEEDR_OSPEED1_Pos);        // Настройка GPIOA пин 1 в режим High_speed
  MODIFY_REG(GPIOA->OSPEEDR, GPIO_OSPEEDR_OSPEED2, High_speed << GPIO_OSPEEDR_OSPEED2_Pos);        // Настройка GPIOA пин 2 в режим High_speed

  MODIFY_REG(GPIOA->PUPDR, GPIO_PUPDR_PUPD0, No_pull_up_No_pull_down << GPIO_PUPDR_PUPD0_Pos);     // Настройка GPIOA пин 0 в режим No_pull_up_No_pull_down
  MODIFY_REG(GPIOA->PUPDR, GPIO_PUPDR_PUPD1, No_pull_up_No_pull_down << GPIO_PUPDR_PUPD1_Pos);     // Настройка GPIOA пин 1 в режим No_pull_up_No_pull_down
  MODIFY_REG(GPIOA->PUPDR, GPIO_PUPDR_PUPD2, No_pull_up_No_pull_down << GPIO_PUPDR_PUPD2_Pos);     // Настройка GPIOA пин 2 в режим No_pull_up_No_pull_down
}

int main(void) {
  GPIO_INIT();
  while (1) {
   for (u8 i=0;i<3;i++){
    GPIOA->ODR = *(CW+i);
    delay_ms(800);
    }
  }
}
/*************************** End of file ****************************/
/**-------------------------------------------------------------------
 \date  11.07.2025
 *
 *  traffic light blink
 *
 *   STM32G431CBU6
 *   ------------
 *  |            |
 *  |            |
 *  |            |
 *  |        PA.0| ---->  LED R
 *  |        PA.1| ---->  LED Y
 *  |        PA.2| ---->  LED G
 *  |            |
 *  |            |
 *  |      +3.3V |
 *  |        GND |
 *
 * ссылка на модуль который использую в примере(LED светофорный модуль): 
 * https://aliexpress.ru/item/32881664301.html?spm=a2g2w.orderdetail.0.0.3ea24aa6tVe7MA&sku_id=65632741499&_ga=2.38745450.837217396.1752098094-1084817420.1750673554
 *
 * Подсмотрено здесь: https://alexgyver.ru/lessons/traffic-light/
 * STM32F10x Standard Peripherals Library:
 * GPIO_pins_define -> http://stm32.kosyak.info/doc/group___g_p_i_o__pins__define.html#gad6ec74e33360395535ad5d91ba6d4781
 *
 *\ authors        PivnevNikolay 
 *\ сode debugging ScuratovaAnna + PivnevNikolay 
 */
//**************************  Пример второй **************************
#include "stm32g4xx.h"
#include "stdbool.h"

typedef uint32_t  u32;
typedef uint16_t  u16;
typedef uint8_t   u8;

#define 	GPIO_Pin_0    ((uint16_t)0x0001)// red
#define 	GPIO_Pin_1    ((uint16_t)0x0002)// yellow
#define 	GPIO_Pin_2    ((uint16_t)0x0004)// green
#define 	GPIO_Pin_3    ((uint16_t)0x0008)
#define 	GPIO_Pin_4    ((uint16_t)0x0010)
#define 	GPIO_Pin_5    ((uint16_t)0x0020)
#define 	GPIO_Pin_6    ((uint16_t)0x0040)
#define 	GPIO_Pin_7    ((uint16_t)0x0080)
#define 	GPIO_Pin_8    ((uint16_t)0x0100)
#define 	GPIO_Pin_9    ((uint16_t)0x0200)
#define 	GPIO_Pin_10   ((uint16_t)0x0400)
#define 	GPIO_Pin_11   ((uint16_t)0x0800)
#define 	GPIO_Pin_12   ((uint16_t)0x1000)
#define 	GPIO_Pin_13   ((uint16_t)0x2000)
#define 	GPIO_Pin_14   ((uint16_t)0x4000)
#define 	GPIO_Pin_15   ((uint16_t)0x8000)

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
//--------------------------------------------------------------------
typedef struct {
    GPIO_TypeDef* port;
    u32 pin;
    bool active_state;  // 0 = активный низкий, 1 = активный высокий
} LED_HandleTypeDef;

typedef struct {
    LED_HandleTypeDef red;
    LED_HandleTypeDef yellow;
    LED_HandleTypeDef green;
} RYG_LED_HandleTypeDef;

const RYG_LED_HandleTypeDef STM32G431_ryg = {
    .red   =  {GPIOA, GPIO_Pin_0, 1},
    .yellow = {GPIOA, GPIO_Pin_1, 1},
    .green  = {GPIOA, GPIO_Pin_2, 1}
};
//--------------------------------------------------------------------
void delay_ms(u32 ms) {
  for (__IO u32 i = 0; i < ms * 2000; i++) {
    __NOP(); // No operation instruction
  }
}

void GPIO_INIT(void) {
  SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOAEN);                                                      // Запуск тактирования порта A

  MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODE0, General_purpose_output_mode << GPIO_MODER_MODE0_Pos); // Настройка GPIOA пин 0 на выход (output mode)
  MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODE1, General_purpose_output_mode << GPIO_MODER_MODE1_Pos); // Настройка GPIOA пин 1 на выход (output mode)
  MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODE2, General_purpose_output_mode << GPIO_MODER_MODE2_Pos); // Настройка GPIOA пин 2 на выход (output mode)

  MODIFY_REG(GPIOA->OTYPER, GPIO_OTYPER_OT0, Output_push_pull << GPIO_OTYPER_OT0_Pos);             // Настройка GPIOA пин 0 в режим Push-Pull
  MODIFY_REG(GPIOA->OTYPER, GPIO_OTYPER_OT1, Output_push_pull << GPIO_OTYPER_OT1_Pos);             // Настройка GPIOA пин 1 в режим Push-Pull
  MODIFY_REG(GPIOA->OTYPER, GPIO_OTYPER_OT2, Output_push_pull << GPIO_OTYPER_OT2_Pos);             // Настройка GPIOA пин 2 в режим Push-Pull

  MODIFY_REG(GPIOA->OSPEEDR, GPIO_OSPEEDR_OSPEED0, High_speed << GPIO_OSPEEDR_OSPEED0_Pos);        // Настройка GPIOA пин 0 в режим High_speed
  MODIFY_REG(GPIOA->OSPEEDR, GPIO_OSPEEDR_OSPEED1, High_speed << GPIO_OSPEEDR_OSPEED1_Pos);        // Настройка GPIOA пин 1 в режим High_speed
  MODIFY_REG(GPIOA->OSPEEDR, GPIO_OSPEEDR_OSPEED2, High_speed << GPIO_OSPEEDR_OSPEED2_Pos);        // Настройка GPIOA пин 2 в режим High_speed

  MODIFY_REG(GPIOA->PUPDR, GPIO_PUPDR_PUPD0, No_pull_up_No_pull_down << GPIO_PUPDR_PUPD0_Pos);     // Настройка GPIOA пин 0 в режим No_pull_up_No_pull_down
  MODIFY_REG(GPIOA->PUPDR, GPIO_PUPDR_PUPD1, No_pull_up_No_pull_down << GPIO_PUPDR_PUPD1_Pos);     // Настройка GPIOA пин 1 в режим No_pull_up_No_pull_down
  MODIFY_REG(GPIOA->PUPDR, GPIO_PUPDR_PUPD2, No_pull_up_No_pull_down << GPIO_PUPDR_PUPD2_Pos);     // Настройка GPIOA пин 2 в режим No_pull_up_No_pull_down
}

void LED_SetState(const LED_HandleTypeDef* led, u8 state) {
    if (led->active_state) {
        // Для активного высокого уровня
        if (state) led->port->BSRR = led->pin;
        else led->port->BSRR = (led->pin << 16);
    } else {
        // Для активного низкого уровня
        if (state) led->port->BSRR = (led->pin << 16);
        else led->port->BSRR = led->pin;
    }
}

void LED_Toggle(const LED_HandleTypeDef* led) {
  for (u8 i = 0; i < 10; i++){
    led->port->ODR ^= led->pin;
    delay_ms(100);
  }
}

// Управление RYG светофором
void RYG_SetColor(const RYG_LED_HandleTypeDef* ryg, u8 r, u8 y, u8 g) {
    LED_SetState(&ryg->red,    r);
    LED_SetState(&ryg->yellow, y);
    LED_SetState(&ryg->green,  g);
}

void RYG_Blink(const RYG_LED_HandleTypeDef* ryg, u32 ryg_ms) {

    static u8 color_state = 0;
    
    switch(color_state) {
        case 0:  // Красный
            RYG_SetColor(ryg, 1, 0, 0);
            delay_ms(50);
            LED_Toggle(&ryg->yellow);// Мигаем Жёлтым при включеном Красном
            break;
        case 1:  // Жёлтый
            RYG_SetColor(ryg, 0, 1, 0);
            delay_ms(50);
            LED_Toggle(&ryg->green);// Мигаем Зелёным при включеном Жёлтом
            break;
        case 2:  // Зелёный
            RYG_SetColor(ryg, 0, 0, 1);
            break;
        case 3:  // Красный + Жёлтый
            RYG_SetColor(ryg, 1, 1, 0);
            break;
        case 4:  // Жёлтый + Зелёный
            RYG_SetColor(ryg, 0, 1, 1);
            break;
        case 5:  // Красный + Зелёный
            RYG_SetColor(ryg, 1, 0, 1);
            break;
        case 6:  // Красный + Жёлтый + Зелёный
            RYG_SetColor(ryg, 1, 1, 1);
            break;
        case 7:  // потушили всё
            RYG_SetColor(ryg, 0, 0, 0);
            break;
    }
    
    color_state = (color_state + 1) % 8;

    delay_ms(ryg_ms);
}
//--------------------------------------------------------------------
int main(void) {
  GPIO_INIT();
  RYG_SetColor(&STM32G431_ryg, 1, 1, 1); // Красный + Жёлтый + Зелёный
  delay_ms(500);
  RYG_SetColor(&STM32G431_ryg, 0, 0, 0); // потушили всё
  delay_ms(500);

  while (1) {
  RYG_Blink(&STM32G431_ryg, 300);//
    }
}
/*************************** End of file ****************************/
