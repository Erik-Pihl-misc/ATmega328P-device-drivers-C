/********************************************************************************
* header.h: Inneh�llande diverse definitioner samt deklarationer f�r
*           implementering av det inbyggda systemet.
********************************************************************************/
#ifndef HEADER_H_
#define HEADER_H_

/* Inkluderingsdirektiv: */
#include "led.h"
#include "button.h"
#include "timer.h"
#include "serial.h"
#include "eeprom.h"
#include "wdt.h"
#include "pwm.h"
#include "led_vector.h"

/* Makrodefinitioner: */
#define TIMEOUT_ADDRESS 100 /* Lagrar antalet passerade Watchdog timeouts. */
#define TIMEOUT_MAX 5       /* Maximalt antal timeouts innan programmet l�ses. */

/* Deklaration av globala objekt: */
extern struct led l1, l2, l3;
extern struct led_vector v1;
extern struct button b1;
extern struct timer t0, t1;
extern struct pwm pwm1;

/********************************************************************************
* setup: Initierar systemet enligt f�ljande:
*
*        1. Deklarerar en array som lagrar adresser till lysdioder l1 - l3.
*           Vektorn v1 initieras med denna array samt antalet lysdioder.

*        2. Initierar lysdioder l1 - l3 anslutna till pin 8 - 10 (PORTB0 - PORTB2).
*
*        3. Initierar tryckknapp b1 ansluten till pin 13 (PORTB5) och
*           aktiverar avbrott vid nedtryckning/uppsl�ppning.
*           Avbrottsvektor f�r avbrottsrutinen �r PCINT0_vect.
*
*        4. Initierar timer t0 till den 8-bitars timerkretsen Timer 0, som
*           l�per ut efter 300 millisekunder efter aktivering i Normal Mode.
*           Avbrottsvektor f�r avbrottsrutinen �r TIMER0_OVF_vect.
*
*        5. Initierar timer t1 till den 16-bitars timerkretsen Timer 1, som
*           l�per ut efter 50 millisekunder efter aktivering i CTC Mode
*           (r�knar upp till 256 och fungerar d�rmed som en 8-bitars timer).
*           Avbrottsvektor f�r avbrottsrutinen �r TIMER0_COMPA_vect.
*
*        6. Initierar seriell �verf�ring med en baud rate p� 9600 kbps f�r
*           att m�jligg�ra utskrift till seriell terminal.
*
*        7. Skriver startv�rdet 0 till adressen 100 i EEPROM-minnet. Denna
*           adress anv�nds f�r att lagra antalet passerade Watchdog timeouts.
*
*        8. Initierar Watchdog-timern med en timeout p� 8192 ms. Avbrott
*           aktiveras s� att timeout medf�r avbrott. Avbrottsvektorn f�r
*           motsvarande avbrottsrutin �r WDT_vect.
*
*        9. Initierar PWM-kontroller pwm1 f�r PWM-styrning av lysdioderna med
*           en periodtid p� 1000 mikrosekunder.
********************************************************************************/
void setup(void);

#endif /* HEADER_H_ */