/********************************************************************************
* eeprom.h: Inneh�ller drivrutiner f�r skrivning samt l�sning till och fr�n
*           EEPROM-minnet.
********************************************************************************/
#ifndef EEPROM_H_
#define EEPROM_H_

/* Inkluderingsdirektiv: */
#include "misc.h"
#include "word.h"

/* Makrodefinitioner: */
#define EEPROM_ADDRESS_MIN 0    /* L�gsta adress i EEPROM-minnet. */
#define EEPROM_ADDRESS_MAX 1023 /* H�gsta adress i EEPROM-minnet. */

/********************************************************************************
* eeprom_write_byte: Skriver en byte best�ende av ett osignerat heltal till 
*                    angiven adress i EEPROM-minnet. Vid lyckad skrivning
*                    returneras 0, annars returneras felkod 1.
*
*                    - address: Adressen i EEPROM-minnet som angiven data
*                               ska lagras p�.
*                    - data   : Datan som ska skrivas.
********************************************************************************/
int eeprom_write_byte(const uint16_t address, 
                      const uint8_t data);

/********************************************************************************
* eeprom_write_word: Skriver tv� byte best�ende av ett osignerat heltal till
*                    angiven samt efterf�ljande adress i EEPROM-minnet. Vid 
*                    lyckad skrivning returneras 0, annars returneras felkod 1.
*
*                    - address_low: Den l�gre adressen i EEPROM-minnet som 
*                                   angiven data ska lagras p�.
*                    - data       : Datan som ska skrivas.
********************************************************************************/
int eeprom_write_word(const uint16_t address_low,
                      const uint16_t data);

/********************************************************************************
* eeprom_read_byte: L�ser en byte p� angiven adress i EEPROM-minnet och 
*                   returnerar detta som ett osignerat heltal. Vid misslyckad
*                   l�sning returneras 0.
*
*                   - address: Adressen i EEPROM-minnet som ska l�sas av.
********************************************************************************/
uint8_t eeprom_read_byte(const uint16_t address);

/********************************************************************************
* eeprom_read_word: L�ser tv� byte p� angiven samt efterf�ljande adress i 
*                   EEPROM-minnet och returnerar detta som ett osignerat heltal.
*                   Vid misslyckad l�sning returneras 0.
*
*                   - address_low: Den l�gre adressen i EEPROM-minnet som
*                                  ska l�sas av.
********************************************************************************/
uint16_t eeprom_read_word(const uint16_t address_low);

#endif /* EEPROM_H_ */