/********************************************************************************
* wdt.c: Innehåller drivrutiner för mikrokontrollerns Watchdog-timer.
********************************************************************************/
#include "wdt.h"

/********************************************************************************
* wdt_init: Initierar Watchdog-timern med angiven timeout mätt i millisekunder.
*
*           - timeout_ms: Timeout mätt i millisekunder.
********************************************************************************/
void wdt_init(const enum wdt_timeout timeout_ms)
{
   wdt_reset();
   asm("CLI");
   WDTCSR = (1 << WDCE) | (1 << WDE);
   WDTCSR = (1 << WDE) | (uint8_t)(timeout_ms);
   asm("SEI");
   return;
}

/********************************************************************************
* wdt_clear: Nollställer Watchdog-timern.
********************************************************************************/
void wdt_clear(void)
{
   wdt_reset();
   WDTCSR = 0x00;
   return;
}

/********************************************************************************
* wdt_reset: Återställer Watchdog-timern, vilket måste ske kontinuerligt innan
*            timern löper ut för att undvika systemåterställning eller avbrott.
********************************************************************************/
void wdt_reset(void)
{
   asm("CLI");
   asm("WDR");
   MCUSR &= ~(1 << WDRF);
   asm("SEI");
   return;
}

/********************************************************************************
* wdt_enable_system_reset: Aktiverar Watchdog-timern i System Reset Mode,
*                          vilket innebär att systemet återställs ifall
*                          Watchdog-timern löper ut.
********************************************************************************/
void wdt_enable_system_reset(void)
{
   wdt_reset();
   WDTCSR |= (1 << WDE);
}

/********************************************************************************
* wdt_disable_system_reset: Inaktiverar Watchdog-timern i System Reset Mode,
*                           vilket innebär att systemet inte återställs ifall
*                           Watchdog-timern löper ut.
********************************************************************************/
void wdt_disable_system_reset(void)
{
   wdt_reset();
   WDTCSR &= ~(1 << WDE);
   return;
}

/********************************************************************************
* wdt_enable_interrupt: Aktiverar Watchdog-timern i Interrupt Mode, vilket
*                       innebär att ett avbrott med avbrottsvektor WDT_vect
*                       äger rum ifall Watchdog-timern löper ut.
********************************************************************************/
void wdt_enable_interrupt(void)
{
   wdt_reset();
   WDTCSR |= (1 << WDIE);
   return;
}

/********************************************************************************
* wdt_disable_interrupt: Inaktiverar Watchdog-timern i Interrupt Mode, vilket
*                        innebär att avbrott inte äger rum ifall Watchdog-timern 
*                        löper ut.
********************************************************************************/
void wdt_disable_interrupts(void)
{
   wdt_reset();
   WDTCSR &= ~(1 << WDIE);
   return;
}