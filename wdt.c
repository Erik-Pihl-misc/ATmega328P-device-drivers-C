/********************************************************************************
* wdt.c: Inneh�ller drivrutiner f�r mikrokontrollerns Watchdog-timer.
********************************************************************************/
#include "wdt.h"

/********************************************************************************
* wdt_init: Initierar Watchdog-timern med angiven timeout m�tt i millisekunder.
*
*           - timeout_ms: Timeout m�tt i millisekunder.
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
* wdt_clear: Nollst�ller Watchdog-timern.
********************************************************************************/
void wdt_clear(void)
{
   wdt_reset();
   WDTCSR = 0x00;
   return;
}

/********************************************************************************
* wdt_reset: �terst�ller Watchdog-timern, vilket m�ste ske kontinuerligt innan
*            timern l�per ut f�r att undvika system�terst�llning eller avbrott.
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
*                          vilket inneb�r att systemet �terst�lls ifall
*                          Watchdog-timern l�per ut.
********************************************************************************/
void wdt_enable_system_reset(void)
{
   wdt_reset();
   WDTCSR |= (1 << WDE);
}

/********************************************************************************
* wdt_disable_system_reset: Inaktiverar Watchdog-timern i System Reset Mode,
*                           vilket inneb�r att systemet inte �terst�lls ifall
*                           Watchdog-timern l�per ut.
********************************************************************************/
void wdt_disable_system_reset(void)
{
   wdt_reset();
   WDTCSR &= ~(1 << WDE);
   return;
}

/********************************************************************************
* wdt_enable_interrupt: Aktiverar Watchdog-timern i Interrupt Mode, vilket
*                       inneb�r att ett avbrott med avbrottsvektor WDT_vect
*                       �ger rum ifall Watchdog-timern l�per ut.
********************************************************************************/
void wdt_enable_interrupt(void)
{
   wdt_reset();
   WDTCSR |= (1 << WDIE);
   return;
}

/********************************************************************************
* wdt_disable_interrupt: Inaktiverar Watchdog-timern i Interrupt Mode, vilket
*                        inneb�r att avbrott inte �ger rum ifall Watchdog-timern 
*                        l�per ut.
********************************************************************************/
void wdt_disable_interrupts(void)
{
   wdt_reset();
   WDTCSR &= ~(1 << WDIE);
   return;
}