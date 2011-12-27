/**
 * \file
 *
 * \brief User board initialization template
 *
 */
#include <asf.h>
#include <board.h>
#include <conf_board.h>

void board_init(void){
	
	// ------------------------------------------------------------
	// Clock Initialization
	// ------------------------------------------------------------
	#if(TRAQPAQ_HW_PLL_ENABLED)
	const pcl_freq_param_t pcl_freq_param = {
		.cpu_f			= APPL_CPU_SPEED,
		.pba_f			= APPL_PBA_SPEED,
		.osc0_f			= FOSC0,
		.osc0_startup	= OSC0_STARTUP
	};
	
	if( pm_configure_clocks(&pcl_freq_param) != PM_FREQ_STATUS_OK ){
		while(1);
	}
	
	#else
	pm_switch_to_osc0(&AVR32_PM, FOSC0, OSC0_STARTUP);
	#endif
	
	#if(TRAQPAQ_HW_USB_ENABLED)
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
	// EBI Initialization
	// ------------------------------------------------------------
	#if(TRAQPAQ_HW_EBI_ENABLED)
	smc_init(APPL_CPU_SPEED);
	#endif




	// ------------------------------------------------------------
	// GPIO Initialization
	// ------------------------------------------------------------
	gpio_clr_gpio_pin(CHARGE_RATE);
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
	// USART0 Initialization
	// ------------------------------------------------------------
	// GPIO for USART0 Hardware
	#if(TRAQPAQ_HW_USART_ENABLED)
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
	// EXTINT for Buttons
	// ------------------------------------------------------------
	#if(TRAQPAQ_HW_EXINT_ENABLED)
	
	eic_options_t eic_options[5];
	
	eic_options[0].eic_mode =	AVR32_EIC_EDGE_IRQ; 
	eic_options[0].eic_edge =	AVR32_EIC_RISING_EDGE; 
	eic_options[0].eic_filter =	AVR32_EIC_FILTER_OFF; 
	eic_options[0].eic_async =	AVR32_EIC_SYNC; 
	eic_options[0].eic_line =	EXTINT_BUTTON0;
	
	eic_options[1].eic_mode =	AVR32_EIC_EDGE_IRQ; 
	eic_options[1].eic_edge =	AVR32_EIC_RISING_EDGE; 
	eic_options[1].eic_filter =	AVR32_EIC_FILTER_OFF; 
	eic_options[1].eic_async =	AVR32_EIC_SYNC; 
	eic_options[1].eic_line =	EXTINT_BUTTON1;
	
	eic_options[2].eic_mode =	AVR32_EIC_EDGE_IRQ; 
	eic_options[2].eic_edge =	AVR32_EIC_RISING_EDGE; 
	eic_options[2].eic_filter =	AVR32_EIC_FILTER_OFF; 
	eic_options[2].eic_async =	AVR32_EIC_SYNC; 
	eic_options[2].eic_line =	EXTINT_BUTTON2;
	
	eic_options[3].eic_mode =	AVR32_EIC_EDGE_IRQ; 
	eic_options[3].eic_edge =	AVR32_EIC_RISING_EDGE; 
	eic_options[3].eic_filter =	AVR32_EIC_FILTER_OFF; 
	eic_options[3].eic_async =	AVR32_EIC_SYNC; 
	eic_options[3].eic_line =	EXTINT_BUTTON3;

	
	static const gpio_map_t EXT_INT_GPIO_MAP = {
		{EXTINT_BUTTON0_PIN, EXTINT_BUTTON0_FUNCTION},		// Button 0
		{EXTINT_BUTTON1_PIN, EXTINT_BUTTON1_FUNCTION},		// Button 1
		{EXTINT_BUTTON2_PIN, EXTINT_BUTTON2_FUNCTION},		// Button 2
		{EXTINT_BUTTON3_PIN, EXTINT_BUTTON3_FUNCTION},		// Button 3
	}; 
	
	gpio_enable_module(EXT_INT_GPIO_MAP, sizeof(EXT_INT_GPIO_MAP)/sizeof(EXT_INT_GPIO_MAP[0]) );
	
	eic_init(&AVR32_EIC, &eic_options, EXTINT_NUMBER_LINES);

	eic_enable_lines(&AVR32_EIC, (1<<eic_options[3].eic_line) |
								 (1<<eic_options[2].eic_line) |
								 (1<<eic_options[1].eic_line) |
								 (1<<eic_options[0].eic_line));
								 
	eic_enable_interrupt_lines(&AVR32_EIC, (1<<eic_options[3].eic_line) |
										   (1<<eic_options[2].eic_line) |
										   (1<<eic_options[1].eic_line) |
										   (1<<eic_options[0].eic_line));
	
	#endif
	
	#if(TRAQPAQ_HW_DEBUG_ENABLED)
	#warning Hardware Debug is turned on!
	// ------------------------------------------------------------
	// USART1 for Debug Interface
	// ------------------------------------------------------------
	// GPIO for USART1 Hardware
	static const gpio_map_t DEBUG_USART_GPIO_MAP = {
		{DEBUG_TXD, DEBUG_TXD_FUNCTION},
		{DEBUG_RXD, DEBUG_RXD_FUNCTION}
	};

	// Options for USART1 Hardware
	static const usart_options_t DEBUG_USART_OPTIONS = {
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
	// SPI1 Initialization
	// ------------------------------------------------------------
	// GPIO for SPI1 Hardware
	#if(TRAQPAQ_HW_SPI_ENABLED)
		
	const gpio_map_t SPI_GPIO_MAP = {
		{DATAFLASH_SPI_SCK_PIN,		DATAFLASH_SPI_SCK_FUNCTION  },  // SPI Clock
		{DATAFLASH_SPI_MISO_PIN,	DATAFLASH_SPI_MISO_FUNCTION },  // MISO
		{DATAFLASH_SPI_MOSI_PIN,	DATAFLASH_SPI_MOSI_FUNCTION },  // MOSI
		{DATAFLASH_SPI_NPCS0_PIN, 	DATAFLASH_SPI_NPCS0_FUNCTION},  // Chip Select
	};

	gpio_enable_module(SPI_GPIO_MAP, sizeof(SPI_GPIO_MAP) / sizeof(SPI_GPIO_MAP[0])); // Assign I/Os to SPI.

	struct spi_device DATAFLASH_CS1 = {
		.id = DATAFLASH_SPI_NPCS
	};

	spi_master_init(DATAFLASH_SPI);

	spi_master_setup_device(DATAFLASH_SPI, &DATAFLASH_CS1, SPI_MODE_0, SPI_BAUDRATE, 0);
	
	spi_enable(DATAFLASH_SPI);
	
	#endif

	// ------------------------------------------------------------
	// TWI Initialization
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
	// USB Initialization
	// ------------------------------------------------------------


	// ------------------------------------------------------------
	// PWM for LEDs
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

}