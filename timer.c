/********************************************************************************
* timer.c: Inneh�ller definitioner av associerade funktioner f�r strukten timer.
********************************************************************************/
#include "timer.h"

/* Makrodefinitioner: */
#define TIME_BETWEEN_INTERRUPTS_MS 0.128 /* Tid mellan varje timergenererat avbrott. */

/* Statiska funktioner: */
static void timer_init_circuit(struct timer* self);
static void timer_disable_circuit(struct timer* self);
static inline uint32_t timer_get_max_count(const double time_ms);

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
                const double time_ms)
{
   self->counter = 0;
   self->max_count = timer_get_max_count(time_ms);
   self->timer_sel = timer_sel;
   timer_init_circuit(self);
   return;
}

/********************************************************************************
* timer_clear: Genomf�r total nollst�llning av angiven timerkrets.
*
*              - self: Pekare till timern som ska nollst�llas.
********************************************************************************/
void timer_clear(struct timer* self)
{
   timer_disable_circuit(self);
   self->counter = 0;
   self->max_count = 0;
   self->timsk = 0;
   self->timsk_bit = 0;
   self->timer_sel = TIMER_SEL_NONE;
   return;
}

/********************************************************************************
* timer_toggle_interrupt: Togglar aktivering av timergenererat avbrott p�
*                         angiven timer. Om avbrott �r aktiverat vid anrop sker 
*                         inaktivering. P� samma s�tt g�ller att om avbrott �r 
*                         inaktiverat vid anrop s� sker aktivering.
*
*                         - self: Pekare till timern som aktivering av
*                                 timergenererat avbrott ska togglas p�.
********************************************************************************/
void timer_toggle_interrupt(struct timer* self)
{
   if (timer_interrupt_enabled(self))
   {
      timer_disable_interrupt(self);
   }
   else
   {
      timer_enable_interrupt(self);
   }
   return;
}

/********************************************************************************
* timer_elapsed: Indikerar ifall angiven timer har l�pt ut genom att returnera
*                true eller false. Ifall timern har l�pt ut nollst�lls r�knaren
*                inf�r n�sta uppr�kning.
*
*                - self: Pekare till timern som ska kontrolleras.
********************************************************************************/
bool timer_elapsed(struct timer* self)
{
   if (self->counter >= self->max_count)
   {
     self->counter = 0;
     return true;
   }
   else
   {
      return false;
   }
}

/********************************************************************************
* timer_reset: �terst�ller angiven timer till startl�get.
*
               - self: Pekare till timern som ska �terst�llas.
********************************************************************************/
void timer_reset(struct timer* self)
{
   timer_disable_interrupt(self);
   self->counter = 0;
   return;
}

/********************************************************************************
* timer_set_new_time: S�tter ny tid p� angiven timerkrets m�tt i millisekunder.
* 
*                     - self   : Pekare till timern vars tid ska uppdateras.
*                     - time_ms: Tiden timern ska s�ttas p� i millisekunder.
********************************************************************************/
void timer_set_new_time(struct timer* self, 
                        const double time_ms)
{
   self->max_count = timer_get_max_count(time_ms);
   return;
}

/********************************************************************************
* timer_init_circuit: Initierar angiven timerkrets. Timer 0 samt Timer 2
*                     initieras i Normal Mode, medan Timer 1 initieras i
*                     CTC Mode med uppr�kning till 256. Vid aktiverat avbrott
*                     p� godtycklig initierad timer sker timergenererat avbrott 
*                     var 0.128:e millisekund. Adresserna till motsvarande
*                     maskregister som bit f�r aktivering av avbrott sparas.
*
*                     - self     : Pekare till timerkretsen som ska initieras.
********************************************************************************/
static void timer_init_circuit(struct timer* self)
{
   if (self->timer_sel == TIMER_SEL_0)
   {
      TCCR0B = (1 << CS01);
      self->timsk = &TIMSK0;
      self->timsk_bit = TOIE0;
   }
   else if (self->timer_sel == TIMER_SEL_1)
   {
      TCCR1B = (1 << CS11) | (1 << WGM12);
      OCR1A = 256;
      self->timsk = &TIMSK1;
      self->timsk_bit = OCIE1A;
   }
   else if (self->timer_sel == TIMER_SEL_2)
   {
      TCCR2B = (1 << CS21);
      self->timsk = &TIMSK2;
      self->timsk_bit = TOIE2;
   }

   asm("SEI");
   return;
}

/********************************************************************************
* timer_disable_circuit: Inaktiverar angiven timerkrets s� att varken uppr�kning
*                        eller avbrott kan ske. 
*
*                        - self: Pekare till timern som ska inaktiveras.  
********************************************************************************/
static void timer_disable_circuit(struct timer* self)
{
   if (self->timer_sel == TIMER_SEL_0)
   {
      TCCR0B = 0x00;
      TIMSK0 = 0x00;
   }
   else if (self->timer_sel == TIMER_SEL_1)
   {
      TCCR1B = 0x00;
      TIMSK1 = 0x00;
      OCR1A = 0x00;
   }
   else if (self->timer_sel == TIMER_SEL_2)
   {
      TCCR2B = 0x00;
      TIMSK2 = 0x00;
   }
   return;
}

/********************************************************************************
* timer_get_max_count: Returnerar antalet timergenererade avbrott som kr�vs
*                      f�r angiven tid, avrundad till n�rmaste heltal.
*
*                      - time_ms: �nskad tid m�tt i millisekunder.
********************************************************************************/
static inline uint32_t timer_get_max_count(const double time_ms)
{
   return (uint32_t)(time_ms / TIME_BETWEEN_INTERRUPTS_MS + 0.5);
}