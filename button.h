/********************************************************************************
* button.h: Inneh�ller drivrutiner f�r tryckknappar samt andra digitala inportar
*           via strukten button samt associerade funktioner. Drivrutinerna
*           innefattar avl�sning av insignaler samt avbrottsgenerering vid
*           logisk f�r�ndring av insignalen.
********************************************************************************/
#ifndef BUTTON_H_
#define BUTTON_H_

/* Inkluderingsdirektiv: */
#include "misc.h"

/********************************************************************************
* button: Strukt f�r implementering av tryckknappar och andra digitala inportar.
*         PCI-avbrott kan aktiveras p� aktuell pin. D�rmed f�r eventdetektering
*         implementeras av anv�ndaren, d� PCI-avbrott inte m�jligg�r kontroll
*         av vilken flank som avbrott ska ske p�.
********************************************************************************/
struct button
{
   uint8_t pin;              /* Tryckknappens pin-nummer p� aktuell I/O-port. */
   volatile uint8_t* pullup; /* Pekare till dataregister (f�r intern pullup-resistor). */
   volatile uint8_t* input;  /* Pekare till pinregister (f�r l�sning av insignaler). */
   volatile uint8_t* pcmsk;  /* Pekare till maskregister f�r aktivering av PCI-avbrott. */
   volatile uint8_t pcint;   /* Bit f�r aktivering av avbrottsvektor p� aktuell I/O-port. */
};

/********************************************************************************
* button_init: Initierar ny tryckknapp p� angiven pin.
*
*              - self: Pekare till tryckknappen som ska initieras.
*              - pin : Tryckknappens pin-nummer p� Arduino Uno, exempelvis 13.
*                      Alternativt kan motsvarande port-nummer p� ATmega328P
*                      anges, exempelvis B5 f�r pin 13 eller D3 f�r pin 3.
********************************************************************************/
void button_init(struct button* self,
                 const uint8_t pin);

/********************************************************************************
* button_clear: Nollst�ller tryckknapp samt motsvarande pin.
*
*               - self: Pekare till tryckknappen som ska nollst�llas.
********************************************************************************/
void button_clear(struct button* self);

/********************************************************************************
* button_is_pressed: L�ser av tryckknappens pin och indikerar ifall denna �r
*                    nedtryckt. I s� fall returneras true, annars false.
*
*                    - self: Pekare till tryckknappen som ska l�sas av.
********************************************************************************/
static inline bool button_is_pressed(const struct button* self)
{
   return (*(self->input) & (1 << self->pin));
}

/********************************************************************************
* button_interrupt_enabled: Indikerar ifall PCI-avbrott �r aktiverat f�r
*                           given tryckknapp.
*
*                           - self: Pekare till tryckknappen som avbrott
*                                   ska kontrolleras f�r.
********************************************************************************/
static inline bool button_interrupt_enabled(const struct button* self)
{
   return (*(self->pcmsk) & (1 << self->pin));
}

/********************************************************************************
* button_enable_interrupt: Aktiverar PCI-avbrott p� angiven tryckknapp s� att
*                          event p� tryckknappens pin medf�r avbrott, b�de p�
*                          stigande och fallande flank. Kontroll av flank m�ste
*                          sk�tas av anv�ndaren, f�rslagsvis via avl�sning i
*                          motsvarande avbrottsrutin.
*
*                          Nedan visas sambandet mellan anv�nd I/O-port samt
*                          avbrottsvektorn f�r motsvarande avbrottsrutin:
*
*                          I/O-port     pin (Arduino Uno)     Avbrottsvektor
*                             B              8 - 13             PCINT0_vect
*                             C             A0 - A5             PCINT1_vect
*                             D              0 - 7              PCINT2_vect
*
*                          - self: Pekare till tryckknappen som PCI-avbrott
*                                  ska aktiveras p�.
********************************************************************************/
void button_enable_interrupt(struct button* self);

/********************************************************************************
* button_disable_interrupt: Inaktiverar PCI-avbrott p� angiven tryckknapp.
*
*                           - self: Pekare till tryckknappen som PCI-avbrott
*                                  ska inaktiveras p�.
********************************************************************************/
static inline void button_disable_interrupt(struct button* self)
{
   *(self->pcmsk) &= ~(1 << self->pin);
   return;
}

/********************************************************************************
* button_toggle_interrupt: Togglar aktivering av PCI-avbrott p� angiven
*                          tryckknapp. Om avbrott �r aktiverat vid anrop
*                          s� sker inaktivering. P� samma s�tt g�ller att om
*                          avbrott �r inaktiverat vid anrop s� sker aktivering.
*
*                          - self: Pekare till tryckknappen som aktivering av
*                                  PCI-avbrott ska inaktiveras p�.
********************************************************************************/
void button_toggle_interrupt(struct button* self);

#endif /* BUTTON_H_ */