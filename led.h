/********************************************************************************
* led.h: Inneh�ller drivrutiner f�r lysdioder ocn andra digitala utportar via
*        strukten led samt associerade funktioner. 
********************************************************************************/
#ifndef LED_H_
#define LED_H_

/* Inkluderingsdirektiv: */
#include "misc.h"

/********************************************************************************
* led: Strukt f�r implementering av lysdioder och andra digitala utportar.
********************************************************************************/
struct led
{
   uint8_t pin;              /* Lysdiodens pin-nummer p� aktuell I/O-port. */
   volatile uint8_t* ddr;    /* Pekare till datariktningsregister. */
   volatile uint8_t* output; /* Pekare till dataregister (f�r t�ndning och sl�ckning). */
   volatile uint8_t* input;  /* Pekare till pinregister (f�r l�sning av insignaler). */
};

/********************************************************************************
* led_init: Initierar ny lysdiod p� angiven pin.
*
*           - self: Pekare till lysdioden som ska initieras.
*           - pin : Lysdiodens pin-nummer p� Arduino Uno, exempelvis 8.
*                   Alternativt kan motsvarande port-nummer p� ATmega328P
*                   anges, exempelvis B0 f�r pin 8 eller D2 f�r pin 2.
********************************************************************************/
void led_init(struct led* self,
              const uint8_t pin);

/********************************************************************************
* led_clear: Nollst�ller lysdiod samt motsvarande pin.
*
*            - self: Pekare till lysdioden som ska nollst�llas.
********************************************************************************/
void led_clear(struct led* self);

/********************************************************************************
* led_on: T�nder angiven lysdiod.
*
*         - self: Pekare till lysdioden som ska t�ndas.
********************************************************************************/
static inline void led_on(struct led* self)
{
   *(self->output) |= (1 << self->pin);
   return;
}

/********************************************************************************
* led_off: Sl�cker angiven lysdiod.
*
*          - self: Pekare till lysdioden som ska sl�ckas.
********************************************************************************/
static inline void led_off(struct led* self)
{
   (*self->output) &= ~(1 << self->pin);
   return;
}

/********************************************************************************
* led_toggle: Togglar utsignalen p� angiven lysdiod. Om lysdioden �r sl�ckt vid
*             anropet s� t�nds den. P� samma s�tt g�ller att om lysdioden �r
*             t�nd vid anropet s� sl�cks den.
*
*             - self: Pekare till lysdioden vars utsignal ska togglas.
********************************************************************************/
static inline void led_toggle(struct led* self)
{
   (*self->input) = (1 << self->pin);
   return;
}

/********************************************************************************
* led_enabled: Indikerar ifall angiven lysdiod �r t�nd.
*
*              - self: Pekare till lysdioden vars tillst�nd ska kontrolleras.
********************************************************************************/
static inline bool led_enabled(const struct led* self)
{
   return ((*self->input) & (1 << self->pin));
}


/********************************************************************************
* led_blink: Blinkar lysdiod en g�ng med angiven blinkhastighet.
*
*            - self          : Pekare till lysdioden som ska blinkas.
*            - blink_speed_ms: Blinkhastigheten m�tt i millisekunder.
********************************************************************************/
void led_blink(struct led* self,
               const uint16_t blink_speed_ms);

#endif /* LED_H_ */