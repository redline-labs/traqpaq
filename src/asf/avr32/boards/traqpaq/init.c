/******************************************************************************
 *
 * Board initialization
 *
 * - Compiler:          GNU GCC for AVR32
 * - Supported devices: traq|paq hardware version 1.4
 * - AppNote:			N/A
 *
 * - Last Author:		Ryan David ( ryan.david@redline-electronics.com )
 *
 *
 * Copyright (c) 2012 Redline Electronics LLC.
 *
 * This file is part of traq|paq.
 *
 * traq|paq is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * traq|paq is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with traq|paq. If not, see http://www.gnu.org/licenses/.
 *
 ******************************************************************************/

#include <asf.h>
#include <board.h>
#include <conf_board.h>

void board_init(void){
	// ------------------------------------------------------------
	// Boot checks
	// ------------------------------------------------------------
	if( gpio_get_pin_value(DATAFLASH_WP) == PIN_HIGH ){
		// Stop boot, the flash is being accessed externally
		// Wait for power cycle
		while(TRUE);
	}
	
	// ------------------------------------------------------------
	// Clock Initialization
	// ------------------------------------------------------------
	sysclk_init();

	// ------------------------------------------------------------
	// Display Initialization (EBI)
	// ------------------------------------------------------------
	smc_init(APPL_HSB_SPEED);

	// ------------------------------------------------------------
	// CRC Initialization
	// ------------------------------------------------------------
	//init_crcccitt_tab();


	// ------------------------------------------------------------
	// GPIO Initialization
	// ------------------------------------------------------------
	gpio_clr_gpio_pin(GPS_RESET);			
	gpio_clr_gpio_pin(PM_SHDN1);			// Boost Converter
	gpio_clr_gpio_pin(LCD_RESET);
	gpio_clr_gpio_pin(ADC_VREF_EN);
	gpio_clr_gpio_pin(PM_ENABLE);			// Main Power Supply
	gpio_clr_gpio_pin(DATAFLASH_WP);		// Flash Write Protect
	gpio_clr_gpio_pin(CHARGE_RATE);			// Battery Charge Rate
	
	gpio_set_gpio_pin(DATAFLASH_HOLD);		// Flash Hold - TO BE OBSOLETED?

	gpio_enable_pin_pull_up(CHARGE_STAT1);
	gpio_enable_pin_pull_up(CHARGE_STAT2);
	gpio_enable_pin_pull_up(CHARGE_PG);
	

	// ------------------------------------------------------------
	// GPS Initialization (USART3)
	// ------------------------------------------------------------
	#if( TRAQPAQ_GPS_EXTERNAL_LOGGING == FALSE )
	const gpio_map_t GPS_USART_GPIO_MAP = {
		{GPS_TXD, GPS_TXD_FUNCTION},
		{GPS_RXD, GPS_RXD_FUNCTION}
	};

	// Options for USART3 Hardware
	const usart_options_t GPS_USART_OPTIONS = {
		.baudrate     = GPS_USART_BAUD,
		.charlength   = 8,
		.paritytype   = USART_NO_PARITY,
		.stopbits     = USART_1_STOPBIT,
		.channelmode  = USART_NORMAL_CHMODE
	};

	gpio_enable_module( GPS_USART_GPIO_MAP, sizeof( GPS_USART_GPIO_MAP ) / sizeof( GPS_USART_GPIO_MAP[0] ) ); // Assign GPIO to Debug USART.
	usart_init_rs232( GPS_USART, &GPS_USART_OPTIONS, APPL_PBA_SPEED ); // Initialize Debug USART in RS232 mode.

	#endif

	// ------------------------------------------------------------
	// Debug Initialization (USART2)
	// ------------------------------------------------------------
	#if( TRAQPAQ_DEBUG_ENABLED == TRUE )
		const gpio_map_t DEBUG_USART_GPIO_MAP = {
			{DEBUG_TXD, DEBUG_TXD_FUNCTION},
			{DEBUG_RXD, DEBUG_RXD_FUNCTION}
		};

		// Options for USART2 Hardware
		const usart_options_t DEBUG_USART_OPTIONS = {
			.baudrate     = DEBUG_USART_BAUD,
			.charlength   = 8,
			.paritytype   = USART_NO_PARITY,
			.stopbits     = USART_1_STOPBIT,
			.channelmode  = USART_NORMAL_CHMODE
		};

		gpio_enable_module(DEBUG_USART_GPIO_MAP, sizeof(DEBUG_USART_GPIO_MAP) / sizeof(DEBUG_USART_GPIO_MAP[0]));	// Assign GPIO to Debug USART.
		usart_init_rs232(DEBUG_USART, &DEBUG_USART_OPTIONS, APPL_PBA_SPEED);	// Initialize Debug USART in RS232 mode.
	#endif
	

	// ------------------------------------------------------------
	// Buttons Initialization (EXTINT)
	// ------------------------------------------------------------
	eic_options_t eic_options[4];
	
	eic_options[0].eic_mode   = EIC_MODE_EDGE_TRIGGERED;	// Enable edge-triggered interrupt.
	eic_options[0].eic_edge   = EIC_EDGE_RISING_EDGE;		// Interrupt will trigger on rising edge.
	eic_options[0].eic_async  = EIC_ASYNCH_MODE;			// Initialize in asynchronous mode
	eic_options[0].eic_filter = EIC_FILTER_ENABLED;			// Enable the glitch filter
	eic_options[0].eic_line   = EXTINT_BUTTON0;			// Set the interrupt line number.
	
	eic_options[1].eic_mode   = EIC_MODE_EDGE_TRIGGERED;	// Enable edge-triggered interrupt.
	eic_options[1].eic_edge   = EIC_EDGE_RISING_EDGE;		// Interrupt will trigger on rising edge.
	eic_options[1].eic_async  = EIC_ASYNCH_MODE;			// Initialize in asynchronous mode
	eic_options[1].eic_filter = EIC_FILTER_ENABLED;			// Enable the glitch filter
	eic_options[1].eic_line   = EXTINT_BUTTON1;			// Set the interrupt line number.
	
	eic_options[2].eic_mode	  = EIC_MODE_EDGE_TRIGGERED;	// Enable edge-triggered interrupt.
	eic_options[2].eic_edge   = EIC_EDGE_RISING_EDGE;		// Interrupt will trigger on rising edge.
	eic_options[2].eic_async  = EIC_ASYNCH_MODE;			// Initialize in asynchronous mode
	eic_options[2].eic_filter = EIC_FILTER_ENABLED;			// Enable the glitch filter
	eic_options[2].eic_line   = EXTINT_BUTTON2;			// Set the interrupt line number.
	
	eic_options[3].eic_mode   = EIC_MODE_EDGE_TRIGGERED;	// Enable edge-triggered interrupt.
	eic_options[3].eic_edge   = EIC_EDGE_RISING_EDGE;		// Interrupt will trigger on rising edge.
	eic_options[3].eic_async  = EIC_ASYNCH_MODE;			// Initialize in asynchronous mode
	eic_options[3].eic_filter = EIC_FILTER_ENABLED;			// Enable the glitch filter
	eic_options[3].eic_line   = EXTINT_BUTTON3;			// Set the interrupt line number.
	
	gpio_enable_module_pin(EXTINT_BUTTON0_PIN, EXTINT_BUTTON0_FUNCTION);
	gpio_enable_module_pin(EXTINT_BUTTON1_PIN, EXTINT_BUTTON1_FUNCTION);
	gpio_enable_module_pin(EXTINT_BUTTON2_PIN, EXTINT_BUTTON2_FUNCTION);
	gpio_enable_module_pin(EXTINT_BUTTON3_PIN, EXTINT_BUTTON3_FUNCTION);
	
	eic_init(&AVR32_EIC, &eic_options, EXTINT_NUMBER_LINES);
	
	eic_enable_line(&AVR32_EIC, eic_options[0].eic_line);
	eic_enable_line(&AVR32_EIC, eic_options[1].eic_line);
	eic_enable_line(&AVR32_EIC, eic_options[2].eic_line);
	eic_enable_line(&AVR32_EIC, eic_options[3].eic_line);

	eic_enable_interrupt_line(&AVR32_EIC, eic_options[0].eic_line);
	eic_enable_interrupt_line(&AVR32_EIC, eic_options[1].eic_line);
	eic_enable_interrupt_line(&AVR32_EIC, eic_options[2].eic_line);
	eic_enable_interrupt_line(&AVR32_EIC, eic_options[3].eic_line);


	// ------------------------------------------------------------
	// Flash Initialization (SPI)
	// ------------------------------------------------------------
	const gpio_map_t FLASH_SPI_GPIO_MAP = {
		{FLASH_SPI_SCK_PIN,		FLASH_SPI_SCK_FUNCTION  },  // SPI Clock
		{FLASH_SPI_MISO_PIN,	FLASH_SPI_MISO_FUNCTION },  // MISO
		{FLASH_SPI_MOSI_PIN,	FLASH_SPI_MOSI_FUNCTION },  // MOSI
		{FLASH_SPI_NPCS0_PIN, 	FLASH_SPI_NPCS0_FUNCTION},  // Chip Select
	};

	gpio_enable_module(FLASH_SPI_GPIO_MAP, sizeof(FLASH_SPI_GPIO_MAP) / sizeof(FLASH_SPI_GPIO_MAP[0])); // Assign I/Os to SPI.
		
	spi_options_t flashSpiOptions = {
		.reg		 = FLASH_SPI_NPCS,
		.baudrate	 = FLASH_SPI_BAUDRATE,
		.bits		 = FLASH_SPI_BITS_PER_XFER,
		.spck_delay  = FLASH_SPI_SCLK_DELAY,
		.trans_delay = FLASH_SPI_XFER_DELAY,
		.stay_act    = FLASH_SPI_STAY_ACTIVE,
		.spi_mode	 = FLASH_SPI_MODE,
		.modfdis	 = FLASH_SPI_FAULT_DETECT, 
	};

	spi_initMaster(FLASH_SPI, &flashSpiOptions);
	spi_selectionMode(FLASH_SPI, FLASH_SPI_VARIABLE_PS, FLASH_SPI_PCS_DECODE, FLASH_SPI_SCLK_DELAY);
	spi_enable(FLASH_SPI);
	spi_setupChipReg(FLASH_SPI, &flashSpiOptions, APPL_PBA_SPEED);
		
	static const pdca_channel_options_t pdcaFlashTx = { 
		.addr = NULL,								// memory address 
		.pid = AVR32_PDCA_PID_SPI0_TX,				// select peripheral - SPI transfer channel 
		.size = NULL,								// transfer counter 
		.r_addr = NULL,								// next memory address 
		.r_size = NULL,								// next transfer counter 
		.transfer_size = PDCA_TRANSFER_SIZE_BYTE	// select size of the transfer 
	};
		
	pdca_init_channel(FLASH_SPI_TX_PDCA_CHANNEL, &pdcaFlashTx);
	pdca_disable(FLASH_SPI_TX_PDCA_CHANNEL);
		
	static const pdca_channel_options_t pdcaFlashRx = { 
		.addr = NULL,								// memory address 
		.pid = AVR32_PDCA_PID_SPI0_RX,				// select peripheral - SPI transfer channel 
		.size = NULL,								// transfer counter 
		.r_addr = NULL,								// next memory address 
		.r_size = NULL,								// next transfer counter 
		.transfer_size = PDCA_TRANSFER_SIZE_BYTE	// select size of the transfer 
	};
		
	pdca_init_channel(FLASH_SPI_RX_PDCA_CHANNEL, &pdcaFlashRx);
	pdca_disable(FLASH_SPI_RX_PDCA_CHANNEL);
		
	
	// ------------------------------------------------------------
	// Accel Initialization (SPI)
	// ------------------------------------------------------------
	const gpio_map_t ACCEL_SPI_GPIO_MAP = {
		{ACCEL_SPI_SCK_PIN,		ACCEL_SPI_SCK_FUNCTION  },  // SPI Clock
		{ACCEL_SPI_MISO_PIN,	ACCEL_SPI_MISO_FUNCTION },  // MISO
		{ACCEL_SPI_MOSI_PIN,	ACCEL_SPI_MOSI_FUNCTION },  // MOSI
		{ACCEL_SPI_NPCS0_PIN, 	ACCEL_SPI_NPCS0_FUNCTION},  // Chip Select
	};

	gpio_enable_module(ACCEL_SPI_GPIO_MAP, sizeof(ACCEL_SPI_GPIO_MAP) / sizeof(ACCEL_SPI_GPIO_MAP[0])); // Assign I/Os to SPI.
	
	spi_options_t accelSpiOptions = {
		.reg		 = ACCEL_SPI_NPCS,
		.baudrate	 = ACCEL_SPI_BAUDRATE,
		.bits		 = ACCEL_SPI_BITS_PER_XFER,
		.spck_delay  = ACCEL_SPI_SCLK_DELAY,
		.trans_delay = ACCEL_SPI_XFER_DELAY,
		.stay_act    = ACCEL_SPI_STAY_ACTIVE,
		.spi_mode	 = ACCEL_SPI_MODE,
		.modfdis	 = ACCEL_SPI_FAULT_DETECT,
	};

	spi_initMaster(ACCEL_SPI, &accelSpiOptions);
	spi_selectionMode(ACCEL_SPI, ACCEL_SPI_VARIABLE_PS, ACCEL_SPI_PCS_DECODE, ACCEL_SPI_SCLK_DELAY);
	spi_enable(ACCEL_SPI);
	spi_setupChipReg(ACCEL_SPI, &accelSpiOptions, APPL_PBA_SPEED);
	
	static const pdca_channel_options_t pdcaAccelTx = {
		.addr = NULL,								// memory address
		.pid = AVR32_PDCA_PID_SPI1_TX,				// select peripheral - SPI transfer channel
		.size = NULL,								// transfer counter
		.r_addr = NULL,								// next memory address
		.r_size = NULL,								// next transfer counter
		.transfer_size = PDCA_TRANSFER_SIZE_BYTE	// select size of the transfer
	};
	
	pdca_init_channel(ACCEL_SPI_TX_PDCA_CHANNEL, &pdcaAccelTx);
	pdca_disable(ACCEL_SPI_TX_PDCA_CHANNEL);
	
	static const pdca_channel_options_t pdcaAccelRx = {
		.addr = NULL,								// memory address
		.pid = AVR32_PDCA_PID_SPI1_RX,				// select peripheral - SPI transfer channel
		.size = NULL,								// transfer counter
		.r_addr = NULL,								// next memory address
		.r_size = NULL,								// next transfer counter
		.transfer_size = PDCA_TRANSFER_SIZE_BYTE	// select size of the transfer
	};
	
	pdca_init_channel(ACCEL_SPI_RX_PDCA_CHANNEL, &pdcaAccelRx);
	

	// ------------------------------------------------------------
	// Fuel Initialization (TWI)
	// ------------------------------------------------------------
	const gpio_map_t TWI_GPIO_MAP = {
		{AVR32_TWI_SDA_0_0_PIN, AVR32_TWI_SDA_0_0_FUNCTION},
		{AVR32_TWI_SCL_0_0_PIN, AVR32_TWI_SCL_0_0_FUNCTION}
	};
	gpio_enable_module(TWI_GPIO_MAP, sizeof(TWI_GPIO_MAP) / sizeof(TWI_GPIO_MAP[0]));

	const twi_options_t TWI_OPTIONS = {
		.pba_hz = APPL_PBA_SPEED,
		.speed 	= FUEL_TWI_BAUDRATE,
		.chip 	= FUEL_ADDRESS,
	};
	twi_master_init(&AVR32_TWI, &TWI_OPTIONS);


	// ------------------------------------------------------------
	// Backlight Initialization (PWM)
	// ------------------------------------------------------------
	volatile pwm_opt_t pwm_opt;
	volatile avr32_pwm_channel_t pwm_channel = {.ccnt = BACKLIGHT_PWM_ID };

	gpio_enable_module_pin(BACKLIGHT_PWM_PIN, BACKLIGHT_PWM_FUNCTION);

	pwm_opt.diva = AVR32_PWM_DIVA_CLK_OFF;
	pwm_opt.divb = AVR32_PWM_DIVB_CLK_OFF;
	pwm_opt.prea = AVR32_PWM_PREA_MCK;
	pwm_opt.preb = AVR32_PWM_PREB_MCK;
	pwm_init(&pwm_opt);

	pwm_channel.CMR.calg = PWM_MODE_LEFT_ALIGNED;       	// Channel mode.
	pwm_channel.CMR.cpol = PWM_POLARITY_HIGH;            	// Channel polarity.
	pwm_channel.CMR.cpd = PWM_UPDATE_DUTY;              	// Not used the first time.
	pwm_channel.CMR.cpre = AVR32_PWM_CPRE_MCK;  			// Channel prescaler.
	pwm_channel.cdty = BACKLIGHT_PWM_MIN;   				// Channel duty cycle, should be < CPRD.
	pwm_channel.cprd = BACKLIGHT_PWM_MAX;  				// Channel period.
	pwm_channel.cupd = 0;   								// Channel update is not used here.

	pwm_channel_init(BACKLIGHT_PWM_CHANNEL, &pwm_channel);
	pwm_start_channels( 1 << BACKLIGHT_PWM_CHANNEL );
	
	
	// ------------------------------------------------------------
	// Diagnostic Initialization (ADC)
	// ------------------------------------------------------------
	static const gpio_map_t ADC_GPIO_MAP = {
		{ADC_VCC_PIN, ADC_VCC_FUNCTION},
		{ADC_VEE_PIN, ADC_VEE_FUNCTION},
		{ADC_3V3_PIN, ADC_3V3_FUNCTION},
	};
	
	gpio_enable_module(ADC_GPIO_MAP, sizeof(ADC_GPIO_MAP) / sizeof(ADC_GPIO_MAP[0]));
	AVR32_ADC.mr |= 0x1 << AVR32_ADC_MR_PRESCAL_OFFSET;
	adc_configure(ADC);
	
	adc_enable(ADC, ADC_3V3_CHANNEL);
	adc_enable(ADC, ADC_VCC_CHANNEL);
	adc_enable(ADC, ADC_VEE_CHANNEL);
}

int board_changeBaud(volatile avr32_usart_t *usart, unsigned int baudrate){
	unsigned int over = (APPL_PBA_SPEED >= 16 * baudrate) ? 16 : 8;
	unsigned int cd_fp = ((1 << AVR32_USART_BRGR_FP_SIZE) * APPL_PBA_SPEED + (over * baudrate) / 2) / (over * baudrate);
	unsigned int cd = cd_fp >> AVR32_USART_BRGR_FP_SIZE;
	unsigned int fp = cd_fp & ((1 << AVR32_USART_BRGR_FP_SIZE) - 1);

	if (cd < 1 || cd > (1 << AVR32_USART_BRGR_CD_SIZE) - 1)
	return USART_INVALID_INPUT;

	/*usart->mr = (usart->mr & ~(AVR32_USART_MR_USCLKS_MASK |
	AVR32_USART_MR_SYNC_MASK |
	AVR32_USART_MR_OVER_MASK)) |
	AVR32_USART_MR_USCLKS_MCK << AVR32_USART_MR_USCLKS_OFFSET |
	((over == 16) ? AVR32_USART_MR_OVER_X16 : AVR32_USART_MR_OVER_X8) << AVR32_USART_MR_OVER_OFFSET;*/

	usart->brgr = cd << AVR32_USART_BRGR_CD_OFFSET |
				  fp << AVR32_USART_BRGR_FP_OFFSET;

	return USART_SUCCESS;
}