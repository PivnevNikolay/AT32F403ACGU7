/**-------------------------------------------------------------------
 \date  20.03.2026
 *
 *   AT32F403ACGT7
 *   ------------
 *  |            |
 *  |            |
 *  |            |
 *  |       PC.13| ---->  LED
 *  |            |
 *  |        PA.0| <----  Button
 *  |            |
 *  |      +3.3V |
 *  |        GND |
 *
 * В качестве кнопки использую  touch button ttp223
 *
 *
 *\ authors        ScuratovaAnna
 *\ сode debugging ScuratovaAnna
 */
//--------------------------------------------------------------------
#include "at32f403a_407_gpio.h"
#include "at32f403a_407_clock.h"
#include "stdbool.h"

volatile bool condition = false;

void AT32_system_clock_config(void);
void AT32_gpio_config(void);

int main(void){
	AT32_system_clock_config();
	AT32_gpio_config();

 while(1){
	 
//-------------------------------------------------------------------- 
//condition = gpio_input_data_bit_read(GPIOA, GPIO_PINS_0);	 
//(condition == true) ? (gpio_bits_set(GPIOC, GPIO_PINS_13)) : (gpio_bits_reset(GPIOC, GPIO_PINS_13));
//--------------------------------------------------------------------	 
//condition = gpio_input_data_bit_read(GPIOA, GPIO_PINS_0); 
//gpio_bits_write(GPIOC, GPIO_PINS_13, condition);
//--------------------------------------------------------------------
	gpio_bits_write(GPIOC, GPIO_PINS_13, !gpio_input_data_bit_read(GPIOA, GPIO_PINS_0));
 }
	
}

//--------------------------------------------------------------------
//  @brief  system clock config program
//  @note   the system clock is configured as follow:
//         system clock (sclk)   = hext * pll_mult(pll_mult = 2) sclk = 16 MHZ
//         system clock source   = HEXT_VALUE
//         - hext                = HEXT_VALUE
//         - sclk                = 16000000
//         - ahbdiv              = 1
//         - ahbclk              = 16000000
//         - apb1div             = 2
//         - apb1clk             = 8000000
//         - apb2div             = 2
//         - apb2clk             = 8000000
//         - pll_mult            = 2
//         - pll_range           = LE72MHZ (less than 72 mhz or equal to 72 mhz)
//--------------------------------------------------------------------
void AT32_system_clock_config(void)
{
//reset crm
  crm_reset();
//enable lick
  crm_clock_source_enable(CRM_CLOCK_SOURCE_LICK, TRUE);
//wait till lick is ready
  while(crm_flag_get(CRM_LICK_STABLE_FLAG) != SET)
  {
  }
// enable hext
  crm_clock_source_enable(CRM_CLOCK_SOURCE_HEXT, TRUE);
// wait till hext is ready
  while(crm_hext_stable_wait() == ERROR)
  {
  }
//enable hick
  crm_clock_source_enable(CRM_CLOCK_SOURCE_HICK, TRUE);
//wait till hick is ready
  while(crm_flag_get(CRM_HICK_STABLE_FLAG) != SET)
  {
  }
//config pll clock resource
  crm_pll_config(CRM_PLL_SOURCE_HEXT, CRM_PLL_MULT_2, CRM_PLL_OUTPUT_RANGE_LE72MHZ);
//enable pll
  crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);
//wait till pll is ready
  while(crm_flag_get(CRM_PLL_STABLE_FLAG) != SET)
  {
  }
//config ahbclk
  crm_ahb_div_set(CRM_AHB_DIV_1);
//config apb2clk, the maximum frequency of APB2 clock is 96 MHz
  crm_apb2_div_set(CRM_APB2_DIV_2);
//config apb1clk, the maximum frequency of APB1 clock is 96 MHz
  crm_apb1_div_set(CRM_APB1_DIV_2);
//select pll as system clock source
  crm_sysclk_switch(CRM_SCLK_PLL);
//wait till pll is used as system clock source
  while(crm_sysclk_switch_status_get() != CRM_SCLK_PLL)
  {
  }
//update system_core_clock global variable
  system_core_clock_update();
}
//--------------------------------------------------------------------
void AT32_gpio_config(void)
{
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);
	
  gpio_init_type gpio_init_struct;
  gpio_default_para_init(&gpio_init_struct);
	
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_0;
  gpio_init_struct.gpio_pull = GPIO_PULL_UP;// <------ !!!
  gpio_init(GPIOA, &gpio_init_struct);

  gpio_bits_reset(GPIOC, GPIO_PINS_13);

  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_13;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOC, &gpio_init_struct);
}
//-----------------------  End of file  ------------------------------
