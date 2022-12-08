/********************************************************************************
* isr.c: Inneh�ller avbrottsrutiner.
********************************************************************************/
#include "header.h"

/********************************************************************************
* ISR (PCINT0_vect): Avbrottsrutin som �ger rum vid nedtryckning/uppsl�ppning
*                    av tryckknapp b1 ansluten till pin 13 (PORTB5).
*                    Vid nedtryckning �terst�lls Watchdog-timern, vilket
*                    skrivs ut i ansluten seriell terminal. D�remot vid
*                    uppsl�ppning g�rs ingenting.
*
*                    Oavsett vad som orsakade avbrottet inaktiveras PCI-avbrott
*                    p� I/O-port B i 300 millisekunder via timer 0 f�r att
*                    undvika multipla avbrott orsakade av kontaktstudsar.
********************************************************************************/
ISR (PCINT0_vect)
{
   disable_pin_change_interrupt(IO_PORTB);
   timer_enable_interrupt(&t0);

   if (button_is_pressed(&b1))
   {
      wdt_reset();
      serial_print_string("Watchdog timer reset!\n");
   }

   return;
}

/********************************************************************************
* ISR (TIMER0_OVF_vect): Avbrottsrutin som �ger rum vid overflow av Timer 0,
*                        dvs. uppr�kning till 256, vilket sker var 0.128:e
*                        millisekund n�r timern �r aktiverad.
*
*                        Timern r�knas upp via uppr�kning av varje passerat
*                        avbrott. N�r timern l�per ut (n�r ber�knat antal
*                        avbrott f�r specificerad tid har r�knats upp) s�
*                        �teraktiveras PCI-avbrott p� I/O-port B (som har
*                        st�ngts av i 300 millisekunder f�r att undvika
*                        multipla avbrott orsakat av kontaktstudsar), f�ljt
*                        av att timern st�ngs av.
********************************************************************************/
ISR (TIMER0_OVF_vect)
{
   timer_count(&t0);

   if (timer_elapsed(&t0))
   {
      enable_pin_change_interrupt(IO_PORTB);
      timer_disable_interrupt(&t0);
   }

   return;
}

/********************************************************************************
* ISR (TIMER1_COMPA_vect): Avbrottsrutin som �ger rum vid uppr�kning till 256 av
*                          Timer 1 i CTC Mode, vilket sker var 0.128:e
*                          millisekund n�r timern �r aktiverad.
*
*                          Timern r�knas upp via uppr�kning av varje passerat
*                          avbrott. N�r timern l�per ut (var 50:e millisekund
*                          n�r timern �r aktiverad) togglas lysdiod l1.
********************************************************************************/
ISR (TIMER1_COMPA_vect)
{
   timer_count(&t1);

   if (timer_elapsed(&t1))
   {
      led_toggle(&l1);
   }

   return;
}

/********************************************************************************
* ISR (WDT_vect): Avbrottsrutin som �ger rum vid Watchdog timeout, vilket sker
*                 om Watchdog-timern inte blir �ters�lld var 1024:e millisekund.
*                 Antalet timeouts r�knas upp och skrivs ut i ansluten seriell
*                 terminal. N�r maximalt antal timeouts har genomf�rts l�ses
*                 systemet i ett tillst�nd d�r lysdioden ansluten till pin 8
*                 (PORTB0) blinkar var 50:e millisekund.
********************************************************************************/
ISR (WDT_vect)
{
   static volatile bool system_lockdown = false;

   if (!system_lockdown)
   {
      uint8_t num_timeouts = eeprom_read_byte(TIMEOUT_ADDRESS);

      serial_print_string("Number of timeouts: ");
      serial_print_unsigned(++num_timeouts);
      serial_print_new_line();

      if (num_timeouts >= TIMEOUT_MAX)
      {
         system_lockdown = true;
         serial_print_string("Maximum number of timeouts has elapsed!\n");
         serial_print_string("System lockdown!\n");

         button_clear(&b1);
         timer_clear(&t0);
         pwm_disable(&pwm1);
         timer_enable_interrupt(&t1);
      }
      else
      {
         eeprom_write_byte(TIMEOUT_ADDRESS, num_timeouts);
      }
   }

   wdt_enable_interrupt();
   return;
}
