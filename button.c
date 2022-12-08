/********************************************************************************
* button.h: Inneh�ller funktionsdefinitioner f�r enkel implementering av
*           tryckknappar samt andra digitala inportar via strukten button.
********************************************************************************/
#include "button.h"

/********************************************************************************
* button_init: Initierar ny tryckknapp p� angiven pin.
*
*              - self: Pekare till tryckknappen som ska initieras.
*              - pin : Tryckknappens pin-nummer p� Arduino Uno, exempelvis 13.
*                      Alternativt kan motsvarande port-nummer p� ATmega328P
*                      anges, exempelvis B5 f�r pin 13 eller D3 f�r pin 3.
********************************************************************************/
void button_init(struct button* self,
                 const uint8_t pin)
{
   if (pin >= 0 && pin <= 7)
   {
      self->pin = pin;
      self->pullup = &PORTD;
      self->input = &PIND;
      self->pcmsk = &PCMSK2;
      self->pcint = PCINT2;
   }
   else if (pin >= 8 && pin <= 13)
   {
      self->pin = pin - 8;
      self->pullup = &PORTB;
      self->input = &PINB;
      self->pcmsk = &PCMSK0;
      self->pcint = PCINT0;
   }
   else if (pin >= 14 && pin <= 19)
   {
      self->pin = pin - 14;
      self->pullup = &PORTC;
      self->input = &PINC;
      self->pcmsk = &PCMSK1;
      self->pcint = PCINT1;
   }

   (*self->pullup) |= (1 << self->pin);
   return;
}

/********************************************************************************
* button_clear: Nollst�ller tryckknapp samt motsvarande pin.
*
*               - self: Pekare till tryckknappen som ska nollst�llas.
********************************************************************************/
void button_clear(struct button* self)
{
   button_disable_interrupt(self);
   (*self->pullup) &= ~(1 << self->pin);

   self->pin = 0;
   self->pullup = 0;
   self->input = 0;
   self->pcmsk = 0;
   self->pcint = 0;
   return;
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
void button_enable_interrupt(struct button* self)
{
   asm("SEI");
   PCICR |= (1 << self->pcint);
   *(self->pcmsk) |= (1 << self->pin);
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
void button_toggle_interrupt(struct button* self)
{
   if (button_interrupt_enabled(self))
   {
      button_disable_interrupt(self);
   }
   else
   {
      button_enable_interrupt(self);
   }

   return;
}