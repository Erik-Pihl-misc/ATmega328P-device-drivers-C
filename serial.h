/********************************************************************************
* serial.h: Inneh�ller drivrutiner f�r seriell �verf�ring via USART.
********************************************************************************/
#ifndef SERIAL_H_
#define SERIAL_H_

/* Inkluderingsdirektiv: */
#include "misc.h"

/********************************************************************************
* serial_init: Initierar USART f�r seriell �verf�ring med angiven baud rate.
*
*              - baud_rate: �verf�ringshastighet m�tt i kilobits per sekund.
********************************************************************************/
void serial_init(const uint32_t baud_rate_kbps);

/********************************************************************************
* serial_print_string: Skriver ut text via seriell �verf�ring.
*
*                      - s: Pekare till det textstycke som ska skrivas ut.
********************************************************************************/
void serial_print_string(const char* s);

/********************************************************************************
* serial_print_integer: Skriver ut ett signerat heltal via seriell �verf�ring.
*
*                      - number: Heltalet som ska skrivas ut.
********************************************************************************/
void serial_print_integer(const int32_t number);

/********************************************************************************
* serial_print_unsigned: Skriver ut ett osignerat heltal via seriell �verf�ring.
*
*                        - number: Heltalet som ska skrivas ut.
********************************************************************************/
void serial_print_unsigned(const uint32_t number);

/********************************************************************************
* serial_print_double: Skriver ut ett flyttal via seriell �verf�ring.
*
*                      - number: Flyttalet som ska skrivas ut.
********************************************************************************/
void serial_print_double(const double number);

/********************************************************************************
* serial_print_char: Skriver ut ett enskilt tecken via seriell �verf�ring.
*
*                    - character: Det tecken som ska skrivas ut.
********************************************************************************/
void serial_print_char(const char character);

/********************************************************************************
* serial_print_new_line: S�tter n�sta utskrift till l�ngst till v�nster p� 
*                        n�sta rad via utskrift av ett nyradstecken.
********************************************************************************/
static inline void serial_print_new_line(void)
{
   serial_print_string("\n");
   return;
}

#endif /* SERIAL_H_ */