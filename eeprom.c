/********************************************************************************
* eeprom.c: Inneh�ller funktionsdefinitioner f�r skrivning samt l�sning till 
*           och fr�n EEPROM-minnet.
********************************************************************************/
#include "eeprom.h"

/********************************************************************************
* eeprom_write_byte: Skriver en byte best�ende av ett osignerat heltal till
*                    angiven adress i EEPROM-minnet. Vid lyckad skrivning
*                    returneras 0, annars returneras felkod 1.
*
*                    1. Om angiven adress �verstiger h�gsta adressen i EEPROM-
*                       minnet sker ingen skrivning och felkod 1 returneras.
*
*                    2. Eventuell f�reg�ende skrivning avslutas innan den
*                       nya skrivningen p�b�rjas.
*
*                    3. Angiven adress samt datan som ska skrivas specificeras.
*
*                    4. Avbrott inaktiveras tempor�rt f�r att inte avbryta
*                       eventuell skrivning (EEPROM-skrivningen m�ste ske
*                       inom fyra klockcykler f�r att lyckas).
*
*                    5. Skrivningen genomf�rs.
*
*                    6. Avbrott �teraktiveras.
*                       
*                    - address: Adressen i EEPROM-minnet som angiven data
*                               ska lagras p�.
*                    - data   : Datan som ska skrivas.
********************************************************************************/
int eeprom_write_byte(const uint16_t address,
                      const uint8_t data)
{
   if (address > EEPROM_ADDRESS_MAX) return 0;
   while (EECR & (1 << EEPE));

   EEAR = address;
   EEDR = data;
   asm("CLI");

   EECR |= (1 << EEMPE);
   EECR |= (1 << EEPE);
   asm("SEI");
   return 0;
}

/********************************************************************************
* eeprom_write_word: Skriver tv� byte best�ende av ett osignerat heltal till
*                    angiven samt efterf�ljande adress i EEPROM-minnet. Vid
*                    lyckad skrivning returneras 0, annars returneras felkod 1.
*
*                    1. Om angiven adress �verstiger n�st h�gsta adressen i
*                       EEPROM-minnet sker ingen skrivning och felkod 1 
*                       returneras.
*
*                    2. Det 16-bitars talet lagras som tv� separata byte.
*
*                    3. Respektive byte skrivs till EEPROM-minnet p� 
*                       konsekutiva adresser.
*
*                    4. Returkod 0 returneras f�r att indikera lyckas skrivning.
*
*                    - address_low: Den l�gre adressen i EEPROM-minnet som
*                                   angiven data ska lagras p�.
*                    - data       : Datan som ska skrivas.
********************************************************************************/
int eeprom_write_word(const uint16_t address_low,
                      const uint16_t data)
{
   if (address_low > EEPROM_ADDRESS_MAX - 1) return 1;
   union word number;
   word_init(&number, data);

   eeprom_write_byte(address_low, number.segmented.low);
   eeprom_write_byte(address_low + 1, number.segmented.high);
   return 0;
}

/********************************************************************************
* eeprom_read_byte: L�ser en byte p� angiven adress i EEPROM-minnet och
*                   returnerar detta som ett osignerat heltal. Vid misslyckad
*                   l�sning returneras 0.
*
*                   1. Om angiven adress �verstiger h�gsta adressen i EEPROM-
*                      minnet sker ingen l�sning och 0 returneras.
*
*                   2. Eventuell f�reg�ende skrivning avslutas innan l�sningen
*                      p�b�rjas.
*
*                   3. Angiven adress som l�sning ska ske fr�n specificeras.
*
*                   4. L�sningen genomf�rs fr�n angiven adress.
*
*                   5. Inneh�llet returneras som ett 8-bitars osignerat heltal.
*
*                   - address: Adressen i EEPROM-minnet som ska l�sas av.
********************************************************************************/
uint8_t eeprom_read_byte(const uint16_t address)
{
   if (address > EEPROM_ADDRESS_MAX) return 0;
   while (EECR & (1 << EEPE));
   EEAR = address;
   EECR |= (1 << EERE);
   return EEDR;
}

/********************************************************************************
* eeprom_read_word: L�ser tv� byte p� angiven samt efterf�ljande adress i
*                   EEPROM-minnet och returnerar detta som ett osignerat heltal.
*                   Vid misslyckad l�sning returneras 0.
*
*                   1. Om angiven adress �verstiger h�gsta adressen i EEPROM-
*                      minnet sker ingen l�sning och 0 returneras.
*
*                   2. Respektive byte l�ses fr�n EEPROM-minnet en i taget
*                      och lagras som ett ord (ett 16-bitars tal).
*
*                   3. Avl�st 16-bitars tal returneras.
*
*                   - address_low: Den l�gre adressen i EEPROM-minnet som
*                                  ska l�sas av.
********************************************************************************/
uint16_t eeprom_read_word(const uint16_t address_low)
{
   if (address_low > EEPROM_ADDRESS_MAX - 1) return 0;
   union word number;
   word_init(&number, 0);

   number.segmented.low = eeprom_read_byte(address_low);
   number.segmented.high = eeprom_read_byte(address_low + 1);
   return number.whole;
}