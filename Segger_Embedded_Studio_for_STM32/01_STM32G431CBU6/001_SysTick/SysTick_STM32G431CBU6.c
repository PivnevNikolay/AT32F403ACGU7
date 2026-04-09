/**-------------------------------------------------------------------
 \date  16.03.2026
 *
 | ** SysTick                                                   ** |
 | ** Код написан в IDE SEGGER Embedded Studio for ARM 7.32a    ** |
 | ** J-Link Software and Documentation pack V7.90 [2023-08-02] ** |
 *
 *   STM32G431CBU6
 *   ------------
 *  |            |
 *  |            |
 *  |            |
 *  |        PC.6| ---->  LED _|¯|_  Period=100ms +Width=50ms -Width=50ms Freq=10.00 Hz
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

#define SYSTICK_LOAD (SystemCoreClock / 1000000)
#define SYSTICK_DELAY_CALIB (SYSTICK_LOAD >> 1)

__IO uint32_t SysTick_CNT = 0;

void SysTick_Init(void);
void DElay_mS(uint32_t mS);
void DELAY_US(uint32_t us);

void SysTick_Handler(void) {
  if (SysTick_CNT > 0)
    SysTick_CNT--;
}

void PortC_6_Init(void) {
  SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOCEN);                                                      // Запуск тактирования порта C
  MODIFY_REG(GPIOC->MODER, GPIO_MODER_MODE6, General_purpose_output_mode << GPIO_MODER_MODE6_Pos); // Настройка GPIOC пин 6 на выход (output mode)
  MODIFY_REG(GPIOC->OTYPER, GPIO_OTYPER_OT6, Output_push_pull << GPIO_OTYPER_OT6_Pos);             // Настройка GPIOC пин 6 в режим Push-Pull
  MODIFY_REG(GPIOC->OSPEEDR, GPIO_OSPEEDR_OSPEED6, High_speed << GPIO_OSPEEDR_OSPEED6_Pos);        // Настройка GPIOC пин 6 в режим High_speed
  MODIFY_REG(GPIOC->PUPDR, GPIO_PUPDR_PUPD6, No_pull_up_No_pull_down << GPIO_PUPDR_PUPD6_Pos);     // Настройка GPIOC пин 6 в режим No_pull_up_No_pull_down
}

int main(void) {
  PortC_6_Init();
  SysTick_Init();
  while (1) {
    SET_BIT(GPIOC->BSRR, GPIO_BSRR_BS6);
    DElay_mS(50);
    SET_BIT(GPIOC->BSRR, GPIO_BSRR_BR6);
    DElay_mS(50);
  }
}

void SysTick_Init(void) {
  CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk); // 0 --> Counter disabled;  1 --> Counter enabled
  CLEAR_BIT(SysTick->LOAD,SysTick_LOAD_RELOAD_Msk);  // 
  SysTick->LOAD = SystemCoreClock / 1000 - 1;   
  CLEAR_BIT(SysTick->VAL,SysTick_VAL_CURRENT_Msk);   // счетчик в 0
  SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk);//установим предделитель таймера
  SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);  //включает прерывание
  SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);   //включает таймер
}

void DElay_mS(uint32_t mS) {
  CLEAR_BIT(SysTick->VAL,SysTick_VAL_CURRENT_Msk);  // значения текущего счета в 0
  SysTick->VAL = SystemCoreClock / 1000 - 1;
  SysTick_CNT = mS;
  while (SysTick_CNT) {
  }
}

void DELAY_US(uint32_t us) {
 CLEAR_BIT(SysTick->VAL,SysTick_VAL_CURRENT_Msk);  // значения текущего счета в 0
  do {
    uint32_t start = SysTick->VAL;
    uint32_t ticks = (us * SYSTICK_LOAD) - SYSTICK_DELAY_CALIB;
    while ((start - SysTick->VAL) < ticks);
  } while (0);
}
/*************************** End of file ****************************/

/**-------------------------------------------------------------------
 \date  17.03.2026
 *
 *   STM32G431CBU6
 *   ------------
 *  |            |
 *  |            |
 *  |            |
 *  |        PC.6| ---->  LED _|¯|_  Period=150ms +Width=75ms -Width=75ms 
 *  |            |
 *  |            |
 *  |            |
 *  |      +3.3V |
 *  |        GND |
 *
 *\ authors        ScuratovaAnna
 *\ сode debugging ScuratovaAnna
 */
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

#define SYSTICK_LOAD (SystemCoreClock / 1000000)
#define SYSTICK_DELAY_CALIB (SYSTICK_LOAD >> 1)

__IO uint32_t SysTick_CNT = 0;

void SysTick_Init(void);
void delay(uint32_t ms);
uint32_t getCurrentMillis(void);

void SysTick_Handler(void) {
    SysTick_CNT++;
}

void PortC_6_Init(void) {
  SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOCEN);                                                      // Запуск тактирования порта C
  MODIFY_REG(GPIOC->MODER, GPIO_MODER_MODE6, General_purpose_output_mode << GPIO_MODER_MODE6_Pos); // Настройка GPIOC пин 6 на выход (output mode)
  MODIFY_REG(GPIOC->OTYPER, GPIO_OTYPER_OT6, Output_push_pull << GPIO_OTYPER_OT6_Pos);             // Настройка GPIOC пин 6 в режим Push-Pull
  MODIFY_REG(GPIOC->OSPEEDR, GPIO_OSPEEDR_OSPEED6, High_speed << GPIO_OSPEEDR_OSPEED6_Pos);        // Настройка GPIOC пин 6 в режим High_speed
  MODIFY_REG(GPIOC->PUPDR, GPIO_PUPDR_PUPD6, No_pull_up_No_pull_down << GPIO_PUPDR_PUPD6_Pos);     // Настройка GPIOC пин 6 в режим No_pull_up_No_pull_down
}

int main(void) {
  PortC_6_Init();
  SysTick_Init();
  while (1) {
    SET_BIT(GPIOC->BSRR, GPIO_BSRR_BS6);
    delay(75);
    SET_BIT(GPIOC->BSRR, GPIO_BSRR_BR6);
    delay(75);
  }
}

void SysTick_Init(void) {
  CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk); // 0 --> Counter disabled;  1 --> Counter enabled
  CLEAR_BIT(SysTick->LOAD,SysTick_LOAD_RELOAD_Msk);  //
  SysTick->LOAD = SystemCoreClock / 1000 - 1;   
  CLEAR_BIT(SysTick->VAL,SysTick_VAL_CURRENT_Msk);   // счетчик в 0
  SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk);//установим предделитель таймера
  SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);  //включает прерывание
  SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);   //включает таймер
}

void delay(uint32_t ms)
{
    if (ms != 0) {
        uint32_t start = getCurrentMillis();
        do {
            __NOP();
        } while (getCurrentMillis() - start < ms);
    }
}

uint32_t getCurrentMillis(void)
{
    return  SysTick_CNT;
}
/*************************** End of file ****************************/

/**-------------------------------------------------------------------
 \date  17.03.2026
 *
 *   STM32G431CBU6
 *   ------------
 *  |            |
 *  |            |
 *  |            |
 *  |        PC.6| ---->  LED _|¯|_  Period=200us +Width=100us -Width=100us 
 *  |            |
 *  |            |
 *  |            |
 *  |      +3.3V |
 *  |        GND |
 *
 *\ authors        ScuratovaAnna
 *\ сode debugging ScuratovaAnna
 */
//*************************** Пример третий **************************
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


__IO uint32_t SysTick_CNT = 0;

void SysTick_Init(void);
void delayMicroseconds(unsigned int us);

void SysTick_Handler(void) {
    SysTick_CNT++;
}

void PortC_6_Init(void) {
  SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOCEN);                                                      // Запуск тактирования порта C
  MODIFY_REG(GPIOC->MODER, GPIO_MODER_MODE6, General_purpose_output_mode << GPIO_MODER_MODE6_Pos); // Настройка GPIOC пин 6 на выход (output mode)
  MODIFY_REG(GPIOC->OTYPER, GPIO_OTYPER_OT6, Output_push_pull << GPIO_OTYPER_OT6_Pos);             // Настройка GPIOC пин 6 в режим Push-Pull
  MODIFY_REG(GPIOC->OSPEEDR, GPIO_OSPEEDR_OSPEED6, Very_high_speed << GPIO_OSPEEDR_OSPEED6_Pos);   // Настройка GPIOC пин 6 в режим High_speed
  MODIFY_REG(GPIOC->PUPDR, GPIO_PUPDR_PUPD6, No_pull_up_No_pull_down << GPIO_PUPDR_PUPD6_Pos);     // Настройка GPIOC пин 6 в режим No_pull_up_No_pull_down
}

int main(void) {
  PortC_6_Init();
  SysTick_Init();
  while (1) {
    SET_BIT(GPIOC->BSRR, GPIO_BSRR_BS6);
    delayMicroseconds(100);
    SET_BIT(GPIOC->BSRR, GPIO_BSRR_BR6);
    delayMicroseconds(100);
  }
}

void SysTick_Init(void) {
  CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk); // 0 --> Counter disabled;  1 --> Counter enabled
  CLEAR_BIT(SysTick->LOAD,SysTick_LOAD_RELOAD_Msk);  
  SysTick->LOAD = SystemCoreClock / 1000 - 1;   
  CLEAR_BIT(SysTick->VAL,SysTick_VAL_CURRENT_Msk);   // счетчик в 0
  SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk);//установим предделитель таймера
  SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);  //включает прерывание
  SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);   //включает таймер
}

/*Аналог функции delayMicroseconds().
  us: количество микросекунд, на которое приостанавливается выполнение программы. (unsigned int)
  Останавливает выполнение программы на заданное в параметре количество микросекунд (1 000 000 микросекунд в 1 секунде).*/
void delayMicroseconds(unsigned int us) {
 SysTick->VAL &= ~SysTick_VAL_CURRENT_Msk;
  __IO uint32_t currentTicks = SysTick->VAL;
  const uint32_t tickPerMs = SysTick->LOAD + 1;
  const uint32_t nbTicks = ((us - ((us > 0) ? 1 : 0)) * tickPerMs) / 1000;
  uint32_t elapsedTicks = 0;
  __IO uint32_t oldTicks = currentTicks;
  do {
    currentTicks = SysTick->VAL;
    elapsedTicks += (oldTicks < currentTicks) ? tickPerMs + oldTicks - currentTicks : oldTicks - currentTicks;
    oldTicks = currentTicks;
  } while (nbTicks > elapsedTicks);
}
/*************************** End of file ****************************/