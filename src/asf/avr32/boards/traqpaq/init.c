/******************************************************************************
 *
 * Board initialization
 *
 * - Compiler:          GNU GCC for AVR32
 * - Supported devices: traq|paq hardware version 1.1
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
	// Clock Initialization (PM)
	// ------------------------------------------------------------
	#if(TRAQPAQ_HW_PLL_ENABLED)
		const pcl_freq_param_t pcl_freq_param = {
			.cpu_f			= APPL_CPU_SPEED,
			.pba_f			= APPL_PBA_SPEED,
			.osc0_f			= FOSC0,
			.osc0_startup	= OSC0_STARTUP
		};
	
		if( pm_configure_clocks(&pcl_freq_param) != PM_FREQ_STATUS_OK ){
			while(1){
				asm("nop");
			}
		}
	
		#else
		pm_switch_to_osc0(&AVR32_PM, FOSC0, OSC0_STARTUP);
	#endif
	
	
	// ------------------------------------------------------------
	// USB Initialization (PM)
	// ------------------------------------------------------------
	#if(TRAQPAQ_HW_USB_ENABLED)
		// Need to replace this with 'pm_configure_usb_clock(void)'
		pm_pll_setup(&AVR32_PM,
			1,  // pll.
			5,   // mul.  WAS 7 FOR FOSC0=12MHz
			1,   // div.
			0,   // osc.
			16); // lockcount.

		pm_pll_set_option(&AVR32_PM, 1, // pll.
			1,  // pll_freq: choose the range 80-180MHz.
			1,  // pll_div2.
			0); // pll_wbwdisable.

		pm_pll_enable(&AVR32_PM, 1);
		pm_wait_for_pll1_locked(&AVR32_PM);

		pm_gc_setup(&AVR32_PM, AVR32_PM_GCLK_USBB,  // gc.
			1,  // osc_or_pll: use Osc (if 0) or PLL (if 1).
			1,  // pll_osc: select Osc0/PLL0 or Osc1/PLL1.
			0,  // diven.
			0); // div.
		pm_gc_enable(&AVR32_PM, AVR32_PM_GCLK_USBB);
	#endif

	// ------------------------------------------------------------
	// Display Initialization (EBI)
	// ------------------------------------------------------------
	#if(TRAQPAQ_HW_EBI_ENABLED)
		smc_init(APPL_CPU_SPEED);
	#endif


	// ------------------------------------------------------------
	// GPIO Initialization
	// ------------------------------------------------------------
	//gpio_clr_gpio_pin(CHARGE_RATE);
	gpio_clr_gpio_pin(GPS_RESET);
	gpio_clr_gpio_pin(GPS_POWER_ON);
	gpio_clr_gpio_pin(PM_SHDN1);
	gpio_clr_gpio_pin(LCD_RESET);
	gpio_clr_gpio_pin(PM_ENABLE);
	//gpio_set_gpio_pin(PM_ENABLE);

	gpio_enable_pin_pull_up(CHARGE_STAT1);
	gpio_enable_pin_pull_up(CHARGE_STAT2);
	gpio_enable_pin_pull_up(CHARGE_PG);


	// ------------------------------------------------------------
	// GPS Initialization (USART)
	// ------------------------------------------------------------
	#if(TRAQPAQ_HW_GPS_ENABLED)
		const gpio_map_t GPS_USART_GPIO_MAP = {
			{GPS_TXD, GPS_TXD_FUNCTION},
			{GPS_RXD, GPS_RXD_FUNCTION}
		};

		// Options for USART0 Hardware
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
	// GPS Initialization (USART)
	// ------------------------------------------------------------
	#if(TRAQPAQ_HW_DEBUG_ENABLED)
		const gpio_map_t DEBUG_USART_GPIO_MAP = {
			{DEBUG_TXD, DEBUG_TXD_FUNCTION},
			{DEBUG_RXD, DEBUG_RXD_FUNCTION}
		};

		// Options for USART1 Hardware
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
	#if(TRAQPAQ_HW_EXINT_ENABLED)
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
	#endif


	// ------------------------------------------------------------
	// Dataflash Initialization (SPI)
	// ------------------------------------------------------------
	#if(TRAQPAQ_HW_SPI_ENABLED)
		const gpio_map_t SPI_GPIO_MAP = {
			{DATAFLASH_SPI_SCK_PIN,		DATAFLASH_SPI_SCK_FUNCTION  },  // SPI Clock
			{DATAFLASH_SPI_MISO_PIN,	DATAFLASH_SPI_MISO_FUNCTION },  // MISO
			{DATAFLASH_SPI_MOSI_PIN,	DATAFLASH_SPI_MOSI_FUNCTION },  // MOSI
			{DATAFLASH_SPI_NPCS0_PIN, 	DATAFLASH_SPI_NPCS0_FUNCTION},  // Chip Select
		};

		gpio_enable_module(SPI_GPIO_MAP, sizeof(SPI_GPIO_MAP) / sizeof(SPI_GPIO_MAP[0])); // Assign I/Os to SPI.

		//struct spi_device DATAFLASH_CS1 = {
		//	.id = DATAFLASH_SPI_NPCS
		//};
		
		spi_options_t dataflashOptions = {
			.reg		 = DATAFLASH_SPI_NPCS,
			.baudrate	 = SPI_BAUDRATE,
			.bits		 = SPI_BITS_PER_XFER,
			.spck_delay  = SPI_SCLK_DELAY,
			.trans_delay = SPI_XFER_DELAY,
			.stay_act    = SPI_STAY_ACTIVE,
			.spi_mode	 = SPI_MODE,
			.modfdis	 = SPI_FAULT_DETECT, 
		};

		//spi_master_init(DATAFLASH_SPI);
		//spi_master_setup_device(DATAFLASH_SPI, &DATAFLASH_CS1, SPI_MODE_0, SPI_BAUDRATE, 0);
		
		spi_initMaster(DATAFLASH_SPI, &dataflashOptions);
		spi_selectionMode(DATAFLASH_SPI, SPI_VARIABLE_PS, SPI_PCS_DECODE, SPI_SCLK_DELAY);
		spi_enable(DATAFLASH_SPI);
		spi_setupChipReg(DATAFLASH_SPI, &dataflashOptions, APPL_PBA_SPEED);
		
		static const pdca_channel_options_t pdcaSPItx = { 
			.addr = NULL,								// memory address 
			.pid = AVR32_PDCA_PID_SPI0_TX,				// select peripheral - SPI transfer channel 
			.size = NULL,								// transfer counter 
			.r_addr = NULL,								// next memory address 
			.r_size = NULL,								// next transfer counter 
			.transfer_size = PDCA_TRANSFER_SIZE_BYTE	// select size of the transfer 
		};
		
		pdca_init_channel(SPI_TX_PDCA_CHANNEL, &pdcaSPItx);
		
		static const pdca_channel_options_t pdcaSPIrx = { 
			.addr = NULL,								// memory address 
			.pid = AVR32_PDCA_PID_SPI0_RX,				// select peripheral - SPI transfer channel 
			.size = NULL,								// transfer counter 
			.r_addr = NULL,								// next memory address 
			.r_size = NULL,								// next transfer counter 
			.transfer_size = PDCA_TRANSFER_SIZE_BYTE	// select size of the transfer 
		};
		
		pdca_init_channel(SPI_RX_PDCA_CHANNEL, &pdcaSPIrx);
		
		
	#endif


	// ------------------------------------------------------------
	// Fuel Initialization (TWI)
	// ------------------------------------------------------------
	#if(TRAQPAQ_HW_TWI_ENABLED)
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
	#endif


	// ------------------------------------------------------------
	// Backlight Initialization (PWM)
	// ------------------------------------------------------------
	#if(TRAQPAQ_HW_PWM_ENABLED)
		volatile pwm_opt_t pwm_opt;
		volatile avr32_pwm_channel_t pwm_channel_0 = {.ccnt = BACKLIGHT_PWM_ID };

		gpio_enable_module_pin(BACKLIGHT_PWM_PIN, BACKLIGHT_PWM_FUNCTION);

		pwm_opt.diva = AVR32_PWM_DIVA_CLK_OFF;
		pwm_opt.divb = AVR32_PWM_DIVB_CLK_OFF;
		pwm_opt.prea = AVR32_PWM_PREA_MCK;
		pwm_opt.preb = AVR32_PWM_PREB_MCK;
		pwm_init(&pwm_opt);

		pwm_channel_0.CMR.calg = PWM_MODE_LEFT_ALIGNED;       	// Channel mode.
		pwm_channel_0.CMR.cpol = PWM_POLARITY_HIGH;            	// Channel polarity.
		pwm_channel_0.CMR.cpd = PWM_UPDATE_DUTY;              	// Not used the first time.
		pwm_channel_0.CMR.cpre = AVR32_PWM_CPRE_MCK;  			// Channel prescaler.
		pwm_channel_0.cdty = BACKLIGHT_PWM_MIN;   				// Channel duty cycle, should be < CPRD.
		pwm_channel_0.cprd = BACKLIGHT_PWM_MAX;  				// Channel period.
		pwm_channel_0.cupd = 0;   								// Channel update is not used here.

		//pwm_channel_init(STATUSLED_PWM_CHANNEL, &pwm_channel_0);
		pwm_channel_init(BACKLIGHT_PWM_CHANNEL, &pwm_channel_0);
		pwm_start_channels( 1 << BACKLIGHT_PWM_CHANNEL );
	#endif
	
	
	// ------------------------------------------------------------
	// Diagnostic Initialization (ADC)
	// ------------------------------------------------------------
	#if( TRAQPAQ_HW_ADC_ENABLED )
		static const gpio_map_t ADC_GPIO_MAP = {
			{ADC_VCC_PIN, ADC_VCC_FUNCTION},
			{ADC_VEE_PIN, ADC_VEE_FUNCTION},
			{ADC_3V3_PIN, ADC_3V3_FUNCTION},
		}
	
		gpio_enable_module(ADC_GPIO_MAP, sizeof(ADC_GPIO_MAP) / sizeof(ADC_GPIO_MAP[0]));
	
		// configure ADC
		// Lower the ADC clock to match the ADC characteristics (because we configured
		// the CPU clock to 12MHz, and the ADC clock characteristics are usually lower;
		// cf. the ADC Characteristic section in the datasheet).
		//AVR32_ADC.mr |= 0x1 << AVR32_ADC_MR_PRESCAL_OFFSET;
	
		adc_enable(ADC, ADC_VCC_CHANNEL);
		adc_enable(ADC, ADC_VEE_CHANNEL);
		adc_enable(ADC, ADC_3V3_CHANNEL);
	#endif

}