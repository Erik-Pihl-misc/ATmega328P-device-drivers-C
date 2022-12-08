/********************************************************************************
* main.c: Demonstration av inbyggt system innefattande PWM-styrning av
*         lysdioder anslutna till pin 8 - 10 (PORTB0 - PORTB2) via en 
*         potentiometer ansluten till analog pin A0 (PORTB). Maximalt fem 
*         Watchdog-timeouts (sker efter 8192 ms utan Watchdog reset) får ske 
*         innan systemet låses.
*
*         För att genomgöra Watchdog reset kan användaren trycka på en
*         tryckknapp ansluten till pin 13 (PORTB5). Efter fem timeouts låses
*         systemet, där det enda som sker är att en lysdiod ansluten till
*         pin 8 (PORTB0) blinkar var 50:e millisekund via Timer 1. 
*
*         Utskrift sker via seriell överföring efter varje Watchdog timeout, 
*         vid Watchdog reset samt vid låsning av systemet. För att undvika
*         multipla avbrott orsakat av kontaktstudsar inaktiveras PCI-avbrott
*         på I/O-port B i 300 millisekunder efter nedtryckning, implementerat
*         via Timer 0.
********************************************************************************/
#include "header.h"

/********************************************************************************
* main: Initierar systemet vid start. Watchdog timeout sker sedan kontinuerligt
*       var 8192:e millisekund om inte användaren under denna tid återställer 
*       Watchdog-timern, vilket åstadkommes genom att trycka ned tryckknapp
*       b1 ansluten till pin 13 (PORTB5). Efter fem timeouts låses systemet.
*       Lysdiod l1 ansluten till pin 8 (PORTB0) kommer då kontinuerligt blinka
*       var 50:e millisekund tills en total systemåterställning genomförs.
*       Övrig tid sker PWM-styrning av lysdioder l1 - l3 anslutna till pin
*       8 - 10 (PORTB0 - PORTB2) via en potentiometer ansluten till analog
*       pin A0 (PORTC0).
********************************************************************************/
int main(void)
{
   setup();
   
   while (1)
   {
      pwm_run(&pwm1);
   }

   return 0;
}

