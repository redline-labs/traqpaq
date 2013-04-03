/*
 * board_macros.h
 *
 * Created: 4/3/2013 4:31:06 PM
 *  Author: Ryan
 */ 


#ifndef BOARD_MACROS_H_
#define BOARD_MACROS_H_

#if(TRAQPAQ_DEBUG_ENABLED == TRUE)
	#define debug_clr_pin0()			gpio_clr_gpio_pin(GPIO_DEBUG0);
	#define debug_set_pin0()			gpio_set_gpio_pin(GPIO_DEBUG0);
	#define debug_tgl_pin0()			gpio_tgl_gpio_pin(GPIO_DEBUG0);

	#define debug_clr_pin1()			gpio_clr_gpio_pin(GPIO_DEBUG1);
	#define debug_set_pin1()			gpio_set_gpio_pin(GPIO_DEBUG1);
	#define debug_tgl_pin1()			gpio_tgl_gpio_pin(GPIO_DEBUG1);

	#define debug_clr_pin2()			gpio_clr_gpio_pin(GPIO_DEBUG2);
	#define debug_set_pin2()			gpio_set_gpio_pin(GPIO_DEBUG2);
	#define debug_tgl_pin2()			gpio_tgl_gpio_pin(GPIO_DEBUG2);

	#define debug_clr_pin3()			gpio_clr_gpio_pin(GPIO_DEBUG3);
	#define debug_set_pin3()			gpio_set_gpio_pin(GPIO_DEBUG3);
	#define debug_tgl_pin3()			gpio_tgl_gpio_pin(GPIO_DEBUG3);
#else
	#define debug_clr_pin0()			
	#define debug_set_pin0()
	#define debug_tgl_pin0()

	#define debug_clr_pin1()
	#define debug_set_pin1()
	#define debug_tgl_pin1()

	#define debug_clr_pin2()
	#define debug_set_pin2()
	#define debug_tgl_pin2()

	#define debug_clr_pin3()
	#define debug_set_pin3()
	#define debug_tgl_pin3()
#endif


#endif /* BOARD_MACROS_H_ */