/********************************************************************************
* timer.h: Inneh�ller drivrutiner f�r interruptbaserade timerkretsar via 
*          strukten timer samt associerade funktioner. Dessa timerkretsar 
*          fungerar ocks� utm�rkt att anv�nda som r�knare.
********************************************************************************/
#ifndef TIMER_H_
#define TIMER_H_

/* Inkluderingsdirektiv: */
#include "misc.h"

/********************************************************************************
* timer_sel: Enumeration f�r val av timerkrets.
********************************************************************************/
enum timer_sel
{
   TIMER_SEL_0,   /* Timer 0. */
   TIMER_SEL_1,   /* Timer 1. */
   TIMER_SEL_2,   /* Timer 2. */
   TIMER_SEL_NONE /* Timer ospecificerad. */
};

/********************************************************************************
* timer: Strukt f�r implementering av interruptbaserade timerkretsar, som
*        vid behov kan anv�ndas som r�knare.
********************************************************************************/
struct timer
{
   volatile uint32_t counter; /* 32-bitars r�knare. */
   uint32_t max_count;        /* Maxv�rde som uppr�kning ska ske till. */
   volatile uint8_t* timsk;   /* Pekare till maskregister f�r aktivering av avbrott. */
   uint8_t timsk_bit;         /* Bit f�r aktivering av avbrott i motsvarande maskregister. */
   enum timer_sel timer_sel;  /* Val av timerkrets. */
};

/********************************************************************************
* timer_init: Initierar ny timerkrets med angiven tid m�tt i millisekunder.
*             Om timern ska anv�ndas som r�knare f�r att r�kna upp till ett
*             specifikt maxv�rde b�r funktionen timer_set_max_count anropas
*             direkt efter initieringen.
*
*             - self     : Pekare till timern som ska initieras.
*             - timer_sel: Val av timerkrets.
*             - time_ms  : Tiden timern ska s�ttas p� m�tt i millisekunder.
********************************************************************************/
void timer_init(struct timer* self, 
                const enum timer_sel timer_sel, 
                const double time_ms);

/********************************************************************************
* timer_clear: Genomf�r total nollst�llning av angiven timerkrets.
*
*              - self: Pekare till timern som ska nollst�llas.
********************************************************************************/
void timer_clear(struct timer* self);

/********************************************************************************
* timer_enable_interrupt: Aktiverar timergenererat avbrott, som �ger rum n�r
*                         timern r�knar upp till overflow eller specificerat max.
*
*                         Timer 0 samt Timer 2 aktiveras i Normal Mode, medan
*                         Timer 1 aktiveras i CTC Mode med uppr�kning till 256,
*                         vilket g�r att tiden mellan varje timergenererat
*                         avbrott �r samma oavsett anv�nd timerkrets.
*
*                         Avbrottsvektorer f�r timerkretsarna deklareras nedan:
*
*                         Timerkrets     Avbrottsvektor
*                           Timer 0     TIMER0_OVF_vect
*                           Timer 1     TIMER1_COMPA_vect
*                           Timer 2     TIMER2_OVF_vect
*
*                         - self: Pekare till timern som timergenererat
*                                 avbrott ska aktiveras p�.
********************************************************************************/
static inline void timer_enable_interrupt(struct timer* self)
{
   *(self->timsk) |= (1 << self->timsk_bit);
   return;
}

/********************************************************************************
* timer_disable_interrupt: Inaktiverar timergenererat avbrott p� angiven timer.
*
*                          - self: Pekare till timern som timergenererat
*                                  avbrott ska inaktiveras p�.
********************************************************************************/
static inline void timer_disable_interrupt(struct timer* self)
{
   *(self->timsk) &= ~(1 << self->timsk_bit);
   return;
}

/********************************************************************************
* timer_toggle_interrupt: Togglar aktivering av timergenererat avbrott p�
*                         angiven timer.
*
*                         - self: Pekare till timern som aktivering av 
*                                 timergenererat avbrott ska togglas p�.
********************************************************************************/
void timer_toggle_interrupt(struct timer* self);

/********************************************************************************
* timer_interrupt_enabled: Indikerar ifall timergenererat avbrott �r aktiverat
*                          p� angiven timer.
*
*                          - self: Pekare till timern vars avbrottstillst�nd
*                                  ska kontrolleras.
********************************************************************************/
static inline bool timer_interrupt_enabled(const struct timer* self)
{
   return (*(self->timsk) & (1 << self->timsk_bit));
}

/********************************************************************************
* timer_count: R�knar upp angiven timer.
*
*              - self: Pekare till timern som ska r�knas upp.
********************************************************************************/
static inline void timer_count(struct timer* self)
{
   self->counter++;
   return;
}

/********************************************************************************
* timer_elapsed: Indikerar ifall angiven timer har l�pt ut genom att returnera
*                true eller false. Ifall timern har l�pt ut nollst�lls r�knaren
*                inf�r n�sta uppr�kning.
*
*                - self: Pekare till timern som ska kontrolleras.
********************************************************************************/
bool timer_elapsed(struct timer* self);

/********************************************************************************
* timer_reset_counter: Nollst�ller r�knaren p� angiven timer.
*
*                      - self: Pekare till timern vars r�knare ska nollst�llas.
********************************************************************************/
static inline void timer_reset_counter(struct timer* self)
{
   self->counter = 0;
   return;
}

/********************************************************************************
* timer_reset: �terst�ller angiven timer till startl�get.
*
               - self: Pekare till timern som ska �terst�llas.
********************************************************************************/
void timer_reset(struct timer* self);

/********************************************************************************
* timer_set_new_time: S�tter ny tid p� angiven timerkrets.
*
*                    - self   : Pekare till timern vars tid ska uppdateras.
*                    - time_ms: Tiden timern ska s�ttas p� m�tt i millisekunder.
********************************************************************************/
void timer_set_new_time(struct timer* self, 
                        const double time_ms);

/********************************************************************************
* timer_set_new_max_count: S�tter nytt maxv�rde f�r uppr�kning av timern n�r
*                          denna ska anv�ndas som en r�knare.
*
*                          - self   : Pekare till timern.
*                          - max_count: Maxv�rde f�r uppr�kningen.
********************************************************************************/
static inline void timer_set_max_count(struct timer* self,
                                       const uint32_t max_count)
{
   self->max_count = max_count;
   return;
}

#endif /* TIMER_H_ */