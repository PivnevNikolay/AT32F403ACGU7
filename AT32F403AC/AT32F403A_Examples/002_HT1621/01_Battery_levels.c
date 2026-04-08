/**-------------------------------------------------------------------
 \date  10.06.2023
 *
 ***********************************************************************************************
 * В данном примере подключим, инициализируем дисплей и выведем три уровня заряда батареи на LCD.
 ***********************************************************************************************
 *
 *   AT32F403ACGT7          HT1624
 *   ------------         ------------
 *  |            |       |
 *  |            |       |
 *  |            |       |
 *  |        PA.0| ----> | CS
 *  |        PA.1| ----> | WR
 *  |        PA.2| ----> | Data
 *  |        PA.3| ----> | LED+ для работы данного вывода необходимо доработать плату экрана
 *  |            |       |
 *  |        +5V | <---> | Vcc
 *  |        GND | <---> | GND
 *
 * Код взят и адаптирован для AT32F403ACGT7 отсюда.
 * https://count-zero.ru/2022/ht1621/
 * LCD Arduino library https://github.com/valerionew/ht1621-7-seg
 * Видео про HT1621
 * https://www.youtube.com/watch?v=O1xRavRY38Y
 *
 * \ author PivnevNikolay 
 * \ сode debugging ScuratovaAnna
 */
#include "at32f403a_407_gpio.h"
#include "at32f403a_407_clock.h"
#include <stdio.h>

#define STEP_DELAY_MS                    (uint32_t)(50)
#define TICK_COUNT_MAX                   (uint32_t)(0xFFFFFF)
#define TICK_COUNT_VALUE                 (SysTick->VAL)

volatile uint32_t ticks_count_us;

#define BIAS    0x52
#define SYSDIS  0X00
#define SYSEN   0X02
#define LCDOFF  0X04
#define LCDON   0X06
#define XTAL    0x28
#define RC256   0X30
#define TONEON  0X12
#define TONEOFF 0X10
#define WDTDIS1 0X0A

uint8_t _buffer[7] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };

#define HT1621_CS_PORT GPIOA
#define HT1621_CS_PIN GPIO_PINS_0

#define HT1621_WR_PORT GPIOA
#define HT1621_WR_PIN GPIO_PINS_1

#define HT1621_DATA_PORT GPIOA
#define HT1621_DATA_PIN GPIO_PINS_2

#define HT1621_CS_LOW() gpio_bits_reset(HT1621_CS_PORT, HT1621_CS_PIN)             //gpio_bits_reset
#define HT1621_CS_HIGH() gpio_bits_set(HT1621_CS_PORT, HT1621_CS_PIN)              //gpio_bits_set

#define HT1621_WR_LOW() gpio_bits_reset(HT1621_WR_PORT, HT1621_WR_PIN)
#define HT1621_WR_HIGH() gpio_bits_set(HT1621_WR_PORT, HT1621_WR_PIN)

#define HT1621_DATA_LOW() gpio_bits_reset(HT1621_DATA_PORT, HT1621_DATA_PIN)
#define HT1621_DATA_HIGH() gpio_bits_set(HT1621_DATA_PORT, HT1621_DATA_PIN)
#define HT1621_DATA_OUT() gpio_bits_write(HT1621_DATA_PORT, HT1621_DATA_PIN, (bit))//gpio_bits_write

void ht1621_system_clock_config(void);
void ht1621_gpio_init(void);
void wrDATA(uint8_t data, uint8_t len);
void wrCMD(uint8_t CMD);
void Config_LCD(void);
void Clear(void);
void wrCLR(uint8_t len);
void wrclrdata(uint8_t addr, uint8_t sdata);
void update(void);
void wrone(uint8_t addr, uint8_t sdata);
void setBatteryLevel(int level);
void new_timebase_init(void);
void new_delay_ms(uint32_t delay);
void new_delay_us(uint32_t delay);

int main(void) {
	ht1621_system_clock_config();
	new_timebase_init();
	ht1621_gpio_init();
	Config_LCD();
	Clear();
	while (1) {
		setBatteryLevel(1);
		new_delay_ms(800);
		setBatteryLevel(2);
		new_delay_ms(800);
		setBatteryLevel(3);
		new_delay_ms(800);
		setBatteryLevel(0);
		new_delay_ms(800);
	}
}
//--------------------------------------------------------------------
void ht1621_gpio_init(void) {
	crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);

	gpio_init_type gpio_init_struct;
	gpio_default_para_init(&gpio_init_struct);

	gpio_bits_reset(GPIOA, GPIO_PINS_0 | GPIO_PINS_1 | GPIO_PINS_2);

	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pins = GPIO_PINS_0 | GPIO_PINS_1 | GPIO_PINS_2;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(GPIOA, &gpio_init_struct);
	__ASM volatile("NOP");
	__ASM volatile("NOP");
	__ASM volatile("NOP");
	// Исходное состояние: CS=1, WR=1, DATA=1
	HT1621_CS_HIGH();
	HT1621_WR_HIGH();
	HT1621_DATA_HIGH();
	__ASM volatile("NOP");
	__ASM volatile("NOP");
	__ASM volatile("NOP");
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
void ht1621_system_clock_config(void) {
//reset crm
	crm_reset();
//enable lick
	crm_clock_source_enable(CRM_CLOCK_SOURCE_LICK, TRUE);
//wait till lick is ready
	while (crm_flag_get(CRM_LICK_STABLE_FLAG) != SET) {
	}
// enable hext
	crm_clock_source_enable(CRM_CLOCK_SOURCE_HEXT, TRUE);
// wait till hext is ready
	while (crm_hext_stable_wait() == ERROR) {
	}
//enable hick
	crm_clock_source_enable(CRM_CLOCK_SOURCE_HICK, TRUE);
//wait till hick is ready
	while (crm_flag_get(CRM_HICK_STABLE_FLAG) != SET) {
	}
//config pll clock resource
	crm_pll_config(CRM_PLL_SOURCE_HEXT, CRM_PLL_MULT_2,
			CRM_PLL_OUTPUT_RANGE_LE72MHZ);
//enable pll
	crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);
//wait till pll is ready
	while (crm_flag_get(CRM_PLL_STABLE_FLAG) != SET) {
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
	while (crm_sysclk_switch_status_get() != CRM_SCLK_PLL) {
	}
//update system_core_clock global variable
	system_core_clock_update();
}
//--------------------------------------------------------------------
void wrDATA(uint8_t data, uint8_t len) {
	for (uint8_t i = 0; i < len; i++) {
		HT1621_WR_LOW();
		gpio_bits_write(HT1621_DATA_PORT, HT1621_DATA_PIN,((data & 0x80) ? SET : RESET));
		HT1621_WR_HIGH();
		data = (data << 1);
	}
}
//--------------------------------------------------------------------
void wrCMD(uint8_t CMD) {
	HT1621_CS_LOW();
	wrDATA(0x80, 4);
	wrDATA(CMD, 8);
	HT1621_CS_HIGH();
}
//--------------------------------------------------------------------
void Config_LCD(void) {
	wrCMD(BIAS);
	wrCMD(RC256);
	wrCMD(SYSDIS);
	wrCMD(WDTDIS1);
	wrCMD(SYSEN);
	wrCMD(LCDON);
}
//--------------------------------------------------------------------
void Clear(void) {
	wrCLR(16);
}

void wrCLR(uint8_t len) {
	uint8_t addr = 0;
	uint8_t i;
	for (i = 0; i < len; i++) {
		wrclrdata(addr, 0x00);
		addr = addr + 2;
	}
}
//--------------------------------------------------------------------
void wrclrdata(uint8_t addr, uint8_t sdata) {
	addr <<= 2;
	HT1621_CS_LOW();
	wrDATA(0xa0, 3);
	wrDATA(addr, 6);
	wrDATA(sdata, 8);
	HT1621_CS_HIGH();
}
//--------------------------------------------------------------------
void update(void) {
	wrone(0, _buffer[5]);
	wrone(2, _buffer[4]);
	wrone(4, _buffer[3]);
	wrone(6, _buffer[2]);
	wrone(8, _buffer[1]);
	wrone(10, _buffer[0]);
}
//--------------------------------------------------------------------
void wrone(uint8_t addr, uint8_t sdata) {
	addr <<= 2;
	HT1621_CS_LOW();
	wrDATA(0xa0, 3);
	wrDATA(addr, 6);
	wrDATA(sdata, 8);
	HT1621_CS_HIGH();
}
//--------------------------------------------------------------------
void setBatteryLevel(int level) {

	_buffer[0] &= 0x7F;
	_buffer[1] &= 0x7F;
	_buffer[2] &= 0x7F;

	switch (level) {
	case 3: // индикатор заряда батареи включен горят все 3 сегмента
		_buffer[0] |= 0x80;
		//break;
	case 2: // индикатор заряда батареи включен  горит 2 сегмента
		_buffer[1] |= 0x80;
		//break;
	case 1: // индикатор заряда батареи включен  горит 1 сегмент
		_buffer[2] |= 0x80;
		//break;
	case 0: // индикатор заряда батареи выключен
	default:
		break;
	}
	update();
}
//--------------------------------------------------------------------
void new_timebase_init(void) {
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
//--------------------------------------------------------------------
void new_delay_ms(uint32_t delay) {
	while (delay) {
		if (delay > STEP_DELAY_MS) {
			new_delay_us(STEP_DELAY_MS * 1000);
			delay -= STEP_DELAY_MS;
		} else {
			new_delay_us(delay * 1000);
			delay = 0;
		}
	}
}
//--------------------------------------------------------------------
void new_delay_us(uint32_t delay) {
	uint32_t delay_ticks, pre_ticks, cur_ticks, delta;
	delay_ticks = delay * ticks_count_us;

	pre_ticks = TICK_COUNT_VALUE;
	do {
		cur_ticks = TICK_COUNT_VALUE;
		/* count down */
		delta = (cur_ticks <= pre_ticks) ?
				(pre_ticks - cur_ticks) :
				((TICK_COUNT_MAX - cur_ticks) + pre_ticks + 1);
	} while (delta < delay_ticks);
}
//-----------------------  End of file  ------------------------------
