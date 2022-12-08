/********************************************************************************
* pwm.h: Inneh�ller drivrutiner f�r PWM-styrning av en godtycklig utenhet,
*        s�som en eller flera lysdioder.
********************************************************************************/
#ifndef PWM_H_
#define PWM_H_

/* Inkluderingsdirektiv: */
#include "misc.h"
#include "adc.h"

/********************************************************************************
* pwm: Strukt f�r PWM-kontrollers, som m�jligg�r PWM-styrning av en godtycklig 
*      utenhet, exempelvis en eller flera lysdioder implementerat via ett 
*      led-objekt eller en vektor inneh�llande pekare till multipla led-objekt. 
*      PWM-styrning kan ske via en analog insignal s�som en potentiometer eller
*      genom att direkt v�lja duty cycle.
********************************************************************************/
struct pwm
{
   struct adc input;               /* Analog inenhet, s�som en potentiometer. */
   uint16_t period_us;             /* Periodtid f�r PWM m�tt i mikrosekunder. */
   void* output;                   /* Pekare till ansluten utenhet. */
   void (*output_high)(void* arg); /* Pekare till funktion f�r att t�nda ansluten utenhet. */
   void (*output_low)(void* arg);  /* Pekare till funktion f�r att sl�cka ansluten utenhet. */
   bool enabled;                   /* Enable-signal f�r kontroll av PWM-generering. */
};

/********************************************************************************
* pwm_init: Initierar PWM-kontroller f�r PWM-styrning av angiven utenhet via
*           angiven analog insignal. Som default �r PWM-styrning aktiverat.
*
*           - self       : Pekare till PWM-kontrollern som ska initieras.
*           - input_pin  : Pekare till analog pin som utg�r insignal.
*           - period_us  : Periodtid f�r PWM m�tt i mikrosekunder.
*           - output     : Pekare till utenheten som ska styras med PWM.
*           - output_high: Funktionspekare f�r aktivering av angiven utenhet.
*           - output_low : Funktionspekare f�r inaktivering av angiven utenhet.
********************************************************************************/
void pwm_init(struct pwm* self, 
              uint8_t input_pin,
              const uint16_t period_us, 
              void* output,
              void* output_high, 
              void* output_low);

/********************************************************************************
* pwm_clear: Nollst�ller angiven PWM-kontroller.
*
*            - self: Pekare till PWM-kontrollern som ska nollst�llas.
********************************************************************************/
void pwm_clear(struct pwm* self);

/********************************************************************************
* pwm_enable: Aktiverar angiven PWM-kontroller s� att ansluten utenhet kan
*             styras via PWM-generering.
*
*             - self: Pekare till PWM-kontrollern som ska aktiveras.
********************************************************************************/
static inline void pwm_enable(struct pwm* self)
{
   self->enabled = true;
   return;
}

/********************************************************************************
* pwm_disable: Inaktiverar angiven PWM-kontroller, vilket medf�r att
*              PWM-styrning av ansluten utenhet inte kan genomf�ras.
*
*
*              - self: Pekare till PWM-kontrollern som ska inaktiveras.
********************************************************************************/
static inline void pwm_disable(struct pwm* self)
{
   self->enabled = false;
   self->output_low(self->output);
   return;
}

/********************************************************************************
* pwm_toggle: Togglar aktivering av angiven PWM-kontroller.
*
*
*              - self: Pekare till PWM-kontrollern som ska togglas.
********************************************************************************/
static inline void pwm_toggle(struct pwm* self)
{
   if (self->enabled)
   {
      pwm_disable(self);
   }
   else
   {
      pwm_enable(self);
   }
   return;
}

/********************************************************************************
* pwm_run: K�r angiven PWM-kontroller under en period och styr ansluten utenhet
*          via avl�sning av ansluten analog insignal, f�rutsatt att 
*          PWM-kontrollern �r aktiverad.
*
*          - self: Pekare till PWM-kontrollern som ska k�ras.
********************************************************************************/
void pwm_run(struct pwm* self);

/********************************************************************************
* pwm_run_with_duty_cycle: K�r angiven PWM-kontroller under en period och styr 
*                          ansluten utenhet med angiven duty cycle, f�rutsatt 
*                          att PWM-kontrollern �r aktiverad. Duty cycle m�ste
*                          anges som ett flyttal mellan 0 - 1, vilket motsvarar
*                          0 - 100 % duty cycle.
*
*         -                - self: Pekare till PWM-kontrollern som ska k�ras.
*                          - duty_cycle: Duty cycle, allts� andelen av aktuell
*                                        periodtid som ansluten utenhet ska
*                                        vara aktiverad, mellan 0 - 1.
********************************************************************************/
void pwm_run_with_duty_cycle(struct pwm* self, const double duty_cycle);

#endif /* PWM_H_ */