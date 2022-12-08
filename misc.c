/********************************************************************************
* misc.c: Inneh�ller diverse funktionsdefinitioner.
********************************************************************************/
#include "misc.h"

/********************************************************************************
* delay_ms: Genererar f�rdr�jning m�tt i millisekunder.
*
*           - delay_time_ms: Angiven f�rdr�jningstid i millisekunder.
********************************************************************************/
void delay_ms(const uint16_t delay_time_ms)
{
   for (uint16_t i = 0; i < delay_time_ms; ++i)
   {
      _delay_ms(1);
   }
   return;
}

/********************************************************************************
* delay_us: Genererar f�rdr�jning m�tt i mikrosekunder.
*
*           - delay_time_us: Angiven f�rdr�jningstid i mikrosekunder.
********************************************************************************/
void delay_us(const uint16_t delay_time_us)
{
   for (uint16_t i = 0; i < delay_time_us; ++i)
   {
      _delay_us(1);
   }
   return;
}

/********************************************************************************
* delay_ms_ptr: Genererar f�rdr�jning m�tt i millisekunder via en pekare.
*
*           - delay_time_um: Pekare till f�rdr�jningstiden m�tt i millisekunder.
********************************************************************************/
void delay_ms_ptr(const volatile uint16_t* delay_time_ms)
{
   for (uint16_t i = 0; i < *delay_time_ms; ++i)
   {
      _delay_ms(1);
   }
   return;
}

/********************************************************************************
* delay_us_ptr: Genererar f�rdr�jning m�tt i mikrosekunder via en pekare.
*
*           - delay_time_us: Pekare till f�rdr�jningstiden m�tt i mikrosekunder.
********************************************************************************/
void delay_us_ptr(const volatile uint16_t* delay_time_us)
{
   for (uint16_t i = 0; i < *delay_time_us; ++i)
   {
      _delay_us(1);
   }
   return;
}