/**-------------------------------------------------------------------
 \date  16.03.2026
 *
 *   AT32F403ACGT7
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
//*************************** Пример первый **************************
#include "at32f403a_407_gpio.h"
#include "at32f403a_407_clock.h"

#define STEP_DELAY_MS                    (uint32_t)(50)
#define TICK_COUNT_MAX                   (uint32_t)(0xFFFFFF)
#define TICK_COUNT_VALUE                 (SysTick->VAL)

volatile uint32_t ticks_count_us;

void new_system_clock_config(void);
void new_timebase_init(void);
void new_gpio_config(void);
void new_delay_us(uint32_t delay);

int main(void)
{
  new_system_clock_config();
	new_timebase_init();
  new_gpio_config();
	void new_delay_ms(uint32_t delay);

  while(1)
  {
   gpio_bits_toggle(GPIOC,GPIO_PINS_13);
   new_delay_ms(20);
  }
}
/********************************************************************/
void new_gpio_config(void)
{
  crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);

  gpio_init_type gpio_init_struct;
  gpio_default_para_init(&gpio_init_struct);

  gpio_bits_reset(GPIOC, GPIO_PINS_13);

  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_13;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOC, &gpio_init_struct);
}
/********************************************************************/
/**
  *         system clock config program
  *         the system clock is configured as follow:
  *         system clock (sclk)   = hext * pll_mult
  *         system clock source   = HEXT_VALUE
  *         - hext                = HEXT_VALUE
  *         - sclk                = 16000000
  *         - ahbdiv              = 1
  *         - ahbclk              = 16000000
  *         - apb1div             = 2
  *         - apb1clk             = 8000000
  *         - apb2div             = 2
  *         - apb2clk             = 8000000
  *         - pll_mult            = 2
  *         - pll_range           = LE72MHZ (less than 72 mhz or equal to 72 mhz)
  */
void new_system_clock_config(void)
{
  /* reset crm */
  crm_reset();

  /* enable lick */
  crm_clock_source_enable(CRM_CLOCK_SOURCE_LICK, TRUE);

  /* wait till lick is ready */
  while(crm_flag_get(CRM_LICK_STABLE_FLAG) != SET)
  {
  }

  /* enable hext */
  crm_clock_source_enable(CRM_CLOCK_SOURCE_HEXT, TRUE);

  /* wait till hext is ready */
  while(crm_hext_stable_wait() == ERROR)
  {
  }

  /* enable hick */
  crm_clock_source_enable(CRM_CLOCK_SOURCE_HICK, TRUE);

  /* wait till hick is ready */
  while(crm_flag_get(CRM_HICK_STABLE_FLAG) != SET)
  {
  }

  /* config pll clock resource */
  crm_pll_config(CRM_PLL_SOURCE_HEXT, CRM_PLL_MULT_2, CRM_PLL_OUTPUT_RANGE_LE72MHZ);

  /* enable pll */
  crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);

  /* wait till pll is ready */
  while(crm_flag_get(CRM_PLL_STABLE_FLAG) != SET)
  {
  }

  /* config ahbclk */
  crm_ahb_div_set(CRM_AHB_DIV_1);

  /* config apb2clk, the maximum frequency of APB2 clock is 96 MHz  */
  crm_apb2_div_set(CRM_APB2_DIV_2);

  /* config apb1clk, the maximum frequency of APB1 clock is 96 MHz  */
  crm_apb1_div_set(CRM_APB1_DIV_2);

  /* select pll as system clock source */
  crm_sysclk_switch(CRM_SCLK_PLL);

  /* wait till pll is used as system clock source */
  while(crm_sysclk_switch_status_get() != CRM_SCLK_PLL)
  {
  }

  /* update system_core_clock global variable */
  system_core_clock_update();
}
/********************************************************************/
 void new_timebase_init(void)
{
  crm_clocks_freq_type crm_clocks;
  uint32_t frequency = 0;
  /* get crm_clocks */
  crm_clocks_freq_get(&crm_clocks);
  frequency = crm_clocks.ahb_freq / 8;
  /* config systick clock source */
  systick_clock_source_config(SYSTICK_CLOCK_SOURCE_AHBCLK_DIV8);
  ticks_count_us = (frequency / 1000000U);
  /* system tick config */
  TICK_COUNT_VALUE = 0UL;
  SysTick->LOAD = TICK_COUNT_MAX;
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}
/********************************************************************/
void new_delay_ms(uint32_t delay)
{
  while(delay)
  {
    if(delay > STEP_DELAY_MS)
    {
      new_delay_us(STEP_DELAY_MS * 1000);
      delay -= STEP_DELAY_MS;
    }
    else
    {
      new_delay_us(delay * 1000);
      delay = 0;
    }
  }
}
/********************************************************************/
void new_delay_us(uint32_t delay)
{
  uint32_t delay_ticks, pre_ticks, cur_ticks, delta;
  delay_ticks = delay * ticks_count_us;

  pre_ticks = TICK_COUNT_VALUE;
  do
  {
    cur_ticks = TICK_COUNT_VALUE;
    /* count down */
    delta = (cur_ticks <= pre_ticks) ? (pre_ticks - cur_ticks) : ((TICK_COUNT_MAX - cur_ticks) + pre_ticks + 1);
  } while(delta < delay_ticks);
}
/********************************************************************/