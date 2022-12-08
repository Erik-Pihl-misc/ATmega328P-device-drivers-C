/********************************************************************************
* main.c: Demonstration av inbyggt system innefattande PWM-styrning av
*         lysdioder anslutna till pin 8 - 10 (PORTB0 - PORTB2) via en 
*         potentiometer ansluten till analog pin A0 (PORTB). Maximalt fem 
*         Watchdog-timeouts (sker efter 8192 ms utan Watchdog reset) f�r ske 
*         innan systemet l�ses.
*
*         F�r att genomg�ra Watchdog reset kan anv�ndaren trycka p� en
*         tryckknapp ansluten till pin 13 (PORTB5). Efter fem timeouts l�ses
*         systemet, d�r det enda som sker �r att en lysdiod ansluten till
*         pin 8 (PORTB0) blinkar var 50:e millisekund via Timer 1. 
*
*         Utskrift sker via seriell �verf�ring efter varje Watchdog timeout, 
*         vid Watchdog reset samt vid l�sning av systemet. F�r att undvika
*         multipla avbrott orsakat av kontaktstudsar inaktiveras PCI-avbrott
*         p� I/O-port B i 300 millisekunder efter nedtryckning, implementerat
*         via Timer 0.
********************************************************************************/
#include "header.h"

/********************************************************************************
* main: Initierar systemet vid start. Watchdog timeout sker sedan kontinuerligt
*       var 8192:e millisekund om inte anv�ndaren under denna tid �terst�ller 
*       Watchdog-timern, vilket �stadkommes genom att trycka ned tryckknapp
*       b1 ansluten till pin 13 (PORTB5). Efter fem timeouts l�ses systemet.
*       Lysdiod l1 ansluten till pin 8 (PORTB0) kommer d� kontinuerligt blinka
*       var 50:e millisekund tills en total system�terst�llning genomf�rs.
*       �vrig tid sker PWM-styrning av lysdioder l1 - l3 anslutna till pin
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

