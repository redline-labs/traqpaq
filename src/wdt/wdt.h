/*
 * wdt.h
 *
 * Created: 11/20/2011 6:17:41 PM
 *  Author: Ryan
 */ 


#ifndef WDT_H_
#define WDT_H_

#define WATCHDOG_TIMEOUT_US			100000	// 100ms Watchdog Timeout
#define WATCHDOG_UPDATE_INTERVAL_MS	90		// 90ms Watchdog clear interval

void wdt_task_init(void);
void wdt_task(void);

unsigned char wdt_triggered(void);


#endif /* WDT_H_ */