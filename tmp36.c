/********************************************************************************
* tmp36.c: Inneh�ller definitioner av drivrutiner f�r temperatursensor TMP36.
********************************************************************************/
#include "tmp36.h"

/********************************************************************************
* tmp36_init: Initierar pin ansluten till temperatursensor TMP36 f�r m�tning
*             samt utskrift av rumstemperaturen. Seriell �verf�ring initieras
*             ocks� med en baud rate (�verf�ringshastighet) p� 9600 kbps.
*
*             - self: Pekare till temperatursensorn som ska initieras.
*             - pin : Analog pin A0 - A5 som temperatursensorn �r ansluten till.
********************************************************************************/
void tmp36_init(struct tmp36* self,
                const uint8_t pin)
{
   adc_init(&self->adc, pin);
   serial_init(9600);
   return;
}

/********************************************************************************
* tmp36_print_temperature: Skriver ut aktuell rumstemperatur avl�st av
*                          temperatursensor TMP36.
*
*                          - self: Pekare till temperatursensor TMP36.
********************************************************************************/
void tmp36_print_temperature(const struct tmp36* self)
{
   serial_print_string("Temperature: ");
   serial_print_double(tmp36_get_temperature(self));
   serial_print_string(" degrees Celcius.\n");
   return;
}

/********************************************************************************
* tmp36_print_voltage: Skriver ut aktuell insp�nning avl�st av temperatursensor
*                      TMP36 m�tt i V.
*
*                      - self: Pekare till temperatursensor TMP36.
********************************************************************************/
void tmp36_print_voltage(const struct tmp36* self)
{
   serial_print_string("Voltage: ");
   serial_print_double(tmp36_get_input_voltage(self));
   serial_print_string(" V\n.");
   return;
}