//https://www.chipdip.ru/product/weh002004alpp5n00100-winstar-8008869258?from=suggest_product
#include "stm32g4xx.h"
#include <stdio.h>
#include <string.h>

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

#define EN_1 SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS1);  //gpio_bit_set   --> 1
#define EN_0 SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR1);  //gpio_bit_reset --> 0
#define RS_1 SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS0);  //gpio_bit_set   --> 1
#define RS_0 SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR0);  //gpio_bit_reset --> 0

#define SYSTICK_LOAD (SystemCoreClock / 1000000)
#define SYSTICK_DELAY_CALIB (SYSTICK_LOAD >> 1)

uint8_t bell[] = { 0x4, 0xe, 0xe, 0xe, 0x1f, 0x0, 0x4 };
uint8_t note[] = { 0x2, 0x3, 0x2, 0xe, 0x1e, 0xc, 0x0 };
uint8_t clock[] = { 0x0, 0xe, 0x15, 0x17, 0x11, 0xe, 0x0 };
uint8_t heart[] = { 0x0, 0xa, 0x1f, 0x1f, 0xe, 0x4, 0x0 };
uint8_t duck[] = { 0x0, 0xc, 0x1d, 0xf, 0xf, 0x6, 0x0 };
uint8_t check[] = { 0x0, 0x1, 0x3, 0x16, 0x1c, 0x8, 0x0 };
uint8_t cross[] = { 0x11, 0x1B, 0x0E, 0x04, 0x04, 0x0E, 0x1B, 0x11 };
uint8_t retarrow[] = { 0x01, 0x01, 0x01, 0x05, 0x09, 0x1F, 0x08, 0x04 };
uint8_t smile[] = { 0x00, 0x1B, 0x1B, 0x00, 0x04, 0x11, 0x0E, 0x00 };
uint8_t sadness[] = { 0x00, 0x1B, 0x1B, 0x00, 0x04, 0x00, 0x0E, 0x11 };

uint8_t battery_0[] = { 0x0E, 0x1F, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1F };
uint8_t battery_1[] = { 0x0E, 0x1F, 0x11, 0x11, 0x11, 0x11, 0x1F, 0x1F };
uint8_t battery_2[] = { 0x0E, 0x1F, 0x11, 0x11, 0x11, 0x1F, 0x1F, 0x1F };
uint8_t battery_3[] = { 0x0E, 0x1F, 0x11, 0x11, 0x1F, 0x1F, 0x1F, 0x1F };
uint8_t battery_4[] = { 0x0E, 0x1F, 0x11, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F };
uint8_t battery_5[] = { 0x0E, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F };


__IO uint32_t SysTick_CNT = 0;

void SysTick_Init(void);
void delay_1ms(uint32_t mS);
void delay_1us(uint32_t us);
void lcd_pulse_enable(void);
void lcd_send_data(uint8_t data);
void lcd_com(uint8_t cmd);
void lcd_init_4bit(void);
void lcd_SetPos(uint8_t x, uint8_t y);
void lcd_print(const char *s);
void lcd_string(char *p, uint8_t x, uint8_t y);
void createChar(uint8_t location, uint8_t charmap[]);
void LCD_PrintNumber(int num);
void cleaning_lcd(void);

void SysTick_Handler(void) {
  if (SysTick_CNT > 0)
    SysTick_CNT--;
}

void GPIO_INIT(void) {

  SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOAEN);  // Запуск тактирования порта A

  MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODE0, General_purpose_output_mode << GPIO_MODER_MODE0_Pos);  // Настройка GPIOA пин 0 на выход (output mode)
  MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODE1, General_purpose_output_mode << GPIO_MODER_MODE1_Pos);  // Настройка GPIOA пин 1 на выход (output mode)
  MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODE2, General_purpose_output_mode << GPIO_MODER_MODE2_Pos);  // Настройка GPIOA пин 2 на выход (output mode)
  MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODE3, General_purpose_output_mode << GPIO_MODER_MODE3_Pos);  // Настройка GPIOA пин 3 на выход (output mode)
  MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODE4, General_purpose_output_mode << GPIO_MODER_MODE4_Pos);  // Настройка GPIOA пин 4 на выход (output mode)
  MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODE5, General_purpose_output_mode << GPIO_MODER_MODE5_Pos);  // Настройка GPIOA пин 5 на выход (output mode)

  MODIFY_REG(GPIOA->OTYPER, GPIO_OTYPER_OT0, Output_push_pull << GPIO_OTYPER_OT0_Pos);  // Настройка GPIOA пин 0 в режим Push-Pull
  MODIFY_REG(GPIOA->OTYPER, GPIO_OTYPER_OT1, Output_push_pull << GPIO_OTYPER_OT1_Pos);  // Настройка GPIOA пин 1 в режим Push-Pull
  MODIFY_REG(GPIOA->OTYPER, GPIO_OTYPER_OT2, Output_push_pull << GPIO_OTYPER_OT2_Pos);  // Настройка GPIOA пин 2 в режим Push-Pull
  MODIFY_REG(GPIOA->OTYPER, GPIO_OTYPER_OT3, Output_push_pull << GPIO_OTYPER_OT3_Pos);  // Настройка GPIOA пин 3 в режим Push-Pull
  MODIFY_REG(GPIOA->OTYPER, GPIO_OTYPER_OT4, Output_push_pull << GPIO_OTYPER_OT4_Pos);  // Настройка GPIOA пин 4 в режим Push-Pull
  MODIFY_REG(GPIOA->OTYPER, GPIO_OTYPER_OT5, Output_push_pull << GPIO_OTYPER_OT5_Pos);  // Настройка GPIOA пин 5 в режим Push-Pull

  MODIFY_REG(GPIOA->OSPEEDR, GPIO_OSPEEDR_OSPEED0, High_speed << GPIO_OSPEEDR_OSPEED0_Pos);  // Настройка GPIOA пин 0 в режим High_speed
  MODIFY_REG(GPIOA->OSPEEDR, GPIO_OSPEEDR_OSPEED1, High_speed << GPIO_OSPEEDR_OSPEED1_Pos);  // Настройка GPIOA пин 1 в режим High_speed
  MODIFY_REG(GPIOA->OSPEEDR, GPIO_OSPEEDR_OSPEED2, High_speed << GPIO_OSPEEDR_OSPEED2_Pos);  // Настройка GPIOA пин 2 в режим High_speed
  MODIFY_REG(GPIOA->OSPEEDR, GPIO_OSPEEDR_OSPEED3, High_speed << GPIO_OSPEEDR_OSPEED3_Pos);  // Настройка GPIOA пин 3 в режим High_speed
  MODIFY_REG(GPIOA->OSPEEDR, GPIO_OSPEEDR_OSPEED4, High_speed << GPIO_OSPEEDR_OSPEED4_Pos);  // Настройка GPIOA пин 4 в режим High_speed
  MODIFY_REG(GPIOA->OSPEEDR, GPIO_OSPEEDR_OSPEED5, High_speed << GPIO_OSPEEDR_OSPEED5_Pos);  // Настройка GPIOA пин 5 в режим High_speed

  MODIFY_REG(GPIOA->PUPDR, GPIO_PUPDR_PUPD0, No_pull_up_No_pull_down << GPIO_PUPDR_PUPD0_Pos);  // Настройка GPIOA пин 0 в режим No_pull_up_No_pull_down
  MODIFY_REG(GPIOA->PUPDR, GPIO_PUPDR_PUPD1, No_pull_up_No_pull_down << GPIO_PUPDR_PUPD1_Pos);  // Настройка GPIOA пин 1 в режим No_pull_up_No_pull_down
  MODIFY_REG(GPIOA->PUPDR, GPIO_PUPDR_PUPD2, No_pull_up_No_pull_down << GPIO_PUPDR_PUPD2_Pos);  // Настройка GPIOA пин 2 в режим No_pull_up_No_pull_down
  MODIFY_REG(GPIOA->PUPDR, GPIO_PUPDR_PUPD3, No_pull_up_No_pull_down << GPIO_PUPDR_PUPD3_Pos);  // Настройка GPIOA пин 3 в режим No_pull_up_No_pull_down
  MODIFY_REG(GPIOA->PUPDR, GPIO_PUPDR_PUPD4, No_pull_up_No_pull_down << GPIO_PUPDR_PUPD4_Pos);  // Настройка GPIOA пин 4 в режим No_pull_up_No_pull_down
  MODIFY_REG(GPIOA->PUPDR, GPIO_PUPDR_PUPD5, No_pull_up_No_pull_down << GPIO_PUPDR_PUPD5_Pos);  // Настройка GPIOA пин 5 в режим No_pull_up_No_pull_down

  SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR0);
  SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR1);
  SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR2);
  SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR3);
  SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR4);
  SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR5);
}

int main(void) {


  GPIO_INIT();
  SysTick_Init();
  lcd_init_4bit();
  //lcd_SetPos(1, 0);
  // lcd_print("Hello LCD");
  lcd_init_4bit();
  createChar(0, bell);
  createChar(1, note);
  createChar(2, clock);
  createChar(3, heart);
  createChar(4, sadness);
  createChar(5, smile);
  createChar(6, cross);
  createChar(7, retarrow);
  lcd_SetPos(0, 0);
  lcd_send_data(0);
  lcd_send_data(1);
  lcd_send_data(2);
  lcd_send_data(3);
  lcd_send_data(4);
  lcd_send_data(5);
  lcd_send_data(6);
  lcd_send_data(7);
  delay_1ms(5000);
  cleaning_lcd();
  createChar(0, battery_0);
  createChar(1, battery_1);
  createChar(2, battery_2);
  createChar(3, battery_3);
  createChar(4, battery_4);
  createChar(5, battery_5);
  while (1) {
    for (int i = 0; i < 6; i++) {
      lcd_SetPos(0, 0);
      lcd_send_data(i);
      lcd_SetPos(0, 1);
      lcd_print("battery:");
      LCD_PrintNumber(i * 20);
      lcd_print("%");
      delay_1ms(1500);
      if (i == 5)
        cleaning_lcd();
    }
  }
}

void SysTick_Init(void) {
  CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);  // 0 --> Counter disabled;  1 --> Counter enabled
  CLEAR_BIT(SysTick->LOAD, SysTick_LOAD_RELOAD_Msk);  //
  SysTick->LOAD = SystemCoreClock / 1000 - 1;
  CLEAR_BIT(SysTick->VAL, SysTick_VAL_CURRENT_Msk);    // счетчик в 0
  SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk);  //установим предделитель таймера
  SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);    //включает прерывание
  SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);     //включает таймер
}

void delay_1ms(uint32_t mS) {
  CLEAR_BIT(SysTick->VAL, SysTick_VAL_CURRENT_Msk);  // значения текущего счета в 0
  SysTick->VAL = SystemCoreClock / 1000 - 1;
  SysTick_CNT = mS;
  while (SysTick_CNT) {
  }
}

void delay_1us(uint32_t us) {
  CLEAR_BIT(SysTick->VAL, SysTick_VAL_CURRENT_Msk);  // значения текущего счета в 0
  do {
    uint32_t start = SysTick->VAL;
    uint32_t ticks = (us * SYSTICK_LOAD) - SYSTICK_DELAY_CALIB;
    while ((start - SysTick->VAL) < ticks)
      ;
  } while (0);
}

void lcd_pulse_enable(void) {
  EN_1;
  delay_1us(1);
  EN_0;
  delay_1us(100);
}
/******************************************************************************
 *                                                                             *
 *******************************************************************************/
void lcd_send_data(uint8_t data) {
  RS_1;                                                                                           // RS = 1 (данные)
  (data & 0x10) ? (SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS2)) : (SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR2));  //LCD_D4 GPIO_PIN_2
  (data & 0x20) ? (SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS3)) : (SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR3));  //LCD_D5 GPIO_PIN_3
  (data & 0x40) ? (SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS4)) : (SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR4));  //LCD_D6 GPIO_PIN_4
  (data & 0x80) ? (SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS5)) : (SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR5));  //LCD_D7 GPIO_PIN_5
  lcd_pulse_enable();
  // Младшие 4 бита
  (data & 0x01) ? (SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS2)) : (SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR2));  //LCD_D4 GPIO_PIN_2
  (data & 0x02) ? (SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS3)) : (SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR3));  //LCD_D5 GPIO_PIN_3
  (data & 0x04) ? (SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS4)) : (SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR4));  //LCD_D6 GPIO_PIN_4
  (data & 0x08) ? (SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS5)) : (SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR5));  //LCD_D7 GPIO_PIN_5
  lcd_pulse_enable();
}
/******************************************************************************
 *                  Функция записи команды в LCD                               *
 *******************************************************************************/
void lcd_com(uint8_t cmd) {
  RS_0;                                                                                          // RS = 0 (команда)
  (cmd & 0x10) ? (SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS2)) : (SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR2));  //LCD_D4 GPIO_PIN_2
  (cmd & 0x20) ? (SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS3)) : (SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR3));  //LCD_D5 GPIO_PIN_3
  (cmd & 0x40) ? (SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS4)) : (SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR4));  //LCD_D6 GPIO_PIN_4
  (cmd & 0x80) ? (SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS5)) : (SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR5));  //LCD_D7 GPIO_PIN_5
  lcd_pulse_enable();
  // Младшие 4 бита
  (cmd & 0x01) ? (SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS2)) : (SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR2));  //LCD_D4 GPIO_PIN_2
  (cmd & 0x02) ? (SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS3)) : (SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR3));  //LCD_D5 GPIO_PIN_3
  (cmd & 0x04) ? (SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS4)) : (SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR4));  //LCD_D6 GPIO_PIN_4
  (cmd & 0x08) ? (SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS5)) : (SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR5));  //LCD_D7 GPIO_PIN_5
  lcd_pulse_enable();
}



/******************************************************************************
 *                   Функция инициализации LCD                                 *
 *******************************************************************************/
void lcd_init_4bit(void) {
  delay_1ms(10);  // Ожидание после подачи питания
  // Шаг 1: Установка 4-битного режима
  lcd_com(0x33);  // Переход в 8-битный режим (временный)
  lcd_com(0x32);  // Переход в 4-битный режим
  // Шаг 2: Основные настройки
  lcd_com(0x28);  // 4-бит, 2 строки, шрифт 5x8
  lcd_com(0x0C);  // Дисплей включен, курсор выключен
  lcd_com(0x06);  // Автоинкремент адреса
  lcd_com(0x01);  // Очистка дисплея
  delay_1ms(2);   // Ожидание завершения очистки
}
/******************************************************************************
 *                   Функция установки позиции курсора                         *
 *******************************************************************************/
void lcd_SetPos(uint8_t x, uint8_t y) {
  switch (y) {
    case 0:
      lcd_com(x | 0x80);
      delay_1ms(1);
      break;
    case 1:
      lcd_com((0x40 + x) | 0x80);
      delay_1ms(1);
      break;
    case 2:
      lcd_com((0x14 + x) | 0x80);
      delay_1ms(1);
      break;
    case 3:
      lcd_com((0x54 + x) | 0x80);
      delay_1ms(1);
      break;
  }
}
/******************************************************************************
 *                   Функция вывода строки на экран LCD                        *
 *******************************************************************************/
void lcd_print(const char *s) {
  unsigned int len = strlen(s);
  unsigned int i = 0;
  for (i = 0; i < len; i++) {
    lcd_send_data(s[i]);
  }
}
/******************************************************************************
 *         Функция вывода строка на экран LCD с заданными координатами         *
 *******************************************************************************/
void lcd_string(char *p, uint8_t x, uint8_t y)  // Вывести строку на дисплей X,Y
/// lcd_string("HI BABY",4,1) ; выведется сторка по конкретным координатам
{
  lcd_SetPos(x, y);
  while (*p) {
    lcd_send_data(*p++);
  }
}

void createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7;  // we only have 8 locations 0-7
  lcd_com(0x40 | (location << 3));
  for (int i = 0; i < 8; i++) {
    lcd_send_data(charmap[i]);
  }
}

void LCD_PrintNumber(int num) {
  char buffer[16];
  snprintf(buffer, sizeof(buffer), "%d", num);
  lcd_print(buffer);
}
/******************************************************************************
 *                  Функция очистки экрана LCD                                 *
 *******************************************************************************/
void cleaning_lcd(void) {
  lcd_com(0x01);
  delay_1ms(2);
}
/******************************* End of file ***********************************/