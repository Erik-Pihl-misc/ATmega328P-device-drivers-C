/********************************************************************************
* led_vector.c: Inneh�ller funktionsdefinitioner f�r implementering av dynamiska
*               vektorer f�r lagring och styrning �ver multipla lysdioder eller
*               andra utportar via strukten led_vector.
********************************************************************************/
#include "led_vector.h"

/********************************************************************************
* led_vector_resize: �ndrar storleken p� angiven vektor s� att den efter
*                    omallokering rymmer angivet antal lysdiodspekare, som kan
*                    tilldelas direkt via index i st�llet f�r en push-operation.
*                    Vid misslyckad minnesallokering returneras felkod 1.
*                    Annars om omallokeringen lyckas s� returneras 0.
*
*                    - self    : Pekare till vektorn vars storlek ska �ndras.
*                    - new_size: Vektorns nya storlek.
********************************************************************************/
int led_vector_resize(struct led_vector* self,
                      const size_t new_size)
{
   if (new_size == 0)
   {
      led_vector_clear(self);
      return 0;
   }
   else
   {
      struct led** copy = (struct led**)realloc(self->leds, sizeof(struct led*) * new_size);
      if (!copy) return 1;
      self->leds = copy;
      self->size = new_size;
      return 0;
   }
}

/********************************************************************************
* led_vector_push: L�gger till en pekare till en ny lysdiod l�ngst bak i angiven 
*                  vektor. Vid misslyckad minnesallokering returneras felkod 1.
*                  Annars om push-operationen lyckas returneras 0.
*
*                  - self   : Pekare till vektorn som ska tilldelas.
*                  - new_led: Pekare till den nya lysdiod som ska l�ggas till.
********************************************************************************/
int led_vector_push(struct led_vector* self,
                    struct led* new_led)
{
   struct led** copy = (struct led**)realloc(self->leds, sizeof(struct led*) * (self->size + 1));
   if (!copy) return 1;
   copy[self->size++] = new_led;
   self->leds = copy;
   return 0;
}

/********************************************************************************
* led_vector_pop: Tar bort eventuellt lysdiodspekare i angiven vektor genom
*                 att minska dess storlek med ett. Vid misslyckad omallokering
*                 returneras felkod 1, annars 0.
*
*                 - self: Pekare till vektorn vars sista element ska tas bort.
********************************************************************************/
int led_vector_pop(struct led_vector* self)
{
   if (self->size <= 1)
   {
      led_vector_clear(self);
      return 0;
   }
   else
   {
      struct led** copy = (struct led**)realloc(self->leds, sizeof(struct led*) * (self->size - 1));
      if (!copy) return 1;
      self->leds = copy;
      self->size--;
      return 0;
   }
}

/********************************************************************************
* led_vector_on: T�nder samtliga lysdioder lagrade i angiven vektor.
*
*                - self: Pekare till vektorn vars lysdioder ska t�ndas.
********************************************************************************/
void led_vector_on(struct led_vector* self)
{
   for (struct led** i = self->leds; i < self->leds + self->size; ++i)
   {
      led_on(*i);
   }
   return;
}


/********************************************************************************
* led_vector_off: Sl�cker samtliga lysdioder lagrade i angiven vektor.
*
*                 - self: Pekare till vektorn vars lysdioder ska sl�ckas.
********************************************************************************/
void led_vector_off(struct led_vector* self)
{
   for (struct led** i = self->leds; i < self->leds + self->size; ++i)
   {
      led_off(*i);
   }
   return;
}

/********************************************************************************
* led_vector_toggle: Togglar samtliga lysdioder lagrade i angiven vektor.
*
*                    - self: Pekare till vektorn vars lysdioder ska togglas.
********************************************************************************/
void led_vector_toggle(struct led_vector* self)
{
   for (struct led** i = self->leds; i < self->leds + self->size; ++i)
   {
      led_toggle(*i);
   }
   return;
}

/********************************************************************************
* led_vector_blink_collectively: Genomf�r kollektiv (synkroniserad) blinkning
*                                av samtliga lysdioder lagrade i angiven vektor.
*
*                                - self          : Pekare till vektorn vars
*                                                  lysdioder ska blinkas.
*                                - blink_speed_ms: Lysdiodernas blinkhastighet
*                                                  m�tt i millisekunder.
********************************************************************************/
void led_vector_blink_collectively(struct led_vector* self,
                                   const uint16_t blink_speed_ms)
{
   led_vector_on(self);
   delay_ms(blink_speed_ms);
   led_vector_off(self);
   delay_ms(blink_speed_ms);
   return;
}

/********************************************************************************
* led_vector_blink_sequentially: Genomf�r sekventiell blinkning av samtliga
*                                lysdioder lagrade i angiven vektor. D�rmed
*                                blinkar lysdioderna i en sekvens en efter en.
*
*                                - self          : Pekare till vektorn vars
*                                                  lysdioder ska blinkas.
*                                - blink_speed_ms: Lysdiodernas blinkhastighet
*                                                  m�tt i millisekunder.
********************************************************************************/
void led_vector_blink_sequentially(struct led_vector* self,
                                   const uint16_t blink_speed_ms)
{
   led_vector_off(self);

   for (struct led** i = self->leds; i < self->leds + self->size; ++i)
   {
      led_on(*i);
      delay_ms(blink_speed_ms);
      led_off(*i);
   }

   return;
}