/********************************************************************************
* adc.h: Inneh�ller funktionalitet f�r enkel implementering av AD-omvandling
*        f�r analoga signaler via strukten adc. AD-omvandling sker via inl�sning
*        av signaler fr�n analoga pinnar A0 - A5 p�  Arduino Uno, vilket
*        motsvarar PORTC0 - PORTC5 p� ATmega328P.
*
*        Analoga insignaler mellan 0 - 5 V AD-omvandlas till digitala
*        motsvarigheter mellan 0 - 1023. Duty cycle kan anv�ndas f�r
*        PWM-generering och ber�knas enligt nedan:
*
*                       duty cycle = ADC_result / ADC_MAX,
*
*       d�r ADC_result �r resultat avl�st fr�n AD-omvandlaren OCH ADC_MAX
*       utg�r h�gsta m�jliga avl�sta v�rde, vilket �r 1023.0.
********************************************************************************/
#ifndef ADC_H_
#define ADC_H_

/* Inkluderingsdirektiv: */
#include "misc.h"

/* Makrodefinitioner: */
#define ADC_MAX 1023.0 /* H�gsta digitala v�rde vid AD-omvandling (motsvarar 5 V). */
#define VCC 5.0        /* 5 V matningssp�nning. */

/********************************************************************************
* adc: Strukt f�r implementering av AD-omvandlare, som m�jligg�r avl�sning
*      av insignaler fr�n analoga pinnar samt ber�kning av on- och off-tid f�r
*      PWM-generering (tiden som PWM-styrd utsignal ska vara h�g respektive l�g).
********************************************************************************/
struct adc
{
   uint8_t pin;         /* Analog pin som ska anv�ndas f�r avl�sning. */
   uint16_t pwm_on_us;  /* On-tid f�r PWM-generering i mikrosekunder. */
   uint16_t pwm_off_us; /* Off-tid f�r PWM-generering i mikrosekunder. */
};

/********************************************************************************
* adc_init: Initierar analog pin f�r avl�sning och AD-omvandling av insignaler,
*           som antingen kan anges som ett tal mellan 0 - 5 eller via konstanter
*           A0 - A5 (som motsvarar heltal 14 - 19).
*
*           - self: Pekare till analog pin som ska anv�ndas f�r AD-omvandling.
*           - pin : Analog pin som ska l�sas f�r AD-omvandling.
********************************************************************************/
void adc_init(struct adc* self,
              const uint8_t pin);

/********************************************************************************
* adc_clear: Nollst�ller angivet adc-objekt.
*
*            - self: Pekare till objektet som ska nollst�llas.
********************************************************************************/
static inline void adc_clear(struct adc* self)
{
   self->pin = 0;
   self->pwm_on_us = 0;
   self->pwm_off_us = 0;
   return;
}

/********************************************************************************
* adc_read: L�ser av en analog insignal och returnerar motsvarande digitala
*           motsvarighet mellan 0 - 1023.
*
*           - self: Pekare till analog pin vars insignal ska AD-omvandlas.
********************************************************************************/
uint16_t adc_read(const struct adc* self);

/********************************************************************************
* adc_duty_cycle: L�ser av en analog insignal och returnerar motsvarande
*                 duty cycle som ett flyttal mellan 0 - 1.
*
*                 - self: Pekare till analog pin vars insignal ska AD-omvandlas.
********************************************************************************/
static inline double adc_duty_cycle(const struct adc* self)
{
   return adc_read(self) / ADC_MAX;
}

/********************************************************************************
* adc_get_pwm_values: L�ser av en analog insignal och ber�knar on- och off-tid
*                     f�r PWM-generering, avrundat till n�rmaste heltal.
*
*                     - self         : Pekare till analog pin som ska l�sas av.
*                     - pwm_period_us: PWM-perioden (on-tid + off-tid) m�tt i
*                                      mikrosekunder (default = 10 000 us).
********************************************************************************/
void adc_get_pwm_values(struct adc* self,
                        uint16_t pwm_period_us);

/********************************************************************************
* adc_get_input_voltage: Returnerar insp�nningen p� angiven analog pin genom att
*                        l�sa av insignalen, omvandla till motsvarande digitala
*                        v�rde och sedan ber�kna motsvarande insp�nning.
*
*                        V�rdet ber�knas efter en matningssp�nning p� 5 V,
*                        vilket medf�r en analog insignal mellan 0 - 5 V. 
*                        Genom att avl�sa duty cycle (mellan 0 - 1) s� erh�lls 
*                        motsvarande analoga insp�nning Uin, som returneras.
*     
*                        - self: Pekare till analog pin som ska l�sas av.
********************************************************************************/
static inline double adc_get_input_voltage(const struct adc* self)
{
   return adc_duty_cycle(self) * VCC;
}

/********************************************************************************
* adc_get_temperature: Returnerar aktuell rumstemperatur via avl�sning av
*                      temperatursensor TMP36, ansluten till angiven pin.
*
*                      Temperaturen T ber�knas via f�ljande formel:
*                      T = 100 * Uin - 50,
*                      d�r Uin �r aktuell analog insp�nning avl�st fr�n 
*                      temperatursensor TMP36.
*
*                      - self: Pekare till analog pin som ska l�sas av.
********************************************************************************/
static inline double adc_get_temperature(const struct adc* self)
{
   return 100 * adc_get_input_voltage(self) - 50;
}

#endif /* ADC_H_ */