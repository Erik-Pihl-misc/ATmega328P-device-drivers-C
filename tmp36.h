/********************************************************************************
* tmp36.h: Inneh�ller drivrutiner f�r temperatursensor TMP36, som anv�nds f�r
*          m�tning samt utskrift av rumstemperaturen. Den analoga insignalen
*          fr�n TMP36 AD-omvandlas till en digital motsvarighet, som anv�nds
*          f�r att ber�kna aktuell temperatur. Utskrift sker till en seriell
*          terminal via USART.
********************************************************************************/
#ifndef TMP36_H_
#define TMP36_H_

/* Inkluderingsdirektiv: */
#include "adc.h"
#include "serial.h"

/* Makrodefinitioner: */
#define VCC 5.0 /* 5 V matningssp�nning. */

/********************************************************************************
* tmp36: Strukt f�r implementering av temperatursensor TMP36, som anv�nds f�r
*        m�tning samt utskrift av rumstemperaturen. Vid avl�sning AD-omvandlas
*        den analoga insignalen till en digital motsvarighet. Utefter detta
*        v�rde ber�knas den analoga insp�nningen Uin via nedanst�ende formel:
*
*        Uin = ADC_result / ADC_MAX * Vcc,
*
*        d�r ADC_result �r den AD-omvandlade insignalen (0 - 1023),
*        ADC_MAX �r h�gsta m�jliga digitala signal (1023) och Vcc �r 
*        mikrodatorns matningssp�nning (5 V).
*
*        Temperaturen T ber�knas utefter detta v�rde via nedanst�ende formel:
*
*        T = 100 * Uin - 50,
*
*        d�r Uin utg�r analog insp�nning avl�st fr�n temperatursensor TMP36.
********************************************************************************/
struct tmp36
{
   struct adc adc; /* AD-omvandlare, omvandlar analog insignal fr�n TMP36. */
};

/********************************************************************************
* tmp36_init: Initierar pin ansluten till temperatursensor TMP36 f�r m�tning
*             samt utskrift av rumstemperaturen. Seriell �verf�ring initieras
*             ocks� med en baud rate (�verf�ringshastighet) p� 9600 kbps.
*
*             - self: Pekare till temperatursensorn som ska initieras.
*             - pin : Analog pin A0 - A5 som temperatursensorn �r ansluten till.
********************************************************************************/
void tmp36_init(struct tmp36* self, 
                const uint8_t pin);

/********************************************************************************
* adc_get_input_voltage: Returnerar insp�nningen fr�n angiven tempsensor genom
*                        att l�sa av insignalen, omvandla till motsvarande
*                        digitala v�rde och sedan ber�kna motsvarande insp�nning.
*                        V�rdet ber�knas efter en matningssp�nning p� 5 V.
*
*                        - self: Pekare till temperatursensor TMP36.
********************************************************************************/
static inline double tmp36_get_input_voltage(const struct tmp36* self)
{
   return adc_duty_cycle(&self->adc) * VCC;
}

/********************************************************************************
* tmp36_get_temperature: Returnerar aktuell rumstemperatur via avl�sning av
*                        angiven temperatursensor TMP36.
*
*                        - self: Pekare till temperatursensor TMP36.
********************************************************************************/
static inline double tmp36_get_temperature(const struct tmp36* self)
{
   return 100 * tmp36_get_input_voltage(self) - 50;
}

/********************************************************************************
* tmp36_print_temperature: Skriver ut aktuell rumstemperatur avl�st av
*                          temperatursensor TMP36.
*
*                          - self: Pekare till temperatursensor TMP36.
********************************************************************************/
void tmp36_print_temperature(const struct tmp36* self);

/********************************************************************************
* tmp36_print_voltage: Skriver ut aktuell insp�nning avl�st av temperatursensor
*                      TMP36 m�tt i V.
*
*                      - self: Pekare till temperatursensor TMP36.
********************************************************************************/
void tmp36_print_voltage(const struct tmp36* self);

#endif /* TMP36_H_ */