/********************************************************************************
* setup.c: Innehåller initieringsrutiner.
********************************************************************************/
#include "header.h"

/* Globala objekt: */
struct led l1, l2, l3;
struct led_vector v1;
struct button b1;
struct timer t0, t1;
struct pwm pwm1;

/********************************************************************************
* setup: Initierar systemet enligt följande:
*
*        1. Deklarerar en array som lagrar adresser till lysdioder l1 - l3.
*           Vektorn v1 initieras med denna array samt antalet lysdioder.

*        2. Initierar lysdioder l1 - l3 anslutna till pin 8 - 10 (PORTB0 - PORTB2).
*
*        3. Initierar tryckknapp b1 ansluten till pin 13 (PORTB5) och 
*           aktiverar avbrott vid nedtryckning/uppsläppning. 
*           Avbrottsvektor för avbrottsrutinen är PCINT0_vect.
*
*        4. Initierar timer t0 till den 8-bitars timerkretsen Timer 0, som 
*           löper ut efter 300 millisekunder efter aktivering i Normal Mode. 
*           Avbrottsvektor för avbrottsrutinen är TIMER0_OVF_vect.
*
*        5. Initierar timer t1 till den 16-bitars timerkretsen Timer 1, som
*           löper ut efter 50 millisekunder efter aktivering i CTC Mode
*           (räknar upp till 256 och fungerar därmed som en 8-bitars timer).
*           Avbrottsvektor för avbrottsrutinen är TIMER0_COMPA_vect.
*
*        6. Initierar seriell överföring med en baud rate på 9600 kbps för
*           att möjliggöra utskrift till seriell terminal.
*
*        7. Skriver startvärdet 0 till adressen 100 i EEPROM-minnet. Denna
*           adress används för att lagra antalet passerade Watchdog timeouts.
*
*        8. Initierar Watchdog-timern med en timeout på 8192 ms. Avbrott
*           aktiveras så att timeout medför avbrott. Avbrottsvektorn för
*           motsvarande avbrottsrutin är WDT_vect.
*
*        9. Initierar PWM-kontroller pwm1 för PWM-styrning av lysdioderna med
*           en periodtid på 1000 mikrosekunder.
********************************************************************************/
void setup(void)
{
   static struct led* leds[] = { &l1, &l2, &l3};
   const size_t num_leds = sizeof(leds) / sizeof(struct led*);

   v1.leds = leds;
   v1.size = num_leds;

   led_init(&l1, 8);
   led_init(&l2, 9);
   led_init(&l3, 10);

   button_init(&b1, 13);
   button_enable_interrupt(&b1);

   timer_init(&t0, TIMER_SEL_0, 300);
   timer_init(&t1, TIMER_SEL_1, 50);

   serial_init(9600);
   eeprom_write_byte(TIMEOUT_ADDRESS, 0);

   wdt_init(WDT_TIMEOUT_8192_MS);
   wdt_enable_interrupt();

   pwm_init(&pwm1, A0, 1000, &v1, &led_vector_on, &led_vector_off);
   return;
}